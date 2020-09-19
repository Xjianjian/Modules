
#ifndef CAN_CFG_EXPORT_H_
#define CAN_CFG_EXPORT_H_

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
#ifndef m_canMod_cnt
    #define m_canMod_cnt 3
#endif




               
/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef enum 
{
    PT_CAN_0x1A0_Msg = 0,
    PT_CAN_0x1B5_Msg,
    PT_CAN_0x1C5_Msg
}C_CAN_msg_t;
typedef enum 
{
    RESERVED_CAN_0x2A0_Msg = 0,
    RESERVED_CAN_0x2B5_Msg,
    RESERVED_CAN_0x2C5_Msg
}RESERVED_CAN_msg_t;
typedef enum 
{
    PT_CAN_0x300_Msg = 0,
    PT_CAN_0x301_Msg,
    PT_CAN_0x302_Msg,
    PT_CAN_0x303_Msg,
    PT_CAN_0x304_Msg,
    PT_CAN_0x305_Msg,
    PT_CAN_0x306_Msg,
    PT_CAN_0x307_Msg,
    PT_CAN_0x309_Msg,
    PT_CAN_0x30A_Msg,
    PT_CAN_0x30D_Msg,
    PT_CAN_0x310_Msg,
    PT_CAN_0x311_Msg,
    PT_CAN_0x312_Msg,
    PT_CAN_0x334_Msg,
    PT_CAN_0x336_Msg,
    PT_CAN_0x33F_Msg,
    PT_CAN_0x3A0_Msg,
    PT_CAN_0x3B5_Msg,
    PT_CAN_0x3C5_Msg,
    PT_CAN_0x797_Msg,
    PT_CAN_0x79A_Msg,
    PT_CAN_0x7DF_Msg
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


extern hs_u1_t PT_CAN_0x301_di_display_msg1_txBuf[8];
extern hs_BOOL PT_CAN_0x301_di_display_msg1_txReady;
extern hs_BOOL PT_CAN_0x301_di_display_msg1_txFail;
extern hs_u1_t PT_CAN_0x302_di_display_msg2_txBuf[8];
extern hs_BOOL PT_CAN_0x302_di_display_msg2_txReady;
extern hs_BOOL PT_CAN_0x302_di_display_msg2_txFail;
extern hs_u1_t PT_CAN_0x303_ai_displayFb_msg1_txBuf[8];
extern hs_BOOL PT_CAN_0x303_ai_displayFb_msg1_txReady;
extern hs_BOOL PT_CAN_0x303_ai_displayFb_msg1_txFail;
extern hs_u1_t PT_CAN_0x304_ai_display_msg1_txBuf[8];
extern hs_BOOL PT_CAN_0x304_ai_display_msg1_txReady;
extern hs_BOOL PT_CAN_0x304_ai_display_msg1_txFail;
extern hs_u1_t PT_CAN_0x305_ai_display_msg2_txBuf[8];
extern hs_BOOL PT_CAN_0x305_ai_display_msg2_txReady;
extern hs_BOOL PT_CAN_0x305_ai_display_msg2_txFail;
extern hs_u1_t PT_CAN_0x306_ai_display_msg3_txBuf[8];
extern hs_BOOL PT_CAN_0x306_ai_display_msg3_txReady;
extern hs_BOOL PT_CAN_0x306_ai_display_msg3_txFail;
extern hs_u1_t PT_CAN_0x307_ai_display_msg4_txBuf[8];
extern hs_BOOL PT_CAN_0x307_ai_display_msg4_txReady;
extern hs_BOOL PT_CAN_0x307_ai_display_msg4_txFail;
extern hs_u1_t PT_CAN_0x309_ai_display_msg6_txBuf[8];
extern hs_BOOL PT_CAN_0x309_ai_display_msg6_txReady;
extern hs_BOOL PT_CAN_0x309_ai_display_msg6_txFail;
extern hs_u1_t PT_CAN_0x30A_di_display_msg3_txBuf[8];
extern hs_BOOL PT_CAN_0x30A_di_display_msg3_txReady;
extern hs_BOOL PT_CAN_0x30A_di_display_msg3_txFail;





extern hs_u1_t RESERVED_CAN_0x2A0_can2WakeUp_display_msg_txBuf[8];
extern hs_BOOL RESERVED_CAN_0x2A0_can2WakeUp_display_msg_txReady;
extern hs_BOOL RESERVED_CAN_0x2A0_can2WakeUp_display_msg_txFail;
extern hs_BOOL RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rxOverLap;
extern hs_u1_t RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rxBuf[8];
extern hs_BOOL RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rxTimeout;
extern hs_BOOL RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_received;
extern hs_u1_t RESERVED_CAN_0x2C5_can2Can_ComTest_msg_txBuf[8];
extern hs_BOOL RESERVED_CAN_0x2C5_can2Can_ComTest_msg_txReady;
extern hs_BOOL RESERVED_CAN_0x2C5_can2Can_ComTest_msg_txFail;
extern hs_u1_t PT_CAN_0x1A0_can3WakeUp_display_msg_txBuf[8];
extern hs_BOOL PT_CAN_0x1A0_can3WakeUp_display_msg_txReady;
extern hs_BOOL PT_CAN_0x1A0_can3WakeUp_display_msg_txFail;
extern hs_BOOL PT_CAN_0x1B5_can3Can_ComTestRec_msg_rxOverLap;
extern hs_u1_t PT_CAN_0x1B5_can3Can_ComTestRec_msg_rxBuf[8];
extern hs_BOOL PT_CAN_0x1B5_can3Can_ComTestRec_msg_rxTimeout;
extern hs_BOOL PT_CAN_0x1B5_can3Can_ComTestRec_msg_received;
extern hs_u1_t PT_CAN_0x1C5_can3Can_ComTest_msg_txBuf[8];
extern hs_BOOL PT_CAN_0x1C5_can3Can_ComTest_msg_txReady;
extern hs_BOOL PT_CAN_0x1C5_can3Can_ComTest_msg_txFail;

/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
extern const hs_u1_t canModuleNum;
extern const canMboxCfgInfo_t canMsgCfgInfoGrp[m_canMod_cnt];
extern canMsgScheData_t * const canMsgScheCfgInfoGrp[m_canMod_cnt];
extern canMsgSortInfo_t * const canMsgSortInfoGrp[m_canMod_cnt];
extern const canDynMap_t canDynMapGrp[m_canMod_cnt];
extern const hs_u1_t udsRxCanBus;
extern const hs_u1_t udsTxCanBus;
extern const hs_u1_t ccpRxCanBus;
extern const hs_u1_t ccpTxCanBus;
extern const hs_u2_t udsRxMsgIndx;
extern const hs_u2_t udsTxMsgIndx;
extern const hs_u2_t ccpRxMsgIndx;
extern const hs_u2_t ccpTxMsgIndx;
extern const hs_u1_t canBaudCfg[3];




/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/


#endif
