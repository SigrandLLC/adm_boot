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
;    Project : Common Plateform
;    Creator : 
;    File    : skbuff.c
;    Abstract: defines the specific items that loader nedded.
;
;Modification History:
;       By              Date     Ver.   Modification Description
;       --------------- -------- -----  --------------------------------------
;
;*****************************************************************************/
#include <ctype.h>
#include <skbuff.h>

//static struct sk_buff skbuff[2];
//static int skb_cur = 0;

int skb_init(void)
{
	//skbuff = (struct sk_buff *)MemAlloc((sizeof(struct sk_buff)*5), TRUE);
	//if(skbuff = NULL)
		//return -1;
	return 0;
}

void skb_headerinit(struct sk_buff *skb)
{
	memset(skb, 0, sizeof(struct sk_buff));
	skb->data = skb->buf;
	skb->len = 0;
	skb->truesize = ETH_FRAME_LEN;
}

/*struct sk_buff *alloc_skb()
{
	struct sk_buff *skb;
	skb = &skbuff[skb_cur];
	skb_cur = (skb_cur + 1) % 2;
	skb_headerinit(skb);
	return skb;
}*/

void free_skb(struct sk_buff *skb)
{
}

unsigned char *skb_put(struct sk_buff *skb, unsigned int len)
{
	skb->len += len;
	return skb->data;
}

unsigned char *skb_pull(struct sk_buff *skb, unsigned int len)
{
	skb->data += len;
	skb->len -= len;
	return skb->data;
}

unsigned char *skb_push(struct sk_buff *skb, unsigned int len)
{
	skb->data -= len;
	skb->len += len;
	return skb->data;
}

void skb_reserve(struct sk_buff *skb, unsigned int len)
{
	skb->data += len;
}


