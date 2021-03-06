/*
 * This file is part of UBIFS.
 *
 * Copyright (C) 2006-2008 Nokia Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Authors: Adrian Hunter
 *          Artem Bityutskiy (Битюцкий Артём)
 */

/*
 * This file implements TNC (Tree Node Cache) which caches indexing nodes of
 * the UBIFS B-tree.
 *
 * At the moment the locking rules of the TNC tree are quite simple and
 * straightforward. We just have a mutex and lock it when we traverse the
 * tree. If a znode is not in memory, we read it from flash while still having
 * the mutex locked.
 */

#include <linux/crc32.h>
#include "ubifs.h"

/*
 * Returned codes of 'matches_name()' and 'fallible_matches_name()' functions.
 * @NAME_LESS: name corresponding to the first argument is less than second
 * @NAME_MATCHES: names match
 * @NAME_GREATER: name corresponding to the second argument is greater than
 *                first
 * @NOT_ON_MEDIA: node referred by zbranch does not exist on the media
 *
 * These constants were introduce to improve readability.
 */
enum {
	NAME_LESS    = 0,
	NAME_MATCHES = 1,
	NAME_GREATER = 2,
	NOT_ON_MEDIA = 3,
};

/**
 * insert_old_idx - record an index node obsoleted since the last commit start.
 * @c: UBIFS file-system description object
 * @lnum: LEB number of obsoleted index node
 * @offs: offset of obsoleted index node
 *
 * Returns %0 on success, and a negative error code on failure.
 *
 * For recovery, there must always be a complete intact version of the index on
 * flash at all times. That is called the "old index". It is the index as at the
 * time of the last successful commit. Many of the index nodes in the old index
 * may be dirty, but they must not be erased until the next successful commit
 * (at which point that index becomes the old index).
 *
 * That means that the garbage collection and the in-the-gaps method of
 * committing must be able to determine if an index node is in the old index.
 * Most of the old index nodes can be found by looking up the TNC using the
 * 'lookup_znode()' function. However, some of the old index nodes may have
 * been deleted from the current index or may have been changed so much that
 * they cannot be easily found. In those cases, an entry is added to an RB-tree.
 * That is what this function does. The RB-tree is ordered by LEB number and
 * offset because they uniquely identify the old index node.
 */
static int insert_old_idx(struct ubifs_info *c, int lnum, int offs)
{
	struct ubifs_old_idx *old_idx, *o;
	struct rb_node **p, *parent = NULL;

	ubifs_assert(lnum >= c->main_first && lnum < c->leb_cnt);
	ubifs_assert(offs >= 0 && offs < c->leb_size);

	old_idx = kmalloc(sizeof(struct ubifs_old_idx), GFP_NOFS);
	if (!old_idx)
		return -ENOMEM;
	old_idx->lnum = lnum;
	old_idx->offs = offs;

	p = &c->old_idx.rb_node;
	while (*p) {
		parent = *p;
		o = rb_entry(parent, struct ubifs_old_idx, rb);
		if (lnum < o->lnum)
			p = &(*p)->rb_left;
		else if (lnum > o->lnum)
			p = &(*p)->rb_right;
		else if (offs < o->offs)
			p = &(*p)->rb_left;
		else if (offs > o->offs)
			p = &(*p)->rb_right;
		else {
			ubifs_err("old idx added twice!");
			kfree(old_idx);
			return 0;
		}
	}
	rb_link_node(&old_idx->rb, parent, p);
	rb_insert_color(&old_idx->rb, &c->old_idx);
	return 0;
}

/**
 * insert_old_idx_znode - record a znode obsoleted since last commit start.
 * @c: UBIFS file-system description object
 * @znode: znode of obsoleted index node
 *
 * Returns %0 on success, and a negative error code on failure.
 */
int insert_old_idx_znode(struct ubifs_info *c, struct ubifs_znode *znode)
{
	if (znode->parent) {
		struct ubifs_zbranch *zbr;

		zbr = &znode->parent->zbranch[znode->iip];
		if (zbr->len)
			return insert_old_idx(c, zbr->lnum, zbr->offs);
	} else
		if (c->zroot.len)
			return insert_old_idx(c, c->zroot.lnum,
					      c->zroot.offs);
	return 0;
}

/**
 * ins_clr_old_idx_znode - record a znode obsoleted since last commit start.
 * @c: UBIFS file-system description object
 * @znode: znode of obsoleted index node
 *
 * Returns %0 on success, and a negative error code on failure.
 */
static int ins_clr_old_idx_znode(struct ubifs_info *c,
				 struct ubifs_znode *znode)
{
	int err;

	if (znode->parent) {
		struct ubifs_zbranch *zbr;

		zbr = &znode->parent->zbranch[znode->iip];
		if (zbr->len) {
			err = insert_old_idx(c, zbr->lnum, zbr->offs);
			if (err)
				return err;
			zbr->lnum = 0;
			zbr->offs = 0;
			zbr->len = 0;
		}
	} else
		if (c->zroot.len) {
			err = insert_old_idx(c, c->zroot.lnum, c->zroot.offs);
			if (err)
				return err;
			c->zroot.lnum = 0;
			c->zroot.offs = 0;
			c->zroot.len = 0;
		}
	return 0;
}

/**
 * destroy_old_idx - destroy the old_idx RB-tree.
 * @c: UBIFS file-system description object
 *
 * During start commit, the old_idx RB-tree is used to avoid overwriting index
 * nodes that were in the index last commit but have since been deleted.  This
 * is necessary for recovery i.e. the old index must be kept intact until the
 * new index is successfully written.  The old-idx RB-tree is used for the
 * in-the-gaps method of writing index nodes and is destroyed every commit.
 */
void destroy_old_idx(struct ubifs_info *c)
{
	struct rb_node *this = c->old_idx.rb_node;
	struct ubifs_old_idx *old_idx;

	while (this) {
		if (this->rb_left) {
			this = this->rb_left;
			continue;
		} else if (this->rb_right) {
			this = this->rb_right;
			continue;
		}
		old_idx = rb_entry(this, struct ubifs_old_idx, rb);
		this = rb_parent(this);
		if (this) {
			if (this->rb_left == &old_idx->rb)
				this->rb_left = NULL;
			else
				this->rb_right = NULL;
		}
		kfree(old_idx);
	}
	c->old_idx = RB_ROOT;
}

/**
 * read_znode - read an indexing node from flash and fill znode.
 * @c: UBIFS file-system description object
 * @lnum: LEB of the indexing node to read
 * @offs: node offset
 * @len: node length
 * @znode: znode to read to
 *
 * This function reads an indexing node from the flash media and fills znode
 * with the read data. Returns zero in case of success and a negative error
 * code in case of failure. The read indexing node is validated and if anything
 * is wrong with it, this function prints complaint messages and returns
 * %-EINVAL.
 */
static int read_znode(struct ubifs_info *c, int lnum, int offs, int len,
		      struct ubifs_znode *znode)
{
	int i, err, type, cmp;
	struct ubifs_idx_node *idx;

	idx = kmalloc(c->max_idx_node_sz, GFP_NOFS);
	if (!idx)
		return -ENOMEM;

	err = ubifs_read_node(c, idx, UBIFS_IDX_NODE, len, lnum, offs);
	if (err < 0)
		goto out;

	znode->child_cnt = le16_to_cpu(idx->child_cnt);
	znode->level = le16_to_cpu(idx->level);

	dbg_tnc("LEB %d:%d, level %d, %d branch",
		lnum, offs, znode->level, znode->child_cnt);

	if (znode->child_cnt > c->fanout || znode->level > UBIFS_MAX_LEVELS) {
		dbg_err("current fanout %d, branch count %d",
			c->fanout, znode->child_cnt);
		dbg_err("max levels %d, znode level %d",
			UBIFS_MAX_LEVELS, znode->level);
		goto out_dump;
	}

	for (i = 0; i < znode->child_cnt; i++) {
		const struct ubifs_branch *br = ubifs_idx_branch(c, idx, i);
		struct ubifs_zbranch *zbr = &znode->zbranch[i];

		key_read(c, &br->key, &zbr->key);
		zbr->lnum = le32_to_cpu(br->lnum);
		zbr->offs = le32_to_cpu(br->offs);
		zbr->len  = le32_to_cpu(br->len);
		zbr->znode = NULL;

		/* Validate branch */

		if (zbr->lnum < c->main_first ||
		    zbr->lnum >= c->leb_cnt || zbr->offs < 0 ||
		    zbr->offs + zbr->len > c->leb_size || zbr->offs & 7) {
			dbg_err("bad branch %d", i);
			goto out_dump;
		}

		switch (key_type(c, &zbr->key)) {
		case UBIFS_INO_KEY:
		case UBIFS_DATA_KEY:
		case UBIFS_DENT_KEY:
		case UBIFS_XENT_KEY:
			break;
		default:
			dbg_key(c, &zbr->key, "bad key type at slot %d: ", i);
			goto out_dump;
		}

		if (znode->level)
			continue;

		type = key_type(c, &zbr->key);
		if (c->ranges[type].max_len == 0) {
			if (zbr->len != c->ranges[type].len) {
				dbg_err("bad target node (type %d) length (%d)",
					type, zbr->len);
				dbg_err("have to be %d", c->ranges[type].len);
				goto out_dump;
			}
		} else if (zbr->len < c->ranges[type].min_len ||
			   zbr->len > c->ranges[type].max_len) {
			dbg_err("bad target node (type %d) length (%d)",
				type, zbr->len);
			dbg_err("have to be in range of %d-%d",
				c->ranges[type].min_len,
				c->ranges[type].max_len);
			goto out_dump;
		}
	}

	/*
	 * Ensure that the next key is greater or equivalent to the
	 * previous one.
	 */
	for (i = 0; i < znode->child_cnt - 1; i++) {
		const union ubifs_key *key1, *key2;

		key1 = &znode->zbranch[i].key;
		key2 = &znode->zbranch[i + 1].key;

		cmp = keys_cmp(c, key1, key2);
		if (cmp > 0) {
			dbg_err("bad key order (keys %d and %d)", i, i + 1);
			goto out_dump;
		} else if (cmp == 0 && !is_hash_key(c, key1)) {
			/* These can only be keys with colliding hash */
			dbg_err("keys %d and %d are not hashed but equivalent",
				i, i + 1);
			goto out_dump;
		}
	}

	kfree(idx);
	return 0;

out:
	kfree(idx);
	return err;

out_dump:
	ubifs_err("bad indexing node at LEB %d:%d", lnum, offs);
	dbg_dump_node(c, idx);
	kfree(idx);
	return -EINVAL;
}

/**
 * load_znode - load znode to TNC cache.
 * @c: UBIFS file-system description object
 * @zbr: znode branch
 * @parent: znode's parent
 * @iip: index in parent
 *
 * This function loads znode pointed to by @zbr into the TNC cache and
 * returns pointer to it in case of success and a negative error code in case
 * of failure.
 */
static struct ubifs_znode *load_znode(struct ubifs_info *c,
				      struct ubifs_zbranch *zbr,
				      struct ubifs_znode *parent, int iip)
{
	int err;
	struct ubifs_znode *znode;

	ubifs_assert(!zbr->znode);
	/*
	 * A slab cache is not presently used for znodes because the znode size
	 * depends on the fanout which is stored in the superblock.
	 */
	znode = kzalloc(c->max_znode_sz, GFP_NOFS);
	if (!znode)
		return ERR_PTR(-ENOMEM);

	err = read_znode(c, zbr->lnum, zbr->offs, zbr->len, znode);
	if (err)
		goto out;

	atomic_long_inc(&c->clean_zn_cnt);

	/*
	 * Increment the global clean znode counter as well. It is OK that
	 * global and per-FS clean znode counters may be inconsistent for some
	 * short time (because we might be preempted at this point), the global
	 * one is only used in shrinker.
	 */
	atomic_long_inc(&ubifs_clean_zn_cnt);

	zbr->znode = znode;
	znode->parent = parent;
	znode->time = get_seconds();
	znode->iip = iip;

	return znode;

out:
	kfree(znode);
	return ERR_PTR(err);
}

/**
 * copy_znode - copy a dirty znode.
 * @c: UBIFS file-system description object
 * @znode: znode to copy
 *
 * A dirty znode being committed may not be changed, so it is copied.
 */
static struct ubifs_znode *copy_znode(struct ubifs_info *c,
				      struct ubifs_znode *znode)
{
	struct ubifs_znode *zn;

	zn = kzalloc(c->max_znode_sz, GFP_NOFS);
	if (!zn)
		return ERR_PTR(-ENOMEM);

	memcpy(zn, znode, c->max_znode_sz);

