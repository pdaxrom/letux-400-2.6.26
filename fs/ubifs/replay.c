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
 * This file contains journal replay code. It runs when the file-system is being
 * mounted and requires no locking.
 *
 * The larger is the journal, the longer it takes to scan it, so the longer it
 * takes to mount UBIFS. This is why the journal has limited size which may be
 * changed depending on the system requirements. But a larger journal gives
 * faster I/O speed because it writes the index less frequently. So this is a
 * trade-off. Also, the journal is indexed by the in-memory index (TNC), so the
 * larger is the journal, the more memory its index may consume.
 */

#include "ubifs.h"

/*
 * Replay flags.
 *
 * REPLAY_DELETION: node was deleted
 * REPLAY_REF: node is a reference node
 */
enum {
	REPLAY_DELETION = 1,
	REPLAY_REF = 2,
};

/**
 * struct replay_entry - replay tree entry.
 * @lnum: logical eraseblock number of the node
 * @offs: node offset
 * @len: node length
 * @sqnum: node sequence number
 * @flags: replay flags
 * @rb: links the replay tree
 * @key: node key
 * @nm: directory entry name
 * @old_size: truncation old size
 * @new_size: truncation new size
 * @free: amount of free space in a bud
 * @dirty: amount of dirty space in a bud from padding and deletion nodes
 *
 * UBIFS journal replay must compare node sequence numbers, which means it must
 * build a tree of node information to insert into the TNC.
 */
struct replay_entry {
	int lnum;
	int offs;
	int len;
	unsigned long long sqnum;
	int flags;
	struct rb_node rb;
	union ubifs_key key;
	union {
		struct qstr nm;
		struct {
			loff_t old_size;
			loff_t new_size;
		};
		struct {
			int free;
			int dirty;
		};
	};
};

/**
 * struct bud_entry - entry in the list of buds to replay.
 * @list: next bud in the list
 * @bud: bud description object
 * @free: free bytes in the bud
 * @sqnum: reference node sequence number
 */
struct bud_entry {
	struct list_head list;
	struct ubifs_bud *bud;
	int free;
	unsigned long long sqnum;
};

/**
 * set_bud_lprops - set free and dirty space used by a bud.
 * @c: UBIFS file-system description object
 * @r: replay entry of bud
 */
static int set_bud_lprops(struct ubifs_info *c, struct replay_entry *r)
{
	const struct ubifs_lprops *lp;
	int err = 0, dirty;

	ubifs_get_lprops(c);

	lp = ubifs_lpt_lookup_dirty(c, r->lnum);
	if (IS_ERR(lp)) {
		err = PTR_ERR(lp);
		goto out;
	}

	dirty = lp->dirty;
	if (r->offs == 0 && (lp->free != c->leb_size || lp->dirty != 0)) {
		dbg_mnt("bud LEB %d was GC'd (%d free, %d dirty)", r->lnum,
			lp->free, lp->dirty);
		dbg_gc("bud LEB %d was GC'd (%d free, %d dirty)", r->lnum,
			lp->free, lp->dirty);
		dirty -= c->leb_size - lp->free;
		if (dirty != 0)
			dbg_msg("LEB %d lp: %d free %d dirty "
				"replay: %d free %d dirty", r->lnum, lp->free,
				lp->dirty, r->free, r->dirty);
	}
	lp = ubifs_change_lp(c, lp, r->free, dirty + r->dirty,
			     lp->flags | LPROPS_TAKEN, 0);
	if (IS_ERR(lp)) {
		err = PTR_ERR(lp);
		goto out;
	}
out:
	ubifs_release_lprops(c);
	return err;
}

/**
 * trun_remove_range - apply a replay entry for a truncation to the TNC.
 * @c: UBIFS file-system description object
 * @r: replay entry of truncation
 */
