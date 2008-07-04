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
;    File    : param.c
;    Abstract: defines the specific items that loader nedded.
;
;Modification History:
;       By              Date     Ver.   Modification Description
;       --------------- -------- -----  --------------------------------------
;
;*****************************************************************************/
#include <ctype.h>
#include <param.h>
#include <uartdrv.h>
#include <bsp_cfg.h>
#include <adm5120.h>
#include <linuxld.h>
#include <mips4kc.h>
/*****************************************************************************************/
void Set_Board_SerialNo(void);
void Set_Board_Version(void);
void Set_Mac(void);
void Set_TFTP_IP(void);

/*****************************************************************************************/
// lnitial boot parameter
static BOARD_CFG_T *cfg;

/*****************************************************************************************/
/*		Parameter Init																	 */
/*****************************************************************************************/
int boot_param_init(void)
{
	int err=-1;
	// Read the Network configuration data.
	cfg = (BOARD_CFG_T *) MemAlloc(sizeof(BOARD_CFG_T), TRUE);
	if (cfg != NULL)
		err=nf_read((char *) cfg, (char *)LINUXLD_FLASH_BOOTPARAM_START, sizeof(BOARD_CFG_T));
	return err;
}

/*****************************************************************************************/
/*		Main Parameter																	 */
/*****************************************************************************************/
int set_boot_param(void)
{	
	UINT8 macaddr[6];
	char *image = (char *)LINUXLD_DOWNLOAD_START;
	
	/* Print Item */
	buart_print(SET_PARAM);	

	/* Set serial number */
	//Set_Board_SerialNo();

	/* Set board version */
	//Set_Board_Version();

	/* Set Mac address */ 	
	Set_Mac();

	/* Set bootloader ip */
	Set_TFTP_IP();

    /* Before Write back, backup original content */
    if (nf_read(image, (char *)LINUXLD_FLASH_BOOTPARAM_START, LINUXLD_FLASH_BOOTPARAM_SIZE) != 0)
    {
        buart_print("\n\rRead buffer error!!");
        return -1;
    }
    memcpy(image, (char *)cfg, sizeof(BOARD_CFG_T));
	
	/* Write back new parameter to flash */
	if (nf_erase((char *)LINUXLD_FLASH_BOOTPARAM_START, LINUXLD_FLASH_BOOTPARAM_SIZE) != 0)
		buart_print("\n\rErase flash error.");
	else if (nf_write((char *)LINUXLD_FLASH_BOOTPARAM_START, image, LINUXLD_FLASH_BOOTPARAM_SIZE) != 0)
		buart_print("\n\rWrite flash error.");
	if (cfg->tftploipmagic == TFTPLOIPMAGIC && cfg->macmagic == MAC_MAGIC)
		arp_add_entry(cfg->mac, cfg->tftp_param.local_ip);

	return 0;
	/* Wait any key */
	//buart_print("\n\r\n\rPlease press any key to continue...");
	//buart_getchar();
}

/******************************************/ 
/*			Board Serial NO				  */
/******************************************/

int bsp_SetBoardSerialNo(UINT8 *s)
{
	int len;
	
	if(s == NULL) return -1;
	if((len=strlen(s)) > BSP_STR_LEN) return -1;
	
	cfg->idmagic = ID_MAGIC;
	strncpy(cfg->serial, s, BSP_STR_LEN+1);
	
	return 0;
}


int bsp_GetBoardSerialNo(UINT8 *buf, int buflen)
{
	int len;

	if(buf == NULL) return -1;

	if(cfg->idmagic != ID_MAGIC) return -1;
	if((len = strlen(cfg->serial)) > buflen) return -1;
	
	strcpy(buf, cfg->serial);
	return 0;
}


