/*
 *  linux/include/asm-mips/mach-jz4730/jz4730.h
 *
 *  JZ4730 common definition.
 *
 *  Copyright (C) 2006 - 2007 Ingenic Semiconductor Inc.
 *
 *  Author: <jlwei@ingenic.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_JZ4730_H__
#define __ASM_JZ4730_H__

#include <asm/mach-jz4730/regs.h>
#include <asm/mach-jz4730/ops.h>
#include <asm/mach-jz4730/dma.h>
#include <asm/mach-jz4730/misc.h>

/*------------------------------------------------------------------
 * Platform definitions
 */
#ifdef CONFIG_JZ4730_PMP
#include <asm/mach-jz4730/board-pmp.h>
#endif
#ifdef CONFIG_JZ4730_MINIPC
#include <asm/mach-jz4730/board-minipc.h>
#endif

/* Add other platform definition here ... */


/*------------------------------------------------------------------
 * Follows are related to platform definitions
 */

#include <asm/mach-jz4730/clock.h>
#include <asm/mach-jz4730/serial.h>

#endif /* __ASM_JZ4730_H__ */
