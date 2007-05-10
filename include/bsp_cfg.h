/*****************************************************************************
;
;   (C) Unpublished Work of ADMtek Technology,  Corp.  All Rights Reserved.
;
;       THIS WORK IS AN UNPUBLISHED WORK AND CONTAINS CONFIDENTIAL,
;       PROPRIETARY AND TRADESECRET INFORMATION OF ADMTEK TECHNOLOGY CORP.
;       ACCESS TO THIS WORK IS RESTRICTED TO (I) ADMTEK EMPLOYEES WHO HAVE A
;       NEED TO KNOW TO PERFORM TASKS WITHIN THE SCOPE OF THEIR ASSIGNMENTS
;       AND (II) ENTITIES OTHER THAN ADMTEK WHO HAVE ENTERED INTO APPROPRIATE
;       LICENSE AGREEMENTS.  NO PART OF THIS WORK MAY BE USED, PRACTICED,
;       PERFORMED, COPIED, DISTRIBUTED, REVISED, MODIFIED, TRANSLATED,
;       ABBRIDGED, CONDENSED, EXPANDED, COLLECTED, COMPILED, LINKED, RECAST,
;       TRANSFORMED OR ADAPTED WITHOUT THE PRIOR WRITTEN CONSENT OF ADMTEK.
;       ANY USE OR EXPLOITATION OF THIS WORK WITHOUT AUTHORIZATION COULD
;       SUBJECT THE PERPERTRATOR TO CRIMINAL AND CIVIL LIABILITY.
;------------------------------------------------------------------------------
;
;    Project : Common plateform
;    Creator : 
;    File    : bsp_cfg.h
;    Abstract: function library for application tasks.
;
;Modification History:
;       By              Date     Ver.   Modification Description
;       --------------- -------- -----  --------------------------------------
;
;*****************************************************************************/

#ifndef __BSP_CFG_H__
#define __BSP_CFG_H__

#define BSP_STR_LEN         	79
#define BSP_MAX_MAC_NUM		8
#define BSP_MAX_IF_CNT		BSP_MAX_MAC_NUM

typedef struct BSP_IF_CFG_S
{
	unsigned long ip;
	unsigned long mask;
	unsigned long gateway;
} BSP_IF_CFG_T;


typedef struct IF_CFG_S
{
	char ifname[BSP_IFNAME_MAX_LEN+1];
	UINT32 ip;
	UINT32 mask;
	UINT32 gateway;
} IF_CFG_T;


enum BOOT_MODE
{
	BTMODE_NORMAL,
	BTMODE_DBG
};

enum DOWNLOAD_METHOD
{
    	DLMETHOD_XMODEM,
   	DLMETHOD_TFTPS,
	DLMETHOD_TFTPC
};

typedef struct BTMODE_CFG_S
{
	UINT32 btmode;
	UINT32 dlmethod;
} BTMODE_CFG_T;

typedef struct TFTP_CFG_S
{
	UINT32 local_ip;
	UINT32 server_ip;
	//char remote_file[15];
	char remote_file_boot[15];
	char remote_file_sys[15];
}TFTP_CFG_T;

typedef struct BOARD_CFG_S
{
	UINT32 blmagic;
	char bootline[BOOT_LINE_SIZE+1];
	UINT32 macmagic;
    	UINT8 mac[8];
    	UINT32 macnum;

	UINT32 idmagic;    
   	UINT8 serial[BSP_STR_LEN+1];

  	UINT32 vermagic;
  	UINT8 ver[BSP_STR_LEN+1];

	UINT32 ifmagic;
	IF_CFG_T ifcfg[8];
	
	UINT32 btmagic;
	BTMODE_CFG_T bootmode;

	UINT32 tftploipmagic;
	UINT32 tftpmagic;
	TFTP_CFG_T tftp_param;	//For bootloader only, winder+
	
}
BOARD_CFG_T;

#define BL_MAGIC			0x6c62676d
#define MAC_MAGIC			0x636d676d
#define VER_MAGIC			0x7276676d
#define ID_MAGIC			0x6469676d
#define IF_MAGIC			0x6669676d

#define BT_MAGIC			0x6c74676d

#define TFTPMAGIC			0x6c76676d
#define TFTPLOIPMAGIC			0x6c76676c

#endif

