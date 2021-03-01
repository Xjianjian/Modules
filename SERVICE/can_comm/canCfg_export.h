
/*
* Copyright (c) File,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:canCfg_export.h
* Description: CAN Mailboxes and messages configuration export header files, including some macros definition, global variables or consts export declartion, etc.
* What is more, this header file can only be included by other modules, should never be included by canCfg module itself
* REQ IDs: 
* History:
* Time:2020/5/7 11:33:53
* User:lixiangping
*/

/* ---- Source switch on ------------------------------------------------------*/
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

}C_CAN_msg_t;
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
extern hs_u1_t CAN_3_0x301_VCU_pwr_txBuf[8];
extern hs_BOOL CAN_3_0x301_VCU_pwr_txReady;
extern hs_BOOL CAN_3_0x301_VCU_pwr_txFail;
extern hs_BOOL CAN_3_0x302_MCUR0_temperature_rxOverLap;
extern hs_u1_t CAN_3_0x302_MCUR0_temperature_rxBuf[8];
extern hs_BOOL CAN_3_0x302_MCUR0_temperature_rxTimeout;
extern hs_BOOL CAN_3_0x302_MCUR0_temperature_received;
extern hs_BOOL CAN_3_0x303_MCUF0_temperature_rxOverLap;
extern hs_u1_t CAN_3_0x303_MCUF0_temperature_rxBuf[8];
extern hs_BOOL CAN_3_0x303_MCUF0_temperature_rxTimeout;
extern hs_BOOL CAN_3_0x303_MCUF0_temperature_received;
extern hs_BOOL CAN_3_0x304_BMS_temperature_rxOverLap;
extern hs_u1_t CAN_3_0x304_BMS_temperature_rxBuf[8];
extern hs_BOOL CAN_3_0x304_BMS_temperature_rxTimeout;
extern hs_BOOL CAN_3_0x304_BMS_temperature_received;
extern hs_u1_t CAN_3_0x5A4_VCU_DutyCycle_txBuf[8];
extern hs_BOOL CAN_3_0x5A4_VCU_DutyCycle_txReady;
extern hs_BOOL CAN_3_0x5A4_VCU_DutyCycle_txFail;
extern hs_u1_t CAN_3_0x1B1_VCU_GCUCtlLmt_txBuf[8];
extern hs_BOOL CAN_3_0x1B1_VCU_GCUCtlLmt_txReady;
extern hs_BOOL CAN_3_0x1B1_VCU_GCUCtlLmt_txFail;
extern hs_u1_t CAN_3_0x2FA_VCU_Indication_txBuf[8];
extern hs_BOOL CAN_3_0x2FA_VCU_Indication_txReady;
extern hs_BOOL CAN_3_0x2FA_VCU_Indication_txFail;
extern hs_u1_t CAN_3_0x5A5_VCU_PTCPower_txBuf[8];
extern hs_BOOL CAN_3_0x5A5_VCU_PTCPower_txReady;
extern hs_BOOL CAN_3_0x5A5_VCU_PTCPower_txFail;
extern hs_u1_t CAN_3_0x331_VCU_Rev_txBuf[8];
extern hs_BOOL CAN_3_0x331_VCU_Rev_txReady;
extern hs_BOOL CAN_3_0x331_VCU_Rev_txFail;
extern hs_u1_t CAN_3_0x330_VCU_RevV2V_txBuf[8];
extern hs_BOOL CAN_3_0x330_VCU_RevV2V_txReady;
extern hs_BOOL CAN_3_0x330_VCU_RevV2V_txFail;
extern hs_u1_t CAN_3_0x32E_VCU_RevV2VPower_txBuf[8];
extern hs_BOOL CAN_3_0x32E_VCU_RevV2VPower_txReady;
extern hs_BOOL CAN_3_0x32E_VCU_RevV2VPower_txFail;
extern hs_u1_t CAN_3_0x321_VCU_alertMatrix0_txBuf[8];
extern hs_BOOL CAN_3_0x321_VCU_alertMatrix0_txReady;
extern hs_BOOL CAN_3_0x321_VCU_alertMatrix0_txFail;
extern hs_u1_t CAN_3_0x561_VCU_range_txBuf[8];
extern hs_BOOL CAN_3_0x561_VCU_range_txReady;
extern hs_BOOL CAN_3_0x561_VCU_range_txFail;
extern hs_u1_t CAN_3_0x551_VCU_standard_txBuf[8];
extern hs_BOOL CAN_3_0x551_VCU_standard_txReady;
extern hs_BOOL CAN_3_0x551_VCU_standard_txFail;
extern hs_u1_t CAN_3_0x111_VCU_state_txBuf[8];
extern hs_BOOL CAN_3_0x111_VCU_state_txReady;
extern hs_BOOL CAN_3_0x111_VCU_state_txFail;
extern hs_u1_t CAN_3_0x401_VCU_subPower_txBuf[8];
extern hs_BOOL CAN_3_0x401_VCU_subPower_txReady;
extern hs_BOOL CAN_3_0x401_VCU_subPower_txFail;
extern hs_u1_t CAN_3_0x511_VCU_tboxGeneralAlert_txBuf[8];
extern hs_BOOL CAN_3_0x511_VCU_tboxGeneralAlert_txReady;
extern hs_BOOL CAN_3_0x511_VCU_tboxGeneralAlert_txFail;
extern hs_u1_t CAN_3_0x521_VCU_thermal_txBuf[8];
extern hs_BOOL CAN_3_0x521_VCU_thermal_txReady;
extern hs_BOOL CAN_3_0x521_VCU_thermal_txFail;
extern hs_u1_t CAN_3_0x531_VCU_thermal2_txBuf[8];
extern hs_BOOL CAN_3_0x531_VCU_thermal2_txReady;
extern hs_BOOL CAN_3_0x531_VCU_thermal2_txFail;
extern hs_u1_t CAN_3_0x5F1_VCU_version_txBuf[8];
extern hs_BOOL CAN_3_0x5F1_VCU_version_txReady;
extern hs_BOOL CAN_3_0x5F1_VCU_version_txFail;
extern hs_u1_t CAN_3_0x101_VCU_torque_txBuf[8];
extern hs_BOOL CAN_3_0x101_VCU_torque_txReady;
extern hs_BOOL CAN_3_0x101_VCU_torque_txFail;
extern hs_u1_t CAN_3_0x441_VCU_vehicle_txBuf[8];
extern hs_BOOL CAN_3_0x441_VCU_vehicle_txReady;
extern hs_BOOL CAN_3_0x441_VCU_vehicle_txFail;
extern hs_BOOL CAN_3_0x3A6_AC_pwtAlloted_rxOverLap;
extern hs_u1_t CAN_3_0x3A6_AC_pwtAlloted_rxBuf[8];
extern hs_BOOL CAN_3_0x3A6_AC_pwtAlloted_rxTimeout;
extern hs_BOOL CAN_3_0x3A6_AC_pwtAlloted_received;
extern hs_BOOL CAN_3_0x731_Diag_PhyReq_VCU_rxOverLap;
extern hs_u1_t CAN_3_0x731_Diag_PhyReq_VCU_rxBuf[8];
extern hs_BOOL CAN_3_0x731_Diag_PhyReq_VCU_rxTimeout;
extern hs_BOOL CAN_3_0x731_Diag_PhyReq_VCU_received;
extern hs_BOOL CAN_3_0x7E7_OBD_PhyReq_VCU_rxOverLap;
extern hs_u1_t CAN_3_0x7E7_OBD_PhyReq_VCU_rxBuf[8];
extern hs_BOOL CAN_3_0x7E7_OBD_PhyReq_VCU_rxTimeout;
extern hs_BOOL CAN_3_0x7E7_OBD_PhyReq_VCU_received;
extern hs_BOOL CAN_3_0x7DF_Diag_FuncReq_rxOverLap;
extern hs_u1_t CAN_3_0x7DF_Diag_FuncReq_rxBuf[8];
extern hs_BOOL CAN_3_0x7DF_Diag_FuncReq_rxTimeout;
extern hs_BOOL CAN_3_0x7DF_Diag_FuncReq_received;
extern hs_u1_t CAN_3_0x79A_CCP_Tx_txBuf[8];
extern hs_BOOL CAN_3_0x79A_CCP_Tx_txReady;
extern hs_BOOL CAN_3_0x79A_CCP_Tx_txFail;
extern hs_BOOL CAN_3_0x797_CCP_Rx_rxOverLap;
extern hs_u1_t CAN_3_0x797_CCP_Rx_rxBuf[8];
extern hs_BOOL CAN_3_0x797_CCP_Rx_rxTimeout;
extern hs_BOOL CAN_3_0x797_CCP_Rx_received;
extern hs_u1_t CAN_3_0x7B1_Diag_Resp_VCU_txBuf[8];
extern hs_BOOL CAN_3_0x7B1_Diag_Resp_VCU_txReady;
extern hs_BOOL CAN_3_0x7B1_Diag_Resp_VCU_txFail;
extern hs_u1_t CAN_3_0x7EF_OBD_Resp_VCU_txBuf[8];
extern hs_BOOL CAN_3_0x7EF_OBD_Resp_VCU_txReady;
extern hs_BOOL CAN_3_0x7EF_OBD_Resp_VCU_txFail;
extern hs_BOOL CAN_3_0x3F3_IC_info_rxOverLap;
extern hs_u1_t CAN_3_0x3F3_IC_info_rxBuf[8];
extern hs_BOOL CAN_3_0x3F3_IC_info_rxTimeout;
extern hs_BOOL CAN_3_0x3F3_IC_info_received;
extern hs_BOOL CAN_3_0x492_IC_state2_rxOverLap;
extern hs_u1_t CAN_3_0x492_IC_state2_rxBuf[8];
extern hs_BOOL CAN_3_0x492_IC_state2_rxTimeout;
extern hs_BOOL CAN_3_0x492_IC_state2_received;
extern hs_BOOL CAN_3_0x394_POT_state_rxOverLap;
extern hs_u1_t CAN_3_0x394_POT_state_rxBuf[8];
extern hs_BOOL CAN_3_0x394_POT_state_rxTimeout;
extern hs_BOOL CAN_3_0x394_POT_state_received;
extern hs_BOOL CAN_3_0x407_OBC_state1_rxOverLap;
extern hs_u1_t CAN_3_0x407_OBC_state1_rxBuf[8];
extern hs_BOOL CAN_3_0x407_OBC_state1_rxTimeout;
extern hs_BOOL CAN_3_0x407_OBC_state1_received;
extern hs_BOOL CAN_3_0x487_OBC_state2_rxOverLap;
extern hs_u1_t CAN_3_0x487_OBC_state2_rxBuf[8];
extern hs_BOOL CAN_3_0x487_OBC_state2_rxTimeout;
extern hs_BOOL CAN_3_0x487_OBC_state2_received;
extern hs_BOOL CAN_3_0x75_IVI_control_rxOverLap;
extern hs_u1_t CAN_3_0x75_IVI_control_rxBuf[8];
extern hs_BOOL CAN_3_0x75_IVI_control_rxTimeout;
extern hs_BOOL CAN_3_0x75_IVI_control_received;
extern hs_BOOL CAN_3_0x7E_TBOX_chargeSet_rxOverLap;
extern hs_u1_t CAN_3_0x7E_TBOX_chargeSet_rxBuf[8];
extern hs_BOOL CAN_3_0x7E_TBOX_chargeSet_rxTimeout;
extern hs_BOOL CAN_3_0x7E_TBOX_chargeSet_received;
extern hs_BOOL CAN_3_0x4FE_TBOX_VehicleMode_rxOverLap;
extern hs_u1_t CAN_3_0x4FE_TBOX_VehicleMode_rxBuf[8];
extern hs_BOOL CAN_3_0x4FE_TBOX_VehicleMode_rxTimeout;
extern hs_BOOL CAN_3_0x4FE_TBOX_VehicleMode_received;
extern hs_BOOL CAN_3_0x514_BMS_metric_rxOverLap;
extern hs_u1_t CAN_3_0x514_BMS_metric_rxBuf[8];
extern hs_BOOL CAN_3_0x514_BMS_metric_rxTimeout;
extern hs_BOOL CAN_3_0x514_BMS_metric_received;
extern hs_BOOL CAN_3_0x574_BMS_tboxRequest_rxOverLap;
extern hs_u1_t CAN_3_0x574_BMS_tboxRequest_rxBuf[8];
extern hs_BOOL CAN_3_0x574_BMS_tboxRequest_rxTimeout;
extern hs_BOOL CAN_3_0x574_BMS_tboxRequest_received;
extern hs_u1_t CAN_3_0x631_NM_VCU_txBuf[8];
extern hs_BOOL CAN_3_0x631_NM_VCU_txReady;
extern hs_BOOL CAN_3_0x631_NM_VCU_txFail;
extern hs_BOOL CAN_3_0x632_NM_MCUR0_rxOverLap;
extern hs_u1_t CAN_3_0x632_NM_MCUR0_rxBuf[8];
extern hs_BOOL CAN_3_0x632_NM_MCUR0_rxTimeout;
extern hs_BOOL CAN_3_0x632_NM_MCUR0_received;
extern hs_BOOL CAN_3_0x634_NM_BMS_rxOverLap;
extern hs_u1_t CAN_3_0x634_NM_BMS_rxBuf[8];
extern hs_BOOL CAN_3_0x634_NM_BMS_rxTimeout;
extern hs_BOOL CAN_3_0x634_NM_BMS_received;
extern hs_BOOL CAN_3_0x637_NM_OBCM_rxOverLap;
extern hs_u1_t CAN_3_0x637_NM_OBCM_rxBuf[8];
extern hs_BOOL CAN_3_0x637_NM_OBCM_rxTimeout;
extern hs_BOOL CAN_3_0x637_NM_OBCM_received;
extern hs_BOOL CAN_3_0x63E_NM_GW_PT_rxOverLap;
extern hs_u1_t CAN_3_0x63E_NM_GW_PT_rxBuf[8];
extern hs_BOOL CAN_3_0x63E_NM_GW_PT_rxTimeout;
extern hs_BOOL CAN_3_0x63E_NM_GW_PT_received;
extern hs_BOOL CAN_3_0x102_MCUR0_torque_rxOverLap;
extern hs_u1_t CAN_3_0x102_MCUR0_torque_rxBuf[8];
extern hs_BOOL CAN_3_0x102_MCUR0_torque_rxTimeout;
extern hs_BOOL CAN_3_0x102_MCUR0_torque_received;
extern hs_BOOL CAN_3_0x103_MCUF0_torque_rxOverLap;
extern hs_u1_t CAN_3_0x103_MCUF0_torque_rxBuf[8];
extern hs_BOOL CAN_3_0x103_MCUF0_torque_rxTimeout;
extern hs_BOOL CAN_3_0x103_MCUF0_torque_received;
extern hs_BOOL CAN_3_0x104_BMS_meas_rxOverLap;
extern hs_u1_t CAN_3_0x104_BMS_meas_rxBuf[8];
extern hs_BOOL CAN_3_0x104_BMS_meas_rxTimeout;
extern hs_BOOL CAN_3_0x104_BMS_meas_received;
extern hs_BOOL CAN_3_0x112_MCUR0_state_rxOverLap;
extern hs_u1_t CAN_3_0x112_MCUR0_state_rxBuf[8];
extern hs_BOOL CAN_3_0x112_MCUR0_state_rxTimeout;
extern hs_BOOL CAN_3_0x112_MCUR0_state_received;
extern hs_BOOL CAN_3_0x113_MCUF0_state_rxOverLap;
extern hs_u1_t CAN_3_0x113_MCUF0_state_rxBuf[8];
extern hs_BOOL CAN_3_0x113_MCUF0_state_rxTimeout;
extern hs_BOOL CAN_3_0x113_MCUF0_state_received;
extern hs_BOOL CAN_3_0x114_BMS_state_rxOverLap;
extern hs_u1_t CAN_3_0x114_BMS_state_rxBuf[8];
extern hs_BOOL CAN_3_0x114_BMS_state_rxTimeout;
extern hs_BOOL CAN_3_0x114_BMS_state_received;
extern hs_BOOL CAN_3_0x126_BMS_RevFaultInfo_rxOverLap;
extern hs_u1_t CAN_3_0x126_BMS_RevFaultInfo_rxBuf[8];
extern hs_BOOL CAN_3_0x126_BMS_RevFaultInfo_rxTimeout;
extern hs_BOOL CAN_3_0x126_BMS_RevFaultInfo_received;
extern hs_BOOL CAN_3_0x134_BMS_driveLimits_rxOverLap;
extern hs_u1_t CAN_3_0x134_BMS_driveLimits_rxBuf[8];
extern hs_BOOL CAN_3_0x134_BMS_driveLimits_rxTimeout;
extern hs_BOOL CAN_3_0x134_BMS_driveLimits_received;
extern hs_BOOL CAN_3_0x2C1_BCM_peps_rxOverLap;
extern hs_u1_t CAN_3_0x2C1_BCM_peps_rxBuf[8];
extern hs_BOOL CAN_3_0x2C1_BCM_peps_rxTimeout;
extern hs_BOOL CAN_3_0x2C1_BCM_peps_received;
extern hs_BOOL CAN_3_0x309_GSM_state_rxOverLap;
extern hs_u1_t CAN_3_0x309_GSM_state_rxBuf[8];
extern hs_BOOL CAN_3_0x309_GSM_state_rxTimeout;
extern hs_BOOL CAN_3_0x309_GSM_state_received;
extern hs_BOOL CAN_3_0x312_MCUR0_state2_rxOverLap;
extern hs_u1_t CAN_3_0x312_MCUR0_state2_rxBuf[8];
extern hs_BOOL CAN_3_0x312_MCUR0_state2_rxTimeout;
extern hs_BOOL CAN_3_0x312_MCUR0_state2_received;
extern hs_BOOL CAN_3_0x313_MCUF0_state2_rxOverLap;
extern hs_u1_t CAN_3_0x313_MCUF0_state2_rxBuf[8];
extern hs_BOOL CAN_3_0x313_MCUF0_state2_rxTimeout;
extern hs_BOOL CAN_3_0x313_MCUF0_state2_received;
extern hs_BOOL CAN_3_0x354_BMS_Rev_rxOverLap;
extern hs_u1_t CAN_3_0x354_BMS_Rev_rxBuf[8];
extern hs_BOOL CAN_3_0x354_BMS_Rev_rxTimeout;
extern hs_BOOL CAN_3_0x354_BMS_Rev_received;
extern hs_BOOL CAN_3_0x387_OBC_state_rxOverLap;
extern hs_u1_t CAN_3_0x387_OBC_state_rxBuf[8];
extern hs_BOOL CAN_3_0x387_OBC_state_rxTimeout;
extern hs_BOOL CAN_3_0x387_OBC_state_received;
extern hs_BOOL CAN_3_0x38D_DCDC_state_rxOverLap;
extern hs_u1_t CAN_3_0x38D_DCDC_state_rxBuf[8];
extern hs_BOOL CAN_3_0x38D_DCDC_state_rxTimeout;
extern hs_BOOL CAN_3_0x38D_DCDC_state_received;
extern hs_BOOL CAN_3_0x38E_ACU_state_rxOverLap;
extern hs_u1_t CAN_3_0x38E_ACU_state_rxBuf[8];
extern hs_BOOL CAN_3_0x38E_ACU_state_rxTimeout;
extern hs_BOOL CAN_3_0x38E_ACU_state_received;
extern hs_BOOL CAN_3_0x39E_AC_temp_pressure_rxOverLap;
extern hs_u1_t CAN_3_0x39E_AC_temp_pressure_rxBuf[8];
extern hs_BOOL CAN_3_0x39E_AC_temp_pressure_rxTimeout;
extern hs_BOOL CAN_3_0x39E_AC_temp_pressure_received;
extern hs_u1_t CCP_CAN_0x201_VCU_frontMotorTorque_txBuf[8];
extern hs_BOOL CCP_CAN_0x201_VCU_frontMotorTorque_txReady;
extern hs_BOOL CCP_CAN_0x201_VCU_frontMotorTorque_txFail;
extern hs_u1_t CCP_CAN_0x211_VCU_rearMotorTorque_txBuf[8];
extern hs_BOOL CCP_CAN_0x211_VCU_rearMotorTorque_txReady;
extern hs_BOOL CCP_CAN_0x211_VCU_rearMotorTorque_txFail;
extern hs_u1_t CCP_CAN_0x3A1_VCU_stateCha1_txBuf[8];
extern hs_BOOL CCP_CAN_0x3A1_VCU_stateCha1_txReady;
extern hs_BOOL CCP_CAN_0x3A1_VCU_stateCha1_txFail;
extern hs_u1_t CCP_CAN_0x241_VCU_stateCha2_txBuf[8];
extern hs_BOOL CCP_CAN_0x241_VCU_stateCha2_txReady;
extern hs_BOOL CCP_CAN_0x241_VCU_stateCha2_txFail;
extern hs_u1_t CCP_CAN_0x3B1_VCU_stateCha3_txBuf[8];
extern hs_BOOL CCP_CAN_0x3B1_VCU_stateCha3_txReady;
extern hs_BOOL CCP_CAN_0x3B1_VCU_stateCha3_txFail;
extern hs_u1_t CCP_CAN_0x101_VCU_vehicleTorque_txBuf[8];
extern hs_BOOL CCP_CAN_0x101_VCU_vehicleTorque_txReady;
extern hs_BOOL CCP_CAN_0x101_VCU_vehicleTorque_txFail;
extern hs_BOOL CCP_CAN_0x232_ESC_torqueReq_front_rxOverLap;
extern hs_u1_t CCP_CAN_0x232_ESC_torqueReq_front_rxBuf[8];
extern hs_BOOL CCP_CAN_0x232_ESC_torqueReq_front_rxTimeout;
extern hs_BOOL CCP_CAN_0x232_ESC_torqueReq_front_received;
extern hs_BOOL CCP_CAN_0x222_ESC_torqueReq_rear_rxOverLap;
extern hs_u1_t CCP_CAN_0x222_ESC_torqueReq_rear_rxBuf[8];
extern hs_BOOL CCP_CAN_0x222_ESC_torqueReq_rear_rxTimeout;
extern hs_BOOL CCP_CAN_0x222_ESC_torqueReq_rear_received;
extern hs_BOOL CCP_CAN_0x58_TBOX_remoteCmd_rxOverLap;
extern hs_u1_t CCP_CAN_0x58_TBOX_remoteCmd_rxBuf[8];
extern hs_BOOL CCP_CAN_0x58_TBOX_remoteCmd_rxTimeout;
extern hs_BOOL CCP_CAN_0x58_TBOX_remoteCmd_received;
extern hs_BOOL CCP_CAN_0x3FE_IVI_timeAndGPS_rxOverLap;
extern hs_u1_t CCP_CAN_0x3FE_IVI_timeAndGPS_rxBuf[8];
extern hs_BOOL CCP_CAN_0x3FE_IVI_timeAndGPS_rxTimeout;
extern hs_BOOL CCP_CAN_0x3FE_IVI_timeAndGPS_received;
extern hs_BOOL CCP_CAN_0x4FE_DiagPhysReqToBMS_rxOverLap;
extern hs_u1_t CCP_CAN_0x4FE_DiagPhysReqToBMS_rxBuf[8];
extern hs_BOOL CCP_CAN_0x4FE_DiagPhysReqToBMS_rxTimeout;
extern hs_BOOL CCP_CAN_0x4FE_DiagPhysReqToBMS_received;
extern hs_BOOL CCP_CAN_0xA5_IVI_pwrSet_rxOverLap;
extern hs_u1_t CCP_CAN_0xA5_IVI_pwrSet_rxBuf[8];
extern hs_BOOL CCP_CAN_0xA5_IVI_pwrSet_rxTimeout;
extern hs_BOOL CCP_CAN_0xA5_IVI_pwrSet_received;
extern hs_BOOL CCP_CAN_0x122_ESC_wheelSpd_Rear_rxOverLap;
extern hs_u1_t CCP_CAN_0x122_ESC_wheelSpd_Rear_rxBuf[8];
extern hs_BOOL CCP_CAN_0x122_ESC_wheelSpd_Rear_rxTimeout;
extern hs_BOOL CCP_CAN_0x122_ESC_wheelSpd_Rear_received;
extern hs_BOOL CCP_CAN_0x124_EPS_state_rxOverLap;
extern hs_u1_t CCP_CAN_0x124_EPS_state_rxBuf[8];
extern hs_BOOL CCP_CAN_0x124_EPS_state_rxTimeout;
extern hs_BOOL CCP_CAN_0x124_EPS_state_received;
extern hs_BOOL CCP_CAN_0x142_ESC_wheelSpd_Front_rxOverLap;
extern hs_u1_t CCP_CAN_0x142_ESC_wheelSpd_Front_rxBuf[8];
extern hs_BOOL CCP_CAN_0x142_ESC_wheelSpd_Front_rxTimeout;
extern hs_BOOL CCP_CAN_0x142_ESC_wheelSpd_Front_received;
extern hs_BOOL CCP_CAN_0x144_EPS_SAS_rxOverLap;
extern hs_u1_t CCP_CAN_0x144_EPS_SAS_rxBuf[8];
extern hs_BOOL CCP_CAN_0x144_EPS_SAS_rxTimeout;
extern hs_BOOL CCP_CAN_0x144_EPS_SAS_received;
extern hs_BOOL CCP_CAN_0x1BE_IVI_chassisSet_rxOverLap;
extern hs_u1_t CCP_CAN_0x1BE_IVI_chassisSet_rxBuf[8];
extern hs_BOOL CCP_CAN_0x1BE_IVI_chassisSet_rxTimeout;
extern hs_BOOL CCP_CAN_0x1BE_IVI_chassisSet_received;
extern hs_BOOL CCP_CAN_0x1C5_iB_esc1_rxOverLap;
extern hs_u1_t CCP_CAN_0x1C5_iB_esc1_rxBuf[8];
extern hs_BOOL CCP_CAN_0x1C5_iB_esc1_rxTimeout;
extern hs_BOOL CCP_CAN_0x1C5_iB_esc1_received;
extern hs_BOOL CCP_CAN_0x1D5_GW_icm_ivi_rxOverLap;
extern hs_u1_t CCP_CAN_0x1D5_GW_icm_ivi_rxBuf[8];
extern hs_BOOL CCP_CAN_0x1D5_GW_icm_ivi_rxTimeout;
extern hs_BOOL CCP_CAN_0x1D5_GW_icm_ivi_received;
extern hs_BOOL CCP_CAN_0x212_ESC_state1_rxOverLap;
extern hs_u1_t CCP_CAN_0x212_ESC_state1_rxBuf[8];
extern hs_BOOL CCP_CAN_0x212_ESC_state1_rxTimeout;
extern hs_BOOL CCP_CAN_0x212_ESC_state1_received;
extern hs_BOOL CCP_CAN_0x219_MRR_acc_rxOverLap;
extern hs_u1_t CCP_CAN_0x219_MRR_acc_rxBuf[8];
extern hs_BOOL CCP_CAN_0x219_MRR_acc_rxTimeout;
extern hs_BOOL CCP_CAN_0x219_MRR_acc_received;
extern hs_BOOL CCP_CAN_0x21A_MPC_state_rxOverLap;
extern hs_u1_t CCP_CAN_0x21A_MPC_state_rxBuf[8];
extern hs_BOOL CCP_CAN_0x21A_MPC_state_rxTimeout;
extern hs_BOOL CCP_CAN_0x21A_MPC_state_received;
extern hs_BOOL CCP_CAN_0x242_ESC_apaSet_rxOverLap;
extern hs_u1_t CCP_CAN_0x242_ESC_apaSet_rxBuf[8];
extern hs_BOOL CCP_CAN_0x242_ESC_apaSet_rxTimeout;
extern hs_BOOL CCP_CAN_0x242_ESC_apaSet_received;
extern hs_BOOL CCP_CAN_0x252_ESC_yawState_rxOverLap;
extern hs_u1_t CCP_CAN_0x252_ESC_yawState_rxBuf[8];
extern hs_BOOL CCP_CAN_0x252_ESC_yawState_rxTimeout;
extern hs_BOOL CCP_CAN_0x252_ESC_yawState_received;
extern hs_BOOL CCP_CAN_0x272_ESC_wssFront_rxOverLap;
extern hs_u1_t CCP_CAN_0x272_ESC_wssFront_rxBuf[8];
extern hs_BOOL CCP_CAN_0x272_ESC_wssFront_rxTimeout;
extern hs_BOOL CCP_CAN_0x272_ESC_wssFront_received;
extern hs_BOOL CCP_CAN_0x282_ESC_wssRear_rxOverLap;
extern hs_u1_t CCP_CAN_0x282_ESC_wssRear_rxBuf[8];
extern hs_BOOL CCP_CAN_0x282_ESC_wssRear_rxTimeout;
extern hs_BOOL CCP_CAN_0x282_ESC_wssRear_received;
extern hs_BOOL CCP_CAN_0x2A1_TSV31_rxOverLap;
extern hs_u1_t CCP_CAN_0x2A1_TSV31_rxBuf[8];
extern hs_BOOL CCP_CAN_0x2A1_TSV31_rxTimeout;
extern hs_BOOL CCP_CAN_0x2A1_TSV31_received;
extern hs_BOOL CCP_CAN_0x2A2_ESC_state2_rxOverLap;
extern hs_u1_t CCP_CAN_0x2A2_ESC_state2_rxBuf[8];
extern hs_BOOL CCP_CAN_0x2A2_ESC_state2_rxTimeout;
extern hs_BOOL CCP_CAN_0x2A2_ESC_state2_received;
extern hs_BOOL CCP_CAN_0x2A5_iB_esc2_rxOverLap;
extern hs_u1_t CCP_CAN_0x2A5_iB_esc2_rxBuf[8];
extern hs_BOOL CCP_CAN_0x2A5_iB_esc2_rxTimeout;
extern hs_BOOL CCP_CAN_0x2A5_iB_esc2_received;
extern hs_BOOL CCP_CAN_0x2C2_ESC_epb_rxOverLap;
extern hs_u1_t CCP_CAN_0x2C2_ESC_epb_rxBuf[8];
extern hs_BOOL CCP_CAN_0x2C2_ESC_epb_rxTimeout;
extern hs_BOOL CCP_CAN_0x2C2_ESC_epb_received;
extern hs_BOOL CCP_CAN_0x2CE_BCM_peps_rxOverLap;
extern hs_u1_t CCP_CAN_0x2CE_BCM_peps_rxBuf[8];
extern hs_BOOL CCP_CAN_0x2CE_BCM_peps_rxTimeout;
extern hs_BOOL CCP_CAN_0x2CE_BCM_peps_received;
extern hs_BOOL CCP_CAN_0x2FE_IC_state_rxOverLap;
extern hs_u1_t CCP_CAN_0x2FE_IC_state_rxBuf[8];
extern hs_BOOL CCP_CAN_0x2FE_IC_state_rxTimeout;
extern hs_BOOL CCP_CAN_0x2FE_IC_state_received;
extern hs_BOOL CCP_CAN_0x319_SWM_Set_rxOverLap;
extern hs_u1_t CCP_CAN_0x319_SWM_Set_rxBuf[8];
extern hs_BOOL CCP_CAN_0x319_SWM_Set_rxTimeout;
extern hs_BOOL CCP_CAN_0x319_SWM_Set_received;
extern hs_BOOL CCP_CAN_0x341_ASC_state1_rxOverLap;
extern hs_u1_t CCP_CAN_0x341_ASC_state1_rxBuf[8];
extern hs_BOOL CCP_CAN_0x341_ASC_state1_rxTimeout;
extern hs_BOOL CCP_CAN_0x341_ASC_state1_received;
extern hs_BOOL CCP_CAN_0x380_ACU_state_rxOverLap;
extern hs_u1_t CCP_CAN_0x380_ACU_state_rxBuf[8];
extern hs_BOOL CCP_CAN_0x380_ACU_state_rxTimeout;
extern hs_BOOL CCP_CAN_0x380_ACU_state_received;
extern hs_BOOL CCP_CAN_0x39E_GW_AC_BCM_CHACAN_rxOverLap;
extern hs_u1_t CCP_CAN_0x39E_GW_AC_BCM_CHACAN_rxBuf[8];
extern hs_BOOL CCP_CAN_0x39E_GW_AC_BCM_CHACAN_rxTimeout;
extern hs_BOOL CCP_CAN_0x39E_GW_AC_BCM_CHACAN_received;
extern hs_u1_t CAN_3_0x181_GW_AC_BCM_CHACAN_txBuf[8];
extern hs_BOOL CAN_3_0x181_GW_AC_BCM_CHACAN_txReady;
extern hs_BOOL CAN_3_0x181_GW_AC_BCM_CHACAN_txFail;
extern hs_BOOL CAN_3_0x324_BMS_alertMatrix0_rxOverLap;
extern hs_u1_t CAN_3_0x324_BMS_alertMatrix0_rxBuf[8];
extern hs_BOOL CAN_3_0x324_BMS_alertMatrix0_rxTimeout;
extern hs_BOOL CAN_3_0x324_BMS_alertMatrix0_received;
extern hs_BOOL CAN_3_0x334_BMS_alertMatrix1_rxOverLap;
extern hs_u1_t CAN_3_0x334_BMS_alertMatrix1_rxBuf[8];
extern hs_BOOL CAN_3_0x334_BMS_alertMatrix1_rxTimeout;
extern hs_BOOL CAN_3_0x334_BMS_alertMatrix1_received;

/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
extern const uint16_t m_udsRx_phyId;
extern const uint16_t m_udsRx_funId;
extern const uint16_t m_udsTx_id;
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
extern const hs_u2_t nmTxMsgIndx;



/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/


#endif

