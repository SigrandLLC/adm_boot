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
;    File    : mx29lv320.c
;    Abstract:
;
;Modification History:
;
;
;*****************************************************************************/
#include <mips4kc.h>
#include <adm5120.h>
#include <hw_profile.h>
#include "flash.h"
#include "mx29lv320.h"

/*************************
  declare global variable
 *************************/
// {HARDWARE PROFILE GENERATOR
static  int mx29lv320_erase(struct FLASH_DESC_S *cp, char *flash, int cells);
static  int mx29lv320_read (struct FLASH_DESC_S *cp, char *flash, char *dst, int cells);
static  int mx29lv320_write(struct FLASH_DESC_S *cp, char *flash, char *src, int cells);

unsigned long mx29lv320_blocks[] =
{
    0x00000000, 0x00002000, 0x00004000, 0x00006000,
    0x00008000, 0x0000A000, 0x0000C000, 0x0000E000,
    0x00010000, 0x00020000, 0x00030000,
    0x00040000, 0x00050000, 0x00060000, 0x00070000,
    0x00080000, 0x00090000, 0x000a0000, 0x000b0000,
    0x000c0000, 0x000d0000, 0x000e0000, 0x000f0000,
    0x00100000, 0x00110000, 0x00120000, 0x00130000,
    0x00140000, 0x00150000, 0x00160000, 0x00170000,
    0x00180000, 0x00190000, 0x001a0000, 0x001b0000,
    0x001c0000, 0x001d0000, 0x001e0000, 0x001f0000,
    0x00200000, 0x00210000, 0x00220000, 0x00230000,
    0x00240000, 0x00250000, 0x00260000, 0x00270000,
    0x00280000, 0x00290000, 0x002a0000, 0x002b0000,
    0x002c0000, 0x002d0000, 0x002e0000, 0x002f0000,
    0x00300000, 0x00310000, 0x00320000, 0x00330000,
    0x00340000, 0x00350000, 0x00360000, 0x00370000,
    0x00380000, 0x00390000, 0x003a0000, 0x003b0000,
    0x003c0000, 0x003d0000, 0x003e0000, 0x003f0000,
};

/*unsigned long mx29lv320_blocks[] =
{
    0x00000000, 0x00004000, 0x00006000, 0x00008000,
    0x00010000, 0x00020000, 0x00030000, 0x00040000,
    0x00050000, 0x00060000, 0x00070000, 0x00080000,
    0x00090000, 0x000a0000, 0x000b0000, 0x000c0000,
    0x000d0000, 0x000e0000, 0x000f0000, 0x00100000,
    0x00110000, 0x00120000, 0x00130000, 0x00140000,
    0x00150000, 0x00160000, 0x00170000, 0x00180000,
    0x00190000, 0x001a0000, 0x001b0000, 0x001c0000,
    0x001d0000, 0x001e0000, 0x001f0000, 0x00200000,
    0x00210000, 0x00220000, 0x00230000, 0x00240000,
    0x00250000, 0x00260000, 0x00270000, 0x00280000,
    0x00290000, 0x002a0000, 0x002b0000, 0x002c0000,
    0x002d0000, 0x002e0000, 0x002f0000, 0x00300000,
    0x00310000, 0x00320000, 0x00330000, 0x00340000,
    0x00350000, 0x00360000, 0x00370000, 0x00380000,
    0x00390000, 0x003a0000, 0x003b0000, 0x003c0000,
    0x003d0000, 0x003e0000, 0x003f0000,
};*/


FLASH_DESC mx29lv320_dev =
{
    0,                  					// next
    0x400000,           					// flash_size=4M
    1,                  					// addr_inc
    2,                  					// byte_width
    (char *)PA2VA(HWPF_5120_SMEM0_BASE),    // start
    mx29lv320_blocks,                       // blocks
    sizeof(mx29lv320_blocks)/sizeof(unsigned long),
    mx29lv320_erase,
    mx29lv320_read,
    mx29lv320_write
};
// HARDWARE PROFILE GENERATOR ...


/*----------------------------------------------------------------------
 * ROUTINE NAME - disable_write
*-----------------------------------------------------------------------
* DESCRIPTION:
*	Disable writing to the flash main blocks
*----------------------------------------------------------------------*/
static void disable_write()
{
// Hardware profile generator ...

    volatile unsigned long data;
    volatile unsigned long *reg;


    /**********************************/
    /* flash write protect enable    */
     /**********************************/

// Hardware profile generator...
    return;
}


/*----------------------------------------------------------------------
 * ROUTINE NAME - enable_write
*-----------------------------------------------------------------------
* DESCRIPTION:
*	Enable writing to the flash main blocks
*----------------------------------------------------------------------*/
static void enable_write()
{
// Hardware profile generator ...

    volatile unsigned long data;
    volatile unsigned long *reg;

    /**********************************/
    /* flash write protect disable    */
    /**********************************/


// Hardware profile generator ...
    return;
}


