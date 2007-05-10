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
;    Project : AM5120
;    Creator : Jeanson Hung
;    File    : nand.c
;    Abstract:
;
;Modification History:
;	10/8/2003,	ProChao		- support the bootloader to be compatible with the
;							  NAND flash driver in application (Linux kernel)
;
;
;*****************************************************************************/
#include <ctype.h>
#include <mips.h>
#include <mips4kc.h>
#include <adm5120.h>

#include "nand.h"
#include "ftl-port.h"	//porting by ProChao, 10/8/2003

//---------------------------------------------------------
volatile UINT8  *base = (UINT8 *) NAND_REG_BASE;
UINT8 ostr[20];

static struct nand_oobinfo  oobinfo_buf = {	// init'ed by ProChao
        1,
	{0, 1, 2, 3, 6, 7}
};

static u_char  databuf[NAND_PAGE_SIZE], oobbuf[NAND_PAGE_OOB_SIZE] = 
{ 0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x85,0x19,0x03,0x20,0x08,0x00,0x00,0x00 };

static inline void print_val(UINT8 *str, int val){
	ostr[8]=0;
	buart_print("\n\r");
	buart_print(str);
	ultoa(val,ostr);	
	buart_print(" = ");
    	buart_print(ostr);
}
static inline void print_val1(UINT8 *str, int val){
	ostr[8]=0;
	buart_print("\r");
	buart_print(str);
	ultoa(val,ostr);	
	buart_print(" = ");
    	buart_print(ostr);
}
static inline void print_ver(int i, int flash, int buf){
	ostr[8]=0;
	buart_print("\n\r");
	ultoa(i,ostr);	
    	buart_print(ostr);
	buart_print(" ");
	ultoa(flash,ostr);	
    	buart_print(ostr);
	buart_print(" ");
	ultoa(buf,ostr);	
    	buart_print(ostr);
}

static void nand_hwcontrol( UINT cmd)
{
	switch (cmd)
	{
		case NAND_CTL_SETCLE:
			*(base + NAND_SET_CLE) = 0x01;
			break;
		case NAND_CTL_CLRCLE:
			*(base + NAND_CLR_CLE) = 0x01;
			break;
		case NAND_CTL_SETALE:
			*(base + NAND_SET_ALE) = 0x01;
			break;
		case NAND_CTL_CLRALE:
			*(base + NAND_CLR_ALE) = 0x01;
			break;
		case NAND_CTL_SETNCE:
			*(base + NAND_SET_CEn) = 0x01;
			break;
		case NAND_CTL_CLRNCE:
			*(base + NAND_CLR_CEn) = 0x01;
			break;
		case NAND_CTL_SETWP:
			*(base + NAND_SET_WPn) = 0x01;
			break;
		case NAND_CTL_CLRWP:
			*(base + NAND_CLR_WPn) = 0x01;
			break;
		default:
			break;
	}
}


//
static UINT8 nand_read_byte( void)
{
	return *base;	// readb( base);
}

static void nand_write_byte( UINT8 byte)
{
	*base = byte;
}

static void nand_select_chip( int chip)
{
	switch (chip)
	{
		case -1:
			nand_hwcontrol( NAND_CTL_CLRNCE);
			break;
		case 0:
			nand_hwcontrol( NAND_CTL_SETNCE);
			break;
		default:	// bugs, should not be here
			break;
	}
}

static void nand_write_buf( const UINT8 *buf, int len)
{
	int i;

	for (i = 0; i < len; i++)
		*base = buf[i];
}

static void nand_write_val( UINT8 val, int len)
{
	int i;

	for (i = 0; i < len; i++)
		*base = val;
}

static void nand_read_buf( UINT8 *buf, int len)
{
	int i;

	for (i = 0; i < len; i++)
		buf[i] = *base;
}


