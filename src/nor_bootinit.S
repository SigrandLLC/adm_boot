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
;    File    : bootinit.S
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
#include <hw5120cfg.h>

	
.set noreorder
.set noat

.text
.org 0
VECTOR(romreset_except, unknown)
	/* Read current status */
	mfc0 	k0, CP0_STATUS_REG
	
	/* Is this a NMI */
	li		k1, CP0_STATUS_NMI_BIT
	and		k1, k0, k1
	beqz	k1, 2f						/* Branch if this is not a NMI. */
	nop									/* Branch delay slot */

	/* NMI wrapper */
	li 		k1, CP0_STATUS_BEV_BIT
	and		k1, k0, k1
1:	
	bnez	k1, 1b						/* Is the program ready to take a NMI */
	nop									/* Branch delay slot */
	
	/* Redirect the NMI to the handler in SDRAM */
	/*
	 *  !!!!  Not implemented yet   !!!!
	 */
1:
	b		1b
	nop
	
2:
	/*  Cold boot or Software reset  */

	/*=====  Initialize CP0 register  ====*/
	/* Set IV bit */	
	li 		t0, CP0_CAUSE_IV_BIT
	mtc0	t0, CP0_CAUSE_REG
	
	/* Clear watch register */	
	mtc0	zero, CP0_WATCHLO_REG
	mtc0	zero, CP0_WATCHHI_REG
	
	/* Set status register */
	li 		t0, CP0_STATUS_DEFAULT
	mtc0	t0, CP0_STATUS_REG
	
	/* Set configure 0 register */
	li 		t0, CP0_CONFIG0_DEFAULT
	mtc0	t0, CP0_CONFIG0_REG

//	jal		init_mpmc_sram
	bal		init_mpmc_sram
	nop

#ifndef KSEG0_UNCACHED
	// Init cache
//	jal		_C_LABEL(init_cache)
	bal		_C_LABEL(init_cache)
	nop									/* Branch delay slot */
#endif
	
	/*====  Jump to KSEG0 with cache enabled  ====*/
	la		t0, 1f
	jr		t0
	nop									/* Branch delay slot */
	
1:	
	/*  In KSEG0, cached  */
//	jal		init_mpmc_sdram
	bal		init_mpmc_sdram
	nop
	
	la		a0, _C_LABEL(romreset_except)
	addu	a0, a0, 0x1000
	jr		a0
	nop									/* Branch delay slot */
	
	/* Not reached here */
3:	
	b		3b
	nop									/* Branch delay slot */
VECTOR_END(romreset_except)

.org 0x200
VECTOR(romTLBrefill_except, unknown)
	/*
	 *	The exception is not handled here.
	 */
	
1:
	b		1b
	nop									/* Branch delay slot */
VECTOR_END(romTLBrefill_except)

.org 0x380
VECTOR(romgeneral_except, unknown)
	/*
	 *  Filter out the interrupt except and redirect it to interrupt_except
	 */

	mfc0	k0, CP0_CAUSE_REG
	and		k0, k0, CP0_CAUSE_EXCCODE_MASK
	srl		k0, k0, CP0_CAUSE_EXCCODE_SHIFT
	subu	k0, k0, EXCCODE_INT
	beqz	k0, _C_LABEL(romint_except)
	nop									/* Branch delay slot */
	
	/*
	 *  Not ready for handling exception yet.
	 */
1:
	b		1b
	nop									/* Branch delay slot */
	
VECTOR_END(romgeneral_except)

.org 0x400
VECTOR(romint_except, unknown)
	/*
	 *  We are not ready to handle interrupt here.
	 *  Clear the IE (interrupt enablet) bit and return
	 */
	mfc0	k0, CP0_STATUS_REG
	li		k1, ~CP0_STATUS_IE_BIT
	and		k0, k0, k1
	mtc0	k0, CP0_STATUS_REG
	eret
	
VECTOR_END(romint_except)

.org 0x480
VECTOR(romdbg_except, unknown)
	mfc0	k0, CP0_STATUS_REG
	
	/* Is the system ready to tak a debug exception */
	li 		k1, CP0_STATUS_BEV_BIT
	and 	k1, k0, k1