	ubifs_assert(!test_bit(OBSOLETE_ZNODE, &znode->flags));
	set_bit(OBSOLETE_ZNODE, &znode->flags);

	if (znode->level != 0) {
		int i;
		const int n = zn->child_cnt;

		/* The children now have new parent */
		for (i = 0; i < n; i++) {
			struct ubifs_zbranch *zbr = &zn->zbranch[i];

			if (zbr->znode)
				zbr->znode->parent = zn;
		}
	}

	zn->cnext = NULL;
	set_bit(DIRTY_ZNODE, &zn->flags);
	clear_bit(COW_ZNODE, &zn->flags);
	atomic_long_inc(&c->dirty_zn_cnt);

	return zn;
}

/**
 * add_idx_dirt - add dirt due to a dirty znode.
 * @c: UBIFS file-system description object
 * @lnum: LEB number of index node
 * @dirt: size of index node
 *
 * This function updates lprops dirty space and the new size of the index.
 */
static int add_idx_dirt(struct ubifs_info *c, int lnum, int dirt)
{
	c->calc_idx_sz -= ALIGN(dirt, 8);
	return ubifs_add_dirt(c, lnum, dirt);
}

/**
 * dirty_cow_znode - ensure a znode is not being committed.
 * @c: UBIFS file-system description object
 * @zbr: branch of znode to check
 *
 * Returns dirtied znode on success or negative error code on failure.
 */
static struct ubifs_znode *dirty_cow_znode(struct ubifs_info *c,
					   struct ubifs_zbranch *zbr)
{
	struct ubifs_znode *znode = zbr->znode;
	struct ubifs_znode *zn;
	int err;

	if (!test_bit(COW_ZNODE, &znode->flags)) {
		/* znode is not being committed */
		if (!test_and_set_bit(DIRTY_ZNODE, &znode->flags)) {
			atomic_long_inc(&c->dirty_zn_cnt);
			atomic_long_dec(&c->clean_zn_cnt);
			atomic_long_dec(&ubifs_clean_zn_cnt);
			err = add_idx_dirt(c, zbr->lnum, zbr->len);
			if (err)
				return ERR_PTR(err);
		}
		return znode;
	}

	zn = copy_znode(c, znode);
	if (IS_ERR(zn))
		return zn;

	if (zbr->len) {
		err = insert_old_idx(c, zbr->lnum, zbr->offs);
		if (err)
			return ERR_PTR(err);

		err = add_idx_dirt(c, zbr->lnum, zbr->len);
	} else
		err = 0;

	zbr->znode = zn;
	zbr->lnum = 0;
	zbr->offs = 0;
	zbr->len = 0;

	if (err)
		return ERR_PTR(err);

	return zn;
}

/**
 * lnc_lookup - lookup the leaf-node-cache.
 * @c: UBIFS file-system description object
 * @zbr: zbranch of leaf node
 * @node: leaf node
 *
 * Leaf nodes are non-index nodes like dent (directory entry) nodes or data
 * nodes.  The purpose of the leaf-node-cache is to save re-reading the same
 * leaf node over and over again.  Most things are cached by VFS, however the
 * file system must cache directory entries for readdir and for resolving hash
 * collisions.  The present implementation of the leaf-node-cache is extremely
 * simple, and allows for error returns that are not used but that may be needed
 * if a more complex implementation is created.
 *
 * This function returns %1 if the leaf node is in the cache, %0 if it is not,
 * and a negative error code otherwise.
 */
static int lnc_lookup(struct ubifs_info *c, struct ubifs_zbranch *zbr,
		      void *node)
{
	if (zbr->leaf == NULL)
		return 0;
	ubifs_assert(zbr->len != 0);
	memcpy(node, zbr->leaf, zbr->len);
	return 1;
}

/**
 * ubifs_validate_entry - validate directory or extended attribute entry node.
 * @c: UBIFS file-system description object
 * @dent: the node to validate
 *
 * This function validates directory or extended attribute entry node @dent.
 * Returns zero if the node is all right and a %-EINVAL if not.
 */
int ubifs_validate_entry(struct ubifs_info *c,
			 const struct ubifs_dent_node *dent)
{
	int key_type, nlen = le16_to_cpu(dent->nlen);

	if (le32_to_cpu(dent->ch.len) != nlen + UBIFS_DENT_NODE_SZ + 1 ||
	    dent->type >= UBIFS_ITYPES_CNT ||
	    nlen > UBIFS_MAX_NLEN || dent->name[nlen] != 0 ||
	    strnlen(dent->name, nlen) != nlen ||
	    le64_to_cpu(dent->inum) > MAX_INUM) {
		const char *node_type;

		if (key_type_flash(c, dent->key) == UBIFS_DENT_KEY)
			node_type = "directory entry";
		else
			node_type = "extended attribute entry";

		ubifs_err("bad %s node", node_type);
		return -EINVAL;
	}

	key_type = key_type_flash(c, dent->key);
	if (key_type_flash(c, dent->key) != UBIFS_DENT_KEY &&
	    key_type_flash(c, dent->key) != UBIFS_XENT_KEY) {
		ubifs_err("bad key type %d", key_type);
		return -EINVAL;
	}

	return 0;
}

/**
 * lnc_add - add a leaf node to the leaf-node-cache.
 * @c: UBIFS file-system description object
 * @zbr: zbranch of leaf node
 * @node: leaf node
 *
 * This function returns %0 to indicate success and a negative error code
 * otherwise.
 */
static int lnc_add(struct ubifs_info *c, struct ubifs_zbranch *zbr,
		   const void *node)
{
	int err;
	void *lnc_node;
	const struct ubifs_dent_node *dent = node;

	ubifs_assert(zbr->leaf == NULL);
	ubifs_assert(zbr->len != 0);

	/* Add all dents, but nothing else */
	if (key_type(c, &zbr->key) != UBIFS_DENT_KEY)
		return 0;

	err = ubifs_validate_entry(c, dent);
	if (err) {
		dbg_dump_node(c, dent);
		return err;
	}

	lnc_node = kmalloc(zbr->len, GFP_NOFS);
	if (!lnc_node)
		return 0; /* We don't have to have the cache, so no error */

	memcpy(lnc_node, node, zbr->len);
	zbr->leaf = lnc_node;
	return 0;
}

/**
 * lnc_free - remove a leaf node from the leaf-node-cache.
 * @zbr: zbranch of leaf node
 * @node: leaf node
 *
 * This function returns %0 to indicate success and a negative error code
 * otherwise.
 */
static void lnc_free(struct ubifs_zbranch *zbr)
{
	if (zbr->leaf == NULL)
		return;
	kfree(zbr->leaf);
	zbr->leaf = NULL;
}

/**
 * tnc_read_node - read a leaf node.
 * @c: UBIFS file-system description object
 * @zbr:  key and position of node
 * @node: node returned
 *
 * This function reads leaf defined node by @zbr and returns zero in case of
 * success or a negative negative error code in case of failure.
 */
static int tnc_read_node(struct ubifs_info *c, struct ubifs_zbranch *zbr,
			 void *node)
{
	union ubifs_key key1, *key = &zbr->key;
	int err, type = key_type(c, key);
	struct ubifs_wbuf *wbuf;

	if (lnc_lookup(c, zbr, node))
		return 0; /* Read from the leaf-node-cache */
	/*
	 * 'zbr' has to point to on-flash node. The node may sit in a bud and
	 * may even be in a write buffer, so we have to take care about this.
	 */
	wbuf = ubifs_get_wbuf(c, zbr->lnum);
	if (wbuf)
		err = ubifs_read_node_wbuf(wbuf, node, type, zbr->len,
					   zbr->lnum, zbr->offs);
	else
		err = ubifs_read_node(c, node, type, zbr->len, zbr->lnum,
				      zbr->offs);

	if (err) {
		dbg_tnc_key(c, key, "key");
		return err;
	}

	/* Make sure the key of the read node is correct */
	key_read(c, key, &key1);
	if (memcmp(node + UBIFS_KEY_OFFSET, &key1, c->key_len)) {
		ubifs_err("bad key in node at LEB %d:%d",
			  zbr->lnum, zbr->offs);
		dbg_tnc_key(c, key, "looked for key");
		dbg_tnc_key(c, &key1, "found node's key");
		dbg_dump_node(c, node);
		return err;
	}

	/* Consider adding the node to the leaf node cache */
	err = lnc_add(c, zbr, node);
	return err;
}

/**
 * try_read_node - read a node if it is a node.
 * @c: UBIFS file-system description object
 * @buf: buffer to read to
 * @type: node type
 * @len: node length (not aligned)
 * @lnum: LEB number of node to read
 * @offs: offset of node to read
 *
 * This function tries to read a node of known type and length, checks it and
 * stores it in @buf. This function returns %1 if a node is present and %0 if
 * a node is not present. A negative error code is returned for I/O errors.
 * This function performs that same function as ubifs_read_node except that
 * it does not require that there is actually a node present and instead
 * the return code indicates if a node was read.
 */
static int try_read_node(const struct ubifs_info *c, void *buf, int type,
			 int len, int lnum, int offs)
{
	int err, node_len;
	struct ubifs_ch *ch = buf;
	uint32_t crc, node_crc;

	dbg_io("LEB %d:%d, %s, length %d", lnum, offs, dbg_ntype(type), len);
	ubifs_assert(lnum >= 0 && lnum < c->leb_cnt && offs >= 0);
	ubifs_assert(len >= UBIFS_CH_SZ && offs + len <= c->leb_size);
	ubifs_assert(!(offs & 7) && offs < c->leb_size);
	ubifs_assert(type >= 0 && type < UBIFS_NODE_TYPES_CNT);

	err = ubi_read(c->ubi, lnum, buf, offs, len);
	if (err) {
		ubifs_err("cannot read node type %d from LEB %d:%d, error %d",
			  type, lnum, offs, err);
		return err;
	}

	if (le32_to_cpu(ch->magic) != UBIFS_NODE_MAGIC)
		return 0;

	if (ch->node_type != type)
		return 0;

	node_len = le32_to_cpu(ch->len);
	if (node_len != len)
		return 0;

	crc = crc32(UBIFS_CRC32_INIT, buf + 8, node_len - 8);
	node_crc = le32_to_cpu(ch->crc);
	if (crc != node_crc)
		return 0;

	return 1;
}

/**
 * fallible_read_node - try to read a leaf node.
 * @c: UBIFS file-system description object
 * @key:  key of node to read
 * @zbr:  position of node
 * @node: node returned
 *
 * This function tries to read a node and returns %1 if the node is read, %0
 * if the node is not present, and a negative error code in the case of error.
 */
static int fallible_read_node(struct ubifs_info *c, const union ubifs_key *key,
			      struct ubifs_zbranch *zbr, void *node)
{
	int ret;

	dbg_tnc_key(c, key, "LEB %d:%d, key", zbr->lnum, zbr->offs);

	if (lnc_lookup(c, zbr, node))
		return 0; /* Read from the leaf-node-cache */

	ret = try_read_node(c, node, key_type(c, key), zbr->len, zbr->lnum,
			    zbr->offs);
	if (ret == 1) {
		union ubifs_key node_key;
		struct ubifs_dent_node *dent = node;

		/* All nodes have key in the same place */
		key_read(c, &dent->key, &node_key);
		if (keys_cmp(c, key, &node_key) == 0) {
			/*
			 * If the node sequence number is greater than the
			 * current replay sequence number, then the node should
			 * not yet be in the index, so this must be a dangling
			 * branch.
			 */
			if (le64_to_cpu(dent->ch.sqnum) > c->replay_sqnum)
				ret = 0;
			else {
				/* Add the node to the leaf node cache */
				int err = lnc_add(c, zbr, node);

				if (err)
					return err;
			}
		} else
			ret = 0;
	}
	if (ret == 0)
		dbg_mnt_key(c, key, "dangling branch LEB %d:%d len %d, key",
			    zbr->lnum, zbr->offs, zbr->len);
	return ret;
}

/**
 * matches_name - determine if a directory or extended attribute entry matches
 *                a given name.
 * @c: UBIFS file-system description object
 * @zbr: zbranch of dent
 * @nm: name to match
 *
 * This function checks if xentry/direntry referred by zbranch @zbr matches name
 * @nm. Returns %NAME_MATCHES if it does, %NAME_LESS if the name referred by
 * @zbr is less than @nm, and %NAME_GREATER if it is greater than @nm. In case
 * of failure, a negative error code is returned.
 */
