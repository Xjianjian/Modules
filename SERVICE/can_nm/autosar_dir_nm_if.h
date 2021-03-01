/*
* Copyright (c) File,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:autosar_dir_nm_if.h
* Description: Autosar direct network manager interface header file.
* REQ IDs:
* History:
* Time:2018/5/4
* User:Devon
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef AUTOSAR_DIR_NM_IF_H_
#define AUTOSAR_DIR_NM_IF_H_ 

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "autosar_dir_nm_types.h"
#include "autosar_dir_nm_cfg.h"
/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/

//#define MODULE_CAN2_TX	 CAN_3_0x631_NM_VCU_txReady

/*nm node number*/
#define nm_ptcan_nodeNum (4u)
#define nm_can2_nodeNum (1u)

#define nmNode_period (60u)/*600ms*/
/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef struct
{
	nm_dWord_t id;
	nm_byte_t *timeoutFlg;
	nm_byte_t *timeoutcnt;
	nm_dWord_t	mask;
}nmIf_Rxpack_t;

/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/
extern nm_bool_t hld_di_ChargeWuBatC;
extern nm_bool_t hld_di_ChargeWuBatB;
extern nm_bool_t hld_di_chgActive;
extern nm_bool_t e_di_ign_sw;
extern nm_bool_t can_0_init_success;
extern nm_bool_t can_2_init_success;


//extern nm_byte_t nm_wkup_source[AUTOSAR_DIRNM_BUS_CNT];



/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
extern nm_byte_t *nm_can2_idPos;

/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/
extern nm_bool_t autosar_dirNm_init_ok(nm_byte_t bus_i);
extern void autosar_dirNm_send(nm_byte_t bus_i,nm_byte_t mode,nm_byte_t * buf);
extern void autosar_dirNm_setTxRxMode(unsigned char bus,unsigned char enable);
extern nm_bool_t autosar_dirNm_getLocalWuFlag(nm_byte_t bus);
extern nm_byte_t autosar_dirNm_getLocalWuSoc(nm_byte_t bus);
extern nm_bool_t autosar_dirNm_networkRequest(nm_byte_t bus);
extern void autosar_dirNm_rec_indication(nm_byte_t bus,nm_dWord_t id, nm_byte_t dlc, nm_byte_t * buf);
extern void autosar_dirNm_comCtrl(nm_byte_t bus, nm_bool_t enable);
extern void autosar_Nm_msgRepeatBitSt(nm_byte_t bus,nm_dWord_t id,nm_byte_t repeatByte);
extern void autosar_dirNm_no_autosar_sleep(void);
extern nm_bool_t autosar_dirNm_rec_realTime(nm_byte_t bus);
extern nm_bool_t autosar_dirNm_getLocalWuFlag_realTime(nm_byte_t bus);
extern void nm_rec_new_msg_clear(nm_byte_t num, nm_byte_t channel);
extern void autosar_Nm_clrRepeatFlag(uint8_t bus);
extern nm_bool_t nm_nodeNotTimeout(nm_byte_t ch);
extern uint8_t autosar_Nm_networkRelease(nm_byte_t bus);
extern void nm_sendSuccessCb(unsigned char bus);
extern void nm_timer_timeout_recvNMreset(nm_byte_t bus);
extern nm_bool_t autosar_dirNm_rec_realTime_timeout(nm_byte_t bus);
extern uint8_t autosar_Nm_appstatus(void);
extern uint8_t autosar_Nm_RepeatReqStatus(uint8_t bus);
#if TEST_MODE_FORTEMP
extern void autosar_dirNm_testSend(nm_byte_t bus_i,nm_byte_t * buf);
#endif
#endif


