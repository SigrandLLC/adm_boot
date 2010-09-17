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
;    Project : Am5120
;    Creator : Jeanson Hung
;    File    : nand.h
;    Abstract:
;
;Modification History:
;
;
;*****************************************************************************/
#include "adm5120.h"

#ifndef __NAND_H__
#define __NAND_H__

/*typedef unsigned char UINT8;
typedef unsigned long UINT32;*/

#define NAND_REG_BASE	PA2VA(ADM5120_SMEM1_BASE)

// NAND boot initial size, 2K for 5120
#define NAND_BOOT_SIZE	0x800

#define K9F5608U0B

#ifdef K9F5608U0B
#define NAND_FLASH_SIZE		0x2000000	//32MB+1MB // revised by ProChao, 10/8/2003
#define NAND_PAGE_SIZE		0x200		//512+16Bytes
#define	NAND_PAGE_OOB_SIZE	0x10		//16Bytes
#define NAND_BLK_PER_PAGE	0x20		//32 pages per block
#define NAND_SIZE_PER_BLK	0x4000		//size per block
#define NAND_FLASH_BLOCK_NO	2048		//2048 blocks per device
#endif

/* check NAND flash parameters */
#if NAND_SIZE_PER_BLK == 0
#error DID NOT DEFINE THE CORRECT NAND FLASH PARAMETERS!!!
#endif


#define CMD_READ1	0x0
#define CMD_READ2	0x1
#define CMD_READ3	0x50
#define CMD_ERASE1	0x60
#define CMD_ERASE2	0xD0
#define CMD_WRITE1	0x80
#define CMD_WRITE2	0x10

// ported by ProChao, 10/8/2003, from Linux platform
#define NAND_CMD_READ0		0x00
#define NAND_CMD_READ1		0x01
#define NAND_CMD_PAGEPROG	0x10
#define NAND_CMD_READOOB	0x50
#define NAND_CMD_ERASE1		0x60
#define NAND_CMD_STATUS		0x70
#define NAND_CMD_SEQIN		0x80
#define NAND_CMD_READID		0x90
#define NAND_CMD_ERASE2		0xd0
#define NAND_CMD_RESET		0xff

//
#define NAND_BADBLOCK_POS	5

//
#define ADD1MASK	0x000000ff
#define ADD2MASK	0x0001fe00
#define ADD3MASK	0x01fe0000
#define ADD2SHIFT	0x9
#define ADD3SHIFT	0x11

#define NAND_ECC
// ported by ProChao, 10/9/2003
/*
 * Enumeration for NAND flash chip state
 */
typedef enum {
	FL_READY,
	FL_READING,
	FL_WRITING,
	FL_ERASING,
	FL_SYNCING
} nand_state_t;

int  nand_read (UINT8 *dst, UINT8 *src, UINT32 len);
int  nand_write(UINT8 *dst, UINT8 *src, UINT32 size);
int  nand_erase(UINT8 *addr, UINT32 len, UINT32 scan);
int  nand_erase_block(UINT32 page);
int  nand_write_boot (UINT8 *to, UINT32 len, UINT32 *retlen, const UINT8 * buf);
int  nand_write_boot1(UINT8 *dst, UINT8 *src, UINT32 size);
void nand_calculate_ecc(const u_char *dat, u_char *ecc_code);
int  nand_write_ecc(UINT8 *to, UINT32 len, UINT32 *retlen, const UINT8 * buf);
int  nand_read_ecc(UINT8 *from, UINT32 len, UINT32 *retlen, UINT8 *buf);
int  nand_correct_data(u_char *dat, u_char *read_ecc, u_char *calc_ecc);

void check_bad_block(void);
void scan_bad_blocks(UINT8 *addr);
void bad_block(void);

int  nf_erase(UINT8 *addr, UINT32 len, UINT32 scan);
int  nf_write(UINT8 *dst, UINT8 *src, UINT32 len);
int  nf_write_boot(UINT8 *dst, UINT8 *src, UINT32 len);
int  nf_read(UINT8 *dst, UINT8 *src, UINT32 len);

#endif