static int matches_name(struct ubifs_info *c, struct ubifs_zbranch *zbr,
			const struct qstr *nm)
{
	struct ubifs_dent_node *dent;
	int nlen, err;

	/* If possible, match against the dent in the leaf-node-cache */
	if (!zbr->leaf) {
		dent = kmalloc(zbr->len, GFP_NOFS);
		if (!dent)
			return -ENOMEM;

		/*
		 * In this case we end up allocating another dent object in
		 * lnc_add(), although it could have just inserted this dent.
		 */
		err = tnc_read_node(c, zbr, dent);
		if (err)
			goto out_free;

		err = ubifs_validate_entry(c, dent);
		if (err) {
			lnc_free(zbr);
			dbg_dump_node(c, dent);
			goto out_free;
		}

		kfree(dent);
	}

	dent = zbr->leaf;
	nlen = le16_to_cpu(dent->nlen);
	err = memcmp(dent->name, nm->name, min_t(int, nlen, nm->len));
	if (err == 0) {
		if (nlen == nm->len)
			return NAME_MATCHES;
		else if (nlen < nm->len)
			return NAME_LESS;
		else
			return NAME_GREATER;
	} else if (err < 0)
		return NAME_LESS;
	else
		return NAME_GREATER;

out_free:
	kfree(dent);
	return err;
}

/**
 * get_znode - get a TNC znode that may not be loaded yet.
 * @c: UBIFS file-system description object
 * @znode: parent znode
 * @n: znode branch slot number
 *
 * This function returns the znode or a negative error code.
 */
static struct ubifs_znode *get_znode(struct ubifs_info *c,
				     struct ubifs_znode *znode, int n)
{
	struct ubifs_zbranch *zbr;

	zbr = &znode->zbranch[n];
	if (zbr->znode)
		znode = zbr->znode;
	else
		znode = load_znode(c, zbr, znode, n);
	return znode;
}

/**
 * tnc_next - find next TNC entry.
 * @c: UBIFS file-system description object
 * @zn: znode is passed and returned here
 * @n: znode branch slot number is passed and returned here
 *
 * This function returns %0 if the next TNC entry is found, %-ENOENT if there is
 * no next entry, or a negative error code otherwise.
 */
static int tnc_next(struct ubifs_info *c, struct ubifs_znode **zn, int *n)
{
	struct ubifs_znode *znode = *zn;
	int nn = *n;

	nn += 1;
	if (nn < znode->child_cnt) {
		*n = nn;
		return 0;
	}
	while (1) {
		struct ubifs_znode *zp;

		zp = znode->parent;
		if (!zp)
			return -ENOENT;
		nn = znode->iip + 1;
		znode = zp;
		if (nn < znode->child_cnt) {
			znode = get_znode(c, znode, nn);
			if (IS_ERR(znode))
				return PTR_ERR(znode);
			while (znode->level != 0) {
				znode = get_znode(c, znode, 0);
				if (IS_ERR(znode))
					return PTR_ERR(znode);
			}
			nn = 0;
			break;
		}
	}
	*zn = znode;
	*n = nn;
	return 0;
}

/**
 * tnc_prev - find previous TNC entry.
 * @c: UBIFS file-system description object
 * @zn: znode is returned here
 * @n: znode branch slot number is passed and returned here
 *
 * This function returns %0 if the previous TNC entry is found, %-ENOENT if
 * there is no next entry, or a negative error code otherwise.
 */
static int tnc_prev(struct ubifs_info *c, struct ubifs_znode **zn, int *n)
{
	struct ubifs_znode *znode = *zn;
	int nn = *n;

	if (nn > 0) {
		*n = nn - 1;
		return 0;
	}
	while (1) {
		struct ubifs_znode *zp;

		zp = znode->parent;
		if (!zp)
			return -ENOENT;
		nn = znode->iip - 1;
		znode = zp;
		if (nn >= 0) {
			znode = get_znode(c, znode, nn);
			if (IS_ERR(znode))
				return PTR_ERR(znode);
			while (znode->level != 0) {
				nn = znode->child_cnt - 1;
				znode = get_znode(c, znode, nn);
				if (IS_ERR(znode))
					return PTR_ERR(znode);
			}
			nn = znode->child_cnt - 1;
			break;
		}
	}
	*zn = znode;
	*n = nn;
	return 0;
}

/**
 * resolve_collision - resolve a collision.
 * @c: UBIFS file-system description object
 * @key: key of a directory or extended attribute entry
 * @zn: znode is returned here
 * @n: zbranch number is passed and returned here
 * @nm: name of the entry
 *
 * This function is called for "hashed" keys to make sure that the found key
 * really corresponds to the looked up node (directory or extended attribute
 * entry). It returns %1 and sets @zn and @n if the collision is resolved.
 * %0 is returned if @nm is not found and @zn and @n are set to the previous
 * entry, i.e. to the entry after which @nm could follow if it were in TNC.
 * This means that @n may be set to %-1 if the leftmost key in @zn is the
 * previous one. A negative error code is returned on failures.
 */
static int resolve_collision(struct ubifs_info *c, const union ubifs_key *key,
			     struct ubifs_znode **zn, int *n,
			     const struct qstr *nm)
{
	int err;

	err = matches_name(c, &(*zn)->zbranch[*n], nm);
	if (unlikely(err < 0))
		return err;
	if (err == NAME_MATCHES)
		return 1;

	if (err == NAME_GREATER) {
		/* Look left */
		while (1) {
			err = tnc_prev(c, zn, n);
			if (err == -ENOENT) {
				ubifs_assert(*n == 0);
				*n = -1;
				return 0;
			}
			if (err < 0)
				return err;
			if (keys_cmp(c, &(*zn)->zbranch[*n].key, key))
				return 0;
			err = matches_name(c, &(*zn)->zbranch[*n], nm);
			if (err < 0)
				return err;
			if (err == NAME_LESS)
				return 0;
			if (err == NAME_MATCHES)
				return 1;
			ubifs_assert(err == NAME_GREATER);
		}
	} else {
		int nn = *n;
		struct ubifs_znode *znode = *zn;

		/* Look right */
		while (1) {
			err = tnc_next(c, &znode, &nn);
			if (err == -ENOENT)
				return 0;
			if (err < 0)
				return err;
			if (keys_cmp(c, &znode->zbranch[nn].key, key))
				return 0;
			err = matches_name(c, &znode->zbranch[nn], nm);
			if (err < 0)
				return err;
			if (err == NAME_GREATER)
				return 0;
			*zn = znode;
			*n = nn;
			if (err == NAME_MATCHES)
				return 1;
			ubifs_assert(err == NAME_LESS);
		}
	}
}

/**
 * fallible_matches_name - determine if a dent matches a given name.
 * @c: UBIFS file-system description object
 * @zbr: zbranch of dent
 * @nm: name to match
 *
 * This is a "fallible" version of 'matches_name()' function which does not
 * panic if the direntry/xentry referred by @zbr does not exist on the media.
 *
 * This function checks if xentry/direntry referred by zbranch @zbr matches name
 * @nm. Returns %NAME_MATCHES it does, %NAME_LESS if the name referred by @zbr
 * is less than @nm, %NAME_GREATER if it is greater than @nm, and @NOT_ON_MEDIA
 * if xentry/direntry referred by @zbr does not exist on the media. A negative
 * error code is returned in case of failure.
 */
static int fallible_matches_name(struct ubifs_info *c, struct ubifs_zbranch *zbr,
				 const struct qstr *nm)
{
	struct ubifs_dent_node *dent;
	int nlen, err;

	/* If possible, match against the dent in the leaf-node-cache */
	if (!zbr->leaf) {
		dent = kmalloc(zbr->len, GFP_NOFS);
		if (!dent)
			return -ENOMEM;

		/*
		 * In this case we end up allocating another dent object in lnc_add(),
		 * although it could have just inserted this dent.
		 */
		err = fallible_read_node(c, &zbr->key, zbr, dent);
		if (err < 0)
			goto out_free;
		if (err == 0) {
			/* The node was not present */
			err = NOT_ON_MEDIA;
			goto out_free;
		}

		ubifs_assert(err == 1);
		err = ubifs_validate_entry(c, dent);
		if (err) {
			lnc_free(zbr);
			dbg_dump_node(c, dent);
			goto out_free;
		}

		kfree(dent);
	}

	dent = zbr->leaf;
	nlen = le16_to_cpu(dent->nlen);
	err = memcmp(dent->name, nm->name, min_t(int, nlen, nm->len));
	if (err == 0) {
		if (nlen == nm->len)
			return NAME_MATCHES;
		else if (nlen < nm->len)
			return NAME_LESS;
		else
			return NAME_GREATER;
	} else if (err < 0)
		return NAME_LESS;
	else
		return NAME_GREATER;

out_free:
	kfree(dent);
	return err;
}

/**
 * fallible_resolve_collision - resolve a collision even if nodes are missing.
 * @c: UBIFS file-system description object
 * @key: key of directory entry
 * @zn: znode is returned here
 * @n: zbranch number is passed and returned here
 * @nm: name of directory entry
 *
 * This is a "fallible" version of the 'resolve_collision()' function which
 * does not panic if one of the nodes referred to by TNC does not exist on the
 * media. This may happen when replaying the journal if a deleted node was
 * Garbage-collected and the commit was not done. The following are return
 * codes:
 *  o if @nm was found, %1 is returned and @zn and @n are set to the found
 *    entry;
 *  o if @nm was not found, but there is a dangling zbranch, which is a zbranch
 *    referring an entry which does not exist, %1 is returned as well and @zn
 *    and @n are set to the dangling entry; this is needed during replay and
 *    basically means that we assume that the dangling entry is the entry we
 *    are looking for; to put it differently, this function is used by the
 *    replay code, and when the replay code hits a deletion entry, it either
 *    deletes an existing entry in the TNC, or a dangling entry, assuming the
 *    corresponding node has just been GC'ed;
 * o if @nm was not found, and no dangling entries were found, %-1 is returned
 *   @zn and @n are set to the previous entry;
 * o a negative error code is returned in case of failure.
 */
static int fallible_resolve_collision(struct ubifs_info *c,
				      const union ubifs_key *key,
				      struct ubifs_znode **zn, int *n,
				      const struct qstr *nm)
{
	struct ubifs_znode *o_znode = NULL, *znode = *zn;
	int uninitialized_var(o_n), err, cmp, unsure = 0, nn = *n;

	cmp = fallible_matches_name(c, &znode->zbranch[nn], nm);
	if (unlikely(cmp < 0))
		return cmp;
	if (cmp == NAME_MATCHES)
		return 1;
	if (cmp == NOT_ON_MEDIA) {
		o_znode = znode;
		o_n = nn;
		/*
		 * We are unlucky and hit a dangling zbranch straight away. Now
		 * we do not really know where to go to find the needed key -
		 * to the left or to the right. Well, let's try left.
		 */
		dbg_mnt_key(c, key, "first dangling match LEB %d:%d len %d ",
			    znode->zbranch[nn].lnum, znode->zbranch[nn].offs,
			    znode->zbranch[nn].len);
		unsure = 1;
	}

	if (cmp == NAME_GREATER || unsure) {
		/* Look left */
		while (1) {
			err = tnc_prev(c, zn, n);
			if (err == -ENOENT) {
				ubifs_assert(*n == 0);
				*n = -1;
				break;
			}
			if (err < 0)
				return err;
			if (keys_cmp(c, &(*zn)->zbranch[*n].key, key))
				break;
			err = fallible_matches_name(c, &(*zn)->zbranch[*n], nm);
			if (err < 0)
				return err;
			if (err == NAME_LESS)
				break;
			if (err == NAME_MATCHES)
				return 1;
			if (err == NOT_ON_MEDIA) {
				o_znode = *zn;
				o_n = *n;
			} else
				unsure = 0;
		}
	}

	if (cmp == NAME_LESS || unsure) {
		/* Look right */
		*zn = znode;
		*n = nn;
		while (1) {
			err = tnc_next(c, &znode, &nn);
			if (err == -ENOENT)
				break;
			if (err < 0)
				return err;
			if (keys_cmp(c, &znode->zbranch[nn].key, key))
				break;
			err = fallible_matches_name(c, &znode->zbranch[nn], nm);
			if (err < 0)
				return err;
			if (err == NAME_GREATER)
				break;
			*zn = znode;
			*n = nn;
			if (err == NAME_MATCHES)
				return 1;
			if (err == NOT_ON_MEDIA) {
				o_znode = znode;
				o_n = nn;
			}
		}
	}

	if (!o_znode)
		return 0;

	dbg_mnt_key(c, key, "dangling match LEB %d:%d len %d ",
		    o_znode->zbranch[o_n].lnum, o_znode->zbranch[o_n].offs,
		    o_znode->zbranch[o_n].len);
	*zn = o_znode;
	*n = o_n;
	return 1;
}