// send command to the NAND device
static void nand_command( UINT command, UINT32 column, UINT32 page_addr)
{
	UINT32  j;
	int     readcmd;

	/* Begin command latch cycle */
	nand_hwcontrol( NAND_CTL_SETCLE);
	/* Write out the command to the device. */
	if (command == NAND_CMD_SEQIN)
	{
		if (column >= NAND_PAGE_SIZE)
		{	// OOB area 
			column -= NAND_PAGE_SIZE;
			readcmd = NAND_CMD_READOOB;
		} else if (column < 256)
		{	// First 256 bytes --> READ0
			readcmd = NAND_CMD_READ0;
		} else
		{	// 2nd 256 bytes
			column -= 256;
			readcmd = NAND_CMD_READ1;
		}
		nand_write_byte( readcmd);
	}
	nand_write_byte( command);

	/* Set ALE and clear CLE to start address cycle */
	nand_hwcontrol( NAND_CTL_CLRCLE);

	if (column != -1 || page_addr != -1)
	{
		nand_hwcontrol( NAND_CTL_SETALE);
		/* Serially input address */
		if (column != -1)
			nand_write_byte( column);
		if (page_addr != -1)
		{
			nand_write_byte((unsigned char) (page_addr & 0xff));
			nand_write_byte((unsigned char) ((page_addr >> 8) & 0xff));
		}
		/* Latch in address */
		nand_hwcontrol( NAND_CTL_CLRALE);
	}

	/* program and erase have their own busy handlers
	 * status and sequential in needs no delay
	 */
	switch (command)
	{
		case NAND_CMD_PAGEPROG:
		case NAND_CMD_ERASE1:
		case NAND_CMD_ERASE2:
		case NAND_CMD_SEQIN:
		case NAND_CMD_STATUS:
			return;

		case NAND_CMD_RESET:
			nand_hwcontrol( NAND_CTL_SETCLE);
			nand_write_byte( NAND_CMD_STATUS);
			nand_hwcontrol( NAND_CTL_CLRCLE);
			while ( !(nand_read_byte() & 0x40));
			return;
		default:
			for (j = 0; j < 20000; j++);
	}
}

static int nand_verify_buf( const UINT8 *buf, int len)
{
	int i, err = 0;
	u_char flash;
	for (i = 0; i < len; i++){
		flash = *base;
		if (buf[i] != flash) {
			print_ver(i, flash, buf[i]);
			//print_val("i",i);
			//print_val("flash",flash);
			//print_val("buf  ",buf[i]);
			//nand_command( NAND_CMD_RESET, -1, -1);
			err = -1;
		}
	}
	return err;
}

static int read_status()
{ 
	UINT8 status;
	int j=0;
	nand_command( NAND_CMD_STATUS, -1, -1);
	while( !( (status=nand_read_byte()) & 0x40 ) ); 
	if(status & 0x01) return -1;
	return 0;		
}

/* Appropriate chip should already be selected */
static int nand_block_bad( UINT32 page)
{
	UINT8	bad;
	if(!page) return 0;
	nand_command( NAND_CMD_READOOB, NAND_BADBLOCK_POS, page);
	bad = nand_read_byte();
	if (bad != 0xff) return -1;
	return 0;
}

static int nand_mark_bad_block( UINT32 page)
{
	int status;
	nand_erase_block(page);
	nand_command( NAND_CMD_SEQIN, 0x00, page);
	nand_write_val( 0x00, NAND_PAGE_SIZE + NAND_PAGE_OOB_SIZE);
	nand_command( NAND_CMD_PAGEPROG, -1, -1);
	status = read_status();
	if (status)
	{
		print_val("nand_mark_bad_block: Failed write marker for bad block", page*NAND_PAGE_SIZE);
		return -1;
	}
	print_val("nand_mark_bad_block: Write marker for bad block ", page*NAND_PAGE_SIZE);
	return 0;
}

