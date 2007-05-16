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
;    Creator : Sumei Chung
;    File    : loader.C
;    Abstract: 
;
;Modification History:
; 
;
;*****************************************************************************/
#include <ctype.h>
#include <mips4kc.h>
#include <adm5120.h>
#include <hw_profile.h>
#include <bconfig.h>
#ifdef NOR_FLASH
	#include <mx29lv160.h>
	#include <mx29lv320.h>
	#include <flash.h>
#else
	#include <nand.h>
#endif
#include <img_header.h>
#include <xmodem.h>
#include <uartdrv.h>
#include <stdlib.h>
#include <zlib.h>
#include <linuxld.h>

//#define AM5120_VINETIC  0

/* linux boot defined */
#define LINUX_ENTRY_POINT		0x800026d8

#define LINUX_IMAGE_SIZE		LINUXLD_FLASH_KERNEL_SIZE

/* gzip defined */
#define _OFFSET(_t, _m)		((u_int) &(((_t *) 0)->_m))
#define Z_EOF	0

UINT8 ostr[20];
/* gzip flag byte */
#define ASCII_FLAG   0x01 /* bit 0 set: file probably ascii text */
#define HEAD_CRC     0x02 /* bit 1 set: header CRC present */
#define EXTRA_FIELD  0x04 /* bit 2 set: extra field present */
#define ORIG_NAME    0x08 /* bit 3 set: original file name present */
#define COMMENT      0x10 /* bit 4 set: file comment present */
#define RESERVED     0xE0 /* bits 5..7: reserved */


static int gz_magic[2] = {0x1f, 0x8b}; /* gzip magic header */
unsigned int loader_heap = LINUXLD_GZIP_HEAP;
char *pass = "\n\rPASS";
char *fail = "\n\rFAIL";

int ungzip(Byte * compr);
extern void _icache_sync_all(void);
void jump_up_map();
void jump_low_map();

/*
 * Update bootloader
 */

void flash_erase(UINT32 start)
{
	buart_print("\n\rEraseing flash.......\n\r");	
	nand_erase((UINT8 *)start, NAND_FLASH_SIZE-start,1);
}

void flash_erase_all()
{
	buart_print("\n\rEraseing flash.......\n\r");
	nand_erase((UINT8 *)LINUXLD_FLASH_LOADER_START, NAND_FLASH_SIZE,0);
}

void check_for_bad()
{
	buart_print("\n\rChecking flash.......\n\r");
	check_bad_block();
}

void find_bad_blocks(UINT32 start)
{
	buart_print("\n\rScaning flash.......\n\r");
	scan_bad_blocks((UINT8 *)start);	
}

create_bad_blocks()
{
	buart_print("\n\rCreating bad blocks.......\n\r");
	bad_block();
}

int update_bootloader()
{
   	void *flash = (void *)LINUXLD_FLASH_LOADER_START;
    	char *image	= (char *)LINUXLD_DOWNLOAD_START;
   	int  len, rc = 0;

   	 /* download loader image to temp using xmodem */
   	buart_print("\n\rDownloading..........");
    	if ((len = xmodem(image, LINUXLD_FLASH_LOADER_SIZE)) == -1)
       		goto fail;
    	else
       		buart_print (pass);
    
   	/* erase flash */
	buart_print("\n\rEraseing flash.......");
    	if (nf_erase(flash , len, 1) != 0)
     	  	goto fail;
	else
		buart_print (pass);
	
	/* write flash */
    	buart_print("\n\rProgramming flash....");
	if (nf_write_boot(flash, image, len) != 0)
        goto fail;
	else
		buart_print (pass);
	
    	goto ok;

fail:
    	buart_print(fail);
    	rc = -1;

ok:
    return (rc);
}

int tftpc_download_boot(void)
{
	void *flash = (void *)LINUXLD_FLASH_LOADER_START;
   	char *image	= (char *)LINUXLD_DOWNLOAD_START;
 	char lenstr[9];
	UINT32	len;
	
	buart_print("\n\rStarting the TFTP download(ESC to stop)..");
	if ((len = tftpc(image, LINUXLD_FLASH_LOADER_SIZE,0)) == 0)
		goto fail;
    	else
        buart_print (pass);

	//print image length and starting address in sdram.
	ultoa(len,lenstr);
	lenstr[8]=0;
	buart_print("\n\rFile total Length: ");
	buart_print(lenstr);
	//ultoa(image,lenstr);
	//buart_print("  Starting address: ");
	//buart_print(lenstr);

    	//erase flash 
    	buart_print("\n\r\n\rEraseing flash.......");
	if (nf_erase(flash,len,1) != 0) goto fail;
    
ERASE_PASS:
	buart_print (pass);
   
    	//write flash
   	buart_print("\n\rProgramming flash....");
	if (nf_write_boot(flash, image, len) != 0) goto fail;
	
WRITE_PASS:
	buart_print (pass);
	goto ok;
    
fail:
	jump_low_map();
    	buart_print(fail);
   	return -1;

ok:
    	return 0;
}

int tftpc_download_sys(void)
{
	void *flash = (void *)LINUXLD_FLASH_KERNEL_START;
   	char *image	= (char *)LINUXLD_DOWNLOAD_START;
 	char lenstr[9];
	UINT32	len;
	
	buart_print("\n\rStarting the TFTP download(ESC to stop)..");
	if ((len = tftpc(image, LINUXLD_FLASH_KERNEL_SIZE,1)) == 0)
		goto fail;
    	else
        buart_print (pass);

	//print image length and starting address in sdram.
	ultoa(len,lenstr);
	lenstr[8]=0;
	buart_print("\n\rFile total Length: ");
	buart_print(lenstr);
	ultoa(image,lenstr);
	buart_print("  Starting address: ");
	buart_print(lenstr);

    	//erase flash 
    	buart_print("\n\r\n\rEraseing flash.......");
	if (nf_erase(flash,NAND_FLASH_SIZE-LINUXLD_FLASH_KERNEL_START,1) != 0) goto fail;
    
ERASE_PASS:
	buart_print (pass);
   
    	//write flash
   	buart_print("\n\rProgramming flash....");
	if (nf_write(flash, image, len) != 0) goto fail;
	
WRITE_PASS:
	buart_print (pass);
	goto ok;
    
fail:
	jump_low_map();
    	buart_print(fail);
   	return -1;

ok:
    	return 0;
}

