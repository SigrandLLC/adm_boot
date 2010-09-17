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
;    File    : param.h
;    Abstract: defines the specific items that loader nedded.
;
;Modification History:
;       By              Date     Ver.   Modification Description
;       --------------- -------- -----  --------------------------------------
;
;*****************************************************************************/
#define PRINT_PARAM			"\n\n\rPrint Boot Parameters.\n\r==========================\n\r"
#define SET_PARAM			"\n\n\rSet Boot Parameters.\n\r==========================\n\r"
#define BSP_IFNAME_MAX_LEN	15
#define HOST_IP_FLAG		0
#define NETMASK_FLAG		1
#define GATEWAY_IP_FLAG		2
/******************************************************************************************/

void Set_Board_SerialNo(void);
void Set_Board_Version(void);
void Set_Mac(void);
void Set_If_config(void);
int  check_ip(unsigned long,int);
void print_tftpc_param(void);
int  set_tftpc_param(void);
int  boot_param_init(void);
int  set_boot_param(void);
void PrintBspParam(void);
int  bsp_GetMacBase(UINT8 *buf, int *macnum);
int  bsp_GetTftpIp(UINT32 *tftpip);
int  bsp_GetGwIp(UINT32 *gwip);
int  get_tftp_param(UINT32 *servip, UINT32 *gwip, char *boot_file, char *sys_file);

