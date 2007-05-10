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
;    Project : ADM5120
;    Creator : 
;    File    : memdev.h
;    Abstract: 
;
;Modification History:
; 
;
;*****************************************************************************/

#ifndef __MEMDEV_H__
#define __MEMDEV_H__

#define ENABLE                                  1
#define DISABLE                                 0




// Bus width of SDRAM bank0. legal values are 16 or 32
#define BOARD_SDRAM0_BUS_WIDTH					32

// memory size of SDRAM chip in Mbits
#define BOARD_SDRAM0_DEV_SIZE					256

// data bus width of SDRAM chip. legal value are 8, 16 or 32
#define BOARD_SDRAM0_DEV_DATA_WIDTH				16

#define BOARD_5120_SDRAM1_STATUS				DISABLE

// Bus width of SDRAM bank0. legal values are 16 or 32
#define BOARD_SDRAM1_BUS_WIDTH				    32

// memory size of SDRAM chip in Mbits
#define BOARD_SDRAM1_DEV_SIZE				    64

// data bus width of SDRAM chip. legal value are 8, 16 or 32
#define BOARD_SDRAM1_DEV_DATA_WIDTH			    32




#endif
