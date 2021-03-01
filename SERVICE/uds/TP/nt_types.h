/*
* Copyright (c) 2015,SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.:ACEC
* File:nt_types.h
* Description:
* REQ IDs:
* History:
* 2015-4-14, 1007040, original
*/

#ifndef __NT_TYPES_H__
#define __NT_TYPES_H__

#include "typedefs.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned int  nt_word_t;
typedef unsigned char nt_bool_t;
typedef unsigned char nt_byte_t;
#ifndef FALSE
#define FALSE	(0)
#endif

#ifndef TRUE
#define TRUE	(1)
#endif

/* 
 * NT status define for all request, response flow
*/
typedef enum __NT_STATUS_T__
{
    NTS_IDLE = 0, /* idle */
    NTS_RX_SF,    /* a SF was be received */
    NTS_RX_FF,    /* a FF was be received */
    NTS_SEND_FC,  /* send flow control */
    NTS_WAIT_CF,  /* wait consecutive frame */
    NTS_APPL_LOCKED, /* application layer locked after a valid request was be received */
    NTS_SEND_SF, /* a SF need to be send */
    NTS_SEND_FF, /* a FF need to be send */
    NTS_WAIT_FC, /* wait flow control */
    NTS_SEND_CF  /* one or more CF need to be send */
}nt_status_t;
#define NTS_COUNT 10 

/*
 * NT timer type define for Placement of network layer timing parameters
*/
typedef enum __NT_TM_TYPE_T__
{
    NT_TM_IDLE = 0, /* No timeout tick running */
    NT_TM_Br,  /* Time delay for send FC (Flow Control) while receive request for receiver */
    NT_TM_Cr,  /* Timeout for wait segmented CF (Consecutive frame) while receive request for receiver */
    NT_TM_Bs,  /* Timeout for wait FC (Flow Control) while transmit response for sender */
    NT_TM_Cs,  /* Time delay for send segemented CF (Consecutive frame) while transmit response for sender */
    /* Ar receiver Send timeout timer, used only when need send success indication */
    NT_TM_Ar,  /* send FC timeout while receive request for sender */
    /* As sender transmit frame timeout timer, used only when need send success indication */
    NT_TM_As   /* Send SF, FF, CF timeout while transmit response for sender */
}nt_tm_type_t;
#define NT_TM_TYPES_COUNT 7


#ifdef __cplusplus
}
#endif


#endif /*End of __NT_TYPES_H__ */
/*--------EOF--------*/