/**
 * matches_position - determine if a zbranch matches a given position.
 * @zbr: zbranch of dent
 * @lnum: LEB number of dent to match
 * @offs: offset of dent to match
 *
 * This function returns %1 if @lnum:@offs matches, and %0 otherwise.
 */
static int matches_position(struct ubifs_zbranch *zbr, int lnum, int offs)
{
	if (zbr->lnum == lnum && zbr->offs == offs)
		return 1;
	else
		return 0;
}

/**
 * resolve_collision_directly - resolve a collision directly.
 * @c: UBIFS file-system description object
 * @key: key of directory entry
 * @zn: znode is passed and returned here
 * @n: zbranch number is passed and returned here
 * @lnum: LEB number of dent node to match
 * @offs: offset of dent node to match
 *
 * This function is used for "hashed" keys to make sure the found directory or
 * extended attribute entry node is what was looked for. It is used when the
 * flash address of the right node is known (@lnum:@offs) which makes it much
 * easier to resolve collisions (no need to read entries and match full
 * names). This function returns %1 and sets @zn and @n if the collision is
 * resolved, %0 if @lnum:@offs is not found and @zn and @n are set to the
 * previous directory entry. Otherwise a negative error code is returned.
 */
static int resolve_collision_directly(struct ubifs_info *c,
				      const union ubifs_key *key,
				      struct ubifs_znode **zn, int *n,
				      int lnum, int offs)
{
	struct ubifs_znode *znode;
	int nn, err;

	znode = *zn;
	nn = *n;
	if (matches_position(&znode->zbranch[nn], lnum, offs))
		return 1;

	/* Look left */
	while (1) {
		err = tnc_prev(c, &znode, &nn);
		if (err == -ENOENT)
			break;
		if (err < 0)
			return err;
		if (keys_cmp(c, &znode->zbranch[nn].key, key))
			break;
		if (matches_position(&znode->zbranch[nn], lnum, offs)) {
			*zn = znode;
			*n = nn;
			return 1;
		}
	}

	/* Look right */
	znode = *zn;
	nn = *n;
	while (1) {
		err = tnc_next(c, &znode, &nn);
		if (err == -ENOENT)
			return 0;
		if (err < 0)
			return err;
		if (keys_cmp(c, &znode->zbranch[nn].key, key))
			return 0;
		*zn = znode;
		*n = nn;
		if (matches_position(&znode->zbranch[nn], lnum, offs)) {
			return 1;
		}
	}
}

/**
 * dirty_cow_bottom_up - dirty a znode and its ancestors.
 * @c: UBIFS file-system description object
 * @znode: znode to dirty
 *
 * If we do not have a unique key that resides in a znode, then we cannot
 * dirty that znode from the top down (i.e. by using lookup_level0_dirty)
 * This function records the path back to the last dirty ancestor, and then
 * dirties the znodes on that path.
 */
static struct ubifs_znode *dirty_cow_bottom_up(struct ubifs_info *c,
					       struct ubifs_znode *znode)
{
	struct ubifs_znode *zp;
	int *path = c->bottom_up_buf, p = 0;

	ubifs_assert(c->zroot.znode != NULL);
	ubifs_assert(znode != NULL);
	if (c->zroot.znode->level > BOTTOM_UP_HEIGHT) {
		kfree(c->bottom_up_buf);
		c->bottom_up_buf = kmalloc(c->zroot.znode->level * sizeof(int),
					   GFP_NOFS);
		if (!c->bottom_up_buf)
			return ERR_PTR(-ENOMEM);
		path = c->bottom_up_buf;
	}
	if (c->zroot.znode->level) {
		/* Go up until parent is dirty */
		while (1) {
			int n;

			zp = znode->parent;
			if (!zp)
				break;
			n = znode->iip;
			ubifs_assert(p < c->zroot.znode->level);
			path[p++] = n;
			if (!zp->cnext && ubifs_zn_dirty(znode))
				break;
			znode = zp;
		}
	}

	/* Come back down, dirtying as we go */
	while (1) {
		struct ubifs_zbranch *zbr;

		zp = znode->parent;
		if (zp) {
			ubifs_assert(path[p - 1] >= 0);
			ubifs_assert(path[p - 1] < zp->child_cnt);
			zbr = &zp->zbranch[path[--p]];
			znode = dirty_cow_znode(c, zbr);
		} else {
			ubifs_assert(znode == c->zroot.znode);
			znode = dirty_cow_znode(c, &c->zroot);
		}
		if (unlikely(IS_ERR(znode)) || !p)
			break;
		ubifs_assert(path[p - 1] >= 0);
		ubifs_assert(path[p - 1] < znode->child_cnt);
		znode = znode->zbranch[path[p - 1]].znode;
	}

	return znode;
}

/**
 * lookup_level0 - search for zero-level znode.
 * @c: UBIFS file-system description object
 * @key:  key to lookup
 * @zn: znode is returned here
 * @n: znode branch slot number is returned here
 *
 * This function looks up the TNC tree and search for zero-level znode which
 * refers key @key. The found zero-level znode is returned in @zn. There are 3
 * cases:
 *   o exact match, i.e. the found zero-level znode contains key @key, then %1
 *     is returned and slot number of the matched branch is stored in @n;
 *   o not exact match, which means that zero-level znode does not contain @key
 *     then %0 is returned and slot number of the closed branch is stored in
 *     @n;
 *   o @key is so small that it is even less than the lowest key of the
 *     leftmost zero-level node, then %0 is returned and %0 is stored in @n.
 *
 * Note, when the TNC tree is traversed, some znodes may be absent, then this
 * function reads corresponding indexing nodes and inserts them to TNC. In
 * case of failure, a negative error code is returned.
 */
static int lookup_level0(struct ubifs_info *c, const union ubifs_key *key,
			 struct ubifs_znode **zn, int *n)
{
	int err, exact;
	struct ubifs_znode *znode;
	unsigned long time = get_seconds();

	dbg_tnc_key(c, key, "search key");

	znode = c->zroot.znode;
	if (unlikely(!znode)) {
		znode = load_znode(c, &c->zroot, NULL, 0);
		if (IS_ERR(znode))
			return PTR_ERR(znode);
	}

	znode->time = time;

	while (1) {
		struct ubifs_zbranch *zbr;

		/*
		 * The below is a debugging hack to make UBIFS eat RAM and
		 * cause fake memory pressure. It is compiled out if it is not
		 * enabled in kernel configuration.
		 */
		dbg_eat_memory();

		exact = ubifs_search_zbranch(c, znode, key, n);

		if (znode->level == 0)
			break;

		if (*n < 0)
			*n = 0;
		zbr = &znode->zbranch[*n];

		if (zbr->znode) {
			znode->time = time;
			znode = zbr->znode;
			continue;
		}

		/* znode is not in TNC cache, load it from the media */
		znode = load_znode(c, zbr, znode, *n);
		if (IS_ERR(znode))
			return PTR_ERR(znode);
	}

	*zn = znode;
	if (exact || !is_hash_key(c, key) || *n != -1) {
		dbg_tnc("found %d, lvl %d, n %d", exact, znode->level, *n);
		return exact;
	}

	/*
	 * Here is a tricky place. We have not found the key and this is a
	 * "hashed" key, which may collide. The rest of the code deals with
	 * situations like this:
	 *
	 *                  | 3 | 5 |
	 *                  /       \
	 *          | 3 | 5 |      | 6 | 7 | (x)
	 *
	 * Or more a complex example:
	 *
	 *                | 1 | 5 |
	 *                /       \
	 *       | 1 | 3 |         | 5 | 8 |
	 *              \           /
	 *          | 5 | 5 |   | 6 | 7 | (x)
	 *
	 * In the examples, if we are looking for key "5", we may reach nodes
	 * marked with "(x)". In this case what we have do is to look at the
	 * left and see if there is "5" key there. If there is, we have to
	 * return it.
	 *
	 * Note, this whole situation is possible because we allow to have
	 * elements which are equivalent to the next key in the parent in the
	 * children of current znode. For example, this happens if we split a
	 * znode like this: | 3 | 5 | 5 | 6 | 7 |, which results in something
	 * like this:
	 *                      | 3 | 5 |
	 *                       /     \
	 *                | 3 | 5 |   | 5 | 6 | 7 |
	 *                              ^
	 * And this becomes what is at the first "picture" after key "5" marked
	 * with "^" is removed. What could be done is we could prohibit
	 * splitting in the middle of the colliding sequence. Also, when
	 * removing the leftmost key, we would have to correct the key of the
	 * parent node, which would introduce additional complications. Namely,
	 * if we changed the the leftmost key of the parent znode, the garbage
	 * collector would be unable to find it (GC is doing this when GC'ing
	 * indexing LEBs). Although we already have an additional RB-tree where
	 * we save such changed znodes (see 'ins_clr_old_idx_znode()') until
	 * after the commit. But anyway, this does not look easy to implement
	 * so we did not try this.
	 */
	err = tnc_prev(c, &znode, n);
	if (err == -ENOENT) {
		dbg_tnc("found 0, lvl %d, n -1", znode->level);
		*n = -1;
		return 0;
	}
	if (unlikely(err < 0))
		return err;
	if (keys_cmp(c, key, &znode->zbranch[*n].key)) {
		dbg_tnc("found 0, lvl %d, n -1", znode->level);
		*n = -1;
		return 0;
	}

	dbg_tnc("found 1, lvl %d, n %d", znode->level, *n);
	*zn = znode;
	return 1;
}

/**
 * lookup_level0_dirty - search for zero-level znode dirtying.
 * @c: UBIFS file-system description object
 * @key:  key to lookup
 * @zn: znode is returned here
 * @n: znode branch slot number is returned here
 *
 * This function looks up the TNC tree and search for zero-level znode which
 * refers key @key. The found zero-level znode is returned in @zn. There are 3
 * cases:
 *   o exact match, i.e. the found zero-level znode contains key @key, then %1
 *     is returned and slot number of the matched branch is stored in @n;
 *   o not exact match, which means that zero-level znode does not contain @key
 *     then %0 is returned and slot number of the closed branch is stored in
 *     @n;
 *   o @key is so small that it is even less than the lowest key of the
 *     leftmost zero-level node, then %0 is returned and %-1 is stored in @n.
 *
 * Additionally all znodes in the path from the root to the located zero-level
 * znode are marked as dirty.
 *
 * Note, when the TNC tree is traversed, some znodes may be absent, then this
 * function reads corresponding indexing nodes and inserts them to TNC. In
 * case of failure, a negative error code is returned.
 */
static int lookup_level0_dirty(struct ubifs_info *c, const union ubifs_key *key,
			       struct ubifs_znode **zn, int *n)
{
	int err, exact;
	struct ubifs_znode *znode;
	unsigned long time = get_seconds();

	dbg_tnc_key(c, key, "search and dirty key");

	znode = c->zroot.znode;
	if (unlikely(!znode)) {
		znode = load_znode(c, &c->zroot, NULL, 0);
		if (IS_ERR(znode))
			return PTR_ERR(znode);
	}

	znode = dirty_cow_znode(c, &c->zroot);
	if (IS_ERR(znode))
		return PTR_ERR(znode);

	znode->time = time;

	while (1) {
		struct ubifs_zbranch *zbr;

		/*
		 * The below is a debugging hack to make UBIFS eat RAM and
		 * cause fake memory pressure. It is compiled out if it is not
		 * enabled in kernel configuration.
		 */
		dbg_eat_memory();

		exact = ubifs_search_zbranch(c, znode, key, n);

		if (znode->level == 0)
			break;

		if (*n < 0)
			*n = 0;
		zbr = &znode->zbranch[*n];

		if (zbr->znode) {
			znode->time = time;
			znode = dirty_cow_znode(c, zbr);
			if (IS_ERR(znode))
				return PTR_ERR(znode);
			continue;
		}

		/* znode is not in TNC cache, load it from the media */
		znode = load_znode(c, zbr, znode, *n);
		if (IS_ERR(znode))
			return PTR_ERR(znode);
		znode = dirty_cow_znode(c, zbr);
		if (IS_ERR(znode))
			return PTR_ERR(znode);
	}

	*zn = znode;
	if (exact || !is_hash_key(c, key) || *n != -1) {
		dbg_tnc("found %d, lvl %d, n %d", exact, znode->level, *n);
		return exact;
	}

	/*
	 * See huge comment at 'lookup_level0_dirty()' what is the rest of the
	 * code.
	 */
	err = tnc_prev(c, &znode, n);
	if (err == -ENOENT) {
		*n = -1;
		dbg_tnc("found 0, lvl %d, n -1", znode->level);
		return 0;
	}
	if (unlikely(err < 0))
		return err;
	if (keys_cmp(c, key, &znode->zbranch[*n].key)) {
		*n = -1;
		dbg_tnc("found 0, lvl %d, n -1", znode->level);
		return 0;
	}

