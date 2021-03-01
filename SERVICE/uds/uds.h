/*
* Copyright (c) 2016,SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File: uds.h
* Description: 
* History: 
* 2016-5-19, 1007040, original
*/

#ifndef __UDS_H__
#define __UDS_H__

#include "uds_types.h"
#include "uds_cfg.h"
#include "uds_user.h"



/* Export for UDS callback and task */
extern void uds_requestProcess(void); /* callback by NT layer */
extern void uds_init(void);
extern void uds_task(void); /* Background task (NT task called in this function) */

/* Export for UDS service process */
extern uint8_t uds_getActiveSession(void);
extern bool uds_saLevelUnlockCheck(uint8_t saLevel);
extern bool uds_saLevelUnlockCheckByIndex(uint8_t saLv_index);
extern void uds_setPopDataIndex(uint16_t index);
extern uint8_t uds_popRequestData(uint8_t defaultDt);
extern uint16_t uds_remainRequestDLC(void);
extern uint8_t uds_curRequestSID(void);
extern uint16_t uds_getRequestDataLen(void);
extern void uds_rspPending(void);
extern bool uds_isRspPending(void);
extern bool uds_isNotYetRsp(void);
extern void uds_preActiveResponse(uint8_t servID);
extern void uds_pushRspData(uint8_t data);
extern void uds_pushRspBuf(uint8_t buf[], uint16_t len);
extern uint16_t uds_getResponseDataLen(void);
extern void uds_sendResponse(void);
extern void uds_noRsp(void);
extern void uds_positiveRsp(uint8_t rspDts[], uint16_t rspLen);
extern void uds_negativeRsp(uint8_t nrspCode);

/* Export of pre-define service */
extern void serv_sessionControl(void);
extern void serv_securityAccess(void);
extern void serv_testPresent(void);
extern void serv_didRead(void);
extern void serv_didWrite(void);
extern void serv_routineControl(void);
extern void serv_dtcInforClear(void);
extern void serv_dtcInforRead(void);
extern void serv_ioControl(void);
extern void serv_readMemoryByAddr(void);
extern void serv_writeMemoryByAddr(void);

#endif /*End of __UDS_SERVICE_H__ */
/*--------EOF--------*/