static int trun_remove_range(struct ubifs_info *c, struct replay_entry *r)
{
	unsigned min_blk, max_blk;
	union ubifs_key min_key, max_key;
	ino_t ino;

	min_blk = r->new_size / UBIFS_BLOCK_SIZE;
	if (r->new_size & (UBIFS_BLOCK_SIZE - 1))
		min_blk += 1;

	max_blk = r->old_size / UBIFS_BLOCK_SIZE;
	if ((r->old_size & (UBIFS_BLOCK_SIZE - 1)) == 0)
		max_blk -= 1;

	ino = key_ino(c, &r->key);

	data_key_init(c, &min_key, ino, min_blk);
	data_key_init(c, &max_key, ino, max_blk);

	return ubifs_tnc_remove_range(c, &min_key, &max_key);
}

/**
 * apply_replay_entry - apply a replay entry to the TNC.
 * @c: UBIFS file-system description object
 * @r: replay entry to apply
 *
 * Apply a replay entry to the TNC.
 */
static int apply_replay_entry(struct ubifs_info *c, struct replay_entry *r)
{
	int err, deletion = ((r->flags & REPLAY_DELETION) != 0);

	dbg_mnt_key(c, &r->key, "LEB %d:%d len %d flgs %d sqnum %llu", r->lnum,
		    r->offs, r->len, r->flags, r->sqnum);
	/*
	 * Set c->replay_sqnum to help 'fallible_read_node()' identify dangling
	 * branches.
	 */
	c->replay_sqnum = r->sqnum;
	if (r->flags & REPLAY_REF)
		err = set_bud_lprops(c, r);
	else if (is_hash_key(c, &r->key)) {
		if (deletion)
			err = ubifs_tnc_remove_nm(c, &r->key, &r->nm);
		else
			err = ubifs_tnc_add_nm(c, &r->key, r->lnum, r->offs,
					       r->len, &r->nm);
	} else {
		if (deletion)
			switch (key_type(c, &r->key)) {
			case UBIFS_INO_KEY:
			{
				ino_t inum = key_ino(c, &r->key);

				err = ubifs_tnc_remove_ino(c, inum);
				break;
			}
			case UBIFS_TRUN_KEY:
				err = trun_remove_range(c, r);
				break;
			default:
				err = ubifs_tnc_remove(c, &r->key);
				break;
			}
		else
			err = ubifs_tnc_add(c, &r->key, r->lnum, r->offs,
					    r->len);
		if (err)
			return err;

		if (c->need_recovery)
			err = ubifs_recover_size_accum(c, &r->key, deletion,
						       r->new_size);
	}

	return err;
}

/**
 * destroy_replay_tree - destroy the replay.
 * @c: UBIFS file-system description object
 *
 * Destroy the replay tree.
 */
static void destroy_replay_tree(struct ubifs_info *c)
{
	struct rb_node *this = c->replay_tree.rb_node;
	struct replay_entry *r;

	while (this) {
		if (this->rb_left) {
			this = this->rb_left;
			continue;
		} else if (this->rb_right) {
			this = this->rb_right;
			continue;
		}
		r = rb_entry(this, struct replay_entry, rb);
		this = rb_parent(this);
		if (this) {
			if (this->rb_left == &r->rb)
				this->rb_left = NULL;
			else
				this->rb_right = NULL;
		}
		if (key_type(c, &r->key) == UBIFS_DENT_KEY)
			kfree(r->nm.name);
		kfree(r);
	}
	c->replay_tree = RB_ROOT;
}

/**
 * apply_replay_tree - apply the replay tree to the TNC.
 * @c: UBIFS file-system description object
 *
 * Apply the replay tree.
 * Returns zero in case of success and a negative error code in case of
 * failure.
 */
static int apply_replay_tree(struct ubifs_info *c)
{
	struct rb_node *this = rb_first(&c->replay_tree);

	while (this) {
		struct replay_entry *r;
		int err;

		cond_resched();

		r = rb_entry(this, struct replay_entry, rb);
		err = apply_replay_entry(c, r);
		if (err)
			return err;
		this = rb_next(this);
	}
	return 0;
}

