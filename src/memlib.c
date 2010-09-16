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
;    File    : memlib.c
;    Abstract:
;
;Modification History:
;
;
;*****************************************************************************/
#include <ctype.h>
#include <mips.h>
#include <mips4kc.h>
#include <adm5120.h>
#include <except.h>
#include <linuxld.h>

void *MemAlloc(UINT32 NumOfByte, UINT32 clear);
void *SharedMemAlloc(UINT32 NumOfByte, UINT32 clear);


/*********  Global Variables ********/
#define MEM_ALIGN				4
#define SHARED_MEM_ALIGN		16

#define MEM_HEAP_PAEND			LINUXLD_MEMPOOL_STATR+LINUXLD_MEMPOOL_SIZE


void *_heap_h;

UINT32 AvilMemHead;


/**********************************************************************************/
/* MemAlloc:																	  */
/**********************************************************************************/
void *MemAlloc(UINT32 NumOfByte, UINT32 clear)
{
	int s;
	UINT32 MemStart, MemEnd;

	// Critical section starts. Disable the interrupts
	s = mips_int_lock();

	MemEnd = AvilMemHead + NumOfByte;

	if(MemEnd & (MEM_ALIGN - 1))
		MemEnd = (MemEnd + MEM_ALIGN -1) & ~(MEM_ALIGN - 1);

	if(MemEnd >= MEM_HEAP_PAEND)
	{
		mips_int_unlock(s);
		return NULL;
	}

	MemStart = AvilMemHead;
    AvilMemHead = MemEnd;

	// Critical section ends. Restore the interrupts mode
	mips_int_unlock(s);

	MemStart = MIPS_KSEG0A(PA2VA(MemStart));

#ifndef RTL_SIMULATION
	if(clear)
	{
		memset((void *)MemStart, 0, NumOfByte);
	}
#endif

	return (void *)MemStart;
}



/**********************************************************************************/
/* SharedMemAlloc:																  */
/**********************************************************************************/
void *SharedMemAlloc(UINT32 NumOfByte, UINT32 clear)
{
	int s;
	UINT32 MemStart, MemEnd;


	// Critical section starts. Disable the interrupts
	s = mips_int_lock();

	// Align the start address to cache line boundry
	if(AvilMemHead & (SHARED_MEM_ALIGN - 1))
		AvilMemHead = (AvilMemHead + SHARED_MEM_ALIGN - 1) & ~(SHARED_MEM_ALIGN - 1);

	MemEnd = AvilMemHead + NumOfByte;

	// !! Always keep the shared memory block aligned on cache line boundry
	if(MemEnd & (SHARED_MEM_ALIGN - 1))
		MemEnd = (MemEnd + SHARED_MEM_ALIGN - 1) & ~(SHARED_MEM_ALIGN - 1);

	if(MemEnd >= MEM_HEAP_PAEND)
	{
		mips_int_unlock(s);
		return NULL;
	}

	MemStart = AvilMemHead;
    AvilMemHead = MemEnd;

	// Critical section ends. Restore the interrupts mode
	mips_int_unlock(s);

	MemStart = MIPS_KSEG1A(PA2VA(MemStart));

#ifndef RTL_SIMULATION
	if(clear)
	{
		memset((void *)MemStart, 0, NumOfByte);
	}
#endif

	return (void *) MemStart;
}




/****************************************************************************/
/*  memlib_init:															*/
/****************************************************************************/
void memlib_init(void)
{
	int s, i;
	unsigned long reg_offset, reg;
	_heap_h=(void *)LINUXLD_MEMPOOL_STATR;
	s = mips_int_lock();

	// map VA to PA
	AvilMemHead = KVA2PA((UINT32)_heap_h);

	if(AvilMemHead & (MEM_ALIGN - 1))
	{
		AvilMemHead = (AvilMemHead + MEM_ALIGN -1 )	& ~(MEM_ALIGN - 1);
	}

	mips_int_unlock(s);
}
