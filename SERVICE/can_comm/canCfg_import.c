
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "globalVarExport.h"
#include "canCfg_signals.h"
#include "canCfg_export.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void PT_CAN_0x3B5_can1Can_ComTestRec_msg_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/
	hs_u1_t byte_i = 0;
	//PT_CAN_0x3C5_can1Can_ComTest_msg_txReady = TRUE;


}


void RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/
	hs_u1_t byte_i = 0;
	RESERVED_CAN_0x2C5_can2Can_ComTest_msg_txReady = TRUE;
	for(byte_i = 0; byte_i < 8; byte_i ++)
	{
		RESERVED_CAN_0x2C5_can2Can_ComTest_msg_txBuf[byte_i] = RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rxBuf[byte_i];
	}

}


void PT_CAN_0x1B5_can3Can_ComTestRec_msg_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/
	hs_u1_t byte_i = 0;
	PT_CAN_0x1C5_can3Can_ComTest_msg_txReady = TRUE;
	for(byte_i = 0; byte_i < 8; byte_i ++)
	{
		PT_CAN_0x1C5_can3Can_ComTest_msg_txBuf[byte_i] = PT_CAN_0x1B5_can3Can_ComTestRec_msg_rxBuf[byte_i];
	}

}

void PT_CAN_0x3A6_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x3F3_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x492_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x394_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x407_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x487_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x075_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x07E_po_ctrl_msg1_rx_cb(void)
{


}

void C_CAN_0x4FE_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x514_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x574_po_ctrl_msg1_rx_cb(void)
{


}
void PT_CAN_0x102_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x103_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x104_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x112_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x113_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x114_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x126_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x134_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x2C1_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x309_po_ctrl_msg1_rx_cb(void)
{


}
void PT_CAN_0x312_po_ctrl_msg1_rx_cb(void)
{


}
void PT_CAN_0x313_po_ctrl_msg1_rx_cb(void)
{


}
void PT_CAN_0x354_po_ctrl_msg1_rx_cb(void)
{


}
void PT_CAN_0x387_po_ctrl_msg1_rx_cb(void)
{


}
void PT_CAN_0x38D_po_ctrl_msg1_rx_cb(void)
{


}
void PT_CAN_0x38E_po_ctrl_msg1_rx_cb(void)
{


}
void C_CAN_0x39E_po_ctrl_msg1_rx_cb(void)
{

}

void C_CAN_0x222_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x232_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x058_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x3FE_po_ctrl_msg1_rx_cb(void)
{

}

void C_CAN_0x0A5_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x122_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x124_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x142_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x144_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x1BE_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x1C5_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x1D5_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x212_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x219_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x21A_po_ctrl_msg1_rx_cb(void)
{

}

void C_CAN_0x242_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x252_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x272_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x282_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x2A1_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x2A2_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x2A5_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x2C2_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x2CE_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x2FE_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x319_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x341_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x380_po_ctrl_msg1_rx_cb(void)
{

}

void PT_CAN_0x300_do_ctrl_msg_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/
	//PT_CAN_0x400_test_txReady = 1;
}

void PT_CAN_0x302_do_ctrl_msg_rx_cb(void)
{

}

void PT_CAN_0x303_do_ctrl_msg_rx_cb(void)
{

}

void PT_CAN_0x304_do_ctrl_msg_rx_cb(void)
{

}

void PT_CAN_0x310_pi_display_msg1_tx_cb(void)
{
    /*add your code here to realize the call-back hook function*/
	//pi_1 = (hs_u2_t)(hld_pi_chargerCP[0] * 2.5f);
	//pi_2 = (hs_u2_t)(hld_pi_chargerCP[1] * 2.5f);

}


void PT_CAN_0x311_pi_display_msg2_tx_cb(void)
{
    /*add your code here to realize the call-back hook function*/
	//pi_3 = (hs_u2_t)(hld_pi_chargerCP[2] * 2.5f);
	//pi_4 = (hs_u2_t)(hld_pi_chargerCP[3] * 2.5f);
	//pi_5 = (hs_u2_t)(hld_pi_chargerCP[4] * 2.5f);

}


void PT_CAN_0x312_pi_display_msg3_tx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}

void PT_CAN_0x731_msg_rx_cb(void)
{


}
/*
* Copyright (c) File ,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:CanCfg_import.c
* Description: this is a file to realize the call-back functions(if needed) for canCfg module,this file is up to user and can be modified freely.
* REQ IDs: 
* History:
* Time:2020/4/16 17:15:32
* User:lixiangping
*/

/* ---- Source switch on ------------------------------------------------------*/

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "globalVarExport.h"
#include "canCfg_signals.h"
#include "canCfg_export.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void CAN_3_0x301_VCU_pwr_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x302_MCUR0_temperature_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x303_MCUF0_temperature_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x304_BMS_temperature_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x5A4_VCU_DutyCycle_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x1B1_VCU_GCUCtlLmt_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/

	cod_vcu_1B1_Rollingcounter++;
}