/**
 * insert_node - insert a node to the replay tree.
 * @c: UBIFS file-system description object
 * @lnum: node logical eraseblock number
 * @offs: node offset
 * @len: node length
 * @key: node key
 * @sqnum: sequence number
 * @deletion: non-zero if this is a deletion
 * @used: number of bytes in use in a LEB
 * @old_size: truncation old size
 * @new_size: truncation new size
 *
 * This function inserts a scanned non-direntry node to the replay tree.
 * Returns zero in case of success and a negative error code in case of
 * failure.
 */
static int insert_node(struct ubifs_info *c, int lnum, int offs, int len,
		       union ubifs_key *key, unsigned long long sqnum,
		       int deletion, int *used, loff_t old_size,
		       loff_t new_size)
{
	struct rb_node **p = &c->replay_tree.rb_node, *parent = NULL;
	struct replay_entry *r;

	if (key_ino(c, key) >= c->highest_inum)
		c->highest_inum = key_ino(c, key);

	dbg_mnt_key(c, key, "add LEB %d:%d, key ", lnum, offs);
	while (*p) {
		parent = *p;
		r = rb_entry(parent, struct replay_entry, rb);
		if (sqnum < r->sqnum) {
			p = &(*p)->rb_left;
			continue;
		} else if (sqnum > r->sqnum) {
			p = &(*p)->rb_right;
			continue;
		}
		ubifs_err("duplicate sqnum in replay");
		return -EINVAL;
	}

	r = kzalloc(sizeof(struct replay_entry), GFP_KERNEL);
	if (!r)
		return -ENOMEM;

	if (!deletion)
		*used += ALIGN(len, 8);
	r->lnum = lnum;
	r->offs = offs;
	r->len = len;
	r->sqnum = sqnum;
	r->flags = (deletion ? REPLAY_DELETION : 0);
	r->old_size = old_size;
	r->new_size = new_size;
	key_copy(c, key, &r->key);

	rb_link_node(&r->rb, parent, p);
	rb_insert_color(&r->rb, &c->replay_tree);
	return 0;
}

/**
 * insert_dent - insert a directory entry node into the replay tree.
 * @c: UBIFS file-system description object
 * @lnum: node logical eraseblock number
 * @offs: node offset
 * @len: node length
 * @key: node key
 * @name: directory entry name
 * @nlen: directory entry name length
 * @sqnum: sequence number
 * @deletion: non-zero if this is a deletion
 * @used: number of bytes in use in a LEB
 *
 * This function inserts a scanned directory entry node to the replay tree.
 * Returns zero in case of success and a negative error code in case of
 * failure.
 *
 * This function is also used for extended attribute entries because they are
 * implemented as directory entry nodes.
 */
static int insert_dent(struct ubifs_info *c, int lnum, int offs, int len,
		       union ubifs_key *key, const char *name, int nlen,
		       unsigned long long sqnum, int deletion, int *used)
{
	struct rb_node **p = &c->replay_tree.rb_node, *parent = NULL;
	struct replay_entry *r;
	char *nbuf;

	if (key_ino(c, key) >= c->highest_inum)
		c->highest_inum = key_ino(c, key);

	dbg_mnt_key(c, key, "add LEB %d:%d, key ", lnum, offs);
	while (*p) {
		parent = *p;
		r = rb_entry(parent, struct replay_entry, rb);
		if (sqnum < r->sqnum) {
			p = &(*p)->rb_left;
			continue;
		}
		if (sqnum > r->sqnum) {
			p = &(*p)->rb_right;
			continue;
		}
		ubifs_err("duplicate sqnum in replay");
		return -EINVAL;
	}

	r = kzalloc(sizeof(struct replay_entry), GFP_KERNEL);
	if (!r)
		return -ENOMEM;
	nbuf = kmalloc(nlen + 1, GFP_KERNEL);
	if (!nbuf) {
		kfree(r);
		return -ENOMEM;
	}

	if (!deletion)
		*used += ALIGN(len, 8);
	r->lnum = lnum;
	r->offs = offs;
	r->len = len;
	r->sqnum = sqnum;
	r->nm.len = nlen;
	memcpy(nbuf, name, nlen);
	nbuf[nlen] = '\0';
	r->nm.name = nbuf;
	r->flags = (deletion ? REPLAY_DELETION : 0);
	key_copy(c, key, &r->key);

	ubifs_assert(!*p);
	rb_link_node(&r->rb, parent, p);
	rb_insert_color(&r->rb, &c->replay_tree);
	return 0;
}

