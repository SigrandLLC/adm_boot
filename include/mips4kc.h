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
;    File    : mips4kc.h
;    Abstract: 
;
;Modification History:
; 
;
;*****************************************************************************/


#ifndef __MIPS4KC_H__
#define __MIPS4KC_H__

#include <mips.h>


/*  Cache control  */
#define CACHECTRL_WRITE_THR_MODE			3
#define CACHECTRL_UNCACHED					2

/*  CP0 registers  */
#define CP0_INDEX_REG						$0
#define CP0_RANDOM_REG						$1
#define CP0_ENTRYLO0_REG					$2
#define CP0_ENTRYLO1_REG					$3
#define CP0_CONTEXT_REG						$4
#define CP0_PAGEMASK_REG					$5
#define CP0_WIRED_REG						$6

#define CP0_BADVADDR_REG					$8
#define CP0_COUNT_REG						$9
#define CP0_ENTRYHI_REG						$10
#define CP0_COMPARE_REG						$11
#define CP0_STATUS_REG						$12
#define CP0_CAUSE_REG						$13
#define CP0_EPC_REG							$14
#define CP0_PRID_REG						$15
#define CP0_CONFIG0_REG						$16
#define CP0_CONFIG0_SEL						0
#define CP0_CONFIG1_REG						$16
#define CP0_CONFIG1_SEL						1
#define CP0_LLADDR_REG						$17
#define CP0_WATCHLO_REG						$18
#define CP0_WATCHHI_REG						$19

#define CP0_DEBUG_REG						$23
#define CP0_DEPC_REG						$24

#define CP0_ERRCTL_REG						$26

#define CP0_TAGLO_REG						$28
#define CP0_TAGHI_REG						$29

#define CP0_ERROREPC_REG					$30
#define CP0_DESAVE_REG						$31


/* CP0_INDEX_REG */
#define CP0_INDEX_MASK						0xF

/* CP0_RANDOM_REG */
#define CP0_RANDOM_MASK						0xF

/* CP0_ENTRYLO0_REG/ CP0_ENTRYLO1_REG */
#define TLBENTRY_GLOBAL					0x0001
#define TLBENTRY_VALID					0x0002
#define TLBENTRY_DIRTY					0x0004

#define TLBENTRY_COHERENCE_MASK			0x0038
#define TLBENTRY_CACHEABLE				0x0018
#define TLBENTRY_UNCACHED				0x0010


/* CP0_STATUS_REG */
#define CP0_STATUS_IE_BIT				0x1
#define CP0_STATUS_EXL_BIT				(0x1 << 1)
#define CP0_STATUS_ERL_BIT				(0x1 << 2)

#define CP0_STATUS_UM_BIT				(0x1 << 4)
#define CP0_STATUS_IM_SHIFT				8
#define CP0_STATUS_IM_MASK				(0xff << CP0_STATUS_IM_SHIFT)
#define CP0_STATUS_NMI_BIT				(0x1 << 19)
#define CP0_STATUS_SR_BIT				(0x1 << 20)
#define CP0_STATUS_TS_BIT				(0x1 << 21)
#define CP0_STATUS_BEV_BIT				(0x1 << 22)
#define CP0_STATUS_RE_BIT				(0x1 << 25)
#define CP0_STATUS_RP_BIT				(0x1 << 27)
#define CP0_STATUS_CU0_BIT				(0x1 << 28)
#define CP0_STATUS_CU1_BIT				(0x1 << 29)
#define CP0_STATUS_CU2_BIT				(0x1 << 30)
#define CP0_STATUS_CU3_BIT				(0x1 << 31)

#define CP0_STATUS_DEFAULT				(CP0_STATUS_BEV_BIT)

/* CP0_CAUSE_REG */
#define CP0_CAUSE_EXCCODE_SHIFT			2
#define CP0_CAUSE_EXCCODE_MASK			(0x3F << CP0_CAUSE_EXCCODE_SHIFT)
#define CP0_CAUSE_IP_SHIFT				8
#define CP0_CAUSE_IP_MASK				(0xFF << CP0_CAUSE_IP_SHIFT)

#define CP0_CAUSE_WP_BIT				(0x1 << 22)
#define CP0_CAUSE_IV_BIT				(0x1 << 23)

#define CP0_CAUSE_CE_SHIFT				28
#define CP0_CAUSE_CE_MASK				(0x3 << CP0_CAUSE_CE_SHIFT)

#define CP0_CAUSE_BD_BIT				(0x1 << 31)

/* Exception Code */
#define EXCCODE_INT						0
#define EXCCODE_TLBMOD					1
#define EXCCODE_TLBL					2
#define EXCCODE_TLBS					3
#define EXCCODE_ADEL					4
#define EXCCODE_ADES					5
#define EXCCODE_IBE						6
#define EXCCODE_DBE						7
#define EXCCODE_SYS						8
#define EXCCODE_BP						9
#define EXCCODE_RI						10
#define EXCCODE_COPROCESSOR				11
#define EXCCODE_OVERFLOW				12
#define EXCCODE_TRAP					13

#define EXCCODE_WATCH					23
#define EXCCODE_MACHINE_CHECK			24


/* CP0_CONFIG0_REG */
#define CP0_CONFIG0_M_BIT				(1 << 31)