	if (znode->cnext || !ubifs_zn_dirty(znode)) {
		znode = dirty_cow_bottom_up(c, znode);
		if (IS_ERR(znode))
			return PTR_ERR(znode);
	}

	dbg_tnc("found 1, lvl %d, n %d", znode->level, *n);
	*zn = znode;
	return 1;
}

/**
 * ubifs_tnc_lookup - look up a file-system node.
 * @c: UBIFS file-system description object
 * @key: node key to lookup
 * @node: the node is returned here
 *
 * This function look up and reads node with key @key. The caller has to make
 * sure the @node buffer is large enough to fit the node. Returns zero in case
 * of success, %-ENOENT if the node was not found, and a negative error code in
 * case of failure.
 */
int ubifs_tnc_lookup(struct ubifs_info *c, const union ubifs_key *key,
		     void *node)
{
	int found, n, err;
	struct ubifs_znode *znode;
	struct ubifs_zbranch zbr, *zt;

	mutex_lock(&c->tnc_mutex);
	found = lookup_level0(c, key, &znode, &n);
	if (!found) {
		err = -ENOENT;
		goto out;
	} else if (found < 0) {
		err = found;
		goto out;
	}
	zt = &znode->zbranch[n];
	if (is_hash_key(c, key)) {
		/*
		 * In this case the leaf-node-cache gets used, so we pass the
		 * address of the zbranch and keep the mutex locked
		 */
		err = tnc_read_node(c, zt, node);
		goto out;
	}
	zbr = znode->zbranch[n];
	mutex_unlock(&c->tnc_mutex);

	err = tnc_read_node(c, &zbr, node);
	return err;

out:
	mutex_unlock(&c->tnc_mutex);
	return err;
}

/**
 * ubifs_tnc_locate - look up a file-system node and return it and its location.
 * @c: UBIFS file-system description object
 * @key: node key to lookup
 * @node: the node is returned here
 * @lnum: LEB number is returned here
 * @offs: offset is returned here
 *
 * This function is the same as 'ubifs_tnc_lookup()' but it returns the node
 * location also. See 'ubifs_tnc_lookup()'.
 */
int ubifs_tnc_locate(struct ubifs_info *c, const union ubifs_key *key,
		     void *node, int *lnum, int *offs)
{
	int found, n, err;
	struct ubifs_znode *znode;
	struct ubifs_zbranch zbr, *zt;

	mutex_lock(&c->tnc_mutex);
	found = lookup_level0(c, key, &znode, &n);
	if (!found) {
		err = -ENOENT;
		goto out;
	} else if (found < 0) {
		err = found;
		goto out;
	}
	zt = &znode->zbranch[n];
	if (is_hash_key(c, key)) {
		/*
		 * In this case the leaf-node-cache gets used, so we pass the
		 * address of the zbranch and keep the mutex locked
		 */
		*lnum = zt->lnum;
		*offs = zt->offs;
		err = tnc_read_node(c, zt, node);
		goto out;
	}
	zbr = znode->zbranch[n];
	mutex_unlock(&c->tnc_mutex);

	*lnum = zbr.lnum;
	*offs = zbr.offs;

	err = tnc_read_node(c, &zbr, node);
	return err;

out:
	mutex_unlock(&c->tnc_mutex);
	return err;
}

/**
 * do_lookup_nm- look up a "hashed" node.
 * directory entry file-system node.
 * @c: UBIFS file-system description object
 * @key: node key to lookup
 * @node: the node is returned here
 * @nm: node name
 *
 * This function look up and reads a node which contains name hash in the key.
 * Since the hash may have collisions, there may be many nodes with the same
 * key, so we have to sequentially look to all of them until the needed one is
 * found. This function returns zero in case of success, %-ENOENT if the node
 * was not found, and a negative error code in case of failure.
 */
static int do_lookup_nm(struct ubifs_info *c, const union ubifs_key *key,
			void *node, const struct qstr *nm)
{
	int found, n, err;
	struct ubifs_znode *znode;
	struct ubifs_zbranch zbr;

	dbg_tnc_key(c, key, "name '%.*s' key", nm->len, nm->name);
	mutex_lock(&c->tnc_mutex);
	found = lookup_level0(c, key, &znode, &n);
	if (!found) {
		err = -ENOENT;
		goto out_unlock;
	} else if (found < 0) {
		err = found;
		goto out_unlock;
	}

	ubifs_assert(n >= 0);

	err = resolve_collision(c, key, &znode, &n, nm);
	dbg_tnc("rc returned %d, znode %p, n %d", err, znode, n);
	if (unlikely(err < 0))
		goto out_unlock;
	if (err == 0) {
		err = -ENOENT;
		goto out_unlock;
	}

	zbr = znode->zbranch[n];
	mutex_unlock(&c->tnc_mutex);

	err = tnc_read_node(c, &zbr, node);
	return err;

out_unlock:
	mutex_unlock(&c->tnc_mutex);
	return err;
}

/**
 * ubifs_tnc_lookup_nm- look up a "hashed" node.
 * directory entry file-system node.
 * @c: UBIFS file-system description object
 * @key: node key to lookup
 * @node: the node is returned here
 * @nm: node name
 *
 * This function look up and reads a node which contains name hash in the key.
 * Since the hash may have collisions, there may be many nodes with the same
 * key, so we have to sequentially look to all of them until the needed one is
 * found. This function returns zero in case of success, %-ENOENT if the node
 * was not found, and a negative error code in case of failure.
 */
int ubifs_tnc_lookup_nm(struct ubifs_info *c, const union ubifs_key *key,
			void *node, const struct qstr *nm)
{
	int err, len;
	const struct ubifs_dent_node *dent = node;

	/*
	 * We assume that in most of the cases there are no name collisions and
	 * 'ubifs_tnc_lookup()' returns us the right direntry.
	 */
	err = ubifs_tnc_lookup(c, key, node);
	if (err)
		return err;

	len = le16_to_cpu(dent->nlen);
	if (nm->len == len && !memcmp(dent->name, nm->name, len))
		return 0;

	/*
	 * Unluckily, there are hash collisions and we have to iterate over
	 * them look at each direntry with colliding name hash sequentially.
	 */
	return do_lookup_nm(c, key, node, nm);
}

/**
 * correct_parent_keys - correct parent znodes' keys.
 * @c: UBIFS file-system description object
 * @znode: znode to correct parent znodes for
 *
 * This is a helper function for 'tnc_insert()'. When the key of the leftmost
 * zbranch changes, keys of parent znodes have to be corrected. This helper
 * function is called in such situations and corrects the keys if needed.
 */
static void correct_parent_keys(const struct ubifs_info *c,
				struct ubifs_znode *znode)
{
	union ubifs_key *key, *key1;

	ubifs_assert(znode->parent);
	ubifs_assert(znode->iip == 0);

	key = &znode->zbranch[0].key;
	key1 = &znode->parent->zbranch[0].key;

	while (keys_cmp(c, key, key1) < 0) {
		key_copy(c, key, key1);
		znode = znode->parent;
		if (!znode->parent || znode->iip)
			break;
		key1 = &znode->parent->zbranch[0].key;
	}
}

/**
 * insert_zbranch - insert a zbranch into a znode.
 * @znode: znode into which to insert
 * @zbr: zbranch to insert
 * @n: slot number to insert to
 *
 * This is a helper function for 'tnc_insert()'. UBIFS does not allow "gaps" in
 * znode's array of zbranches and keeps zbranches consolidated, so when a new
 * zbranch has to be inserted to the @znode->zbranches[]' array at the @n-th
 * slot, zbranches starting from @n have to be moved right.
 */
static void insert_zbranch(struct ubifs_znode *znode,
			   const struct ubifs_zbranch *zbr, int n)
{
	int i;

	ubifs_assert(ubifs_zn_dirty(znode));

	if (znode->level) {
		for (i = znode->child_cnt; i > n; i--) {
			znode->zbranch[i] = znode->zbranch[i - 1];
			if (znode->zbranch[i].znode)
				znode->zbranch[i].znode->iip = i;
		}
		if (zbr->znode)
			zbr->znode->iip = n;
	} else
		for (i = znode->child_cnt; i > n; i--)
			znode->zbranch[i] = znode->zbranch[i - 1];

	znode->zbranch[n] = *zbr;
	znode->child_cnt += 1;

	/*
	 * After inserting at slot zero, the lower bound of the key range of
	 * this znode may have changed. If this znode is subsequently split
	 * then the upper bound of the key range may change, and furthermore
	 * it could change to be lower than the original lower bound. If that
	 * happens, then it will no longer be possible to find this znode in the
	 * TNC using the key from the index node on flash. That is bad because
	 * if it is not found, we will assume it is obsolete and may overwrite
	 * it. Then if there is an unclean unmount, we will start using the
	 * old index which will be broken.
	 *
	 * So we first mark znodes that have insertions at slot zero, and then
	 * if they are split we add their lnum/offs to the old_idx tree.
	 */
	if (n == 0)
		znode->alt = 1;
}

/**
 * tnc_insert - insert a node into TNC.
 * @c: UBIFS file-system description object
 * @znode: znode to insert into
 * @zbr: branch to insert
 * @n: slot number to insert new zbranch to
 *
 * This function inserts a new node described by @zbr into znode @znode. If
 * znode does not have a free slot for new zbranch, it is split. Parent znodes
 * are splat as well if needed. Returns zero in case of success or a negative
 * error code in case of failure.
 */
static int tnc_insert(struct ubifs_info *c, struct ubifs_znode *znode,
		      struct ubifs_zbranch *zbr, int n)
{
	struct ubifs_znode *zn, *zi, *zp;
	int i, keep, move, appending = 0;
	union ubifs_key *key = &zbr->key;

	ubifs_assert(n >= 0 && n <= c->fanout);

	/* Implement naive insert for now */
again:
	zp = znode->parent;
	if (znode->child_cnt < c->fanout) {
		ubifs_assert(n != c->fanout);
		dbg_tnc_key(c, key, "inserted at %d level %d, key ", n,
			    znode->level);

		insert_zbranch(znode, zbr, n);

		/* Ensure parent's key is correct */
		if (n == 0 && zp && znode->iip == 0)
			correct_parent_keys(c, znode);

		return 0;
	}

	/*
	 * Unfortunately, @znode does not have more empty slots and we have to
	 * split it.
	 */
	dbg_tnc_key(c, key, "splitting level %d, key ", znode->level);

	if (znode->alt)
		/*
		 * We can no longer be sure of finding this znode by key, so we
		 * record it in the old_idx tree.
		 */
		ins_clr_old_idx_znode(c, znode);

	zn = kzalloc(c->max_znode_sz, GFP_NOFS);
	if (!zn)
		return -ENOMEM;
	zn->parent = zp;
	zn->level = znode->level;

	/* Decide where to split */
	if (znode->level == 0 && n == c->fanout &&
	    key_type(c, key) == UBIFS_DATA_KEY) {
		union ubifs_key *key1;

		/*
		 * If this is an inode which is being appended - do not split
		 * it because no other zbranches can be inserted between
		 * zbranches of consecutive data nodes anyway.
		 */
		key1 = &znode->zbranch[n - 1].key;
		if (key_ino(c, key1) == key_ino(c, key) &&
		    key_type(c, key1) == UBIFS_DATA_KEY &&
		    key_block(c, key1) == key_block(c, key) - 1)
			appending = 1;
	}

	if (appending) {
		keep = c->fanout;
		move = 0;
	} else {
		keep = (c->fanout + 1) / 2;
		move = c->fanout - keep;
	}

	/*
	 * Although we don't at present, we could look at the neighbors and see
	 * if we can move some zbranches there.
	 */

	if (n < keep) {
		/* Insert into existing znode */
		zi = znode;
		move += 1;
		keep -= 1;
	} else {
		/* Insert into new znode */
		zi = zn;
		n -= keep;
		/* Re-parent */
		if (zn->level != 0)
			zbr->znode->parent = zn;
	}

	set_bit(DIRTY_ZNODE, &zn->flags);
	atomic_long_inc(&c->dirty_zn_cnt);

	zn->child_cnt = move;
	znode->child_cnt = keep;

	dbg_tnc("moving %d, keeping %d", move, keep);

	/* Move zbranch */
	for (i = 0; i < move; i++) {
		zn->zbranch[i] = znode->zbranch[keep + i];
		/* Re-parent */
		if (zn->level != 0)
			if (zn->zbranch[i].znode) {
				zn->zbranch[i].znode->parent = zn;
				zn->zbranch[i].znode->iip = i;
			}
	}