static int nand_read_page( UINT8 *data_poi, UINT32 page, UINT32 from, UINT32 len)
{
	int j, ecc_status, *oob_config = oobinfo_buf.eccpos;
	UINT8  ecc_calc[6], ecc_code[6];

	nand_command( NAND_CMD_READ0, 0x00, page);
	nand_read_buf( databuf, NAND_PAGE_SIZE);
	nand_read_buf( oobbuf, NAND_PAGE_OOB_SIZE);
	nand_calculate_ecc( &databuf[0],   &ecc_calc[0]);
	nand_calculate_ecc( &databuf[256], &ecc_calc[3]);
	for (j = 0; j < 6; j++)
			ecc_code[j] = oobbuf[oob_config[j]];
	ecc_status = nand_correct_data( &databuf[0], &ecc_code[0], &ecc_calc[0]);
	if (ecc_status == -1)
	{
		print_val("nand_read_page: Failed ECC read 0-255, page", page*NAND_PAGE_SIZE);
		return -1;
	}
	if (ecc_status == 1 || ecc_status == 2) print_val("Corrected ECC error 0-255, page", page*NAND_PAGE_SIZE);
	ecc_status = nand_correct_data( &databuf[256], &ecc_code[3], &ecc_calc[3]);
	if (ecc_status == -1)
	{
		print_val("nand_read_page: Failed ECC read 256-511, page", page*NAND_PAGE_SIZE);
		return -1;
	}
	if (ecc_status == 1 || ecc_status == 2) print_val("Corrected ECC error 256-511, page", page*NAND_PAGE_SIZE);
	for(j = 0; j < len; j++) data_poi[j] = databuf[j+from];
	return 0;
}

/*
 *	Nand_page_program function is used for write and writev !
 *	This function will always program a full page of data
 *	If you call it with a non page aligned buffer, you're lost :)
 */
static int nand_write_page( const UINT8 *data, UINT32 page,UINT32 len)
{
	int     i, status;
	UINT8  ecc_code[6], *data_poi;
	int     *oob_config = oobinfo_buf.eccpos;

	//for (i = 0; i < NAND_PAGE_OOB_SIZE; i++) oobbuf[i] = 0xff;
	/* Write out complete page of data, take care of eccmode */
	if(len < NAND_PAGE_SIZE) {
		for (i = 0; i <NAND_PAGE_SIZE; i++) {
		databuf[i] = i < len ?  data[i] : 0xff;
		}
		data_poi = databuf;
	}
	data_poi = data;
	nand_calculate_ecc( &data_poi[0], &ecc_code[0]);
	for (i = 0; i < 3; i++) oobbuf[ oob_config[i]] = ecc_code[i];
	/* Calculate and write the second ECC for 512 Byte page size */
	// must be 512-byte page size
	nand_calculate_ecc( &data_poi[256], &ecc_code[3]);
	for (i = 3; i < 6; i++) oobbuf[ oob_config[i]] = ecc_code[i];

	//for(i = 0; i < NAND_PAGE_OOB_SIZE; i++) print_val("",oobbuf[i]);
			//
	nand_command( NAND_CMD_SEQIN, 0x00, page);
	nand_write_buf( data_poi, NAND_PAGE_SIZE);
	/* Write out OOB data */
    	nand_write_buf( oobbuf, NAND_PAGE_OOB_SIZE);
	
	/* Send command to actually program the data */
	nand_command( NAND_CMD_PAGEPROG, -1, -1);
	/* call wait ready function */
	status = read_status();
	/* See if device thinks it succeeded */
	if (status)
	{
		print_val("Failed write, page", page*NAND_PAGE_SIZE);
		return -1;
	}
	/* Send command to read back the page */
	nand_command( NAND_CMD_READ0, 0x00, page);
	/* Loop through and verify the data */
	if (nand_verify_buf( data_poi, NAND_PAGE_SIZE))
	{
		print_val("nand_write_page: Failed verify, page", page*NAND_PAGE_SIZE);
		return -1;
	}
	if (nand_verify_buf( oobbuf, NAND_PAGE_OOB_SIZE))
	{
		print_val("nand_write_page: Failed verify oob, page", page*NAND_PAGE_SIZE);
		return -1;
	}
	return 0;
}

int nand_erase_block(UINT32 page)
{
	nand_command(NAND_CMD_ERASE1,-1, page);
	nand_command(NAND_CMD_ERASE2,-1, -1);
	if( read_status() ){
		print_val("nand_erase_block: Erase ERROR, block", page*NAND_PAGE_SIZE);
		return -1;			
	}	
	return 0;
}