void CAN_3_0x2FA_VCU_Indication_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x5A5_VCU_PTCPower_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x331_VCU_Rev_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x330_VCU_RevV2V_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x32E_VCU_RevV2VPower_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x321_VCU_alertMatrix0_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x561_VCU_range_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x551_VCU_standard_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x111_VCU_state_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/

	cod_vcu_111_RollingCounter++;
}


void CAN_3_0x401_VCU_subPower_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/

	cod_vcu_401_Rollingcounter++;
}


void CAN_3_0x511_VCU_tboxGeneralAlert_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x521_VCU_thermal_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x531_VCU_thermal2_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x5F1_VCU_version_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x101_VCU_torque_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/

	cod_vcu_101_Rollingcounter++;
}


void CAN_3_0x441_VCU_vehicle_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x3A6_AC_pwtAlloted_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x731_Diag_PhyReq_VCU_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x7E7_OBD_PhyReq_VCU_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x7DF_Diag_FuncReq_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x79A_CCP_Tx_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x797_CCP_Rx_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x7B1_Diag_Resp_VCU_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x7EF_OBD_Resp_VCU_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x3F3_IC_info_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x492_IC_state2_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x394_POT_state_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x407_OBC_state1_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x487_OBC_state2_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x75_IVI_control_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x7E_TBOX_chargeSet_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x4FE_TBOX_VehicleMode_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x514_BMS_metric_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x574_BMS_tboxRequest_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x631_NM_VCU_tx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x632_NM_MCUR0_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x634_NM_BMS_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x637_NM_OBCM_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x63E_NM_GW_PT_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x102_MCUR0_torque_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x103_MCUF0_torque_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x104_BMS_meas_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x112_MCUR0_state_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x113_MCUF0_state_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x114_BMS_state_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x126_BMS_RevFaultInfo_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x134_BMS_driveLimits_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x2C1_BCM_peps_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x309_GSM_state_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x312_MCUR0_state2_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x313_MCUF0_state2_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x354_BMS_Rev_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x387_OBC_state_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x38D_DCDC_state_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x38E_ACU_state_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CAN_3_0x39E_AC_temp_pressure_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x201_VCU_frontMotorTorque_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/

	cod_vcu_201_Rollingcounter++;
}


void CCP_CAN_0x211_VCU_rearMotorTorque_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/

	cod_vcu_211_Rollingcounter++;
}


void CCP_CAN_0x3A1_VCU_stateCha1_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/
	cod_vcu_3A1_rollingCounter++;

}


void CCP_CAN_0x241_VCU_stateCha2_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/

	cod_vcu_241_rollingCounter++;
}


void CCP_CAN_0x3B1_VCU_stateCha3_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x101_VCU_vehicleTorque_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/

	cod_vcu_101_veTorRollingcounter++;
}


void CCP_CAN_0x232_ESC_torqueReq_front_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x222_ESC_torqueReq_rear_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x58_TBOX_remoteCmd_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x3FE_IVI_timeAndGPS_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x4FE_DiagPhysReqToBMS_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0xA5_IVI_pwrSet_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x122_ESC_wheelSpd_Rear_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x124_EPS_state_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x142_ESC_wheelSpd_Front_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x144_EPS_SAS_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x1BE_IVI_chassisSet_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x1C5_iB_esc1_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x1D5_GW_icm_ivi_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x219_MRR_acc_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x21A_MPC_state_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x242_ESC_apaSet_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x252_ESC_yawState_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x272_ESC_wssFront_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x282_ESC_wssRear_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x2A1_TSV31_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x2A2_ESC_state2_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x2A5_iB_esc2_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x2C2_ESC_epb_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x2CE_BCM_peps_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x2FE_IC_state_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x319_SWM_Set_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x341_ASC_state1_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x380_ACU_state_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x39E_GW_AC_BCM_CHACAN_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}

void RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}


void CCP_CAN_0x739_ccp_txSuccessCb(void)
{
	ccpTxConfirmCb();
}


void CAN_3_0x181_GW_AC_BCM_CHACAN_txSuccessCb(void)
{
    /*add your code here to realize the call-back hook function*/
	cod_vcu_181_Rollingcounter++;

}
void CAN_3_0x324_BMS_alertMatrix0_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}
void CAN_3_0x334_BMS_alertMatrix1_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}

void CAN_3_0x631_NM_VCU_txSuccessCb(void)
{
	nm_sendSuccessCb(0);
}

void CAN_3_0x330_VCU_RevV2V_tx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}
/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/


/* ---- Source switch off ---------------------------------------------------- */

/*--- End of file ------------------------------------------------------------*/
