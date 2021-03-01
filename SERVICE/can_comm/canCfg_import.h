/*
* Copyright (c) File,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:canCfg_import.h
* Description: external declarations of  the call-back functions(if needed) for canCfg module,this file is up to user and can be modified freely.
* REQ IDs: 
* History:
* Time:2020/4/16 17:15:32
* User:lixiangping
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef CAN_CFG_IMPORT_H_
#define CAN_CFG_IMPORT_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "importTypes.h"

/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/



               
/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/


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
extern void CAN_3_0x301_VCU_pwr_txSuccessCb(void);
extern void CAN_3_0x302_MCUR0_temperature_rx_cb(void);
extern void CAN_3_0x303_MCUF0_temperature_rx_cb(void);
extern void CAN_3_0x304_BMS_temperature_rx_cb(void);
extern void CAN_3_0x5A4_VCU_DutyCycle_txSuccessCb(void);
extern void CAN_3_0x1B1_VCU_GCUCtlLmt_txSuccessCb(void);
extern void CAN_3_0x2FA_VCU_Indication_txSuccessCb(void);
extern void CAN_3_0x5A5_VCU_PTCPower_txSuccessCb(void);
extern void CAN_3_0x331_VCU_Rev_txSuccessCb(void);
extern void CAN_3_0x330_VCU_RevV2V_txSuccessCb(void);
extern void CAN_3_0x32E_VCU_RevV2VPower_txSuccessCb(void);
extern void CAN_3_0x321_VCU_alertMatrix0_txSuccessCb(void);
extern void CAN_3_0x561_VCU_range_txSuccessCb(void);
extern void CAN_3_0x551_VCU_standard_txSuccessCb(void);
extern void CAN_3_0x111_VCU_state_txSuccessCb(void);
extern void CAN_3_0x401_VCU_subPower_txSuccessCb(void);
extern void CAN_3_0x511_VCU_tboxGeneralAlert_txSuccessCb(void);
extern void CAN_3_0x521_VCU_thermal_txSuccessCb(void);
extern void CAN_3_0x531_VCU_thermal2_txSuccessCb(void);
extern void CAN_3_0x5F1_VCU_version_txSuccessCb(void);
extern void CAN_3_0x101_VCU_torque_txSuccessCb(void);
extern void CAN_3_0x441_VCU_vehicle_txSuccessCb(void);
extern void CAN_3_0x3A6_AC_pwtAlloted_rx_cb(void);
extern void CAN_3_0x731_Diag_PhyReq_VCU_rx_cb(void);
extern void CAN_3_0x7E7_OBD_PhyReq_VCU_rx_cb(void);
extern void CAN_3_0x7DF_Diag_FuncReq_rx_cb(void);
extern void CAN_3_0x79A_CCP_Tx_txSuccessCb(void);
extern void CAN_3_0x797_CCP_Rx_rx_cb(void);
extern void CAN_3_0x7B1_Diag_Resp_VCU_txSuccessCb(void);
extern void CAN_3_0x7EF_OBD_Resp_VCU_txSuccessCb(void);
extern void CAN_3_0x3F3_IC_info_rx_cb(void);
extern void CAN_3_0x492_IC_state2_rx_cb(void);
extern void CAN_3_0x394_POT_state_rx_cb(void);
extern void CAN_3_0x407_OBC_state1_rx_cb(void);
extern void CAN_3_0x487_OBC_state2_rx_cb(void);
extern void CAN_3_0x75_IVI_control_rx_cb(void);
extern void CAN_3_0x7E_TBOX_chargeSet_rx_cb(void);
extern void CAN_3_0x4FE_TBOX_VehicleMode_rx_cb(void);
extern void CAN_3_0x514_BMS_metric_rx_cb(void);
extern void CAN_3_0x574_BMS_tboxRequest_rx_cb(void);
extern void CAN_3_0x631_NM_VCU_tx_cb(void);
extern void CAN_3_0x632_NM_MCUR0_rx_cb(void);
extern void CAN_3_0x634_NM_BMS_rx_cb(void);
extern void CAN_3_0x637_NM_OBCM_rx_cb(void);
extern void CAN_3_0x63E_NM_GW_PT_rx_cb(void);
extern void CAN_3_0x102_MCUR0_torque_rx_cb(void);
extern void CAN_3_0x103_MCUF0_torque_rx_cb(void);
extern void CAN_3_0x104_BMS_meas_rx_cb(void);
extern void CAN_3_0x112_MCUR0_state_rx_cb(void);
extern void CAN_3_0x113_MCUF0_state_rx_cb(void);
extern void CAN_3_0x114_BMS_state_rx_cb(void);
extern void CAN_3_0x126_BMS_RevFaultInfo_rx_cb(void);
extern void CAN_3_0x134_BMS_driveLimits_rx_cb(void);
extern void CAN_3_0x2C1_BCM_peps_rx_cb(void);
extern void CAN_3_0x309_GSM_state_rx_cb(void);
extern void CAN_3_0x312_MCUR0_state2_rx_cb(void);
extern void CAN_3_0x313_MCUF0_state2_rx_cb(void);
extern void CAN_3_0x354_BMS_Rev_rx_cb(void);
extern void CAN_3_0x387_OBC_state_rx_cb(void);
extern void CAN_3_0x38D_DCDC_state_rx_cb(void);
extern void CAN_3_0x38E_ACU_state_rx_cb(void);
extern void CAN_3_0x39E_AC_temp_pressure_rx_cb(void);
extern void CCP_CAN_0x201_VCU_frontMotorTorque_txSuccessCb(void);
extern void CCP_CAN_0x211_VCU_rearMotorTorque_txSuccessCb(void);
extern void CCP_CAN_0x3A1_VCU_stateCha1_txSuccessCb(void);
extern void CCP_CAN_0x241_VCU_stateCha2_txSuccessCb(void);
extern void CCP_CAN_0x3B1_VCU_stateCha3_txSuccessCb(void);
extern void CCP_CAN_0x101_VCU_vehicleTorque_txSuccessCb(void);
extern void CCP_CAN_0x232_ESC_torqueReq_front_rx_cb(void);
extern void CCP_CAN_0x222_ESC_torqueReq_rear_rx_cb(void);
extern void CCP_CAN_0x58_TBOX_remoteCmd_rx_cb(void);
extern void CCP_CAN_0x3FE_IVI_timeAndGPS_rx_cb(void);
extern void CCP_CAN_0x4FE_DiagPhysReqToBMS_rx_cb(void);
extern void CCP_CAN_0xA5_IVI_pwrSet_rx_cb(void);
extern void CCP_CAN_0x122_ESC_wheelSpd_Rear_rx_cb(void);
extern void CCP_CAN_0x124_EPS_state_rx_cb(void);
extern void CCP_CAN_0x142_ESC_wheelSpd_Front_rx_cb(void);
extern void CCP_CAN_0x144_EPS_SAS_rx_cb(void);
extern void CCP_CAN_0x1BE_IVI_chassisSet_rx_cb(void);
extern void CCP_CAN_0x1C5_iB_esc1_rx_cb(void);
extern void CCP_CAN_0x1D5_GW_icm_ivi_rx_cb(void);
extern void CCP_CAN_0x219_MRR_acc_rx_cb(void);
extern void CCP_CAN_0x21A_MPC_state_rx_cb(void);
extern void CCP_CAN_0x242_ESC_apaSet_rx_cb(void);
extern void CCP_CAN_0x252_ESC_yawState_rx_cb(void);
extern void CCP_CAN_0x272_ESC_wssFront_rx_cb(void);
extern void CCP_CAN_0x282_ESC_wssRear_rx_cb(void);
extern void CCP_CAN_0x2A1_TSV31_rx_cb(void);
extern void CCP_CAN_0x2A2_ESC_state2_rx_cb(void);
extern void CCP_CAN_0x2A5_iB_esc2_rx_cb(void);
extern void CCP_CAN_0x2C2_ESC_epb_rx_cb(void);
extern void CCP_CAN_0x2CE_BCM_peps_rx_cb(void);
extern void CCP_CAN_0x2FE_IC_state_rx_cb(void);
extern void CCP_CAN_0x319_SWM_Set_rx_cb(void);
extern void CCP_CAN_0x341_ASC_state1_rx_cb(void);
extern void CCP_CAN_0x380_ACU_state_rx_cb(void);
extern void CCP_CAN_0x39E_GW_AC_BCM_CHACAN_rx_cb(void);
extern void RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rx_cb(void);
extern void CAN_3_0x181_GW_AC_BCM_CHACAN_txSuccessCb(void);
extern void CCP_CAN_0x739_ccp_txSuccessCb(void);
extern void CAN_3_0x324_BMS_alertMatrix0_rx_cb(void);
extern void CAN_3_0x631_NM_VCU_txSuccessCb(void);
extern void CAN_3_0x334_BMS_alertMatrix1_rx_cb(void);
extern void CAN_3_0x330_VCU_RevV2V_tx_cb(void);

#endif