static int nand_write_boot_page( const UINT8 *data_poi, UINT32 page)
{
	int    status, j;
	for (j = 0; j < 10000; j++);
	//buart_print("");	
	//print_val("Writing, page", page*NAND_PAGE_SIZE);
	nand_command( NAND_CMD_SEQIN, 0x00, page);
	nand_write_buf( data_poi, NAND_PAGE_SIZE + NAND_PAGE_OOB_SIZE);
	nand_command( NAND_CMD_PAGEPROG, -1, -1);
	/* call wait ready function */
	status = read_status();
	/* See if device thinks it succeeded */
	if (status)
	{
		print_val("nand_write_boot_page: Failed write, page", page*NAND_PAGE_SIZE);
		return -1;
	}
	/* Send command to read back the page */
	nand_command( NAND_CMD_READ0, 0x00, page);
	
	/* Loop through and verify the data */
	if (nand_verify_buf( data_poi, NAND_PAGE_SIZE + NAND_PAGE_OOB_SIZE))
	{
		print_val("nand_write_boot_page: Failed verify, page", page*NAND_PAGE_SIZE);
		return -1;
	}
	return 0;
}

/* NAND read with ECC */
int nand_read_ecc( UINT8 *from, UINT32 len, UINT32 *retlen, UINT8 *buf)
{
	int     i, j, col, page, page1st, page_count, col1, len1;//, ret;
	int     read = 0;
	int     *oob_config;

	oob_config = oobinfo_buf.eccpos;
	/* Do not allow reads past end of device */
	if (((UINT32)from + len) > NAND_FLASH_SIZE)
	{
		buart_print("nand_read_ecc: Attempt read beyond end of device\n");
		*retlen = 0;
		return -EINVAL;
	}
	/* First we calculate the starting page */
	page = (UINT32)from >> ADD2SHIFT;	//9, this->page_shift;
	col = (UINT32)from & (NAND_PAGE_SIZE - 1);
	/* Loop until all data read */
	nand_select_chip( 0);
	nand_hwcontrol(NAND_CTL_CLRWP);
	while (read < len)
	{
		if(read) { col1 = 0; len1 = (len-read) < NAND_PAGE_SIZE ? len-read : NAND_PAGE_SIZE;}
		else     { col1 = col; len1 = (len + col) < NAND_PAGE_SIZE ? len : NAND_PAGE_SIZE - col; }
		/* If the read is not page aligned, we have to read into data buffer
		 * due to ecc, else we read into return buffer direct*/
		page1st = (page / NAND_BLK_PER_PAGE) * NAND_BLK_PER_PAGE;
		page_count = page - page1st;
		/* get the first page of this current block */
		if (page_count == 0 || read == 0)
		{	// checking this block, if not checked yet
			while(nand_block_bad( page1st) != 0)
			{	// this block is bad block, double confirm this is bad one, then try next block
				print_val("nand_read_ecc: Bad block detected ", page1st*NAND_PAGE_SIZE);
				// move forward to the 1st page of next block
				page += NAND_BLK_PER_PAGE;
				page1st += NAND_BLK_PER_PAGE;
			}
		}
		if (nand_read_page( &buf[read], page, col1, len1)){
			nand_mark_bad_block(page1st);
			print_val("nand_read_ecc: New bad block detected ", page*NAND_PAGE_SIZE);
			buart_print("\n\rYou should reprogramm flash egain!!!!!!!!!!!!!!!!!!!!!");
			return -1;
		}

		read += len1;
		/* Increment page address */
		page++;
	}
	*retlen = read;
	//nand_hwcontrol(NAND_CTL_SETWP);
	nand_select_chip(-1);

	return 0;
}


#define NOTALIGNED(x) ((x & (NAND_PAGE_SIZE-1)) != 0)