void Set_Board_SerialNo(void)
{
	char buf[BOOT_LINE_SIZE+1];
	
	if(bsp_GetBoardSerialNo(buf, BOOT_LINE_SIZE+1) == 0)
	{
		buart_print("Current Serial Number: ");
		buart_print(buf);
		buart_print("\n\r");
	}
	buart_print("Enter new serial number: ");
	buf[0] = 0;
	ReadLine(buf, BOOT_LINE_SIZE);
	if(buf[0] != 0)
	{
		if(bsp_SetBoardSerialNo(buf) != 0)
		{
			buart_print("Failed to change serial number.\n\r");
		}
		else
		{
			buart_print("Serial number updated successfully.\n\r");
		}
	}
	else
	{
		buart_print("Serial Number unchanged.\n\r");
	}
}

/******************************************/ 
/*			Board Version				  */
/******************************************/

int bsp_SetBoardVersion(UINT8 *s)
{
	int len;
	
	if(s == NULL) return -1;
	if((len=strlen(s)) > BSP_STR_LEN) return -1;
	
	cfg->vermagic = VER_MAGIC;
	strncpy(cfg->ver, s, BSP_STR_LEN+1);
		
	return 0;
}

int bsp_GetBoardVersion(UINT8 *buf, int buflen)
{
	int len;

	if(buf == NULL) return -1;

	if(cfg->vermagic != VER_MAGIC) return -1;
	if((len = strlen(cfg->ver)) > buflen) return -1;
	
	strcpy(buf, cfg->ver);
	return 0;
}

void Set_Board_Version(void)
{
	char buf[BOOT_LINE_SIZE+1];
	
	
	if(bsp_GetBoardVersion(buf, BOOT_LINE_SIZE+1) == 0)
	{
		buart_print("Current hardware version: ");
		buart_print(buf);
		buart_print("\n\r");
	}
	buart_print("Enter new hardware version: ");
	buf[0] = 0;
	ReadLine(buf, BOOT_LINE_SIZE);
	if(buf[0] != 0)
	{
		if(bsp_SetBoardVersion(buf) != 0)
		{
			buart_print("Failed to change hardware version.");
		}
		else
		{
			buart_print("Hardware version updated successfully.");
		}
	}
	else
	{
		buart_print("Hardware version unchanged.");
	}
}

/******************************************/ 
/*			Board MAC Addr				  */
/******************************************/

int bsp_SetMac(UINT8 *mac, int macnum)
{
	int len, i;
	UINT32 sum;
	
	if((mac == NULL) || (macnum < 1) || (macnum > BSP_MAX_MAC_NUM))
		return -1;

	// Allow only unicast mac address
	if(mac[0] != 0) return -1;

	cfg->macmagic = MAC_MAGIC;
	
	// have to use memcpy here. 
	memcpy(cfg->mac, mac, 6);
	cfg->mac[7] = cfg->mac[8] = 0;
	cfg->macnum = macnum;

	// Ripple adder
	sum = macnum - 1;
	for(i=5; i>2; i--)
	{
		if((sum += cfg->mac[i]) > 255)
		{
			// ripple carry to the next byte.
			cfg->mac[i] = (unsigned char) (sum &0xff);
			sum >>= 8;
		}
		else
		{
			cfg->mac[i] = (unsigned char) sum;
			break;
		}
	}
	
	if(i == 2) return ERROR;

	// have to use memcpy here. 
	memcpy(cfg->mac, mac, 6);
	
	return 0;
}


int bsp_GetMacBase(UINT8 *buf, int *macnum)
{
	int len;

	if(buf == NULL) return -1;
	
	if(cfg->macmagic != MAC_MAGIC) return -1;
	
	// have to use memcpy here. 
	memcpy(buf, cfg->mac, 6);

	*macnum = (int) cfg->macnum;
	
	return 0;
}

