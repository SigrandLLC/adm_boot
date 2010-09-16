/*****************************************************************************
;
;   (C) Unpublished Work of ADMtek Incorporated.  All Rights Reserved.
;
;       THIS WORK IS AN UNPUBLISHED WORK AND CONTAINS CONFIDENTIAL,
;       PROPRIETARY AND TRADESECRET INFORMATION OF ADMTEK INCORPORATED.
;       ACCESS TO THIS WORK IS RESTRICTED TO (I) ADMTEK EMPLOYEES WHO HAVE A
;       NEED TO KNOW TO PERFORM TASKS WITHIN THE SCOPE OF THEIR ASSIGNMENTS
;       AND (II) ENTITIES OTHER THAN ADMTEK WHO HAVE ENTERED INTO APPROPRIATE
;       LICENSE AGREEMENTS.  NO PART OF THIS WORK MAY BE USED, PRACTICED,
;       PERFORMED, COPIED, DISTRIBUTED, REVISED, MODIFIED, TRANSLATED,
;       ABBRIDGED, CONDENSED, EXPANDED, COLLECTED, COMPILED, LINKED, RECAST,
;       TRANSFORMED OR ADAPTED WITHOUT THE PRIOR WRITTEN CONSENT OF ADMTEK.
;       ANY USE OR EXPLOITATION OF THIS WORK WITHOUT AUTHORIZATION COULD
;       SUBJECT THE PERPERTRATOR TO CRIMINAL AND CIVIL LIABILITY.
;
;------------------------------------------------------------------------------
;
;    Project : ADM5120
;    Creator :
;    File    : ldrinit.S
;    Abstract:
;
;Modification History:
;
;
;*****************************************************************************/


#include <mips.h>
#include <mips4kc.h>
#include <asm.h>
#include <adm5120.h>

#define SYS_STACK_SIZE			0x4000

.text
LEAF(_ldrinit)
	.set noreorder
	/*  Disable interrupt  */
	mfc0	v0, CP0_STATUS_REG
	and		v0, CP0_STATUS_BEV_BIT
	mtc0	v0, CP0_STATUS_REG

	/*  Get current position  */
	bal		_C_LABEL(_get_pc)
	nop

	/*  v0 now hold the run time address of _mark1  */
_mark1:
	move	a0, v0

#ifndef RTL_SIMULATION
	la		a1, _C_LABEL(_ldrinit)

	la		t0, _C_LABEL(_mark1)

	/*  Calculate the offset from _ldrinit to _mark1  */
	subu	t0, t0, a1		// t0 holds the length from _sysinit to _mark1
	subu	a0, a0, t0		// a0 holds the start address of _sysinit in rom
	move	s0, a0

	la		t1, _C_LABEL(_etext)
	subu	a2, t1, a1		// a2 holds the length of the program

	/*
	 *  Copy the program code from rom to SDRAM
	 *		a0 : Source address of program image in ROM
	 *		a1 : Destination address
	 *		a2 : length of program image
	 */

	 /*  Copy code loop  */
1:
	lw		t0, 0(a0)
	subu	a2, a2, 4*4			// substract the length to be copied
	lw		t1, 4(a0)
	sw		t0, 0(a1)
	lw		t2, 8(a0)
	sw		t1, 4(a1)
	lw		t3, 12(a0)
	sw		t2, 8(a1)
	addu	a0, a0, 4*4			// next source address
	sw		t3, 12(a1)

	/*
	 *  Invalidate the corresponding ICACHE line
	 */

	cache	ICACHE_HIT_INVALIDATE, 0(a1)
	bgtz	a2, 1b
	addu	a1, a1, 4*4			// next destination address // Branch delay slot


	la		a0, _C_LABEL(_ldrinit)
	la		a1, _C_LABEL(_etext)	// start of data segment

	subu	a0, a1, a0
	addu	a0, s0, a0				// start of data in ROM


	la		a2, _C_LABEL(_edata)	// end of data segment
	subu	a2, a2, a1				// length of data segment

	/*
	 *  Copy the program data from rom to SDRAM
	 *		a0 : Source address of program image in ROM
	 *		a1 : Destination address
	 *		a2 : length of program image
	 */

	 /*  Copy data loop  */
1:
	lw		t0, 0(a0)
	subu	a2, a2, 4*4			// substract the length to be copied
	lw		t1, 4(a0)
	sw		t0, 0(a1)
	lw		t2, 8(a0)
	sw		t1, 4(a1)
	lw		t3, 12(a0)
	sw		t2, 8(a1)
	addu	a0, a0, 4*4			// next source address
	sw		t3, 12(a1)

	bgtz	a2, 1b
	addu	a1, a1, 4*4			// next destination address // Branch delay slot
#endif

	// Jump to SDRAM
	la		t0, 2f
	jr		t0
	nop							/*	Branch delay slot  */
2:
	// We are in SDRAM now
	/* clear BSS section  */
	la		a0, _C_LABEL(__bss_start)
	la		a1, _C_LABEL(_end)
	subu	a1, a1, a0

	/*  Clear BSS Loop */
3:
#ifndef RTL_SIMULATION
	sw		zero, 0(a0)
	sw		zero, 4(a0)
	subu	a1, a1, 4*4			// substract the length to be zeroed
	sw		zero, 8(a0)
	sw		zero, 12(a0)
	bgtz	a1, 3b
	addu	a0, a0, 4*4			// next destination address // Branch delay slot
#endif

	/*  Load gp  */
	la		gp, _C_LABEL(_gp)

	/*  Initialize stack  */
	la		t0, _C_LABEL(_end)
	li		t1, SYS_STACK_SIZE
	add		sp, t0, t1

	/* Align sp to 16byte boundry*/
	li		t0, ~0xf
	and		sp, sp, t0

	/* Store the first free memory address */
	addu	t0, sp, 0x10
	la		t1, _C_LABEL(_heap_h)
	sw		t0, 0(t1)

	// Call c_entry and never returned
	jal		_C_LABEL(c_entry)
	nop							// Branch delay slot

	/*  Never Reached  */
1:
	b		1b
	nop

	.set reorder
END(_ldrinit)


/*  Get program counter  */
LEAF(_get_pc)
	.set noreorder
	jr		ra
	move	v0, ra				/*	Branch delay slot */
	.set reorder
END(_get_pc)


LEAF(_icache_sync_all)
	.set noreorder
	mtc0	zero, CP0_TAGLO_REG
	mtc0	zero, CP0_TAGHI_REG
	li		a0, MEM_KSEG0_BASE
	addiu	a1, a0, CPU_ICACHE_DFE_SIZE - 0x10*8

1:
	cache	ICACHE_INDEX_STORETAG, 0x00(a0)
	cache	ICACHE_INDEX_STORETAG, 0x10(a0)
	cache	ICACHE_INDEX_STORETAG, 0x20(a0)
	cache	ICACHE_INDEX_STORETAG, 0x30(a0)
	cache	ICACHE_INDEX_STORETAG, 0x40(a0)
	cache	ICACHE_INDEX_STORETAG, 0x50(a0)
	cache	ICACHE_INDEX_STORETAG, 0x60(a0)
	cache	ICACHE_INDEX_STORETAG, 0x70(a0)
	bne		a0, a1, 1b
	addiu	a0, a0, 0x10*8					/* Branch delay slot */

	jr		ra
	nop										/* Branch delay slot */
	.set reorder
END(_icache_sync_all)