	/* Insert new key and branch */
	dbg_tnc_key(c, key, "inserting at %d level %d, key ", n,
		    zn->level);

	insert_zbranch(zi, zbr, n);

	/* Insert new znode (produced by spitting) into the parent */
	if (zp) {
		i = n;
		/* Locate insertion point */
		n = znode->iip + 1;
		if (appending && n != c->fanout)
			appending = 0;

		if (i == 0 && zi == znode && znode->iip == 0)
			correct_parent_keys(c, znode);

		/* Tail recursion */
		zbr->key = zn->zbranch[0].key;
		zbr->znode = zn;
		zbr->lnum = 0;
		zbr->offs = 0;
		zbr->len = 0;
		znode = zp;

		goto again;
	}

	/* We have to split root znode */
	dbg_tnc("creating new zroot at level %d", znode->level + 1);

	zi = kzalloc(c->max_znode_sz, GFP_NOFS);
	if (!zi)
		return -ENOMEM;

	zi->child_cnt = 2;
	zi->level = znode->level + 1;

	set_bit(DIRTY_ZNODE, &zi->flags);
	atomic_long_inc(&c->dirty_zn_cnt);

	zi->zbranch[0].key = znode->zbranch[0].key;
	zi->zbranch[0].znode = znode;
	zi->zbranch[0].lnum = c->zroot.lnum;
	zi->zbranch[0].offs = c->zroot.offs;
	zi->zbranch[0].len = c->zroot.len;
	zi->zbranch[1].key = zn->zbranch[0].key;
	zi->zbranch[1].znode = zn;

	c->zroot.lnum = 0;
	c->zroot.offs = 0;
	c->zroot.len = 0;
	c->zroot.znode = zi;

	zn->parent = zi;
	zn->iip = 1;
	znode->parent = zi;
	znode->iip = 0;

	return 0;
}

/**
 * ubifs_tnc_add - add a node to TNC.
 * @c: UBIFS file-system description object
 * @key: key to add
 * @lnum: LEB number of node
 * @offs: node offset
 * @len: node length
 *
 * This function adds a node with key @key to TNC. The node may be new or it may
 * obsolete some existing one. Returns %0 on success or negative error code on
 * failure.
 */
int ubifs_tnc_add(struct ubifs_info *c, const union ubifs_key *key, int lnum,
		  int offs, int len)
{
	int found, n, err = 0;
	struct ubifs_znode *znode;

	mutex_lock(&c->tnc_mutex);
	dbg_tnc_key(c, key, "%d:%d, len %d, key", lnum, offs, len);
	found = lookup_level0_dirty(c, key, &znode, &n);
	if (!found) {
		struct ubifs_zbranch zbr;

		zbr.znode = NULL;
		zbr.lnum = lnum;
		zbr.offs = offs;
		zbr.len = len;
		key_copy(c, key, &zbr.key);
		err = tnc_insert(c, znode, &zbr, n + 1);
	} else if (found == 1) {
		struct ubifs_zbranch *zbr = &znode->zbranch[n];

		lnc_free(zbr);
		err = ubifs_add_dirt(c, zbr->lnum, zbr->len);
		zbr->lnum = lnum;
		zbr->offs = offs;
		zbr->len = len;
	} else
		err = found;
	if (!err)
		err = dbg_check_tnc(c, 0);
	mutex_unlock(&c->tnc_mutex);

	return err;
}

/**
 * ubifs_tnc_replace - replace a node in the TNC only if the old node is found.
 * @c: UBIFS file-system description object
 * @key: key to add
 * @old_lnum: LEB number of old node
 * @old_offs: old node offset
 * @lnum: LEB number of node
 * @offs: node offset
 * @len: node length
 *
 * This function replaces a node with key @key in the TNC only if the old node
 * is found.  This function is called by garbage collection when node are moved.
 * Returns %0 on success or negative error code on failure.
 */
int ubifs_tnc_replace(struct ubifs_info *c, const union ubifs_key *key,
		      int old_lnum, int old_offs, int lnum, int offs, int len)
{
	int found, n, err = 0;
	struct ubifs_znode *znode;

	mutex_lock(&c->tnc_mutex);
	dbg_tnc_key(c, key, "old LEB %d:%d, new LEB %d:%d, len %d, key",
		    old_lnum, old_offs, lnum, offs, len);
	found = lookup_level0_dirty(c, key, &znode, &n);
	if (found < 0) {
		err = found;
		goto out_unlock;
	}

	if (found == 1) {
		struct ubifs_zbranch *zbr = &znode->zbranch[n];

		found = 0;
		if (zbr->lnum == old_lnum && zbr->offs == old_offs) {
			lnc_free(zbr);
			err = ubifs_add_dirt(c, zbr->lnum, zbr->len);
			if (err)
				goto out_unlock;
			zbr->lnum = lnum;
			zbr->offs = offs;
			zbr->len = len;
			found = 1;
		} else if (is_hash_key(c, key)) {
			found = resolve_collision_directly(c, key, &znode, &n,
							   old_lnum, old_offs);
			dbg_tnc("rc returned %d, znode %p, n %d, LEB %d:%d",
				found, znode, n, old_lnum, old_offs);
			if (found < 0) {
				err = found;
				goto out_unlock;
			}

			if (found) {
				/* Ensure the znode is dirtied */
				if (znode->cnext || !ubifs_zn_dirty(znode)) {
					    znode = dirty_cow_bottom_up(c,
									znode);
					    if (IS_ERR(znode)) {
						    err = PTR_ERR(znode);
						    goto out_unlock;
					    }
				}
				zbr = &znode->zbranch[n];
				lnc_free(zbr);
				err = ubifs_add_dirt(c, zbr->lnum,
						     zbr->len);
				if (err)
					goto out_unlock;
				zbr->lnum = lnum;
				zbr->offs = offs;
				zbr->len = len;
			}
		}
	}

	if (!found)
		err = ubifs_add_dirt(c, lnum, len);

	if (!err)
		err = dbg_check_tnc(c, 0);

out_unlock:
	mutex_unlock(&c->tnc_mutex);
	return err;
}

/**
 * ubifs_tnc_add_nm - add a "hashed" node to TNC.
 * @c: UBIFS file-system description object
 * @key: key to add
 * @lnum: LEB number of node
 * @offs: node offset
 * @len: node length
 * @nm: node name
 *
 * This is the same as 'ubifs_tnc_add()' but it should be used with keys which
 * may have collisions, like directory entry keys.
 */
int ubifs_tnc_add_nm(struct ubifs_info *c, const union ubifs_key *key,
		     int lnum, int offs, int len, const struct qstr *nm)
{
	int found, n, err = 0;
	struct ubifs_znode *znode;

	mutex_lock(&c->tnc_mutex);
	dbg_tnc_key(c, key, "LEB %d:%d, name '%.*s', key",
		    lnum, offs, nm->len, nm->name);
	found = lookup_level0_dirty(c, key, &znode, &n);
	if (found < 0) {
		err = found;
		goto out_unlock;
	}

	if (found == 1) {
		if (c->replaying)
			found = fallible_resolve_collision(c, key, &znode, &n,
							   nm);
		else
			found = resolve_collision(c, key, &znode, &n, nm);
		dbg_tnc("rc returned %d, znode %p, n %d", found, znode, n);
		if (found < 0) {
			err = found;
			goto out_unlock;
		}

		/* Ensure the znode is dirtied */
		if (znode->cnext || !ubifs_zn_dirty(znode)) {
			    znode = dirty_cow_bottom_up(c, znode);
			    if (IS_ERR(znode)) {
				    err = PTR_ERR(znode);
				    goto out_unlock;
			    }
		}

		if (found == 1) {
			struct ubifs_zbranch *zbr = &znode->zbranch[n];

			lnc_free(zbr);
			err = ubifs_add_dirt(c, zbr->lnum, zbr->len);
			zbr->lnum = lnum;
			zbr->offs = offs;
			zbr->len = len;
			goto out_unlock;
		}
	}

	if (!found) {
		struct ubifs_zbranch zbr;

		zbr.znode = NULL;
		zbr.lnum = lnum;
		zbr.offs = offs;
		zbr.len = len;
		key_copy(c, key, &zbr.key);
		err = tnc_insert(c, znode, &zbr, n + 1);
	}

out_unlock:
	if (!err)
		err = dbg_check_tnc(c, 0);
	mutex_unlock(&c->tnc_mutex);
	return err;
}

/**
 * tnc_delete - delete a znode form TNC.
 * @c: UBIFS file-system description object
 * @znode: znode to delete from
 * @n: zbranch slot number to delete
 *
 * This function deletes a leaf node from @n-th slot of @znode. Returns zero in
 * case of success and a negative error code in case of failure.
 */
static int tnc_delete(struct ubifs_info *c, struct ubifs_znode *znode, int n)
{
	struct ubifs_zbranch *zbr;
	struct ubifs_znode *zp;
	int i, err;

	/* Delete without merge for now */
	ubifs_assert(znode->level == 0);
	ubifs_assert(n >= 0 && n < c->fanout);
	dbg_tnc_key(c, &znode->zbranch[n].key, "deleting");

	zbr = &znode->zbranch[n];
	lnc_free(zbr);

	err = ubifs_add_dirt(c, zbr->lnum, zbr->len);
	if (err) {
		dbg_dump_znode(c, znode);
		return err;
	}

	/* We do not "gap" zbranch slots */
	for (i = n; i < znode->child_cnt - 1; i++)
		znode->zbranch[i] = znode->zbranch[i + 1];
	znode->child_cnt -= 1;

	if (znode->child_cnt > 0)
		return 0;

	/*
	 * This was the last zbranch, we have to delete this znode from the
	 * parent.
	 */

	do {
		ubifs_assert(!test_bit(OBSOLETE_ZNODE, &znode->flags));
		ubifs_assert(ubifs_zn_dirty(znode));

		zp = znode->parent;
		n = znode->iip;

		atomic_long_dec(&c->dirty_zn_cnt);

		err = insert_old_idx_znode(c, znode);
		if (err)
			return err;

		if (znode->cnext) {
			set_bit(OBSOLETE_ZNODE, &znode->flags);
			atomic_long_inc(&c->clean_zn_cnt);
			atomic_long_inc(&ubifs_clean_zn_cnt);
		} else
			kfree(znode);
		znode = zp;
	} while (znode->child_cnt == 1); /* while removing last child */

	/* Remove from znode, entry n - 1 */
	znode->child_cnt -= 1;
	ubifs_assert(znode->level != 0);
	for (i = n; i < znode->child_cnt; i++) {
		znode->zbranch[i] = znode->zbranch[i + 1];
		if (znode->zbranch[i].znode)
			znode->zbranch[i].znode->iip = i;
	}

	/*
	 * If this is the root and it has only 1 child then
	 * collapse the tree.
	 */
	if (znode->parent == NULL) {
		while (znode->child_cnt == 1 && znode->level != 0) {
			zp = znode;
			zbr = &znode->zbranch[0];
			znode = get_znode(c, znode, 0);
			if (IS_ERR(znode))
				return PTR_ERR(znode);
			znode = dirty_cow_znode(c, zbr);
			if (IS_ERR(znode))
				return PTR_ERR(znode);
			znode->parent = NULL;
			znode->iip = 0;
			if (c->zroot.len) {
				err = insert_old_idx(c, c->zroot.lnum,
						     c->zroot.offs);
				if (err)
					return err;
			}
			c->zroot.lnum = zbr->lnum;
			c->zroot.offs = zbr->offs;
			c->zroot.len = zbr->len;
			c->zroot.znode = znode;
			ubifs_assert(!test_bit(OBSOLETE_ZNODE,
				     &zp->flags));
			ubifs_assert(test_bit(DIRTY_ZNODE, &zp->flags));
			atomic_long_dec(&c->dirty_zn_cnt);

			if (zp->cnext) {
				set_bit(OBSOLETE_ZNODE, &zp->flags);
				atomic_long_inc(&c->clean_zn_cnt);
				atomic_long_inc(&ubifs_clean_zn_cnt);
			} else
				kfree(zp);
		}
	}

	return 0;
}

/**
 * ubifs_tnc_remove - remove an index entry of a node.
 * @c: UBIFS file-system description object
 * @key: key of node
 *
 * Returns %0 on success or negative error code on failure.
 */
int ubifs_tnc_remove(struct ubifs_info *c, const union ubifs_key *key)
{
	int found, n, err = 0;
	struct ubifs_znode *znode;

	mutex_lock(&c->tnc_mutex);
	dbg_tnc_key(c, key, "key");
	found = lookup_level0_dirty(c, key, &znode, &n);
	if (found < 0) {
		err = found;
		goto out_unlock;
	}
	if (found == 1)
		err = tnc_delete(c, znode, n);
	if (!err)
		err = dbg_check_tnc(c, 0);

out_unlock:
	mutex_unlock(&c->tnc_mutex);
	return err;
}