1:
	bnez	k1, 1b						/* Stop here if BEV is set */
	nop									/* Branch delay slot */

	/* Redirect the exception to the handler in SDRAM */
	/*
	 *  !!!!  Not implemented yet   !!!!
	 */
	
	/*	Not reached */	
2:
	b		2b
	nop									/* Branch delay slot */
VECTOR_END(romdbg_except)

.set at
.set reorder


.org 0x500
.set noreorder
LEAF(init_cache)
	/*====  Initialize Cache  ====*/

	/* Read cache configuration  */
#if 1
	mfc0	a0, CP0_CONFIG1_REG, CP0_CONFIG1_SEL

	/*  Determine ICACHE size  */
	li		t0, CP0_CONFIG1_IL_MASK
	and		t0, t0, a0
	srl		t0, t0, CP0_CONFIG1_IL_SHIFT
	
	beqz	t0, 1f
	li		s0, 0								/* Branch delay slot */
	// ICACHE line size is 16 bytes
	li		s1, 0x10

	li		t0, CP0_CONFIG1_IS_MASK
	and		t0, t0, a0
	srl		t0, t0, CP0_CONFIG1_IS_SHIFT
	li		t1, 64
	sll		t1, t1, t0							// lines per way
	
	li		t0, CP0_CONFIG1_IA_MASK	
	and		t2, t0, a0
	srl		t2, t2, CP0_CONFIG1_IA_SHIFT
	addiu	t2, t2, 1							// Number of way

	mul		t0, t1, t2							// total ICACHE line number
	mul		s0, t0, s1							// ICACHE size

1:
	/*  Determine DCACHE size  */
	li		t0, CP0_CONFIG1_DL_MASK
	and		t0, t0, a0
	srl		t0, t0, CP0_CONFIG1_DL_SHIFT
	
	beqz	t0, 2f
	li		s2, 0								/* Branch delay slot */
	// DCACHE line size is 16 bytes
	li		s3, 0x10

	li		t0, CP0_CONFIG1_DS_MASK
	and		t0, t0, a0
	srl		t0, t0, CP0_CONFIG1_DS_SHIFT
	li		t1, 64
	sll		t1, t1, t0							// lines per way

	li		t0, CP0_CONFIG1_DA_MASK	
	and		t2, t0, a0
	srl		t2, t2, CP0_CONFIG1_DA_SHIFT
	addiu	t2, t2, 1							// Number of way

	mul		t0, t1, t2							// total ICACHE line number
	mul		s2, t0, s3							// ICACHE size

#else
	li		t0, PA2VA(ADM5120_SWCTRL_BASE)
	lw		t1, CODE_REG(t0)

	li		t2, 0x1000			// Constant: 4K
	li		t3, 0x800			// Constant: 2K

	/*  Determine ICACHE size  */
	li		t4, CPU_ICACHE_2K_WAY
	move	s0, t2
	and		t4, t4, t1
	movn	s0, t3, t4
	
	li		t4, CPU_ICACHE_2WAY
	sll		t5, s0, 1
	and		t4, t4, t1
	movn	s0, t5, t4
	
	li		s1, CPU_CACHE_LINE_SIZE
	
	/*  Determine DCAHCE size  */
	li		t4, CPU_DCACHE_2K_WAY
	move	s2, t2
	and		t4, t4, t1
	movn	s2, t3, t4

	li		t4, CPU_DCACHE_2WAY
	sll		t5, s2, 1
	and		t4, t4, t1
	movn	s2, t5, t4
	
	li		s3, CPU_CACHE_LINE_SIZE
#endif

2:
	/*====  Invalidate ICACHE and DCACHE  ====*/
	mtc0	zero, CP0_TAGLO_REG
	mtc0	zero, CP0_TAGHI_REG
	li		t0, MEM_KSEG0_BASE
	
	beqz	s0, 4f
	nop									/* Branch delay slot */
	
	/*  Invalidate ICACHE  */
	addu	t1, t0, s0
	sll		s1, s1, 3
	subu	t1, t1, s1

	move	t2, t0
