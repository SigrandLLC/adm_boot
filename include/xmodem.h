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
;    Project :
;    Creator :
;    File    : xmodem.h
;    Abstract:
;
;Modification History:
;
;
;*****************************************************************************/

#if __cplusplus
extern "C" {
#endif

#ifndef _XMODEM_H_
#define _XMODEM_H

#define XMODEM_SIZE             128


/* ASCII characters */
#define NAK                     025     /* no acnowledge */
#define SOH                     001     /* start of header */
#define EOT                     004     /* end of transmission */
#define ACK                     006     /* acnowledge */
#define CAN                     030     /* cancel */
#define LF                      0xa

#define ERRORMAX                15     /* max errors tolerated */
#define FILE_CORRUPT            0x80


int xmodem(uchar *buf, int buf_size);


#endif /* _XMODEM_H */

#if __cplusplus
}
#endif