/**
 * ubifs_tnc_remove_nm - remove an index entry for a "hashed" node.
 * @c: UBIFS file-system description object
 * @key: key of node
 * @nm: directory entry name
 *
 * Returns %0 on success or negative error code on failure.
 */
int ubifs_tnc_remove_nm(struct ubifs_info *c, const union ubifs_key *key,
			const struct qstr *nm)
{
	int n, err;
	struct ubifs_znode *znode;

	mutex_lock(&c->tnc_mutex);
	dbg_tnc_key(c, key, "%.*s, key", nm->len, nm->name);
	err = lookup_level0_dirty(c, key, &znode, &n);
	if (err < 0)
		goto out_unlock;

	if (err) {
		if (c->replaying)
			err = fallible_resolve_collision(c, key, &znode, &n,
							 nm);
		else
			err = resolve_collision(c, key, &znode, &n, nm);
		dbg_tnc("rc returned %d, znode %p, n %d", err, znode, n);
		if (err < 0)
			goto out_unlock;
		if (err) {
			/* Ensure the znode is dirtied */
			if (znode->cnext || !ubifs_zn_dirty(znode)) {
				    znode = dirty_cow_bottom_up(c, znode);
				    if (IS_ERR(znode)) {
					    err = PTR_ERR(znode);
					    goto out_unlock;
				    }
			}
			err = tnc_delete(c, znode, n);
		}
	}

out_unlock:
	if (!err)
		err = dbg_check_tnc(c, 0);
	mutex_unlock(&c->tnc_mutex);
	return err;
}

/**
 * key_in_range - determine if a key falls within a range of keys.
 * @c: UBIFS file-system description object
 * @key: key to check
 * @from_key: lowest key in range
 * @to_key: highest key in range
 *
 * This function returns %1 if the key is in range and %0 otherwise.
 */
static int key_in_range(struct ubifs_info *c, union ubifs_key *key,
			union ubifs_key *from_key, union ubifs_key *to_key)
{
	if (keys_cmp(c, key, from_key) < 0)
		return 0;
	if (keys_cmp(c, key, to_key) > 0)
		return 0;
	return 1;
}

/**
 * ubifs_tnc_remove_range - remove index entries in range.
 * @c: UBIFS file-system description object
 * @from_key: lowest key to remove
 * @to_key: highest key to remove
 *
 * This function removes index entries starting at @from_key and ending at
 * @to_key.  This function returns zero in case of success and a negative error
 * code in case of failure.
 */
int ubifs_tnc_remove_range(struct ubifs_info *c, union ubifs_key *from_key,
			   union ubifs_key *to_key)
{
	int i, n, k, err = 0;
	struct ubifs_znode *znode;
	union ubifs_key *key;

	mutex_lock(&c->tnc_mutex);
	while (1) {
		/* Find first level 0 znode that contains keys to remove */
		err = lookup_level0(c, from_key, &znode, &n);
		if (err < 0)
			goto out_unlock;

		if (err)
			key = from_key;
		else {
			err = tnc_next(c, &znode, &n);
			if (err == -ENOENT) {
				err = 0;
				goto out_unlock;
			}
			if (err < 0)
				goto out_unlock;
			key = &znode->zbranch[n].key;
			if (!key_in_range(c, key, from_key, to_key)) {
				err = 0;
				goto out_unlock;
			}
		}

		/* Ensure the znode is dirtied */
		if (znode->cnext || !ubifs_zn_dirty(znode)) {
			    znode = dirty_cow_bottom_up(c, znode);
			    if (IS_ERR(znode)) {
				    err = PTR_ERR(znode);
				    goto out_unlock;
			    }
		}

		/* Remove all keys in range except the first */
		for (i = n + 1, k = 0; i < znode->child_cnt; i++, k++) {
			key = &znode->zbranch[i].key;
			if (!key_in_range(c, key, from_key, to_key))
				break;
			lnc_free(&znode->zbranch[i]);
			err = ubifs_add_dirt(c, znode->zbranch[i].lnum,
					     znode->zbranch[i].len);
			if (err) {
				dbg_dump_znode(c, znode);
				goto out_unlock;
			}
			dbg_tnc_key(c, key, "removing");
		}
		if (k) {
			for (i = n + 1 + k; i < znode->child_cnt; i++)
				znode->zbranch[i - k] = znode->zbranch[i];
			znode->child_cnt -= k;
		}

		/* Now delete the first */
		err = tnc_delete(c, znode, n);
		if (err)
			goto out_unlock;
	}

out_unlock:
	if (!err)
		err = dbg_check_tnc(c, 0);
	mutex_unlock(&c->tnc_mutex);
	return err;
}

/**
 * ubifs_tnc_remove_ino - remove an inode from TNC.
 * @c: UBIFS file-system description object
 * @inum: inode number to remove
 *
 * This function remove inode @inum and all the extended attributes associated
 * with the anode from TNC and returns zero in case of success or a negative
 * error code in case of failure.
 */
int ubifs_tnc_remove_ino(struct ubifs_info *c, ino_t inum)
{
	union ubifs_key key1, key2;
	struct ubifs_dent_node *xent, *pxent = NULL;
	struct qstr nm = { .name = NULL };

	dbg_tnc("ino %lu", inum);

	/*
	 * Walk all extended attribute entries and remove them together with
	 * corresponding extended attribute inodes.
	 */
	lowest_xent_key(c, &key1, inum);
	while (1) {
		ino_t xattr_inum;
		int err;

		xent = ubifs_tnc_next_ent(c, &key1, &nm);
		if (IS_ERR(xent)) {
			err = PTR_ERR(xent);
			if (err == -ENOENT)
				break;
			return err;
		}

		xattr_inum = le64_to_cpu(xent->inum);
		dbg_tnc("xent '%s', ino %lu", xent->name, xattr_inum);

		nm.name = xent->name;
		nm.len = le16_to_cpu(xent->nlen);
		err = ubifs_tnc_remove_nm(c, &key1, &nm);
		if (err) {
			kfree(xent);
			return err;
		}

		lowest_ino_key(c, &key1, xattr_inum);
		highest_ino_key(c, &key2, xattr_inum);
		err = ubifs_tnc_remove_range(c, &key1, &key2);
		if (err) {
			kfree(xent);
			return err;
		}

		kfree(pxent);
		pxent = xent;
		key_read(c, &xent->key, &key1);
	}

	kfree(pxent);
	lowest_ino_key(c, &key1, inum);
	highest_ino_key(c, &key2, inum);

	return ubifs_tnc_remove_range(c, &key1, &key2);
}

/**
 * ubifs_tnc_next_ent - walk directory or extended attribute entries.
 * @c: UBIFS file-system description object
 * @key: key of last entry
 * @nm: name of last entry found or %NULL
 *
 * This function finds and reads the next directory or extended attribute entry
 * after the given key (@key) if there is one. @name is used to resolve
 * collisions. If the fist entry has to be found, @key has to contain the
 * lowest possible key value for this inode and @name has to be %NULL.
 *
 * This function returns the found directory or extended attribute entry node
 * in case of success, %-ENOENT is returned if no entry is found, or a negative
 * error code in case of failure.
 */
struct ubifs_dent_node *ubifs_tnc_next_ent(struct ubifs_info *c,
					   union ubifs_key *key,
					   const struct qstr *nm)
{
	int n, err, type = key_type(c, key), dlen = 0;
	struct ubifs_znode *znode;
	struct ubifs_dent_node *dent = NULL;
	struct ubifs_zbranch *zbr;
	union ubifs_key *dkey;

	dbg_tnc_key(c, key, "%s", nm->name ? (char *)nm->name : "(lowest)");
	ubifs_assert(type == UBIFS_DENT_KEY || type == UBIFS_XENT_KEY);

	mutex_lock(&c->tnc_mutex);
	err = lookup_level0(c, key, &znode, &n);
	if (unlikely(err < 0))
		goto out_free;

	/* Handle collisions */
	if (err) {
		err = resolve_collision(c, key, &znode, &n, nm);
		dbg_tnc("rc returned %d, znode %p, n %d", err, znode, n);
		if (unlikely(err < 0))
			goto out_free;
	}

again:
	/* Now find next entry */
	err = tnc_next(c, &znode, &n);
	if (err)
		goto out_free;

	dkey = &znode->zbranch[n].key;
	zbr = &znode->zbranch[n];

	if (key_ino(c, dkey) != key_ino(c, key) ||
	    key_type(c, dkey) != type) {
		err = -ENOENT;
		goto out_free;
	}

	if (!dent || dlen < zbr->len) {
		kfree(dent);
		dlen = zbr->len;
		dent = kmalloc(dlen, GFP_NOFS);
		if (!dent) {
			err = -ENOMEM;
			goto out_free;
		}
	}

	err = tnc_read_node(c, zbr, dent);
	if (unlikely(err))
		goto out_free;

	if (dent->inum == 0)
		/* This is a deletion entry, skip it */
		goto again;

	mutex_unlock(&c->tnc_mutex);
	return dent;

out_free:
	kfree(dent);
	mutex_unlock(&c->tnc_mutex);
	return ERR_PTR(err);
}

/**
 * tnc_destroy_cnext - destroy left-over obsolete znodes from a failed commit.
 * @c: UBIFS file-system description object
 *
 * Destroy left-over obsolete znodes from a failed commit.
 */
static void tnc_destroy_cnext(struct ubifs_info *c)
{
	struct ubifs_znode *cnext;

	if (!c->cnext)
		return;
	ubifs_assert(c->cmt_state == COMMIT_BROKEN);
	cnext = c->cnext;
	do {
		struct ubifs_znode *znode = cnext;

		cnext = cnext->cnext;
		if (test_bit(OBSOLETE_ZNODE, &znode->flags))
			kfree(znode);
	} while (cnext != NULL && cnext != c->cnext);
}

/**
 * ubifs_tnc_close - close TNC subsystem and free all related resources.
 * @c: UBIFS file-system description object
 */
void ubifs_tnc_close(struct ubifs_info *c)
{
	long clean_freed;

	tnc_destroy_cnext(c);
	if (c->zroot.znode) {
		clean_freed = ubifs_destroy_tnc_subtree(c->zroot.znode);
		atomic_long_sub(clean_freed, &ubifs_clean_zn_cnt);
	}
	kfree(c->cbuf);
	kfree(c->gap_lebs);
	kfree(c->ilebs);
	destroy_old_idx(c);
}

/**
 * left_znode - get the znode to the left.
 * @c: UBIFS file-system description object
 * @znode: znode
 *
 * This function returns a pointer to the znode to the left of @znode or NULL if
 * there is not one. A negative error code is returned on failure.
 */
static struct ubifs_znode *left_znode(struct ubifs_info *c,
				      struct ubifs_znode *znode)
{
	int level = znode->level;

	while (1) {
		int n = znode->iip - 1;

		/* Go up until we can go left */
		znode = znode->parent;
		if (!znode)
			return NULL;
		if (n >= 0) {
			/* Now go down the rightmost branch to 'level' */
			znode = get_znode(c, znode, n);
			if (IS_ERR(znode))
				return znode;
			while (znode->level != level) {
				n = znode->child_cnt - 1;
				znode = get_znode(c, znode, n);
				if (IS_ERR(znode))
					return znode;
			}
			break;
		}
	}
	return znode;
}

/**
 * right_znode - get the znode to the right.
 * @c: UBIFS file-system description object
 * @znode: znode
 *
 * This function returns a pointer to the znode to the right of @znode or NULL
 * if there is not one. A negative error code is returned on failure.
 */
static struct ubifs_znode *right_znode(struct ubifs_info *c,
				       struct ubifs_znode *znode)
{
	int level = znode->level;

	while (1) {
		int n = znode->iip + 1;

		/* Go up until we can go right */
		znode = znode->parent;
		if (!znode)
			return NULL;
		if (n < znode->child_cnt) {
			/* Now go down the leftmost branch to 'level' */
			znode = get_znode(c, znode, n);
			if (IS_ERR(znode))
				return znode;
			while (znode->level != level) {
				znode = get_znode(c, znode, 0);
				if (IS_ERR(znode))
					return znode;
			}
			break;
		}
	}
	return znode;
}