3:
	cache	ICACHE_INDEX_STORETAG, 0x00(t2)
	cache	ICACHE_INDEX_STORETAG, 0x10(t2)
	cache	ICACHE_INDEX_STORETAG, 0x20(t2)
	cache	ICACHE_INDEX_STORETAG, 0x30(t2)
	cache	ICACHE_INDEX_STORETAG, 0x40(t2)
	cache	ICACHE_INDEX_STORETAG, 0x50(t2)
	cache	ICACHE_INDEX_STORETAG, 0x60(t2)
	cache	ICACHE_INDEX_STORETAG, 0x70(t2)
	bne		t2, t1, 3b
	addu	t2, t2, s1					/* Branch delay slot */

4:	
	beqz	s2, 6f
	nop									/* Branch delay slot */
	/*  Invalidate DCACHE  */
	addu	t1, t0, s2
	sll		s3, s3, 3
	subu	t1, t1, s3

	move	t2, t0
5:
	cache	DCACHE_INDEX_STORETAG, 0x00(t2)
	cache	DCACHE_INDEX_STORETAG, 0x10(t2)
	cache	DCACHE_INDEX_STORETAG, 0x20(t2)
	cache	DCACHE_INDEX_STORETAG, 0x30(t2)
	cache	DCACHE_INDEX_STORETAG, 0x40(t2)
	cache	DCACHE_INDEX_STORETAG, 0x50(t2)
	cache	DCACHE_INDEX_STORETAG, 0x60(t2)
	cache	DCACHE_INDEX_STORETAG, 0x70(t2)
	bne		t2, t1, 5b
	addu	t2, t2, s3					/* Branch delay slot */

6:	
	jr		ra
	nop									/* Branch delay slot */
END(init_cache)
.set reorder


LEAF(init_mpmc_sram)
	/* Program Mem_control Register */

	li		a0, PA2VA(ADM5120_SWCTRL_BASE)
	li 		t0, HWPF_5120_MEMCNTL_CFG
	sw 		t0, Mem_control_REG(a0)

	li		a1, PA2VA(ADM5120_MPMC_BASE)
	
	/*  Static memory bank0 init  */
	addi	a2, a1, MPMC_SM_CONFIG1_REG
	
	li		t0, SmemBank0_Config
	sw		t0, 0(a2)
	li		t1, SmemBank0_WaitWen
	sw		t1, 4(a2)
	li		t2, SmemBank0_WaitOen
	sw		t2, 8(a2)
	li		t3, SmemBank0_WaitRd
	sw		t3, 12(a2)
	li		t4, SmemBank0_WaitPage
	sw		t4, 16(a2)
	li		t5, SmemBank0_WaitWr
	sw		t5, 20(a2)
	li		t6, SmemBank0_WaitTurn
	sw		t6, 24(a2)
	
#if (HWPF_5120_SMEM1_EN == OPTION_ENABLE)
	/*  Static memory bank1 init  */
	addi	a2, a1, MPMC_SM_CONFIG0_REG
	
	li		t0, SmemBank1_Config
	sw		t0, 0(a2)
	li		t1, SmemBank1_WaitWen
	sw		t1, 4(a2)
	li		t2, SmemBank1_WaitOen
	sw		t2, 8(a2)
	li		t3, SmemBank1_WaitRd
	sw		t3, 12(a2)
	li		t4, SmemBank1_WaitPage
	sw		t4, 16(a2)
	li		t5, SmemBank1_WaitWr
	sw		t5, 20(a2)
	li		t6, SmemBank1_WaitTurn
	sw		t6, 24(a2)
#endif	

	jr		ra
	
END(init_mpmc_sram)

