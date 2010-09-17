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
;    Project : Am5106/5107
;    Creator : David Weng
;    File    : uartdrv.h
;    Abstract:
;
;Modification History:
;
;
;*****************************************************************************/


#ifndef  __UARTDRV_H__
#define  __UARTDRV_H__

#include <ctype.h>

#define UART_PORT0			0
#define UART_PORT1			1


#define UART_NOT_INIT		0
#define UART_RUNNING		1

#ifndef BOOT_LINE_SIZE
#define BOOT_LINE_SIZE 31
#endif


typedef struct _UART_RATE_ENTRY_
{
	int	rate;
	UINT32 div;
} UART_RATE_ENTRY, *PUART_RATE_ENTRY;


typedef struct _UART_OBJ_
{
	UINT32 status;
	UINT32 io_base;
	UINT32 ctrl;

	int	baudrate;
} UART_OBJ, *PUART_OBJ;


void ReadLine(char *buf, int num);

#endif
