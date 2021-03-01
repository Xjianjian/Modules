/*
* Copyright (c) 2015,SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.:ACEC
* File:nt_cfg.h
* Description:
* REQ IDs:
* History:
* 2015-4-14, 1007040, original
*/

#ifndef __NT_CFG_H__
#define __NT_CFG_H__

#include "nt_export.h"

#define PARAM_NT_TASK_CYCLE     10/*10ms*/ /* unit: 100us, eg. set 100 as 10ms, Note: this value can not be zero */

#define PARAM_NT_NPDU_START_POS 0 /* 0, for normal address mode, 1 for extern address mode (NT N_PDU byte 0 used to address) */
#define PARAM_NT_RSP_EXT_ADDR   0x00  /* response address byte when when enable external address mode for NT */

/* Params set based nt_task call period, set value is Ticks */
#define PARAM_NT_TM_Br          0      						 /* Time delay until transmission of the next FlowControl N_PDU */
#define PARAM_NT_TM_Cr          (150/PARAM_NT_TASK_CYCLE)     /* Timeout until reception of the next ConsecutiveFrame N_PDU */
#define PARAM_NT_TM_Bs          (150/PARAM_NT_TASK_CYCLE)    /* Timeout until reception of the next FlowControl N_PDU */
#define PARAM_NT_TM_Cs_Red      1     						 /* above 0 (0 as same action as 1), Time until transmission of the next ConsecutiveFrame N_PDU (STmin + Cs_Red) */
#define PARAM_NT_TM_Ar          (70/PARAM_NT_TASK_CYCLE)      						 /* Timeout for transmission of the CAN frame (any N_PDU) on the receiver side */
#define PARAM_NT_TM_As          0//(70/PARAM_NT_TASK_CYCLE)  							/* Timeout for transmission of the CAN frame (any N_PDU) on the sender side */

#define PARAM_PDU_FILL_BYTE     0x55u /* N_PDU un-used data byte fill */
/* FC parameter when receive segmented frame (send FC after receive FF) */
#define PARAM_PDU_FC_BS         1u    /* FC Blocksize */
#define PARAM_PDU_FC_STMIN      0u    /* FC STmin */
/* application layer response pending message repeat send cycle */
#define PRAM_UDS_RSP_PD_CYCLE   500
/* NT layer application used buffer length */
/* application (service) data buffer size define */
#define PARAM_APP_RECEIVE_BUF_SIZE    4095
#define PARAM_APP_SEND_BUF_SIZE       4095

/*Devon modify it base on NDS08 standard*/
#define PARAM_NT_AUTO_REP_TIME	(10000/PARAM_NT_TASK_CYCLE)

extern void nt_recIndication(uint32_t rxId, uint8_t rxDLC,uint8_t * rxBuf);

#endif /*End of __NT_CFG_H__ */
/*--------EOF--------*/