/**
 * replay_bud - replay a bud logical eraseblock.
 * @c: UBIFS file-system description object
 * @lnum: bud logical eraseblock number to replay
 * @offs: bud start offset
 * @jhead: journal head to which this bud belongs
 * @free: amount of free space in the bud is returned here
 * @dirty: amount of dirty space from padding and deletion nodes is returned
 * here
 *
 * This function returns zero in case of success and a negative error code in
 * case of failure.
 */
static int replay_bud(struct ubifs_info *c, int lnum, int offs, int jhead,
		      int *free, int *dirty)
{
	int err = 0, used = 0;
	struct ubifs_scan_leb *sleb;
	struct ubifs_scan_node *snod;
	struct ubifs_bud *bud;

	dbg_mnt("replay bud LEB %d, head %d", lnum, jhead);
	if (c->need_recovery)
		sleb = ubifs_recover_leb(c, lnum, offs, c->sbuf, jhead != GCHD);
	else
		sleb = ubifs_scan(c, lnum, offs, c->sbuf);
	if (IS_ERR(sleb))
		return PTR_ERR(sleb);

	/*
	 * The bud does not have to start from offset zero - the beginning of
	 * the 'lnum' LEB may contain previously committed data. One of the
	 * things we have to do in replay is to correctly update lprops with
	 * newer information about this LEB.
	 *
	 * At this point lprops thinks that this LEB has 'c->leb_size - offs'
	 * bytes of free space because it only contain information about
	 * committed data.
	 *
	 * But we know that real amount of free space is 'c->leb_size -
	 * sleb->endpt', and the space in the 'lnum' LEB between 'offs' and
	 * 'sleb->endpt' is used by bud data. We have to correctly calculate
	 * how much of these data are dirty and update lprops with this
	 * information.
	 *
	 * The dirt in that LEB region is comprised of padding nodes, deletion
	 * nodes, truncation nodes and nodes which are obsoleted by subsequent
	 * nodes in this LEB. So instead of calculating clean space, we
	 * calculate used space ('used' variable).
	 */

	list_for_each_entry(snod, &sleb->nodes, list) {
		int deletion = 0;

		cond_resched();

		if (snod->sqnum >= SQNUM_WATERMARK) {
			ubifs_err("file system's life ended");
			goto out_dump;
		}

		if (snod->sqnum > c->max_sqnum)
			c->max_sqnum = snod->sqnum;

		switch (snod->type) {
		case UBIFS_INO_NODE:
		{
			struct ubifs_ino_node *ino = snod->node;
			loff_t new_size = le64_to_cpu(ino->size);

			if (le32_to_cpu(ino->nlink) == 0)
				deletion = 1;
			err = insert_node(c, lnum, snod->offs, snod->len,
					  &snod->key, snod->sqnum, deletion,
					  &used, 0, new_size);
			break;
		}
		case UBIFS_DATA_NODE:
		{
			struct ubifs_data_node *dn = snod->node;
			loff_t new_size = le32_to_cpu(dn->size) +
					  key_block(c, &snod->key) *
					  UBIFS_BLOCK_SIZE;

			err = insert_node(c, lnum, snod->offs, snod->len,
					  &snod->key, snod->sqnum, deletion,
					  &used, 0, new_size);
			break;
		}
		case UBIFS_DENT_NODE:
		case UBIFS_XENT_NODE:
		{
			struct ubifs_dent_node *dent = snod->node;

			err = ubifs_validate_entry(c, dent);
			if (err)
				goto out_dump;

			err = insert_dent(c, lnum, snod->offs, snod->len,
					  &snod->key, dent->name,
					  le16_to_cpu(dent->nlen), snod->sqnum,
					  !le64_to_cpu(dent->inum), &used);
			break;
		}
		case UBIFS_TRUN_NODE:
		{
			struct ubifs_trun_node *trun = snod->node;
			loff_t old_size = le64_to_cpu(trun->old_size);
			loff_t new_size = le64_to_cpu(trun->new_size);

			/* Validate truncation node */
			if (old_size < 0 || old_size > c->max_inode_sz ||
			    new_size < 0 || new_size > c->max_inode_sz ||
			    old_size <= new_size) {
				ubifs_err("bad truncation node");
				goto out_dump;
			}

			err = insert_node(c, lnum, snod->offs, snod->len,
					  &snod->key, snod->sqnum, 1, &used,
					  old_size, new_size);
			break;
		}
		default:
			ubifs_err("unexpected node type %d in bud LEB %d:%d",
				  snod->type, lnum, snod->offs);
			err = -EINVAL;
			goto out_dump;
		}
		if (err)
			goto out;
	}

	bud = ubifs_search_bud(c, lnum);
	if (!bud)
		BUG();

	ubifs_assert(bud->lnum == lnum);
	ubifs_assert(bud->start == offs);
	ubifs_assert(bud->jhead == jhead);
	ubifs_assert(sleb->endpt - offs >= used);
	ubifs_assert(sleb->endpt % c->min_io_size == 0);

	if (sleb->endpt + c->min_io_size <= c->leb_size &&
	    !(c->vfs_sb->s_flags & MS_RDONLY))
		err = ubifs_wbuf_seek_nolock(&c->jheads[jhead].wbuf, lnum,
					     sleb->endpt, UBI_SHORTTERM);

	*dirty = sleb->endpt - offs - used;
	*free = c->leb_size - sleb->endpt;

out:
	ubifs_scan_destroy(sleb);
	return err;

out_dump:
	ubifs_err("bad node is at LEB %d:%d", lnum, snod->offs);
	dbg_dump_node(c, snod->node);
	ubifs_scan_destroy(sleb);
	return -EINVAL;
}

