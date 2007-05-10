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
;    Project : Common plateform
;    Creator :
;    File    : loader.C
;    Abstract: 
;
;Modification History:
; 
;
;*****************************************************************************/
#include <mips4kc.h>
#include <adm5120.h>
#include <hw_profile.h>
#include <ctype.h>
#ifdef	NOR_FLASH
	#include <mx29lv160.h>
	#include <mx29lv320.h>
	#include <flash.h>
#else
	#include <nand.h>
#endif
#include <bconfig.h>
#include <uartdrv.h>
#include <if_5120.h>
#include <linuxld.h>
#include <banner.h>
#include <param.h>
#include <bsp_cfg.h>

#define LOADER_BACK_DOOR_TIME   3
#define LOADER_BACK_DOOR_HINT   (' ')
#define LOADER_BACK_DOOR_COUNT  3
#define LOADER_START_HINT       '@'


char hexdigit[] = "0123456789abcdef";
char bt_name[] = "\r\nADM0000 Boot:";
void *_heap_h;

extern void _icache_sync_all(void);

/*
 * This routine prints the main menu 
 */
static void print_menu()
{
    buart_print("\n\r"
                "\n\rSigrand MR16 router:"
                "\n\r=================================="
                "\n\rBootloader Menu"		
                "\n\r [1] Xmodem Download"
                "\n\r [2] TFTP Download"
                "\n\r [3] Print Boot Params"
                "\n\r [4] Set Boot Params"
                "\n\r [5] Check flash"
                "\n\r [6] Exit\n"
                "\n\rPlease enter your number:");
}

/*
 * This routine is the C entry pointer of the loader 
 */
static void print_tftpc_menu(void)
{
	buart_print("\n\rTFTP Client Menu");	
	buart_print("\n\r===============================");
	buart_print("\n\r [B]: Update bootloader ");
	buart_print("\n\r [S]: Update system ");
	buart_print("\n\r [A]: Update all ");
	buart_print("\n\r [P]: Set parameters");
	buart_print("\n\r [X]: exit");
	buart_print("\n\rEnter your option:");
}

void tftp_client_menu(void)
{
	char key;
	UINT32 ticks;	
	while (1)
	{
		print_tftpc_param();
		print_tftpc_menu();
		key=buart_getchar();
		buart_put(key);
		switch(key)
		{
			case 'B':
			case 'b':
				tftpc_download_boot();
				break;

			case 'S':
			case 's':
				tftpc_download_sys();
				break;

			case 'A':
			case 'a':
				tftpc_download_boot();
				tftpc_download_sys();
				break;
				
			case 'P':
			case 'p':
				set_tftpc_param();				
				break;

			case 'X':
			case 'x':
				return;
				
			default:
				break;
		}
	}

}

static void print_xmodem_menu(void)
{
	buart_print("\n\rXmodem Client Menu");	
	buart_print("\n\r===============================");
	buart_print("\n\r [B]: Update bootloader ");
	buart_print("\n\r [S]: Update system ");
	buart_print("\n\r [X]: Exit");
	buart_print("\n\rEnter your option:");
}

void xmodem_client_menu(void)
{
	char key;
	UINT32 ticks;	
	while (1)
	{
		print_xmodem_menu();
		key=buart_getchar();
		buart_put(key);
		switch(key)
		{
			case 'B':
			case 'b':
				update_bootloader();
				break;

			case 'S':
			case 's':
				xmodem_download();
				break;

			case 'X':
			case 'x':
				return;
				
			default:
				break;
		}
	}

}

static void print_flash_menu(void)
{
	buart_print("\n\rFlash Client Menu");	
	buart_print("\n\r===============================");
	buart_print("\n\r [P]: Print existent bad blocks ");
	buart_print("\n\r [S]: Scan flash for new bad blocks");
	buart_print("\n\r [E]: Erase flash");
	buart_print("\n\r [A]: Erase flash and bad blocks");
	buart_print("\n\r [C]: Create bad blocks");
	buart_print("\n\r [X]: exit");
	buart_print("\n\rEnter your option:");
}