//
/** NAND write with ECC */
int nand_write_ecc( UINT8 *to, UINT32 len, UINT32 *retlen, const UINT8 * buf)
{
	unsigned long	page1st;
	int 	page, written = 0, page_count=-1;
	UINT32	col;

	/* Do not allow write past end of device */
	if (((UINT32)to + len) > NAND_FLASH_SIZE)
	{
		buart_print("\n\rnand_write_ecc: Attempt to write past end of page");
		return -EINVAL;
	}
	/* reject writes, which are not page aligned */
	if (NOTALIGNED((UINT32)to))
	{
		buart_print("\n\rnand_write_ecc: Attempt to write not page aligned data");
		return -EINVAL;
	}
	/* Shift to get page */
	page = ((UINT32)to >> ADD2SHIFT);

	/* Select the NAND device */
	nand_select_chip( 0);
	nand_hwcontrol(NAND_CTL_CLRWP);
	nand_command( NAND_CMD_RESET, -1, -1);

	/* Loop until all data is written */
	while (written < len)
	{	
		// before doing write, checking if this block is bad block or not ?
		// 32-pages per block, determine the 1st page
		page1st = (page / NAND_BLK_PER_PAGE) * NAND_BLK_PER_PAGE;
		page_count = page - page1st;
		/* get the first page of this current block */
		if (page_count == 0 || written == 0)
		{	// checking this block, if not checked yet
			while(nand_block_bad( page1st))
			{	// this block is bad block, double confirm this is bad one, then try next block
				print_val("nand_write_ecc: Bad block detected ", page1st*NAND_PAGE_SIZE);
				// move forward to the 1st page of next block
				page += NAND_BLK_PER_PAGE;
				page1st += NAND_BLK_PER_PAGE;
			}
		//print_val("page     ", page*NAND_PAGE_SIZE);
		//print_val("written  ", written);
		}
		// Write error. Marking this block as is bad block
		col = (written +NAND_PAGE_SIZE) > len ? len - written : NAND_PAGE_SIZE;
		col = NAND_PAGE_SIZE;
		if (nand_write_page( &buf[written], page, col)){
			print_val("nand_write_ecc: New bad block detected ", page*NAND_PAGE_SIZE);
			nand_mark_bad_block(page1st);
			// move forward to the 1st page of next block
			written -= (page_count + 1)*NAND_PAGE_SIZE;
			page = page1st + NAND_BLK_PER_PAGE -1;
			//goto out;
		}
		/* Update written bytes count */
		written += NAND_PAGE_SIZE;
		/* Increment page address */
		if (page > NAND_BLK_PER_PAGE*NAND_FLASH_BLOCK_NO) break;
		page++;
	}
	*retlen = written;
	//print_val("nand_write_ecc", written);
	//nand_hwcontrol(NAND_CTL_SETWP);
	nand_select_chip( -1);
	return 0;
}

int nand_write_boot( UINT8 *to, UINT32 len, UINT32 *retlen, const UINT8 * buf)
{
	unsigned long	page1st, blkAddr = -1;
	int 	page, ret = 0, oob = 0, written = 0, page_count=-1;
	//UINT8	*data_poi;

	/* reject writes, which are not page aligned */
	if (NOTALIGNED((UINT32)to))
	{
		buart_print("\n\rnand_write_boot: Attempt to write not page aligned data");
		return -EINVAL;
	}
	/* Shift to get page */
	page = ((UINT32)to >> ADD2SHIFT);

	/* Select the NAND device */
	nand_select_chip( 0);
	nand_hwcontrol(NAND_CTL_CLRWP);
	nand_command( NAND_CMD_RESET, -1, -1);
	//nand_block_bad( page);
	/* Loop until all data is written */
	while (written < len)
	{	
		// Write error. Marking this block as is bad block
		if (nand_write_boot_page( &buf[written], page)){
			print_val("nand_write_boot: New bad block detected ", page*NAND_PAGE_SIZE);
			buart_print("\n\rYou should replace flash chip!!!!!!!!!!!!!!!!!!!!!");
			return -1;
		}
		/* Update written bytes count */
		written += NAND_PAGE_SIZE;
		written += NAND_PAGE_OOB_SIZE;
		/* Increment page address */
		page++;
	}
	*retlen = written;
	//print_val("nand_write_boot", written);
	//nand_hwcontrol(NAND_CTL_SETWP);
	nand_select_chip( -1);
	return 0;
}