void Set_Mac(void)
{
	char buf[BOOT_LINE_SIZE+1];
	char mac[8];
	char mactmp[]="00-00-00-00-00-00-";
	int macnum, newmacnum, i;
	int flags = 0;

	#define FLAG_OLD_MAC_VALID		1
	#define FLAG_MAC_MODIFIED		2
	#define FLAG_MAC_NUM_MODIFIED	4
	
		
	if(bsp_GetMacBase(mac, &macnum) == 0)
	{
		flags |= FLAG_OLD_MAC_VALID;
		mactostr(mac,mactmp);
		mactmp[17]='\0';
		buart_print("\n\rCurrent MAC address: ");
		buart_print(mactmp);
		buart_print("\n\r");
		buart_print("Number of MAC address: ");
		buart_put(macnum+'0');
	}
	
mac_again:	
	buart_print("\n\rEnter new MAC address (AA-AA-AA-AA-AA-AA): ");
	buf[0] = 0;
	ReadLine(buf, BOOT_LINE_SIZE);
	
	if(buf[0] != 0)
	{
		i = macscanf(mac,buf);
			
		if(i< 6 || mac[0] != 0x00)
		{
			buart_print("Either the format of the input MAC address or the mac address is incorrect.\n\r");
			goto mac_again;
		}
		
		flags |= FLAG_MAC_MODIFIED;
	}

num_again:
	buart_print("Enter new number of MAC address (between 1-8): ");
	newmacnum = buart_getchar();
	buart_put(newmacnum);
	
	if (!((newmacnum == 0x0d)||(newmacnum == 0x0a)))
	{
		newmacnum = newmacnum - '0';
		if(newmacnum < 1 || newmacnum > 8)
		{
			buart_print("\n\rNumber of MAC addresses must be in the range [1-8].\n\r");
			goto num_again;
		}
		
		macnum = newmacnum;
		flags |= FLAG_MAC_NUM_MODIFIED;
	}

	if((flags & FLAG_MAC_MODIFIED) || ((flags & FLAG_OLD_MAC_VALID) && (flags & FLAG_MAC_NUM_MODIFIED)))
	{
		if(!(flags & FLAG_MAC_NUM_MODIFIED) && !(flags & FLAG_OLD_MAC_VALID))
		{
			macnum = 1;
		}
		
		if(bsp_SetMac(mac, macnum) != 0)
		{
			buart_print("\n\rFailed to change MAC address.");
		}
		else
		{
			memcpy(mactmp,cfg->mac,6);
			ProgramMac(0,mactmp);		// Change current MAC addr.
			eth_reinit(mactmp);
			buart_print("\n\rMAC address updated successfully.");
		}
	}
	else
	{
		buart_print("\n\rMAC address unchanged.");
	}
}

void Set_TFTP_IP(void)
{
	UINT32 loip;
	char str[]="xxx.xxx.xxx.xxx";
	char buf[BOOT_LINE_SIZE+1];
	if(cfg->tftploipmagic == TFTPLOIPMAGIC)
	{
		buart_print("\n\rIP address for this board: ");
		IpAddrToStr(cfg->tftp_param.local_ip,str);
		buart_print(str);
	}
loip_again:
	buf[0] = 0;
	buart_print("\n\rEnter new IP address for this board: ");
	ReadLine(buf, BOOT_LINE_SIZE);
	if(buf[0] != 0)
	{
		if(ipscanf(&loip,buf) != 4)
		{
			buart_print("Invalid IP address.\n\r");
			goto loip_again;
		}
		if(check_ip(loip,HOST_IP_FLAG))
		{
			buart_print("Invalid IP address.\n\r");
			goto loip_again;
		}

		cfg->tftp_param.local_ip = loip;
		cfg->tftploipmagic = TFTPLOIPMAGIC;
		ip_reinit(cfg->tftp_param.local_ip);
		udp_init();
		buart_print("IP updated successfully.\n\r");
	}
	else
	{
		buart_print("IP unchanged.\n\r");
	}
}


/******************************************/ 
/*		Print Main Param				  */
/******************************************/
int bsp_GetTftpIp(UINT32 *tftpip)
{
	if(tftpip == NULL) return -1;
	if(cfg->tftploipmagic != TFTPLOIPMAGIC) return -1;
	*tftpip = cfg->tftp_param.local_ip;
	return 0;
}

