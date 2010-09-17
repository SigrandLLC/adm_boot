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
;    File    : timer.c
;    Abstract:
;
;Modification History:
;
;
;*****************************************************************************/
#include <ctype.h>
#include <mips4kc.h>
#include <adm5120.h>
#include <timer.h>

static UINT32 timeticks = 0;

/**********************************************************************************/
/* InitTimer:																	  */
/**********************************************************************************/
void InitTimer(void)
{
	ADM5120_SW_REG(Timer_int_REG) = SW_TIMER_INT_DISABLE | SW_TIMER_INT;
	ADM5120_SW_REG(Timer_REG) = SW_TIMER_10MS_TICKS | SW_TIMER_EN;
	timeticks = 1;
}


/**********************************************************************************/
/* UpTime:																		  */
/**********************************************************************************/
UINT32 UpTime(void)
{
	unsigned long status;

	status = ADM5120_SW_REG(Timer_int_REG);

	if(status & SW_TIMER_INT)
	{
		/* Acknowledge the timer int */
		ADM5120_SW_REG(Timer_int_REG) = status;
		timeticks++;
	}

	return timeticks;
}

void DisableTimer(void)
{
	/*buart_print("\n\rTimer irqdisable..");
	irqDisconnect(INT_LVL_TIMER);
	IrqDisable(INT_LVL_TIMER);*/
}

