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
;    Creator : Simon
;    File    : nv_drv.h
;
;Modification History:
;   Location    Resonder   Modification Description
; ------------ ---------- ----------------------------------------------
;
;*****************************************************************************/
#if __cplusplus
extern "C" {
#endif

#ifndef _FLASH_H_
#define _FLASH_H_

enum    FLASH_E
{
    FLASH_NOT_FIT_IN = -2,
    FLASH_ERROR = -1,
    FLASH_OK = 0,
    FLASH_PARTIAL_DONE
};

typedef struct FLASH_DESC_S
{
    struct FLASH_DESC_S *next;
    int flash_size;
    int addr_inc;
    int byte_width;
    char *start;
    unsigned long *blocks;
    int num;
    int (*erase)(struct FLASH_DESC_S *cp, char *flash, int cells);
    int (*read) (struct FLASH_DESC_S *cp, char *flash, char *dst, int cells);
    int (*write)(struct FLASH_DESC_S *cp, char *flash, char *src, int cells);
}
FLASH_DESC;


int flash_init();
int flash_add(FLASH_DESC *cp);
int flash_erase(char *flash, int size);
int flash_read (char *dst, char *flash, int size);
int flash_write(char *flash, char *src, int size);


#endif  /* _FLASH_H_   */

#if __cplusplus
}
#endif