int bsp_GetGwIp(UINT32 *gwip)
{
	if(gwip == NULL) return -1;
	*gwip = cfg->tftp_param.gw_ip;
	return 0;
}

void PrintBspParam(void)
{
	UINT32 tftpip;
	unsigned char buf[BOOT_LINE_SIZE + 1];
	unsigned char mactmp[]="00-00-00-00-00-00-";
	unsigned char ipstr[]="xxx.xxx.xxx.xxx";
	int macnum;
	
	/* Print Item */
	buart_print(PRINT_PARAM);

	/* Print serial number */
//	if(bsp_GetBoardSerialNo(buf, BOOT_LINE_SIZE+1) != 0)
//	{
//		buf[0] = 0;
//	}
//	buart_print("\n\rSerial number: ");
//	buart_print(buf);

	/* Print board version */
//	if(bsp_GetBoardVersion(buf, BOOT_LINE_SIZE+1) != 0)
//	{
//		buf[0] = 0;
//	}
//	buart_print("\n\rHardware version: ");
//	buart_print(buf);

	/* Print Mac address */
	if(bsp_GetMacBase(buf, &macnum) != 0)
	{
		buart_print("\n\rMac address:");
		buart_print("\n\rNumber of Mac address: 0");
	}
	else
	{
		mactostr(buf,mactmp);
		mactmp[17]='\0';
		buart_print("\n\rMac addres:");
		buart_print(mactmp);
		buart_print("\n\rNumber of Mac address:");
		buart_put(macnum+'0');
	}

	/* Print IP for TFTP Download */
	if(bsp_GetTftpIp(&tftpip) != 0)
	{
		ipstr[0]=0;
	}
	else
		IpAddrToStr(tftpip,ipstr);
	buart_print("\n\rIP address for this board:");
	buart_print(ipstr);
}

/*************************************************************************************/
/*		Tftp Parameter																 */
/*************************************************************************************/

void print_tftpc_param(void)
{
	char str[]="xxx.xxx.xxx.xxx";
	if(cfg->tftpmagic == TFTPMAGIC)
	{
		buart_print("\n\r\n\rServer IP: ");
		IpAddrToStr(cfg->tftp_param.server_ip,str);
		buart_print(str);
		buart_print("\n\rGateway IP: ");
		IpAddrToStr(cfg->tftp_param.gw_ip,str);
		buart_print(str);
		//buart_print("\n\rRemote File bootloader: ");
		//buart_print(cfg->tftp_param.remote_file_boot);
		buart_print("\n\rRemote File system: ");
		buart_print(cfg->tftp_param.remote_file_sys);
		buart_print("\n\r");
	}
}

//int get_tftp_param(UINT32 *servip,UINT32 *gwip,char *boot_file, char *sys_file)
int get_tftp_param(UINT32 *servip,UINT32 *gwip, char *sys_file)
{
	//if(servip == NULL || boot_file == NULL || sys_file == NULL) return -1;
	if(servip == NULL ||  sys_file == NULL) return -1;
	if(cfg->tftpmagic != TFTPMAGIC) return -1;
	*servip = cfg->tftp_param.server_ip;
	*gwip = cfg->tftp_param.gw_ip;
	//strcpy(boot_file,cfg->tftp_param.remote_file_boot);
	strcpy(sys_file,cfg->tftp_param.remote_file_sys);
	return 0;
}

