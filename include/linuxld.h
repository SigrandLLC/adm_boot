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
;    Creator : Daniel
;    File    : linuxld.h
;    Abstract: defines the specific items that loader nedded.
;
;Modification History:
;       By              Date     Ver.   Modification Description
;       --------------- -------- -----  --------------------------------------
;
;*****************************************************************************/
#if __cplusplus
extern "C" {
#endif

#ifndef _LINUXLD_H_
#define _LINUXLD_H_

#include <hw5120cfg.h>


/******************************************************************************************
#### System Map (SDRAM 16 Mbyte at least) ####

      0x0           +------------------------------------+	LINUXLD_SDRAM_START
                    |     Exception (8 KB)				 |
      0x2000        +------------------------------------+	LINUXLD_KERNEL_START
                    |     Linux Kernel                   |
                    |       Code & Data Segment          |
                    |       (6 MB)                       |   
                    |                                    |
                    |                                    |
      0x600000      +------------------------------------+	LINUXLD_MEMPOOL_STATR
                    |     Bootloader memory pool         |
                    |       (2 MB)                       |
                    |                                    |
      0x800000      +------------------------------------+	LINUXLD_LOADER_START
                    |     Boot loader                    | 
                    |       Code & Data Segement         |
                    |       (64 KB)                      | 
      0x810000      +------------------------------------+
                    |	  Reserve Area (64KB)			 |
                    |									 |
      0x820000      +------------------------------------+	LINUXLD_DOWNLOAD_START
                    |     Download Buffer                |       
                    |       (4 MB)                       |
                    |                                    |
                    |                                    |
      0xc00000      +------------------------------------+	LINUXLD_GZIP_HEAP
                    |     gzip heap                      |
                    |       (3 MB)                       |
                    |                                    |
                    |                                    |
      0x1000000     +------------------------------------+
*******************************************************************************************/
#define LINUXLD_EXCEP_SIZE		0x2000
#define LINUXLD_KERNEL_SIZE		(0x600000-LINUXLD_EXCEP_SIZE) 	//0x5fe000
#define LINUXLD_MEMPOOL_SIZE		0x200000
#define LINUXLD_LOADER_SIZE		0x20000
#define LINUXLD_RESERVE_SIZE		0x00000
#define LINUXLD_DOWNLOAD_SIZE		0x400000

#define LINUXLD_SDRAM_START		PA2VA(ADM5120_SDRAM_BASE)
#define LINUXLD_KERNEL_START		(LINUXLD_SDRAM_START + LINUXLD_EXCEP_SIZE)
#define LINUXLD_MEMPOOL_STATR		(LINUXLD_KERNEL_START + LINUXLD_KERNEL_SIZE)
#define LINUXLD_LOADER_START		(LINUXLD_MEMPOOL_STATR + LINUXLD_MEMPOOL_SIZE)
#define LINUXLD_DOWNLOAD_START		(LINUXLD_LOADER_START + LINUXLD_LOADER_SIZE + LINUXLD_RESERVE_SIZE)
#define LINUXLD_GZIP_HEAP		(LINUXLD_DOWNLOAD_START + LINUXLD_DOWNLOAD_SIZE)

/*******************************************************************************************
#### System Map (NOR Flash) ####

                    +------------------------------------+	LINUXLD_NORFLASH_LOADER_START
                    |     Boot loader (64 KB)            |
                    +------------------------------------+	LINUXLD_NORFLASH_BOOTPARAM_START
                    |     System Parameter (64 KB)       |
                    +------------------------------------+	LINUXLD_NORFLASH_KERNEL_START
                    |                                    |
                    |                                    |
                    |     Linux Kernel (4MB)               |
                    ~                                    ~
                    ~                                    ~
                    |                                    |   
                    |                                    |
                    +------------------------------------+
*********************************************************************************************/
#define LINUXLD_NORFLASH_LOADER_SIZE	     	0x10000
#define LINUXLD_NORFLASH_BOOTPARAM_SIZE  	0x10000
#define LINUXLD_NORFLASH_KERNEL_SIZE	0x400000	//Kernel size max 4MB
/*#define LINUXLD_NORFLASH_LINUXNV_SIZE		0x20000
 #define LINUXLD_NORFLASH_KERNEL_SIZE		HWPF_5120_SMEM0_BANK_SIZE - LINUXLD_NORFLASH_LOADER_SIZE - \
							LINUXLD_NORFLASH_BOOTPARAM_SIZE */

#define LINUXLD_NORFLASH_LOADER_START		PA2VA(ADM5120_SMEM0_BASE)
#define LINUXLD_NORFLASH_BOOTPARAM_START	(LINUXLD_NORFLASH_LOADER_START + LINUXLD_NORFLASH_LOADER_SIZE)
#define LINUXLD_NORFLASH_KERNEL_START		(LINUXLD_NORFLASH_BOOTPARAM_START + LINUXLD_NORFLASH_BOOTPARAM_SIZE)

/********************************************************************************************
#### System Map (NAND Flash) ####

					+-------------------------+  LINUXLD_NANDFLASH_LOADER_START
					|	Linuxld (64k)		  |
					+-------------------------+  LINUXLD_NANDFLASH_BOOTPARAM_START
					|	Boot param (64k)	  |
					+-------------------------+  LINUXLD_NANDFLASH_KERNEL_START
					|						  |
					|	Linux kernel (4MB)	  |
					|						  |
					+-------------------------+	
*********************************************************************************************/

#define LINUXLD_NANDFLASH_LOADER_SIZE		0x14000
#define LINUXLD_NANDFLASH_BOOTPARAM_SIZE	0x0C000
#define LINUXLD_NANDFLASH_KERNEL_SIZE	0x200000	//Kernel size max 4MB
/*#define LINUXLD_NANDFLASH_LINUXNV_SIZE        0x40000
#define LINUXLD_NANDFLASH_KERNEL_SIZE		\
	NAND_FLASH_SIZE - LINUXLD_NANDFLASH_LOADER_SIZE - LINUXLD_NANDFLASH_BOOTPARAM_SIZE*/

#define LINUXLD_NANDFLASH_LOADER_START		0x0
#define LINUXLD_NANDFLASH_BOOTPARAM_START	(LINUXLD_NANDFLASH_LOADER_START + LINUXLD_NANDFLASH_LOADER_SIZE)
#define LINUXLD_NANDFLASH_KERNEL_START		(LINUXLD_NANDFLASH_BOOTPARAM_START + LINUXLD_NANDFLASH_BOOTPARAM_SIZE)


/*------------------------------------------------------------------------------------------*/
/* Flash Map Definition																		*/
/*------------------------------------------------------------------------------------------*/
#ifdef	NOR_FLASH
	#define LINUXLD_FLASH_LOADER_SIZE	LINUXLD_NORFLASH_LOADER_SIZE
	#define LINUXLD_FLASH_BOOTPARAM_SIZE	LINUXLD_NORFLASH_BOOTPARAM_SIZE
	//#define LINUXLD_FLASH_LINUXNV_SIZE	LINUXLD_NORFLASH_LINUXNV_SIZE
	#define LINUXLD_FLASH_KERNEL_SIZE	LINUXLD_NORFLASH_KERNEL_SIZE
	
	#define LINUXLD_FLASH_LOADER_START	LINUXLD_NORFLASH_LOADER_START
	#define LINUXLD_FLASH_BOOTPARAM_START	LINUXLD_NORFLASH_BOOTPARAM_START
	#define LINUXLD_FLASH_KERNEL_START	LINUXLD_NORFLASH_KERNEL_START
#else
	#define LINUXLD_FLASH_LOADER_SIZE	LINUXLD_NANDFLASH_LOADER_SIZE
	#define LINUXLD_FLASH_BOOTPARAM_SIZE	LINUXLD_NANDFLASH_BOOTPARAM_SIZE
	//#define LINUXLD_FLASH_LINUXNV_SIZE	LINUXLD_NANDFLASH_LINUXNV_SIZE
	#define LINUXLD_FLASH_KERNEL_SIZE	LINUXLD_NANDFLASH_KERNEL_SIZE

	#define LINUXLD_FLASH_LOADER_START	LINUXLD_NANDFLASH_LOADER_START
	#define LINUXLD_FLASH_BOOTPARAM_START	LINUXLD_NANDFLASH_BOOTPARAM_START
	#define LINUXLD_FLASH_KERNEL_START	LINUXLD_NANDFLASH_KERNEL_START
#endif

/*------------------------------------------------------------------------------------------*/

/********************************************************************************************
 * Add for VINETIC project																	*
*********************************************************************************************/
#define LINUXLD_NORFLASH_REMAIN     (0x200000 - LINUXLD_NORFLASH_LOADER_SIZE - \
        LINUXLD_NORFLASH_BOOTPARAM_SIZE)

#define LINUXLD_NORFLASH_MAP_SIZE   0x200000

/********************************************************************************************/
#endif /* _LINUXLD_H */

#if __cplusplus
}
#endif