LEAF(init_mpmc_sdram)
	/* Program Mem_control Register */

	li		a0, PA2VA(ADM5120_SWCTRL_BASE)
	li		a1, PA2VA(ADM5120_MPMC_BASE)

	/*====  SDRAM init  ====*/
	/* Make sure the the address mirror is off*/
	lw		t0, MPMC_CONTROL_REG(a1)
	li		t1, ~MPMC_ADDR_MIRROR	
	and		t0, t0, t1
	sw		t0,  MPMC_CONTROL_REG(a1)

	/*  Set SDRAM accessing timing  */	
	addi	a2, a1, MPMC_DM_TRP_REG

	li		t0, HWPF_5120_SDRAM_tRP
	sw		t0, 0(a2)
	li		t1, HWPF_5120_SDRAM_tRAS
	sw		t1, 4(a2)
	li		t2, HWPF_5120_SDRAM_tSREX
	sw		t2, 8(a2)
	li		t3, HWPF_5120_SDRAM_tAPR
	sw		t3, 12(a2)
	li		t4, HWPF_5120_SDRAM_tDAL
	sw		t4, 16(a2)
	li		t5, HWPF_5120_SDRAM_tWR
	sw		t5, 20(a2)
	li		t6, HWPF_5120_SDRAM_tRC
	sw		t6, 24(a2)
	li		t7, HWPF_5120_SDRAM_tRFC
	sw		t7, 28(a2)
	li		t8, HWPF_5120_SDRAM_tXSR
	sw		t8, 32(a2)
	li		t9, HWPF_5120_SDRAM_tRRD
	sw		t9, 36(a2)
	li		s0, HWPF_5120_SDRAM_tMRD
	sw		s0, 40(a2)
	
	
	/* Get MPMC_DM_CONTROL_REG */
	lw		s0, MPMC_DM_CONTROL_REG(a1)
	li		t1, ~DM_SDRAM_OPMODE_MASK
	and		s0, s0, t1
	
	// Make sure CLK ENABLE is HIGH during the initializing process
	or		t0, s0, (DM_CLKEN_ALWAYS|DM_CLKOUT_ALWAYS)
	sw		t0, MPMC_DM_CONTROL_REG(a1)

	// Turn off SDRAM0 buffer of all banks
	lw		t0, MPMC_DM_CONFIG0_REG(a1)
	li		t1, ~DM_CFG_BUFFER_EN
	and		t0, t0, t1
	sw		t0, MPMC_DM_CONFIG0_REG(a1)
	
#if HWPF_5120_SDRAM_BANK_NUM == 2
	// Turn off SDRAM1 buffer of all banks
	lw		t0, MPMC_DM_CONFIG1_REG(a1)
//	li		t1, ~DM_CFG_BUFFER_EN
	and		t0, t0, t1
	sw		t0, MPMC_DM_CONFIG1_REG(a1)
#endif	
	
#ifndef RTL_SIMULATION
	// Prepare some constant
	li		t5, SW_TIMER_100US_TICKS
	li		t6, (SW_TIMER_INT_DISABLE | SW_TIMER_INT)
	li		t7, 0xffff
    li		t8, SW_TIMER_EN
#endif

	/*  wait at leaset 1ms until SDRAMs power and clocks stabilized.  */
#ifndef RTL_SIMULATION
	/* stop timer */
	sw		t7, Timer_REG(a0)
	/* clear timer interrupt status bit */ 
	sw		t6, Timer_int_REG(a0)

	/* Start timer to count 1.6 ms */
	sll		t0, t5, 4				/*  100us * 2^4 */
	or		t0, t0, t8
	sw		t0, Timer_REG(a0)
	
1:
	lw		t1, Timer_int_REG(a0)
	and		t0, t1, SW_TIMER_INT
	beqz	t0, 1b
	
#else

	li		t0, 0x10
1:
	nop
	nop        
	subu 	t0, t0, 1
	bnez	t0, 1b
#endif

	/* Issue NOP command to ALL SDRAM. */
	or		t0, s0, DM_SDRAM_NOP
	sw		t0, MPMC_DM_CONTROL_REG(a1)

	/*  wait at least 200us  */
#ifndef RTL_SIMULATION
	/* stop timer */
	sw		t7, Timer_REG(a0)
	/* clear timer interrupt status bit */ 
	sw		t6, Timer_int_REG(a0)

	/* Start timer to count 200us */
	sll		t0, t5, 1				/*  100us * 2^1		*/
	or		t0, t0, t8
	sw		t0, Timer_REG(a0)
	
1:
	lw		t1, Timer_int_REG(a0)
	and		t0, t1, SW_TIMER_INT
	beqz	t0, 1b
#else
	li		t0, 0x20
1:
	nop
	nop
	subu 	t0, t0, 1
	bnez	t0, 1b
