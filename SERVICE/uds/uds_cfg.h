/*
* Copyright (c) 2016,SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File: uds_cfg.h
* Description: 
* History: 
* 2016-5-19, 1007040, original
*/

#ifndef __UDS_CFG_H__
#define __UDS_CFG_H__

#include "uds_types.h"


#include "global_var.h"



/* ----------------------- Session Support Define ---------------------- */
/* UDS Session Support Define
 * Note: Not the real uds session value
 *       The real session value used sessionTypeMap to define
 *       Max support session count is 8
 *       First session (enum value 0) must define the default session (standard session)
*/
typedef enum
{
    ssSpt_std = 0,
    ssSpt_prog,
    ssSpt_ext
}ssSpt_t; /* must start from 0, and continue index (it not used in service protocol) */

#define ssSptCnt   3 /* Must equre ssSpt_t item count, Max support session cnt is 8 */

/* = 0x01 << ssSpt_std */
#define ssSptMask_std  0x01u
/* = 0x01 << ssSpt_prog */
#define ssSptMask_prog 0x02u
/* = 0x01 << ssSpt_ext */
#define ssSptMask_ext  0x04u
#define ssSptMask_all  0xFFu

/* Exports */
extern const uint8_t sessionTypeMap[ssSptCnt];
extern const uint8_t sessionTypeCnt;
/* ----------------------- End Session Support Define  ---------------- */

/* ----------------------- Security Access Level Support Define ------- */
/* UDS Security Access Level Define
 * Note: Not the real security access value
 *       The real security access value used saLevelMap to define
 *       Max support security access level count is 8
*/

typedef enum
{
    saLvSpt_fbl = 0,
	saLvSpt_cali,
	saLvSpt_lvl1,
}saLvSpt_t;

#define saLvSptCnt 3

#define saLv_fbl_lvl  0x09u
#define saLv_cali_lvl 0x73u
#define saLv_lvl1_lvl 0x01u

#define saLvSptMask_none 0x00u
#define saLvSptMask_fbl  0x01u
#define saLvSptMask_cali 0x02u
#define saLvSptMask_lvl1 0x03u

typedef struct
{

	uint32_t startAddr;
	uint32_t endAddr;
	uint8_t (*readCallback)(void);
	uint8_t (*writeCallback)(void);
}chipMemCfg_t;


#define chipMem_uTestNvm		0x00
#define chipMem_dataFlash		0x01
#define chipMem_codeFlash		0x02
#define chipMem_ram				0x03
#define chipMem_cnt				0x04		/*should always be less than 0xFF*/


#define udsIoCtrl_itemCnt		1



/* Exports */
extern const saLevelMap_t saLevelMap[saLvSptCnt];

/* ----------------------- End Security Access Level Support Define --- */

/* ----------------------- Service Support Config --------------------- */
#define udsServiceSptCnt 17
extern const udsServCfg_t udsServCfg[udsServiceSptCnt];
extern const uint8_t udsServCnt;
/* ----------------------- End Service Support Config ----------------- */

/* ----------------------- DID Support Config ------------------------- */

/* ----------------------- End DID Support Config --------------------- */

/* ----------------------- RID Support(Routine Control) Config -------- */
extern const uint8_t udsRtGlobalMask;
#define udsRtIDsptCnt 4
extern const udsRtIDcfg_t udsRtIDcfg[udsRtIDsptCnt];
extern const uint16_t udsRtIDcnt;
/* ----------------------- End RID Support(Routine Control) Config ---- */

/* --------Chip Memory configuration  -------- */
extern const chipMemCfg_t chipMemCfg[chipMem_cnt];
/* --------End Chip Memory configuration  -------- */


/* --------IO Control -------- */
extern const udsIoCtrlCfg_t udsIoCtrlCfg[udsIoCtrl_itemCnt];
/* --------End IO Control -------- */


/* --------Clear diagnostic information -------- */
#define DTC_POWERTRAIN_GROUP 0x00000000
#define DTC_CHASSIS_GROUP 0x00400000
#define DTC_BODY_GROUP 0x00800000
#define DTC_NETWORK_COMMUNICATION_GROUP 0x00C00000
#define DTC_ALL_GROUP 0x00FFFFFF
/* --------End Clear diagnostic information -------- */



/* ----------------------- Parameters config export ------------------- */
extern const uint32_t ssSwStd_saLockTime;
extern const uint8_t saLevelCnt;
extern const uint8_t saProtectLimitCnt;
extern const uint32_t saProtectDelayTime;
/* ----------------------- End Parameters config export --------------- */

/* ----------------------- Default set service logic function exxport ----------------------- */
extern void uds_sessionTimeout(void);
extern uint8_t uds_sessionSwitch(uint8_t actSession, uint8_t tragetSession);
extern void uds_saUpdateSeed(uint8_t saLevel);
extern uint8_t uds_saCheckKey(uint8_t saLevel);
extern void uds_didRwReady(void);
extern uint8_t  uds_getMemIndx(uint32_t memory, uint16_t size);
extern bool uds_ioCtrlAllowed(void);
/* ----------------------- End Default set service logic function exxport ------------------- */


#endif /*End of __UDS_CFG_H__ */
/*--------EOF--------*/

