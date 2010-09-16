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
;
;------------------------------------------------------------------------------
;
;    Project : Common plateform
;    Creator : Simon
;    File    : flash.C
;    Abstract:
;
;Modification History:
;
;
;*****************************************************************************/
//#include <netmall.h>
//#include <sys/semaphore.h>
#include <flash.h>


/*************************
  declare global variable
 *************************/
static  FLASH_DESC *chips = 0;
//sem_t flash_sem = {0};



/*----------------------------------------------------------------------
* ROUTINE NAME - flash_init
*-----------------------------------------------------------------------
* DESCRIPTION:
*	The routine is intialize the flash driver
*
*----------------------------------------------------------------------*/
int flash_init()
{
#if 0
    if (flash_sem.lock == 0)
    {
        strcpy(flash_sem.name, "Flah Drv");
        flash_sem.sem_count = 1;

        //if (sem_init(&flash_sem, 0, 0))
         //   return ERROR;
    }
#endif
    return 0;
}


/*----------------------------------------------------------------------
* ROUTINE NAME - flash_add
*-----------------------------------------------------------------------
* DESCRIPTION:
*	The routine is intialize the flash driver
*
*----------------------------------------------------------------------*/
int flash_add(FLASH_DESC *dev)
{
    register FLASH_DESC *cp = chips;

    dev->next = 0;

    while (cp)
    {
        /* Already installed */
        if (cp == dev)
            return 0;

        if (cp->next == 0)
            break;
        else
            cp = cp->next;
    }

    if (cp == 0)
        chips = dev;
    else
        cp->next = dev;

    return 0;
}


/*----------------------------------------------------------------------
* ROUTINE NAME - flash_fit
*-----------------------------------------------------------------------
* DESCRIPTION:
*	The routine is to check the range of the input data
*
* INPUT:
*   cp      -- flash control block
*	flash   -- Address of the block to read.
*   cells   -- cell number to check.
*----------------------------------------------------------------------*/
static int flash_fit (struct FLASH_DESC_S *cp, unsigned long addr, int *size)
{
    register int len;
    register unsigned long start = (unsigned long)cp->start;
    register unsigned long end = (unsigned long)cp->start + cp->flash_size;

    /* check range */
    if (addr < start || addr >= end)
    {
        return FLASH_NOT_FIT_IN;        /* out of range */
    }

    /* Get the size to write */
    len = (end - addr)/cp->addr_inc;
    if (*size > len)
    {
        *size = len;
        return FLASH_PARTIAL_DONE;      /* partial written */
    }
    else
    {
        switch (cp->byte_width)
        {
        case 16:
            *size = ((*size + 1)/2)*2;
            break;

        case 32:
            *size = ((*size + 3)/4)*4;

        default:
            break;
        }
    }

    return FLASH_OK;
}


/*----------------------------------------------------------------------
* ROUTINE NAME - flash_read
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
int flash_read(char *dst, char *flash, int cells)
{
    int size = cells;
    register int rc;
    register FLASH_DESC *cp = chips;

    //if (flash_sem.lock)
      //  sem_wait(&flash_sem);

    while (cp && cells > 0)
    {
        if (flash_fit(cp, (unsigned long)flash, &size) != FLASH_NOT_FIT_IN)
        {
            rc = (*cp->read)(cp, flash, dst, size);
            if (rc == FLASH_ERROR)
                break;

            flash += size * cp->addr_inc;
            dst   += size;
            cells -= size;
        }

        size = cells;
        cp = cp->next;
    }

    //if (flash_sem.lock)
      //  sem_post(&flash_sem);

    return rc;
}


/*----------------------------------------------------------------------
* ROUTINE NAME - flash_write
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
int flash_write(char *flash, char *src, int cells)
{
    int size = cells;
    register int rc;
    register FLASH_DESC *cp = chips;

    //if (flash_sem.lock)
      //  sem_wait(&flash_sem);

    while (cp && cells > 0)
    {
        if (flash_fit(cp, (unsigned long)flash, &size) != FLASH_NOT_FIT_IN)
        {
            rc = (*cp->write)(cp, flash, src, size);
            if (rc == FLASH_ERROR)
                break;

            flash += size * cp->addr_inc;
            src   += size;
            cells -= size;
        }

        size = cells;
        cp = cp->next;
    }

    //if (flash_sem.lock)
      //  sem_post(&flash_sem);

    return rc;
}


/*----------------------------------------------------------------------
* ROUTINE NAME - flash_erase
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
int flash_erase(char *flash, int cells)
{
    int size = cells;
    register int rc;
    register FLASH_DESC *cp = chips;

    //if (flash_sem.lock)
      //  sem_wait(&flash_sem);

    while (cp && cells > 0)
    {
        if (flash_fit(cp, (unsigned long)flash, &size) != FLASH_NOT_FIT_IN)
        {
            rc = (*cp->erase)(cp, flash, size);
            if (rc == FLASH_ERROR)
                break;

            flash += size * cp->addr_inc;
            cells -= size;
        }
		size = cells;
        cp = cp->next;
    }

    //if (flash_sem.lock)
      //  sem_post(&flash_sem);

    return rc;
}