#endif

	/* Issues PRE-ALL to ALL SDRAM  */
	or		t0, s0, DM_SDRAM_PRECHARGE_ALL
	sw		t0, MPMC_DM_CONTROL_REG(a1)

	/* Write 2 to refresh timier.
	 * The memory controller will issue refresh every 32 sdram clocks. 
	 */
	li		t0, 2
	sw		t0, MPMC_DM_REFRESH_REG(a1)

	/* wait at least 8 refresh cycle (32*8=256 sdram clock or 512 CPU clock) */
	li		t0, 64
1:
	nop							/*	1	*/
	nop        					/*	2	*/
	nop							/*	3	*/
	nop        					/*	4	*/
	nop							/*	5	*/
	subu 	t0, t0, 1			/*	6	*/
	bnez	t0, 1b				/* 7,8	*/

	/* Set refresh timer to normal operation value  */
	li		t0, HWPF_5120_SDRAM_RFTIME
	sw		t0, MPMC_DM_REFRESH_REG(a1)

	/* Program the RAS/CAS value of SDRAM bank 0 */
	li		t0, SDRAM_BANK0_RAS_CAS
	sw		t0, MPMC_DM_RASCAS0_REG(a1)
	
	/* Program the configuration of SDRAM bank 0 */
	li		t0, MPMC_SDRAM_BANK0_CFG
	// make sure that the read/write buffer is disabled
	li		t1, ~DM_CFG_BUFFER_EN
	and		t0, t0, t1
	sw		t0, MPMC_DM_CONFIG0_REG(a1)

#if HWPF_5120_SDRAM_BANK_NUM == 2
	/* Program the RAS/CAS value of SDRAM bank 1 */
	li 		t0, SDRAM_BANK1_RAS_CAS
	sw		t0, MPMC_DM_RASCAS1_REG(a1)
	
	/* Program the configuration of SDRAM bank 1 */
	li		t0, MPMC_SDRAM_BANK1_CFG
	// make sure that the read/write buffer is disabled
	and		t0, t0, t1
	sw		t0, MPMC_DM_CONFIG1_REG(a1)
#endif	// ! HWPF_5120_SDRAM_BANK_NUM == 2

	/****  Program the mode register in SDRAM  ****/	
	// MPMC sdram mode setting 
	or		t0, s0, DM_SDRAM_MODE_SETTING
	sw		t0, MPMC_DM_CONTROL_REG(a1)
	
	/* Program the mode register of SDRAM bank0*/
	li		t0, PA2VA(HWPF_5120_SDRAM0_BASE)
	li		t1, SDRAM_BANK0_MODE
	sll		t1, t1, SDRAM_BANK0_MODE_SHIFT
	add		t0, t1, t0
	lw		t1, 0(t0)


#if HWPF_5120_SDRAM_BANK_NUM == 2
	/* Program the mode register of SDRAM bank1*/
	li		t0, PA2VA(HWPF_5120_SDRAM1_BASE)
	li		t1, SDRAM_BANK1_MODE
	sll		t1, t1, SDRAM_BANK1_MODE_SHIFT
	add		t0, t1, t0
	lw		t1, 0(t0)
#endif	// ! HWPF_5120_SDRAM_BANK_NUM == 2
	
	// SDRAM is ready now, put it to normal operation mode
	or		t0, s0, DM_SDRAM_NORMAL_OP
	
	// Clear DM_CLKEN_ALWAYS bit
	li		t1, ~DM_CLKEN_ALWAYS
	and		t0, t0, t1
	sw		t0, MPMC_DM_CONTROL_REG(a1)
	
	// Enable the Read/Write buffers of SDRAM banks0
	lw 		t0, MPMC_DM_CONFIG0_REG(a1)
	li		t1, DM_CFG_BUFFER_EN
	or		t0, t0, t1
	sw		t0, MPMC_DM_CONFIG0_REG(a1)

#if HWPF_5120_SDRAM_BANK_NUM == 2
	// Enable the Read/Write buffers of SDRAM banks1
	lw 		t0, MPMC_DM_CONFIG1_REG(a1)
	or		t0, t0, t1
	sw		t0, MPMC_DM_CONFIG1_REG(a1)
#endif  // ! HWPF_5120_SDRAM_BANK_NUM == 2

	jr		ra
END(init_mpmc_sdram)