// note that, the addr must be the block starting address
int nand_erase(UINT8 *addr, UINT32 len, UINT32 scan)
{
	unsigned long	page1st;
	int 	page, written = 0, page_count=-1;

	/* Do not allow erase past end of device */
	/* Shift to get page */
	page = ((UINT32)addr >> ADD2SHIFT);

	/* Select the NAND device */
	nand_select_chip( 0);
	//*(base + NAND_SET_WP_REG) = 1;
	nand_hwcontrol(NAND_CTL_CLRWP);
	nand_command( NAND_CMD_RESET, -1, -1);

	/* Loop until all data is written */
	while (written < len)
	{	
		if(scan){
			while(nand_block_bad( page))
			{	
				print_val("nand_erase: Bad block detected ", page*NAND_PAGE_SIZE);
				// move forward to the 1st page of next block
				page += NAND_BLK_PER_PAGE;
			}
		}
		if (nand_erase_block(page)){
			print_val("nand_erase: New bad block detected ", page*NAND_PAGE_SIZE);
			//buart_print("\n\rNew bad block detected!");
			nand_mark_bad_block(page);
			// move forward to the 1st page of next block
			page +=NAND_BLK_PER_PAGE;
		}
		/* Update written bytes count */
		page +=NAND_BLK_PER_PAGE;
		written += NAND_SIZE_PER_BLK;
		/* Increment page address */
	}
	//nand_hwcontrol(NAND_CTL_SETWP);
	nand_select_chip( -1);
	return 0;
}

void check_bad_block(void)
{
	int i, page_no=NAND_FLASH_BLOCK_NO*NAND_BLK_PER_PAGE;
	nand_select_chip( 0);
	nand_hwcontrol(NAND_CTL_CLRWP);
	for(i=NAND_BLK_PER_PAGE; i < page_no; i+=NAND_BLK_PER_PAGE) {
		if(nand_block_bad(i)!=0){
			print_val("Bad block", i*NAND_PAGE_SIZE);
		}
	}
	nand_hwcontrol(NAND_CTL_SETWP);
	nand_select_chip(-1);
}

void scan_bad_blocks(UINT8 *addr)
{
	int i,j,page,size = NAND_PAGE_SIZE + NAND_PAGE_OOB_SIZE, page_num = NAND_FLASH_BLOCK_NO*NAND_BLK_PER_PAGE;
	char data[size];
	for(j=0; j < size; j++) data[j] = 0;
	nand_select_chip( 0);
	nand_hwcontrol(NAND_CTL_CLRWP);
	nand_command( NAND_CMD_RESET, -1, -1);
	page = ((UINT32)addr >> ADD2SHIFT);

	for(i=page; i< page_num; i+= NAND_BLK_PER_PAGE){
		print_val1("block",i*NAND_PAGE_SIZE);
		while(nand_block_bad( i))
		{	
			print_val("nand_erase: Bad block detected ", i*NAND_PAGE_SIZE);
			//buart_print("\n\r");
			// move forward to the 1st page of next block
			i += NAND_BLK_PER_PAGE;
		}
		if(nand_erase_block(i)){
			buart_print("\n\rNew bad block detected!\n\r");
			//buart_print("\n\r");
			nand_mark_bad_block(i);	
		}
		for(j=0; j < NAND_BLK_PER_PAGE;j++){
			page = j+i;
			if(nand_write_page( data,page,NAND_PAGE_SIZE )){
				buart_print("\n\rNew bad block detected!\n\r");
				//buart_print("\n\r");
				nand_mark_bad_block((page / NAND_BLK_PER_PAGE) * NAND_BLK_PER_PAGE);
				break;	
			}
		}
	}
	buart_print("\n\r");
	//nand_hwcontrol(NAND_CTL_SETWP);
	nand_select_chip( -1);
}

void bad_block(void)
{
	nand_select_chip( 0);
	nand_hwcontrol(NAND_CTL_CLRWP);
	//nand_mark_bad_block(0x140);	//0x028000
	nand_mark_bad_block(0x1300);	//0x260000
	nand_mark_bad_block(0x1320);	//0x264000
	nand_mark_bad_block(0xA800);	//0x1500000
	//nand_hwcontrol(NAND_CTL_SETWP);
	nand_select_chip( -1);
}