#define CP0_CONFIG0_MM_SHIFT			17
#define CP0_CONFIG0_MM_MASK				(0x3 << CP0_CONFIG0_MM_SHIFT)
#define CP0_CONFIG0_MM_NOMERGE			0
#define CP0_CONFIG0_MM_SYSADMERGE		(0x1 << CP0_CONFIG0_MM_SHIFT)
#define CP0_CONFIG0_MM_FULLMERGE		(0x2 << CP0_CONFIG0_MM_SHIFT)

#define CP0_CONFIG0_BM					(1 << 16)
#define CP0_CONFIG0_BE					(1 << 15)

#define CP0_CONFIG0_K0COHERENCY_MASK	0x7

#ifdef KSEG0_UNCACHED
#define CP0_CONFIG0_K0COHERENCY_DEFAULE	CACHECTRL_UNCACHED
#else
#define CP0_CONFIG0_K0COHERENCY_DEFAULE	CACHECTRL_WRITE_THR_MODE
#endif

#define CP0_CONFIG0_DEFAULT				CP0_CONFIG0_K0COHERENCY_DEFAULE


/*  CP0_CONFIG1_REG  */
#define CP0_CONFIG1_M_BIT				(1 << 31)
#define CP0_CONFIG1_MMU_SIZE_SHIFT		25
#define CP0_CONFIG1_MMU_SIZE_MASK		(0x3f << CP0_CONFIG1_MMU_SIZE_SHIFT)

#define CP0_CONFIG1_IS_SHIFT			22
#define	CP0_CONFIG1_IS_MASK				(0x7 << CP0_CONFIG1_IS_SHIFT)

#define CP0_CONFIG1_IL_SHIFT			19
#define CP0_CONFIG1_IL_MASK				(0x7 << CP0_CONFIG1_IL_SHIFT)

#define CP0_CONFIG1_IA_SHIFT			16
#define CP0_CONFIG1_IA_MASK				(0x7 << CP0_CONFIG1_IA_SHIFT)

#define CP0_CONFIG1_DS_SHIFT			13
#define	CP0_CONFIG1_DS_MASK				(0x7 << CP0_CONFIG1_DS_SHIFT)

#define CP0_CONFIG1_DL_SHIFT			10
#define CP0_CONFIG1_DL_MASK				(0x7 << CP0_CONFIG1_DL_SHIFT)

#define CP0_CONFIG1_DA_SHIFT			7
#define CP0_CONFIG1_DA_MASK				(0x7 << CP0_CONFIG1_DA_SHIFT)


/*  Cache instruction op code */
#define CACHE_OPCODE(oper, target)		(((oper) << 2) | (target))

// target code
#define CACHEOP_TGT_I					0
#define CACHEOP_TGT_D					1

// cache operation code
#define OPER_INDEX_INVAL				0
#define OPER_INDEX_LOADTAG				1
#define OPER_INDEX_STORETAG				2
#define OPER_HIT_INVAL					4
#define OPER_FILL						5
#define OPER_FETCH_LOCK					7

#define ICACHE_INDEX_INVALIDATE		CACHE_OPCODE(OPER_INDEX_INVAL, CACHEOP_TGT_I)
#define ICACHE_INDEX_LOADTAG		CACHE_OPCODE(OPER_INDEX_LOADTAG, CACHEOP_TGT_I)
#define ICACHE_INDEX_STORETAG		CACHE_OPCODE(OPER_INDEX_STORETAG, CACHEOP_TGT_I)
#define ICACHE_HIT_INVALIDATE		CACHE_OPCODE(OPER_HIT_INVAL, CACHEOP_TGT_I)
#define DCACHE_INDEX_INVALIDATE		CACHE_OPCODE(OPER_INDEX_INVAL, CACHEOP_TGT_D)
#define DCACHE_INDEX_LOADTAG		CACHE_OPCODE(OPER_INDEX_LOADTAG, CACHEOP_TGT_D)
#define DCACHE_INDEX_STORETAG		CACHE_OPCODE(OPER_INDEX_STORETAG, CACHEOP_TGT_D)
#define DCACHE_HIT_INVALIDATE		CACHE_OPCODE(OPER_FILL, CACHEOP_TGT_D)

/*  Exception vector  */
#define MIPS_TLBRIFLL_VECTOR			0x80000000
#define MIPS_GENERAL_VECTOR				0x80000180
#define MIPS_INTERRUPT_VECTOR			0x80000200

/*  Kernel mode VA Memory Map  */
#define MEM_KUSEG_BASE					0x0
#define MEM_KSEG0_BASE					0x80000000
#define MEM_KSEG1_BASE					0xA0000000
#define MEM_KSEG2_BASE					0xC0000000
#define MEM_KSEG3_BASE					0xE0000000
#define MEM_SEG_MASK					0xE0000000

/* Debug mode VA */
#define MEM_DSEG_BASE					0xFF200000
#define MEM_DMSEG_BASE					0xFF200000
#define MEM_DRSEG_BASE					0xFF300000
#define MEM_DMSEG_TOP					MEM_DRSEG_BASE
#define MEM_DRSEG_TOP					0xFF400000


#define PA2VA(_addr)					((_addr) + MEM_KSEG1_BASE)
#define PA2CACHEVA(_addr)				((_addr) + MEM_KSEG0_BASE)
#define KVA2PA(_addr)					((_addr) & ~MEM_SEG_MASK)
#define MIPS_KSEG0A(_addr)				(KVA2PA(_addr) | MEM_KSEG0_BASE)
#define MIPS_KSEG1A(_addr)				(KVA2PA(_addr) | MEM_KSEG1_BASE)

#endif