void flash_client_menu(void)
{
	char key;
	UINT32 ticks;	
	while (1)
	{
		print_flash_menu();
		key=buart_getchar();
		buart_put(key);
		switch(key)
		{
			case 'P':
			case 'p':
				check_for_bad();
				break;

			case 'S':
			case 's':
				find_bad_blocks(LINUXLD_FLASH_LOADER_START);
				break;

			case 'E':
			case 'e':
				flash_erase(LINUXLD_FLASH_LOADER_START);
				break;
				
			case 'A':
			case 'a':
				flash_erase_all();				
				break;

			case 'C':
			case 'c':
				create_bad_blocks();				
				break;

			case 'X':
			case 'x':
				return;
				
			default:
				break;
		}
	}

}


void c_entry(void)
{
    int type;
    register int i;
    register int c;
    unsigned long   tick1,tick2;
    void    (*funcptr)( int );
	int countdown=3;
	
    /* initialize uart, timer and flash */
    buart_init();
	
	//init mem space
	memlib_init();
	
	//init Timer		
	InitTimer();
#ifdef NOR_FLASH
	#if (HWPF_5120_SMEM0_BANK_SIZE == 0x200000)
    	mx29lv160_init();
	#endif

	#if (HWPF_5120_SMEM0_BANK_SIZE == 0x400000)
    	mx29lv320_init();
    	ADM5120_SW_REG(Mem_control_REG) |= 0x050000;
	#endif
    flash_init();
#endif

    //print '@' at the very beginning
	i = ADM5120_SW_REG(CODE_REG) & CODE_ID_MASK;
	bt_name[5] = hexdigit[(i>>12)&0xf];		bt_name[6] = hexdigit[(i>>8)&0xf];
	bt_name[7] = hexdigit[(i>>4)&0xf];		bt_name[8] = hexdigit[i&0xf];
	buart_print(bt_name);
	buart_print("\n\r\n\r");
	buart_print("\n\rCopyright 2007 Sigrand, Inc.");
	buart_print("\n\rCPU: Infineon ");buart_print(CPU_NO);buart_put('-');buart_print(CPU_CLOCK);
	buart_print("\n\rSDRAM: ");buart_print(SDRAM_SIZE);
	buart_print("\n\rFlash: ");buart_print(FLASH_TYPE);buart_put('-');buart_print(FLASH_SIZE);
	buart_print("\n\rBoot System: ");buart_print(BOOT_SYSTEM);
	buart_print("\n\rLoader Version: ");buart_print(LOADER_VERSION);
	buart_print("\n\rCreation Date: ");buart_print("17.04.2007");//buart_print(CREATE_DATE);
	buart_print("\n\r\n\rPress <space> key tree times to enter boot menu..");
	buart_print("\n\r");
	
    /* wait 3 second for back door */
    buart_put(countdown + '0');
    tick1 = UpTime();
    for (i = 0; i < 3; )
    {
    	tick2 = UpTime();
        if ((tick2 - tick1) >= 200)
        {
        	countdown--;
        	if (countdown<0)
            	break;
        	buart_put('\r');
        	buart_put(countdown+'0');
        	tick1 = UpTime();
        }
        c = buart_get(0);
        switch (c)
        {
        case -1:
            break;

        case LOADER_BACK_DOOR_HINT:
            if (++i == LOADER_BACK_DOOR_COUNT)
                goto main_menu;
            break;

        default:
            i = 0;
            break;
        }
    }

	/* boot Linux kernel */
	boot_linux();
	
    /* go back door */
main_menu:
	// Read the Network configuration data.
	if (boot_param_init() != 0)
	{
		buart_print("\n\rRead bsp error.");
		panic();
	}

	// Init Switch.
	if (if5120_init() != 0)
	{
		buart_print("\n\rSwitch init false.");
		panic();
	}

    // Init networks for TFTP.
	check_for_bad();
	tftp_init();
	//check_bad_block();
 	//Bootloader Function Start.
   while (1)
    {
        print_menu();

        (char)c=buart_getchar();

        buart_put(c);

        switch (c)
        {
        case '1':
		xmodem_client_menu();
            	break;

        case '2':
        	tftp_client_menu();
        	break;

        case '3':
        	PrintBspParam();
        	break;
        	
        case '4':
        	set_boot_param();
        	break;

        case '5':
		flash_client_menu();
		break;
        
        case '6':
		boot_linux();
           	break;
        default:
            break;
        }
    }
}

