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
;    Project : Am5106/5107
;    Creator : David Weng
;    File    : hwconfig.h
;    Abstract:
;
;Modification History:
;
;
;*****************************************************************************/

#ifndef __HWCONFIG_H__
#define __HWCONFIG_H__

#ifndef _HW_PROFILE_H_
#include <hw_profile.h>
#endif

//===================================
//  SMC configuration
#define RESET_ROM_START 		HWPF_RESET_FLASH1_START

#ifndef HWPF_FLASH_DEV_SIZE
#error HWPF_FLASH_DEV_SIZE must be defined first!!
#endif


#if (HWPF_FLASH_DEV_SIZE <= 0x40000)
#define ROM_DEVICE_SIZE			ROM_SIZE_256KBYTES
#elif (HWPF_FLASH_DEV_SIZE <= 0x80000)
#define ROM_DEVICE_SIZE			ROM_SIZE_512KBYTES
#elif (HWPF_FLASH_DEV_SIZE <= 0x100000)
#define ROM_DEVICE_SIZE			ROM_SIZE_1MBYTES
#elif (HWPF_FLASH_DEV_SIZE <= 0x200000)
#define ROM_DEVICE_SIZE			ROM_SIZE_2MBYTES
#else
#error	Flashs which size is larger than 2M bytes are not supported!!
#endif

#define SMCBCR0_IDCY			0x0f
#define SMCBCR0_WST1			0x1f
#define SMCBCR0_WST2			0x1f


#define AM5106_SMCBCR1_DEFAULE  0x2000ffef

//===================================
//	SDRAM configuration

// SDRAM refresh time
#define AM5106_SDRAM_RFTIME		800

#ifndef HWPF_SDRAM_CAS_LATENCY
#error HWPF_SDRAM_CAS_LATENCY must be defined first!!
#endif

#ifndef HWPF_SDRAM_RAS_LATENCY
#error HWPF_SDRAM_RAS_LATENCY must be defined first!!
#endif



#if (HWPF_SDRAM_CAS_LATENCY == 2)
#define SDRAM_CAS_LATENCY		SDRAM_CAS_LATENCY_2
#define SDC_CAS_LATENCY			SDC_CAS_LATENCY_2
#elif (HWPF_SDRAM_CAS_LATENCY == 3)
#define SDRAM_CAS_LATENCY		SDRAM_CAS_LATENCY_3
#define SDC_CAS_LATENCY			SDC_CAS_LATENCY_3
#else
#error HWPF_SDRAM_CAS_LATENCY must be either 2 or 3!!
#endif

#if (HWPF_SDRAM_RAS_LATENCY == 1)
#define SDC_RAS_LATENCY			SDC_RAS_LATENCY_2
#elif (HWPF_SDRAM_RAS_LATENCY == 2)
#define SDC_RAS_LATENCY			SDC_RAS_LATENCY_2
#elif (HWPF_SDRAM_RAS_LATENCY == 3)
#define SDC_RAS_LATENCY			SDC_RAS_LATENCY_3
#else
#error HWPF_SDRAM_RAS_LATENCY must be either 1, 2, or 3!!
#endif


/*  Burst length must be 4!!!! */
#define AM5106_SDRAM_MODE_DEFAULT (SDRAM_BTLEN_4		\
								| SDRAM_CAS_LATENCY		\
								| SDRAM_OPMODE_STANDARD \
								| SDRAM_WBTMODE_ENABLE)

#define AM5106_SDCCFG0_COMMON	( SDC_AUTO_PRE_CHARGE	\
								| SDC_RAS_LATENCY		\
								| SDC_CAS_LATENCY)


#if (HWPF_SDRAM_DEV_BUS_WIDTH == 8)
#define SDRAM_DEV_BUS_WIDTH		SDC_8BIT_SDRAM
#else
#define SDRAM_DEV_BUS_WIDTH		0
#endif

#if (HWPF_SDRAM_DEV_BANKS == 4)
#define SDRAM_DEV_BANKS			SDC_4BANK_SDRAM
#else
#define SDRAM_DEV_BANKS			0
#endif

#if (HWPF_SDRAM_DEV_SIZE == 256)
#define SDRAM_DEV_SIZE			SDC_256M_SDRAM
#else
#define SDRAM_DEV_SIZE			0
#endif



#define AM5106_SDCCFG0_DEFAULT	(AM5106_SDCCFG0_COMMON	\
								| SDRAM_DEV_BANKS		\
								| SDRAM_DEV_BUS_WIDTH	\
								| SDRAM_DEV_SIZE)


// SDC write buffer timeout
#define AM5106_WRITEBUF_TIMEOUT 0x40




#endif