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
;    File    : irqlib.h
;    Abstract:
;
;Modification History:
;
;
;*****************************************************************************/
#include <if_5120.h>

#ifndef __IRQLIB_H__
#define __IRQLIB_H_


typedef void (*IRQ_HANDLER)(int, UINT32,UINT32);

typedef enum _IRQL_STATUS_
{
	IRQL_NOT_CONNECTED=0,
	IRQL_DISABLED,
	IRQL_ENABLED
} IRQL_STATUS, *PIRQL_STATUS;


typedef struct _IRQ_OBJ_
{
	IRQL_STATUS status;
	UINT32 irqmode;
	IRQ_HANDLER handler;
	UINT32 parm0, parm1;
} IRQ_OBJ, *PIRQ_OBJ;


/* IRQ_MODE */
#define FIQ_INT_MODE		1
#define IRQ_INT_MODE		0



/************************  Inline Functions **************************/



/* exported functions */
// extern int disable_int(void);
// extern void restore_int(int);

extern UINT32 irqConnect(int irql, int mode, IRQ_HANDLER handler, UINT32 parm0, UINT32 parm1);
extern void irqDisconnect(int irql);

extern void IrqEnable(int irql);
extern void IrqDisable(int irql);

int sys_irq_init(void);

#endif