/**
 * insert_ref_node - insert a ref node to the replay tree.
 * @c: UBIFS file-system description object
 * @lnum: node logical eraseblock number
 * @offs: node offset
 * @sqnum: sequence number
 * @free: amount of free space in bud
 * @dirty: amount of dirty space from padding and deletion nodes
 */
static int insert_ref_node(struct ubifs_info *c, int lnum, int offs,
			   unsigned long long sqnum, int free, int dirty)
{
	struct rb_node **p = &c->replay_tree.rb_node, *parent = NULL;
	struct replay_entry *r;
	union ubifs_key key;
	int cmp;

	dbg_mnt("add ref LEB %d:%d", lnum, offs);
	highest_ino_key(c, &key, -1);
	while (*p) {
		parent = *p;
		r = rb_entry(parent, struct replay_entry, rb);
		cmp = keys_cmp(c, &key, &r->key);
		if (sqnum < r->sqnum) {
			p = &(*p)->rb_left;
			continue;
		} else if (sqnum > r->sqnum) {
			p = &(*p)->rb_right;
			continue;
		}
		ubifs_err("duplicate sqnum in r");
		return -EINVAL;
	}

	r = kzalloc(sizeof(struct replay_entry), GFP_KERNEL);
	if (!r)
		return -ENOMEM;

	r->lnum = lnum;
	r->offs = offs;
	r->sqnum = sqnum;
	r->flags = REPLAY_REF;
	r->free = free;
	r->dirty = dirty;
	key_copy(c, &key, &r->key);

	rb_link_node(&r->rb, parent, p);
	rb_insert_color(&r->rb, &c->replay_tree);
	return 0;
}

