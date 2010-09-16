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
;    File    : banner.h
;    Abstract: defines the specific items that loader nedded.
;
;Modification History:
;       By              Date     Ver.   Modification Description
;       --------------- -------- -----  --------------------------------------
;
;*****************************************************************************/
#define	CPU_NO			"5120"
#define	CPU_CLOCK		"175MHz"
#if SDRAM_BANK0_SIZE == 4
	#define	SDRAM_SIZE		"8MB"
#elif SDRAM_BANK0_SIZE == 8
	#define	SDRAM_SIZE		"16MB"
#elif SDRAM_BANK0_SIZE == 16
	#define	SDRAM_SIZE		"32MB"
#elif SDRAM_BANK0_SIZE == 64
	#define	SDRAM_SIZE		"64MB"
//#elif SDRAM_BANK0_SIZE == 64
//	#define	SDRAM_SIZE		"128MB"
#elif SDRAM_BANK0_SIZE == 128
	#define	SDRAM_SIZE		"256MB"
#elif SDRAM_BANK0_SIZE == 256
	#define	SDRAM_SIZE		"512MB"
#endif
#ifdef	NOR_FLASH
	#define	FLASH_TYPE		"NOR"
	#ifdef 	FLASH_4M
		#define	FLASH_SIZE		"4MB"
	#endif
	#ifdef 	FLASH_2M
		#define	FLASH_SIZE		"2MB"
	#endif
#else
	#define	FLASH_TYPE		"NAND"
	#ifdef K9F5608U0B
		#define	FLASH_SIZE		"32MB"
	#endif
	#ifdef K9F6408U0C
		#define	FLASH_SIZE		"8MB"
	#endif
#endif
#define	BOOT_SYSTEM		"Linux-5120"
#define	LOADER_VERSION		"1.00.03"
#define	CREATE_DATE		"2004.06.04"

