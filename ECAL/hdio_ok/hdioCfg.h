/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:hdioCfg.h
* Description:
* REQ IDs:
* History:
* 2016-08-11, Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef HDIO_CFG_H_
#define HDIO_CFG_H_
#include "typedefs.h"
#include "platformTypes.h"
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_hdio_debRuler		FALSE


/*High Side Inuput*/
#define m_hal_di_DI_IGN_SW_HS (0)
#define m_hal_di_BRAKE1_SW_HS (1)
#define m_hal_di_BRAKE2_SW_HS (2)
#define m_hal_di_FS0B_LS (3)
#define m_hal_di_05_HS (4)
#define m_hal_di_06_HS (5)
#define m_hal_di_07_HS (6)
#define m_hal_di_08_HS (7)
#define m_hal_di_09_LS (8)
#define m_hal_di_10_LS (9)
#define m_hal_di_11_LS (10)
#define m_hal_di_12_LS (11)
#define m_hal_di_LSD_SPARE10_FB_LS (12)
#define m_hal_di_LSD_SPARE9_FB_LS (13)
#define m_hal_di_APS1_PG_LS (14)
#define m_hal_di_APS2_PG_LS (15)
#define m_hal_di_LIQUID_LEVEL_SENSOR_PG_LS (16)
#define m_hal_di_PS_PG_LS (17)
#define m_hal_di_CAN0_ERR_LS (18)
#define m_hal_di_CAN1_ERR_LS (19)

#define m_hal_do_12V_DIGITAL_POWER_HS (20)
#define m_hal_do_PS_EN_HS (21)
#define m_hal_do_BD8LA700_RESET1_LS (22)
#define m_hal_do_BD8LA700_RESET2_LS (23)
#define m_hal_do_LSD_SPARE9_EN_HS (24)
#define m_hal_do_LSD_SPARE10_EN_HS (25)
#define m_hal_do_OPEN_LOAD_HS (26)
#define m_hal_do_H0_DIAGNOSE_EN_HS (27)
#define m_hal_do_DCDC_ENABLE_HS (28)
#define m_hal_do_BAT_COOLING_POWER_HS (29)
#define m_hal_do_MAIN_INTAKE_GRID_POWER_HS (30)
#define m_hal_do_BAT_PTC_POWER_HS (31)
#define m_hal_do_HO_CURRENT2_FB_CHOSE0 (32)
#define m_hal_do_HO_CURRENT2_FB_CHOSE1 (33)
#define m_hal_do_THREE_WAY_VALVE_POWER_HS (34)
#define m_hal_do_FOUR_WAY_VALVE_POWER_HS (35)
#define m_hal_do_ARI_COOLING_POWER_HS (36)
#define m_hal_do_COMPRESSOR_POWER_HS (37)
#define m_hal_do_HO_CURRENT1_FB_CHOSE0 (38)
#define m_hal_do_HO_CURRENT1_FB_CHOSE1 (39)
#define m_hal_do_BODY_CAN_STANDBY_H (40)
#define m_hal_do_CAN_EN (41)
#define m_hal_do_INTER_CAN_STANDBY_H (42)
#define m_hal_do_CAN_WAKE_HS (43)
#define m_hal_do_LIN_MODE_H (44)
#define m_hal_do_LIN_WAKE_H (45)
#define m_hdio_cnt  (46)

#define m_hdio_firstDiIndex 			 m_hal_di_DI_IGN_SW_HS
#define m_hdio_LastDiIndex	 			 m_hal_di_CAN1_ERR_LS
#define m_hdio_diSigCnt				    (m_hdio_LastDiIndex - m_hdio_firstDiIndex + 1)
#define m_hdio_firstDoIndex 		 	 m_hdio_diSigCnt
#define m_hdio_LastDoIndex 				(m_hdio_cnt - 1)
#define m_hdio_doSigCnt					(m_hdio_LastDoIndex - m_hdio_firstDoIndex + 1)
#define m_hdio_sigCnt					m_hdio_cnt

#define m_hdio_stLow 					0
#define m_hdio_stHigh 					1
#define m_hdio_stUnknown 				2



/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/


typedef void (* phdioStUpdt) (uint8_t);

typedef struct
{
	/*--->region: common*/
	uint8_t * sig;
	uint8_t  chanId;
	uint8_t  dir;
	uint8_t  pullType;
	bool     safeMode;
	bool     ioStd;	/*TRUE:normal IO, FALSE:inverse IO*/
	bool 	 slewFast;
	bool 	 *appLock;
	/*<---end region: common*/
	
	/*--->region: output pin private*/
	/*only be significant with the output pins*/
	bool     openDrain;
	bool 	 writeCheck;
	bool  	 initVal;
	/*<---end region: output pin private*/
	bool	 *lockBuf;
	
	/*--->region: input pin private*/
	/*only be significant with the input pins*/
	bool	 *readDeb;
	uint8_t * debBuf;
	phdioStUpdt fun;
	/*<---end region: input pin private*/
}hdioCfg_t;


/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/
extern uint8_t  e_di_ign_sw;
extern uint8_t	e_di_brake1_sw;
extern uint8_t 	e_di_brake2_sw;
extern uint8_t 	e_di_fs0b;
extern uint8_t 	e_di_can0_err;
extern uint8_t 	e_di_can1_err;
extern uint8_t 	e_di_aps1_pg;
extern uint8_t 	e_di_aps2_pg;
extern uint8_t 	e_di_liquid_lvl_sensor;
extern uint8_t 	e_di_ps_pg;
extern uint8_t 	e_di_04;//04-12 is reserve
extern uint8_t 	e_di_05;
extern uint8_t 	e_di_06;
extern uint8_t 	e_di_07;
extern uint8_t 	e_di_08;
extern uint8_t 	e_di_09;
extern uint8_t 	e_di_10;
extern uint8_t 	e_di_11;
extern uint8_t 	e_di_12;

extern uint8_t 	e_do_12v_digital_pwr;
extern uint8_t 	e_do_ps_en;
extern uint8_t 	e_do_open_load_en;
extern uint8_t 	e_do_three_way_pwr;
extern uint8_t 	e_do_four_way_pwr;
extern uint8_t 	e_do_ari_cooling_pwr;
extern uint8_t 	e_do_compressor_pwr;
extern uint8_t 	e_do_diagnose_en;
extern uint8_t 	e_do_curr1_fb_chose0;
extern uint8_t 	e_do_curr1_fb_chose1;
extern uint8_t 	e_do_dcdc_en;
extern uint8_t 	e_do_bat_cooling_pwr;
extern uint8_t 	e_do_main_intake_grid_pwr;
extern uint8_t 	e_do_bat_ptc_pwr;
extern uint8_t 	e_do_curr2_fb_chose0;
extern uint8_t 	e_do_curr2_fb_chose1;
extern uint8_t 	e_do_bd8l700_reset1;
extern uint8_t 	e_do_bd8l700_reset2;
extern uint8_t 	e_do_body_can_standby;
extern uint8_t 	e_do_can_en;
extern uint8_t 	e_do_inter_can_standby;
extern uint8_t 	e_do_lin_mode;
extern uint8_t 	e_do_lin_wake;



/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
extern const hdioCfg_t  hdioCfgGrp[m_hdio_sigCnt];
/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/


#endif