/**
 * replay_buds - replay all buds.
 * @c: UBIFS file-system description object
 *
 * This function returns zero in case of success and a negative error code in
 * case of failure.
 */
static int replay_buds(struct ubifs_info *c)
{
	struct bud_entry *b;
	int err, uninitialized_var(free), uninitialized_var(dirty);

	list_for_each_entry(b, &c->replay_buds, list) {
		err = replay_bud(c, b->bud->lnum, b->bud->start, b->bud->jhead,
				 &free, &dirty);
		if (err)
			return err;
		err = insert_ref_node(c, b->bud->lnum, b->bud->start, b->sqnum,
				      free, dirty);
		if (err)
			return err;
	}

	return 0;
}

/**
 * destroy_bud_list - destroy the list of buds to replay.
 * @c: UBIFS file-system description object
 */
static void destroy_bud_list(struct ubifs_info *c)
{
	struct bud_entry *b;

	while (!list_empty(&c->replay_buds)) {
		b = list_entry(c->replay_buds.next, struct bud_entry, list);
		list_del(&b->list);
		kfree(b);
	}
}

/**
 * add_replay_bud - add a bud to the list of buds to replay.
 * @c: UBIFS file-system description object
 * @lnum: bud logical eraseblock number to replay
 * @offs: bud start offset
 * @jhead: journal head to which this bud belongs
 * @sqnum: reference node sequence number
 *
 * This function returns zero in case of success and a negative error code in
 * case of failure.
 */
static int add_replay_bud(struct ubifs_info *c, int lnum, int offs, int jhead,
			  unsigned long long sqnum)
{
	struct ubifs_bud *bud;
	struct bud_entry *b;

	dbg_mnt("add replay bud LEB %d:%d, head %d", lnum, offs, jhead);

	bud = kmalloc(sizeof(struct ubifs_bud), GFP_KERNEL);
	if (!bud)
		return -ENOMEM;

	b = kmalloc(sizeof(struct bud_entry), GFP_KERNEL);
	if (!b) {
		kfree(bud);
		return -ENOMEM;
	}

	bud->lnum = lnum;
	bud->start = offs;
	bud->jhead = jhead;
	ubifs_add_bud(c, bud);

	b->bud = bud;
	b->sqnum = sqnum;
	list_add_tail(&b->list, &c->replay_buds);

	return 0;
}

/**
 * validate_ref - validate a reference node.
 * @c: UBIFS file-system description object
 * @ref: the reference node to validate
 * @ref_lnum: LEB number of the reference node
 * @ref_offs: reference node offset
 *
 * This function returns %1 if a bud reference already exists for the LEB. %0 is
 * returned if the reference node is new, otherwise %-EINVAL is returned if
 * validation failed.
 */
static int validate_ref(struct ubifs_info *c, const struct ubifs_ref_node *ref)
{
	struct ubifs_bud *bud;
	int lnum = le32_to_cpu(ref->lnum);
	unsigned int offs = le32_to_cpu(ref->offs);
	unsigned int jhead = le32_to_cpu(ref->jhead);

	/*
	 * ref->offs may point to the end of LEB when the journal head points
	 * to the end of LEB and we write reference node for it during commit.
	 * So this is why we require 'offs > c->leb_size'.
	 */
	if (jhead >= c->jhead_cnt || lnum >= c->leb_cnt ||
	    lnum < c->main_first || offs > c->leb_size ||
	    offs & (c->min_io_size - 1))
		return -EINVAL;

	/* Make sure we have not already looked at this bud */
	bud = ubifs_search_bud(c, lnum);
	if (bud) {
		if (bud->jhead == jhead && bud->start <= offs)
			return 1;
		ubifs_err("bud at LEB %d:%d was already referred", lnum, offs);
		return -EINVAL;
	}

	return 0;
}

