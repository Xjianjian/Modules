
/*
* Copyright (c) File,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:canCfg.h
* Description: CAN Mailboxes and messages configrration header file, this is auto-generated
* by CCAG tool which is developed by HSNE and should never be modified manually
* REQ IDs: 
* History:
* Time:2020/5/7 11:33:53
* User:lixiangping
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef CAN_CFG_H_
#define CAN_CFG_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "hs_types.h"


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
/*message type definition*/
#define m_can_norMsg 1
#define m_can_udsMsg 2
#define m_can_ccpMsg 3
#define m_can_nmMsg  4


/*message mode definition*/
#define m_can_periodMsg 0x00
#define m_can_eventMsg 0x01
#define m_can_peMsg 0x02


/*message direction definition*/
#define m_can_dirRx 0x02
#define m_can_dirTx 0x01


/*can bus definition*/
#define m_CLASS_CAN 0x00
#define m_RESERVED_CAN 0x01
#define m_PT_CAN 0x02
#define m_can_noneCan 0xFF


/*invalid message index definition,the Macro value should be not less than 64(0x40)*/
#define m_can_invalidMsgIndx 0xFF


#define m_canBaud_250kps 0
#define m_canBaud_500kps 1
#define m_canBaud_1Mps 2
/*UDS Msg information*/
#define m_can_udsTxMsgIndx 26
#define m_can_udsRxMsgIndx 20
#define m_can_udsRxBus m_PT_CAN
#define m_can_udsTxBus m_PT_CAN
/*CCP Msg information*/
#define m_can_ccpTxMsgIndx 62
#define m_can_ccpRxMsgIndx 61
#define m_can_ccpRxBus m_PT_CAN
#define m_can_ccpTxBus m_PT_CAN



               
/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef enum 
{
       CCP_CAN_0x201_Msg = 0,
    CCP_CAN_0x211_Msg,
    CCP_CAN_0x3A1_Msg,
    CCP_CAN_0x241_Msg,
    CCP_CAN_0x101_Msg,
    CCP_CAN_0x232_Msg,
    CCP_CAN_0x222_Msg,
    CCP_CAN_0x58_Msg,
    CCP_CAN_0x3FE_Msg,
    CCP_CAN_0x4FE_Msg,
    CCP_CAN_0xA5_Msg,
    CCP_CAN_0x122_Msg,
    CCP_CAN_0x124_Msg,
    CCP_CAN_0x142_Msg,
    CCP_CAN_0x144_Msg,
    CCP_CAN_0x1BE_Msg,
    CCP_CAN_0x1C5_Msg,
    CCP_CAN_0x1D5_Msg,
    CCP_CAN_0x212_Msg,
    CCP_CAN_0x219_Msg,
    CCP_CAN_0x21A_Msg,
    CCP_CAN_0x242_Msg,
    CCP_CAN_0x252_Msg,
    CCP_CAN_0x272_Msg,
    CCP_CAN_0x282_Msg,
    CCP_CAN_0x2A1_Msg,
    CCP_CAN_0x2A2_Msg,
    CCP_CAN_0x2A5_Msg,
    CCP_CAN_0x2C2_Msg,
    CCP_CAN_0x2CE_Msg,
    CCP_CAN_0x2FE_Msg,
    CCP_CAN_0x319_Msg,
    CCP_CAN_0x341_Msg,
    CCP_CAN_0x380_Msg,
    CCP_CAN_0x39E_Msg
}CLASS_CAN_msg_t;
typedef enum 
{
    RESERVED_CAN_0x2A0_Msg = 0,
    RESERVED_CAN_0x2B5_Msg,
    RESERVED_CAN_0x2C5_Msg
}RESERVED_CAN_msg_t;
typedef enum 
{
    CAN_3_0x301_Msg = 0,
    CAN_3_0x302_Msg,
    CAN_3_0x303_Msg,
    CAN_3_0x304_Msg,
    CAN_3_0x5A4_Msg,
    CAN_3_0x2FA_Msg,
    CAN_3_0x5A5_Msg,
    CAN_3_0x331_Msg,
    CAN_3_0x321_Msg,
    CAN_3_0x561_Msg,
    CAN_3_0x551_Msg,
    CAN_3_0x111_Msg,
    CAN_3_0x401_Msg,
    CAN_3_0x511_Msg,
    CAN_3_0x521_Msg,
    CAN_3_0x531_Msg,
    CAN_3_0x5F1_Msg,
	CAN_3_0x181_Msg,
    CAN_3_0x101_Msg,
    CAN_3_0x441_Msg,
    CAN_3_0x3A6_Msg,
    CAN_3_0x731_Msg,
    CAN_3_0x7E7_Msg,
    CAN_3_0x7DF_Msg,
    CAN_3_0x330_Msg,
    CAN_3_0x797_Msg,
    CAN_3_0x7B1_Msg,
    CAN_3_0x7EF_Msg,
    CAN_3_0x3F3_Msg,
    CAN_3_0x492_Msg,
    CAN_3_0x394_Msg,
    CAN_3_0x407_Msg,
    CAN_3_0x487_Msg,
    CAN_3_0x75_Msg,
    CAN_3_0x7E_Msg,
    CAN_3_0x4FE_Msg,
    CAN_3_0x514_Msg,
    CAN_3_0x574_Msg,
    CAN_3_0x631_Msg,
    CAN_3_0x632_Msg,
    CAN_3_0x634_Msg,
    CAN_3_0x637_Msg,
    CAN_3_0x63E_Msg,
    CAN_3_0x102_Msg,
    CAN_3_0x103_Msg,
    CAN_3_0x104_Msg,
    CAN_3_0x112_Msg,
    CAN_3_0x113_Msg,
    CAN_3_0x114_Msg,
    CAN_3_0x126_Msg,
    CAN_3_0x134_Msg,
    CAN_3_0x2C1_Msg,
    CAN_3_0x309_Msg,
    CAN_3_0x312_Msg,
    CAN_3_0x313_Msg,
	 CAN_3_0x324_Msg,
    CAN_3_0x387_Msg,
    CAN_3_0x38D_Msg,
    CAN_3_0x38E_Msg,
    CAN_3_0x39E_Msg,
    CAN_3_0x334_Msg
}PT_CAN_msg_t;
typedef struct
{
    hs_u4_t     id;                 /*can message id*/
    hs_u1_t     msgType;            /*message type: uds,ccp or normal*/
    hs_u1_t     msgMode;            /*message mode, period, event or both*/
    hs_BOOL     isrEn;              /*interrupt enable:TRUE or FALSE*/
    hs_BOOL     extFrame;           /*can ID format: TRUE,Extended; FALSE, Standard*/
    hs_u1_t     dir;                /*CAN dir: Tx or Rx*/
    hs_u1_t     dlc;                /*Data length,unit: bytes*/
    hs_u2_t     period;             /*[m_can_dirRx]: the rx maximum tolerable waiting time, 0: unlimited waiting;    [m_can_dirTx]: tx period*/
    hs_u2_t     txFirstDelay;       /*only available for tx message,it is for the delay of the first frame of if*/
    hs_BOOL     *rxOverlap;         /*indicates if allow new rx message to overwrite the older one or not*/
    hs_u1_t     *buf;               /*tx buffer or rx buffer*/
    hs_BOOL     *rxTimeout;         /*to indicate a period rx message  is timeout or not*/
    hs_BOOL     *received;          /*to indicate a rx message  is received or not*/
    hs_BOOL     *txRdy;             /*whether a tx message is ready for transmission*/
    hs_BOOL     *txFailed;          /*whether a tx message is failed or not*/
    void        (*pack_unpcak)(void);/*pack or unpack*/
    void        (*cbHook)(void);    /*call back hook*/
    void        (*txSuccessCb)(void);/*tx success hook, only for the output message*/
}canMboxCfg_t;


typedef struct
{
    const volatile canMboxCfg_t * pCfg;
    hs_u2_t msgNum;
}canMboxCfgInfo_t;


typedef struct
{
    hs_BOOL msgRec;		/*only be available with the rx message, actually it is only efficient with the Rx  enable mailbox*/
    hs_u2_t timeRemain;	/*rx message : the timeout counter, when reaches 0, timeout occurred * tx message : the time interval for sending the message, when reaches 0, this message will be sent*/
    hs_u2_t txToCnt;	   /*tx message: once txToCnt reaches 0, means tx confirmation timeout*/
}canMsgScheData_t;


typedef struct
{
    hs_u4_t id;
    hs_u2_t orgIndx;
    hs_u1_t prioVal;
}canMsgSortInfo_t;


typedef struct
{
    const hs_u1_t * msg2Mbox;
    const hs_u1_t * mbox2Msg;
}canDynMap_t;




/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/


#endif