/*----------------------------------------------------------------------
* ROUTINE NAME - mx29lv320_erase_block
*-----------------------------------------------------------------------
* DESCRIPTION:
*	The routine is called by NV RAM driver or the image updating module
*   to erase the blocks, whereafter this block can accept new data
*   written.
*
* INPUT:
*	flash_P -- Address of the block to erase.
*----------------------------------------------------------------------*/
static int mx29lv320_erase_block(struct FLASH_DESC_S *cp, char *flash)
{
    register int i, ticks;
    unsigned short val;
	volatile unsigned short *block = (unsigned short *)flash;

    volatile unsigned short *p555 = (unsigned short *)((unsigned long)cp->start + 0x0aaa);
    volatile unsigned short *p2aa = (unsigned short *)((unsigned long)cp->start + 0x0554);


    /***********************/
    /* Unprotect Flash     */
    /***********************/
    enable_write();

    /***********************/
    /* Issue erase command */
    /***********************/
    *block  = 0xf0;			    /* Make sure in read state */
    *p555   = 0xaa;
    *p2aa   = 0x55;
    *p555   = 0x80;
    *p555   = 0xaa;
    *p2aa   = 0x55;
    *block  = 0x30;

    for (ticks=0, i=0;; i++)
    {
        val  = *block;

        /* When Erase operation is completed, DQ7 will produce 1 "1".
		   and  DQ6 stops toggling. */
        if ((val & 0x80) == 0x80)
        {
			// Check DQ6 toggle
			unsigned short s_val;

			// Read the second time.
			s_val = *block;

			if ((s_val & 0x80) &&
				(s_val & 0x40) == (val & 0x40))
			{
	            *block = 0xf0;	 /* Do reset */

				disable_write();
				return FLASH_OK;
			}
        }
        else
        {
            if (i >= 1024)
            {
                i = 0;

                if (++ticks > 7*100*10)
                {
                    break;
                }
            }
        }
    }

    *block = 0xf0;	    /* Do reset */

    disable_write();
    return FLASH_ERROR;
}


/*----------------------------------------------------------------------
* ROUTINE NAME - mx29lv320_erase
*-----------------------------------------------------------------------
* DESCRIPTION:
*	The routine is called by NV RAM driver or the image updating module
*   to erase the blocks, whereafter this block can accept new data
*   written.
*
* INPUT:
*	flash   -- Address of the flash start address to be erased
*   size    -- size of the flash portion to be erased
*----------------------------------------------------------------------*/
static int mx29lv320_erase(struct FLASH_DESC_S *cp, char *flash, int size)
{
    register int i;
    register char *next;
    register char *start = cp->start;

    for (i=0; i<cp->num; i++)
    {
        if (flash != (char *)(start + cp->blocks[i]))
            continue;

        if (mx29lv320_erase_block(cp, flash) != FLASH_OK)
            return FLASH_ERROR;

        // Get next block
        if (i == cp->num - 1)
            next = (char *)(start + cp->flash_size);
        else
            next = (char *)(start + cp->blocks[i+1]);

        // substract erased size
        size -= (next - flash);
        if (size <= 0)
            break;

        flash = next;
    }

    return FLASH_OK;
}


/*----------------------------------------------------------------------
* ROUTINE NAME - mx29lv320_read
*-----------------------------------------------------------------------
* DESCRIPTION:
*	The routine is called by NV RAM driver to read NV data.
*
*
* INPUT:
*	flash   -- Address of the block to read.
*   dst 	-- address of this data buffer.
*   size    -- data size to read from the flash.
*----------------------------------------------------------------------*/
static int mx29lv320_read(struct FLASH_DESC_S *cp, char *flash, char *dst, int size)
{
    register int i;
    volatile unsigned short *block = (volatile unsigned short *)flash;
    volatile unsigned short *data  = (volatile unsigned short *)dst;

	/* Convert size in byte to size in word.*/
	size = (size+1)/2;

    /***********************/
    /* Read data           */
    /***********************/
    for (i=0; i<size; i++)
    {
		*data++ = *block++;
	}

    return FLASH_OK;
}


/*----------------------------------------------------------------------
* ROUTINE NAME - mx29lv320_write
*-----------------------------------------------------------------------
* DESCRIPTION:
*	The routine is called by NV RAM driver or the image updating module
*   to update the date content.
*
* INPUT:
*	flash   -- Address of the block to wirte.
*   src	    -- Source address of this image.
*   size    -- image size to write to the flash.
*----------------------------------------------------------------------*/
static int mx29lv320_write(struct FLASH_DESC_S *cp, char *flash, char *src, int size)
{
    register int i, j;
    register int rc = FLASH_OK;
    volatile short  val;
    volatile short  *block = (volatile short *) flash;
    volatile short  *data  = (volatile short *) src;

    volatile short  *p555 = (short *)((unsigned long)cp->start + 0x0aaa);
    volatile short  *p2aa = (short *)((unsigned long)cp->start + 0x0554);

	/* Convert size in bytes into size in word. */
	size=(size+1)/2;

    /***********************/
    /* Write data          */
    /***********************/
    enable_write();         // unprotect flash

    *block = 0xf0;          // Set to reset state
    for (i=0; i<size; i++)
    {
        /* Write data */
        *p555 = 0xaa;
        *p2aa = 0x55;
        *p555 = 0xa0;
        *block = *data;

        j = 0;
        while (1)
        {
            val = *block;

            // Wait complement completed
            if ((val & 0x80) == (*data & 0x80))
            {
                /* write successfully */
                break;              /* Do next byte */
            }
            else
            {
                if (j++ > 1024)
                {
                    if ((*block & 0x80) != (*data & 0x80))
                    {
                        rc = FLASH_ERROR;
                        goto exit_write;
                    }
                }
            }
        } //end of while

        block++;
        data++;
    }

    /* Update the flash and source pointer */
    block--;    /* back to the previous chip */

exit_write:
    *block = 0xf0;		    /* Reset to read array */

    disable_write();
    return rc;
}


/*----------------------------------------------------------------------
 * ROUTINE NAME - mx29lv320_init
*-----------------------------------------------------------------------
* DESCRIPTION:
*	Disable writing to the flash main blocks
*----------------------------------------------------------------------*/
int mx29lv320_init()
{
    flash_add (&mx29lv320_dev);
    return 0;
}