/**
 * replay_log_leb - replay a log logical eraseblock.
 * @c: UBIFS file-system description object
 * @lnum: log logical eraseblock to replay
 * @offs: offset to start replaying from
 * @sbuf: scan buffer
 *
 * This function replays a log LEB and returns zero in case of success, %1 if
 * this is the last LEB in the log, and a negative error code in case of
 * failure.
 */
static int replay_log_leb(struct ubifs_info *c, int lnum, int offs, void *sbuf)
{
	int err;
	struct ubifs_scan_leb *sleb;
	struct ubifs_scan_node *snod;
	const struct ubifs_cs_node *node;

	dbg_mnt("replay log LEB %d:%d", lnum, offs);
	sleb = ubifs_scan(c, lnum, offs, sbuf);
	if (IS_ERR(sleb)) {
		if (c->need_recovery)
			sleb = ubifs_recover_log_leb(c, lnum, offs, sbuf);
		if (IS_ERR(sleb))
			return PTR_ERR(sleb);
	}

	if (sleb->nodes_cnt == 0) {
		err = 1;
		goto out;
	}

	node = sleb->buf;

	snod = list_entry(sleb->nodes.next, struct ubifs_scan_node, list);
	if (c->cs_sqnum == 0) {
		/*
		 * This is the first log LEB we are looking at, make sure that
		 * the first node is a commit start node. Also record its
		 * sequence number so that UBIFS can determine where the log
		 * ends, because all nodes which were have higher sequence
		 * numbers.
		 */
		if (snod->type != UBIFS_CS_NODE) {
			dbg_err("first log node at LEB %d:%d is not CS node",
				lnum, offs);
			goto out_dump;
		}
		if (le64_to_cpu(node->cmt_no) != c->cmt_no) {
			dbg_err("first CS node at LEB %d:%d has wrong "
				"commit number %llu expected %llu",
				lnum, offs, le64_to_cpu(node->cmt_no),
				c->cmt_no);
			goto out_dump;
		}

		c->cs_sqnum = le64_to_cpu(node->ch.sqnum);
		dbg_mnt("commit start sqnum %llu", c->cs_sqnum);
	}

	if (snod->sqnum < c->cs_sqnum) {
		/*
		 * This means that we reached end of log and now
		 * look to the older log data, which was already
		 * committed but the eraseblock was not erased (UBIFS
		 * only unmaps it). So this basically means we have to
		 * exit with "end of log" code.
		 */
		err = 1;
		goto out;
	}

	/* Make sure the first node sits at offset zero of the LEB */
	if (snod->offs != 0) {
		dbg_err("first node is not at zero offset");
		goto out_dump;
	}

	list_for_each_entry(snod, &sleb->nodes, list) {

		cond_resched();

		if (snod->sqnum >= SQNUM_WATERMARK) {
			ubifs_err("file system's life ended");
			goto out_dump;
		}

		if (snod->sqnum < c->cs_sqnum) {
			dbg_err("bad sqnum %llu, commit sqnum %llu",
				snod->sqnum, c->cs_sqnum);
			goto out_dump;
		}

		if (snod->sqnum > c->max_sqnum)
			c->max_sqnum = snod->sqnum;

		switch (snod->type) {
		case UBIFS_REF_NODE: {
			const struct ubifs_ref_node *ref = snod->node;

			err = validate_ref(c, ref);
			if (err == 1)
				break; /* Already have this bud */
			if (err)
				goto out_dump;

			err = add_replay_bud(c, le32_to_cpu(ref->lnum),
					     le32_to_cpu(ref->offs),
					     le32_to_cpu(ref->jhead),
					     snod->sqnum);
			if (err)
				goto out;

			break;
		}
		case UBIFS_CS_NODE:
			/* Make sure it sits at the beginning of LEB */
			if (snod->offs != 0) {
				ubifs_err("unexpected node in log");
				goto out_dump;
			}
			break;
		default:
			ubifs_err("unexpected node in log");
			goto out_dump;
		}
	}

	if (sleb->endpt || c->lhead_offs >= c->leb_size) {
		c->lhead_lnum = lnum;
		c->lhead_offs = sleb->endpt;
	}

	err = !sleb->endpt;
out:
	ubifs_scan_destroy(sleb);
	return err;

out_dump:
	ubifs_err("log error detected while replying the log at LEB %d:%d",
		  lnum, offs + snod->offs);
	dbg_dump_node(c, snod->node);
	ubifs_scan_destroy(sleb);
	return -EINVAL;
}

