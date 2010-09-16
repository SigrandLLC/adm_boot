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
;    Project : Common Plateform
;    Creator :
;    File    : nf.c
;    Abstract: defines the specific items that loader nedded.
;
;Modification History:
;       By              Date     Ver.   Modification Description
;       --------------- -------- -----  --------------------------------------
;
;*****************************************************************************/
#include <ctype.h>
#include "nand.h"

int nf_erase(UINT8 *addr, UINT32 len, UINT32 scan)
{
	return nand_erase(addr,len, scan);
}

int nf_write(UINT8 *dst, UINT8 *src, UINT32 len)
{
	UINT32	retlen;
	return nand_write_ecc(dst, len, &retlen, src);
}

int nf_write_boot(UINT8 *dst, UINT8 *src, UINT32 len)
{
	UINT32	retlen;
	nand_write_boot(dst, NAND_BOOT_SIZE, &retlen, src);
	return nand_write_ecc(&dst[NAND_BOOT_SIZE], len - NAND_BOOT_SIZE, &retlen, &src[NAND_BOOT_SIZE]);
}

int nf_read(UINT8 *dst, UINT8 *src, UINT32 len)
{
	UINT32	retlen;
	return nand_read_ecc(src, len, &retlen, dst);
}