int set_tftpc_param(void)
{
	UINT32 servip, gwip;
	char buf[BOOT_LINE_SIZE+1];
	char *image = (char *)LINUXLD_DOWNLOAD_START;

servip_again:
	buf[0] = 0;
	buart_print("\n\r\n\rPlease enter TFTP server IP : ");
	ReadLine(buf, BOOT_LINE_SIZE);
	if(buf[0] != 0)
	{
		if(ipscanf(&servip,buf) != 4)
		{
			buart_print("Invalid IP address.\n\r");
			goto servip_again;
		}
		
		if(check_ip(servip,HOST_IP_FLAG))
		{
			buart_print("Invalid IP address.\n\r");
			goto servip_again;
		}
		cfg->tftp_param.server_ip = servip;
	}
	else
	{
		buart_print("Server IP unchanged..\n\r");
	}
gwip_again:
	buf[0] = 0;
	buart_print("\n\rPlease enter gateway IP : ");
	ReadLine(buf, BOOT_LINE_SIZE);
	if(buf[0] != 0)
	{
		if(ipscanf(&gwip,buf) != 4)
		{
			buart_print("Invalid IP address.\n\r");
			goto gwip_again;
		}
		
		if(check_ip(gwip,HOST_IP_FLAG))
		{
			buart_print("Invalid IP address.\n\r");
			goto gwip_again;
		}
		cfg->tftp_param.gw_ip = gwip;
	}
	else
	{
		buart_print("Gateway IP unchanged..\n\r");
	}
//filename_boot:	
//	buf[0] = 0;		
//	buart_print("Enter remote bootloader file name : ");
//	ReadLine(buf, BOOT_LINE_SIZE);
//	if(buf[0] != 0)
//	{
//		if(strlen(buf) > BSP_IFNAME_MAX_LEN) 
//		{
//			buart_print("Ifname is too long. Maximum name length is 15 characters.\n\r");
//			goto filename_boot;
//		}
//		strcpy(cfg->tftp_param.remote_file_boot, buf);
//	}
//	else
//		buart_print("Remote file unchanged.\n\r");

filename_sys:	
	buf[0] = 0;		
	buart_print("Enter remote system file name : ");
	ReadLine(buf, BOOT_LINE_SIZE);
	if(buf[0] != 0)
	{
		if(strlen(buf) > BSP_IFNAME_MAX_LEN) 
		{
			buart_print("Ifname is too long. Maximum name length is 15 characters.\n\r");
			goto filename_sys;
		}
		strcpy(cfg->tftp_param.remote_file_sys, buf);
	}
	else
		buart_print("Remote file unchanged.\n\r");

	cfg->tftpmagic = TFTPMAGIC;
	
    /* Before Write back, backup original content */
    if (nf_read(image, (char *)LINUXLD_FLASH_BOOTPARAM_START, LINUXLD_FLASH_BOOTPARAM_SIZE) != 0)
    {
        buart_print("\n\rRead buffer error!!");
        return -1;
    }
    memcpy(image, (char *)cfg, sizeof(BOARD_CFG_T));
	
	/* Write back new parameter to flash */
	if (nf_erase((char *)LINUXLD_FLASH_BOOTPARAM_START, LINUXLD_FLASH_BOOTPARAM_SIZE) != 0)
		buart_print("\n\rErase flash error.");
	else if (nf_write((char *)LINUXLD_FLASH_BOOTPARAM_START, image, LINUXLD_FLASH_BOOTPARAM_SIZE) != 0)
		buart_print("\n\rWrite flash error.");

	buart_print("\n\r");
	return 0;
}

/*************************************************************************************/
/*AuxiliaryItem																 */
/*************************************************************************************/
/******************************************/ 
/*		Check Correct IP 				  */
/******************************************/

int check_ip(UINT32 ipcheck,int flag)
{
	UINT8 reg0, reg1, reg2, reg3;
	if(flag == HOST_IP_FLAG)
	{
		if(ipcheck == 0)
			return 1;
	}
	
	if(flag == NETMASK_FLAG)
	{
		if(ipcheck == 0)
			return 1;
	
		reg3 = (ipcheck>>24) & 0xff;
		reg2 = (ipcheck>>16) & 0xff;
		reg1 = (ipcheck>>8) & 0xff;
		reg0 = (ipcheck) & 0xff;
		if(reg3<reg2 || reg2 < reg1 || reg1 < reg0)
			return 1;
	}

	return 0;
}