/**
 * take_ihead - update the status of the index head in lprops to 'taken'.
 * @c: UBIFS file-system description object
 *
 * This function returns the amount of free space in the index head LEB or a
 * negative error code.
 */
static int take_ihead(struct ubifs_info *c)
{
	const struct ubifs_lprops *lp;
	int err, free;

	ubifs_get_lprops(c);

	lp = ubifs_lpt_lookup_dirty(c, c->ihead_lnum);
	if (IS_ERR(lp)) {
		err = PTR_ERR(lp);
		goto out;
	}

	free = lp->free;

	lp = ubifs_change_lp(c, lp, -1, -1, lp->flags | LPROPS_TAKEN, 0);
	if (IS_ERR(lp)) {
		err = PTR_ERR(lp);
		goto out;
	}

	err = free;
out:
	ubifs_release_lprops(c);
	return err;
}

/**
 * ubifs_replay_journal - replay journal.
 * @c: UBIFS file-system description object
 *
 * This function scans the journal, replays and cleans it up. It makes sure all
 * memory data structures related to uncommitted journal are built (dirty TNC
 * tree, tree of buds, modified lprops, etc).
 */
int ubifs_replay_journal(struct ubifs_info *c)
{
	int err, i, lnum, offs, free;
	void *sbuf = NULL;

	/* Update the status of the index head in lprops to 'taken' */
	free = take_ihead(c);
	if (free < 0)
		return free; /* Error code */

	if (c->ihead_offs != c->leb_size - free) {
		ubifs_err("bad index head LEB %d:%d", c->ihead_lnum,
			  c->ihead_offs);
		return -EINVAL;
	}

	sbuf = vmalloc(c->leb_size);
	if (!sbuf)
		return -ENOMEM;

	dbg_mnt("start replaying the journal");

	c->replaying = 1;

	lnum = c->ltail_lnum = c->lhead_lnum;
	offs = c->lhead_offs;

	for (i = 0; i < c->log_lebs; i++, lnum++) {
		if (lnum >= UBIFS_LOG_LNUM + c->log_lebs) {
			/*
			 * The log is logically circular, we reached the last
			 * LEB, switch to the first one.
			 */
			lnum = UBIFS_LOG_LNUM;
			offs = 0;
		}
		err = replay_log_leb(c, lnum, offs, sbuf);
		if (err == 1)
			/* We hit the end of the log */
			break;
		if (err)
			goto out;
		offs = 0;
	}

	err = replay_buds(c);
	if (err)
		goto out;

	err = apply_replay_tree(c);
	if (err)
		goto out;

	ubifs_assert(c->bud_bytes <= c->max_bud_bytes || c->need_recovery);
	dbg_mnt("finished, log head LEB %d:%d, max_sqnum %llu, "
		"highest_inum %lu", c->lhead_lnum, c->lhead_offs, c->max_sqnum,
		c->highest_inum);
out:
	destroy_replay_tree(c);
	destroy_bud_list(c);
	vfree(sbuf);
	c->replaying = 0;
	return err;
}