/*
 * download linux image to flash using xmodem
 */
int xmodem_download(void)
{
	void *flash = (void *)LINUXLD_FLASH_KERNEL_START;
    char *image   = (char *)LINUXLD_DOWNLOAD_START;
    int  len, rc = 0;

    /* download linux image to temp using xmodem */
    buart_print("\n\rDownloading..........");
    if ((len = xmodem(image, LINUX_IMAGE_SIZE)) == -1)
        goto fail;
    else
        buart_print (pass);
    
    /* erase flash */
    buart_print("\n\rEraseing flash.......");
    
    if (nf_erase(flash , len,1) != 0)
        goto fail;
    
ERASE_PASS:
	buart_print (pass);
   
    /* write flash */
    buart_print("\n\rProgramming flash....");
    
	if (nf_write(flash, image, len) != 0)
        goto fail;
	
WRITE_PASS:
        buart_print (pass);
    
    goto ok;

fail:
    buart_print(fail);
    rc = -1;

ok:
    return (rc);
}

/*
 * linux boot function
 */
void boot_linux()
{
	int status;
	void (*funcptr)(void);
	buart_print("\n\rBooting Linux... ");

	nf_read((UINT8 *)LINUXLD_DOWNLOAD_START, (UINT8 *)LINUXLD_FLASH_KERNEL_START, LINUX_IMAGE_SIZE);
		// decompressing
	buart_print("\n\rKernel decompress ... ");	//okay!");
	status = ungzip((unsigned char *)LINUXLD_DOWNLOAD_START);
	
	if (status != Z_OK)    // failed in unzipping
	{
		buart_print(fail);
		return;		// return if cannot boot
	}
	else
		buart_print(pass);
		buart_print("\n\r");
	DisableTimer();
	funcptr = (void *)LINUX_ENTRY_POINT;
	_icache_sync_all();
    	funcptr();
}


/*
 * decompress function
 */
int ungzip(unsigned char *zimg)
{
    int i, err, gzflags;
    z_stream d_stream;          // decompression stream

	if((zimg[0] != gz_magic[0]) || (zimg[1] != gz_magic[1]))
		return (Z_DATA_ERROR);

	if(zimg[2] != Z_DEFLATED) return (Z_DATA_ERROR);	

	gzflags = zimg[3];
	
	// Skip the gzip header
	zimg += 10;

    if ((gzflags & EXTRA_FIELD) != 0) 
    {  	 /* skip the extra field */
		i =  (*zimg++) + (*zimg++)<<8 ;
		while ((i-- != 0) && (*zimg++) != Z_EOF);
    }
    
    if ((gzflags & ORIG_NAME) != 0) 
    {  	/* skip the original file name */
		while (*zimg++ != Z_EOF);
    }
    
    if ((gzflags & COMMENT) != 0) 
    {   /* skip the .gz file comment */
		while (*zimg++ != Z_EOF);
    }
    
    if ((gzflags & HEAD_CRC) != 0) 
    {  /* skip the header crc */
		zimg += 2;
    }

	/* Decompress the image now */
    d_stream.zalloc = NULL;
    d_stream.zfree = NULL;
    d_stream.opaque = (voidpf)0;

    d_stream.next_in  = zimg;               // address of compress image
    d_stream.avail_in = LINUX_IMAGE_SIZE;   // size of compress image

    if((err = inflateInit2(&d_stream, -MAX_WBITS)) != Z_OK)
    {
    	return err;
    }
    
    d_stream.next_out = (char *)LINUXLD_KERNEL_START; // address of decompress image
	d_stream.avail_out = LINUXLD_KERNEL_SIZE; // size of decompress space
    if((err = inflate(&d_stream, Z_NO_FLUSH)) != Z_STREAM_END)
    {
    	return err;
    }
    
    err = inflateEnd(&d_stream); 

  	return err;
}


/* 
 *  Because there is no OS support, the memory management functions in 
 *  STD C libray cann't be used. To overcome the problem, a very simple
 *  memory management skime is implemented here.
 */
void * calloc(size_t items, size_t size)
{
	size_t memsize = items * size;
	size_t bkstart;
	
	/* Make sure the memory block is always aligned on 4-byte boundry*/
	if (loader_heap & 0x03)
		loader_heap = (loader_heap+3) & ~0x03;
		
	bkstart = loader_heap;
	loader_heap += memsize;
		
	return (void *) bkstart;
}


/* 
 * dummy function
 */
/*void free(void *ptr)
{
	return;
}*/

/*
 * Jump to no-flash upper 2MB
 */
void jump_up_map()
{
    int i;
    
    /* Set GPIO[3](CS) to high */
    for (i = 0; i < 10000; i++);
    ADM5120_SW_REG(GPIO_conf0_REG) = 0x080800f7;
    for (i = 0; i < 10000; i++);
}



/*
 * Jump to no-flash upper 2MB
 */
 void jump_low_map()
{
    int i;
    
    /* Set GPIO[3](CS) to low */
    for (i = 0; i < 10000; i++);
    ADM5120_SW_REG(GPIO_conf0_REG) = 0x3ff;
    for (i = 0; i < 10000; i++);
}