/**
 * lookup_znode - find a particular indexing node from TNC.
 * @c: UBIFS file-system description object
 * @key: index node key to lookup
 * @level: index node level
 * @lnum: index node LEB number
 * @offs: index node offset
 *
 * This function searches an indexing node by its first key @key and its
 * address @lnum:@offs. It looks up the indexing tree by pulling all indexing
 * nodes it traverses to TNC. This function is called fro indexing nodes which
 * were found on the media by scanning, for example when garbage-collecting or
 * when doing in-the-gaps commit. This means that the indexing node which is
 * looked for does not have to have exactly the same leftmost key @key, because
 * the leftmost key may have been changed, in which case TNC will contain a
 * dirty znode which still refers the same @lnum:@offs. This function is clever
 * enough to recognize such indexing nodes.
 *
 * Note, if a znode was deleted or changed too much, then this function will
 * not find it. For situations like this UBIFS has the old index RB-tree
 * (indexed by @lnum:@offs).
 *
 * This function returns a pointer to the znode found or %NULL if it is not
 * found. A negative error code is returned on failure.
 */
static struct ubifs_znode *lookup_znode(struct ubifs_info *c,
					union ubifs_key *key, int level,
					int lnum, int offs)
{
	struct ubifs_znode *znode, *zn;
	int n, nn;

	/*
	 * The arguments have probably been read off flash, so don't assume
	 * they are valid.
	 */
	if (level < 0)
		return ERR_PTR(-EINVAL);

	/* Get the root znode */
	znode = c->zroot.znode;
	if (!znode) {
		znode = load_znode(c, &c->zroot, NULL, 0);
		if (IS_ERR(znode))
			return znode;
	}
	/* Check if it is the one we are looking for */
	if (c->zroot.lnum == lnum && c->zroot.offs == offs)
		return znode;
	/* Descend to the parent level i.e. (level + 1) */
	if (level >= znode->level)
		return NULL;
	while (1) {
		ubifs_search_zbranch(c, znode, key, &n);
		if (n < 0) {
			/*
			 * We reached a znode where the leftmost key is greater
			 * than the key we are searching for. This is the same
			 * situation as the one described in a huge comment at
			 * the end of the 'lookup_level0()' function. And for
			 * exactly the same reasons we have to try to look left
			 * before giving up.
			 */
			znode = left_znode(c, znode);
			if (!znode)
				return NULL;
			if (IS_ERR(znode))
				return znode;
			ubifs_search_zbranch(c, znode, key, &n);
			ubifs_assert(n >= 0);
		}
		if (znode->level == level + 1)
			break;
		znode = get_znode(c, znode, n);
		if (IS_ERR(znode))
			return znode;
	}
	/* Check if the child is the one we are looking for */
	if (znode->zbranch[n].lnum == lnum && znode->zbranch[n].offs == offs)
		return get_znode(c, znode, n);
	/* If the key is unique, there is nowhere else to look */
	if (!is_hash_key(c, key))
		return NULL;
	/*
	 * The key is not unique and so may be also in the znodes to either
	 * side.
	 */
	zn = znode;
	nn = n;
	/* Look left */
	while (1) {
		/* Move one branch to the left */
		if (n)
			n -= 1;
		else {
			znode = left_znode(c, znode);
			if (znode == NULL)
				break;
			if (IS_ERR(znode))
				return znode;
			n = znode->child_cnt - 1;
		}
		/* Check it */
		if (znode->zbranch[n].lnum == lnum &&
		    znode->zbranch[n].offs == offs)
			return get_znode(c, znode, n);
		/* Stop if the key is less than the one we are looking for */
		if (keys_cmp(c, &znode->zbranch[n].key, key) < 0)
			break;
	}
	/* Back to the middle */
	znode = zn;
	n = nn;
	/* Look right */
	while (1) {
		/* Move one branch to the right */
		if (++n >= znode->child_cnt) {
			znode = right_znode(c, znode);
			if (znode == NULL)
				break;
			if (IS_ERR(znode))
				return znode;
			n = 0;
		}
		/* Check it */
		if (znode->zbranch[n].lnum == lnum &&
		    znode->zbranch[n].offs == offs)
			return get_znode(c, znode, n);
		/* Stop if the key is greater than the one we are looking for */
		if (keys_cmp(c, &znode->zbranch[n].key, key) > 0)
			break;
	}
	return NULL;
}

/**
 * is_idx_node_in_tnc - determine if an index node is in the TNC.
 * @c: UBIFS file-system description object
 * @key: key of index node
 * @level: index node level
 * @lnum: LEB number of index node
 * @offs: offset of index node
 *
 * This function returns %0 if the index node is not referred to in the TNC, %1
 * if the index node is referred to in the TNC and the corresponding znode is
 * dirty, %2 if an index node is referred to in the TNC and the corresponding
 * znode is clean, and a negative error code in case of failure.
 *
 * Note, the @key argument has to be the key of the first child. Also note,
 * this function relies on the fact that 0:0 is never a valid LEB number and
 * offset for a main-area node.
 */
int is_idx_node_in_tnc(struct ubifs_info *c, union ubifs_key *key, int level,
		       int lnum, int offs)
{
	struct ubifs_znode *znode;

	znode = lookup_znode(c, key, level, lnum, offs);
	if (znode == NULL)
		return 0;
	if (IS_ERR(znode))
		return PTR_ERR(znode);

	return ubifs_zn_dirty(znode) ? 1 : 2;
}

/**
 * is_leaf_node_in_tnc - determine if a non-indexing not is in the TNC.
 * @c: UBIFS file-system description object
 * @key: node key
 * @lnum: node LEB number
 * @offs: node offset
 *
 * This function returns %1 if the node is referred to in the TNC, %0 if it is
 * not, and a negative error code in case of failure.
 *
 * Note, this function relies on the fact that 0:0 is never a valid LEB number
 * and offset for a main-area node.
 */
static int is_leaf_node_in_tnc(struct ubifs_info *c, union ubifs_key *key,
			       int lnum, int offs)
{
	struct ubifs_zbranch *zbr;
	struct ubifs_znode *znode, *zn;
	int n, found, err, nn;
	const int unique = !is_hash_key(c, key);

	found = lookup_level0(c, key, &znode, &n);
	if (found < 0)
		return found; /* Error code */
	if (!found)
		return 0;
	zbr = &znode->zbranch[n];
	if (lnum == zbr->lnum && offs == zbr->offs)
		return 1; /* Found it */
	if (unique)
		return 0;
	/*
	 * Because the key is not unique, we have to look left
	 * and right as well
	 */
	zn = znode;
	nn = n;
	/* Look left */
	while (1) {
		err = tnc_prev(c, &znode, &n);
		if (err == -ENOENT)
			break;
		if (err)
			return err;
		if (keys_cmp(c, key, &znode->zbranch[n].key))
			break;
		zbr = &znode->zbranch[n];
		if (lnum == zbr->lnum && offs == zbr->offs)
			return 1; /* Found it */
	}
	/* Look right */
	znode = zn;
	n = nn;
	while (1) {
		err = tnc_next(c, &znode, &n);
		if (err) {
			if (err == -ENOENT)
				return 0;
			return err;
		}
		if (keys_cmp(c, key, &znode->zbranch[n].key))
			break;
		zbr = &znode->zbranch[n];
		if (lnum == zbr->lnum && offs == zbr->offs)
			return 1; /* Found it */
	}
	return 0;
}

/**
 * ubifs_tnc_has_node - determine whether a node is in the TNC.
 * @c: UBIFS file-system description object
 * @key: node key
 * @level: index node level (if it is an index node)
 * @lnum: node LEB number
 * @offs: node offset
 * @is_idx: non-zero if the node is an index node
 *
 * This function returns %1 if the node is in the TNC, %0 if it is not, and a
 * negative error code in case of failure. For index nodes, @key has to be the
 * key of the first child. An index node is considered to be in the TNC only if
 * the corresponding znode is clean or has not been loaded.
 */
int ubifs_tnc_has_node(struct ubifs_info *c, union ubifs_key *key, int level,
		       int lnum, int offs, int is_idx)
{
	int err;

	mutex_lock(&c->tnc_mutex);
	if (is_idx) {
		err = is_idx_node_in_tnc(c, key, level, lnum, offs);
		if (err < 0)
			goto out_unlock;
		if (err == 1)
			/* The index node was found but it was dirty */
			err = 0;
		else if (err == 2)
			/* The index node was found and it was clean */
			err = 1;
		else
			BUG_ON(err != 0);
	} else
		err = is_leaf_node_in_tnc(c, key, lnum, offs);

out_unlock:
	mutex_unlock(&c->tnc_mutex);
	return err;
}

/**
 * ubifs_dirty_idx_node - dirty an index node.
 * @c: UBIFS file-system description object
 * @key: index node key
 * @level: index node level
 * @lnum: index node LEB number
 * @offs: index node offset
 *
 * This function loads and dirties an index node so that it can be garbage
 * collected. The @key argument has to be the key of the first child. This
 * function relies on the fact that 0:0 is never a valid LEB number and offset
 * for a main-area node. Returns %0 on success and a negative error code on
 * failure.
 */
int ubifs_dirty_idx_node(struct ubifs_info *c, union ubifs_key *key, int level,
			 int lnum, int offs)
{
	struct ubifs_znode *znode;
	int err = 0;

	mutex_lock(&c->tnc_mutex);
	znode = lookup_znode(c, key, level, lnum, offs);
	if (!znode)
		goto out_unlock;
	if (IS_ERR(znode)) {
		err = PTR_ERR(znode);
		goto out_unlock;
	}
	znode = dirty_cow_bottom_up(c, znode);
	if (IS_ERR(znode)) {
		err = PTR_ERR(znode);
		goto out_unlock;
	}

out_unlock:
	mutex_unlock(&c->tnc_mutex);
	return err;
}

#ifdef CONFIG_UBIFS_FS_DEBUG

/**
 * dbg_walk_sub_tree - walk index subtree.
 * @c: UBIFS file-system description object
 * @znode: root znode of the subtree to walk
 * @leaf_cb: called for each leaf node
 * @znode_cb: called for each indexing node
 * @priv: private date which is passed to callbacks
 *
 * This is a helper function which recursively walks the UBIFS index, reading
 * each indexing node from the media if needed. Returns zero in case of success
 * and a negative error code in case of failure.
 */
static int dbg_walk_sub_tree(struct ubifs_info *c, struct ubifs_znode *znode,
			     dbg_leaf_callback leaf_cb,
			     dbg_znode_callback znode_cb, void *priv)
{
	int n, err;

	cond_resched();

	if (znode_cb) {
		err = znode_cb(c, znode, priv);
		if (err)
			return err;
	}

	if (znode->level == 0) {
		if (!leaf_cb)
			return 0;

		for (n = 0; n < znode->child_cnt; n++) {
			struct ubifs_zbranch *zbr = &znode->zbranch[n];

			err = leaf_cb(c, zbr, priv);
			if (err)
				return err;
		}
	} else
		for (n = 0; n < znode->child_cnt; n++) {
			struct ubifs_znode *zn;

			zn = get_znode(c, znode, n);
			if (IS_ERR(zn))
				return PTR_ERR(zn);
			err = dbg_walk_sub_tree(c, zn, leaf_cb, znode_cb, priv);
			if (err)
				return err;
		}

	return 0;
}

/**
 * dbg_walk_index - walk the on-flash index.
 * @c: UBIFS file-system description object
 * @leaf_cb: called for each leaf node
 * @znode_cb: called for each indexing node
 * @priv: private date which is passed to callbacks
 *
 * This function walks the UBIFS index and calls the @leaf_cb for each leaf
 * node and @znode_cb for each indexing node. Returns zero in case of success
 * and a negative error code in case of failure.
 *
 * Because 'dbg_walk_sub_tree()' is recursive, it runs the risk of exceeding the
 * stack space.
 *
 * It would be better if this function removed every znode it pulled to into
 * the TNC, so that the behavior more closely matched the non-debugging
 * behavior.
 */
int dbg_walk_index(struct ubifs_info *c, dbg_leaf_callback leaf_cb,
		   dbg_znode_callback znode_cb, void *priv)
{
	int err = 0;

	mutex_lock(&c->tnc_mutex);
	if (!c->zroot.znode) {
		c->zroot.znode = load_znode(c, &c->zroot, NULL, 0);
		if (IS_ERR(c->zroot.znode)) {
			err = PTR_ERR(c->zroot.znode);
			c->zroot.znode = NULL;
			goto out_unlock;
		}
	}

	err = dbg_walk_sub_tree(c, c->zroot.znode, leaf_cb, znode_cb, priv);

out_unlock:
	mutex_unlock(&c->tnc_mutex);
	return err;
}

int dbg_read_leaf_nolock(struct ubifs_info *c, struct ubifs_zbranch *zbr,
			 void *node)
{
	ubifs_assert(mutex_is_locked(&c->tnc_mutex));
	return tnc_read_node(c, zbr, node);
}

#endif /* CONFIG_UBIFS_FS_DEBUG */
