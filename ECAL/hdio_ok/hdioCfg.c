/*
 * @Descripttion: 
 * @version: v1.0
 * @Author: lixiangping
 * @Date: 2019-10-11 10:08:42
 * @LastEditors: lixiangping
 * @LastEditTime: 2019-10-15 09:58:48
 */
/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:hdioCfg.c
* Description:
* REQ IDs:
* History:
* 2016-08-12, Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#define HDIO_CFG_C_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "dio.h"
#include "global_var.h"
#include "hdioCfg.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/


#define  m_hdi_strCatRaw(x)		x##_raw
#define  m_hdi_strCatFilter(x)	x##_filter


#define  m_hdo_strCatLock(x)	x##_lock
#define  m_hdo_strCatLockVal(x)	x##_lock_val


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/
//static void hdio_ignUpdtHook(uint8_t st);


/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/
uint8_t  	e_di_ign_sw;
uint8_t		e_di_brake1_sw;
uint8_t 	e_di_brake2_sw;
uint8_t 	e_di_fs0b;
uint8_t 	e_di_can0_err;
uint8_t 	e_di_can1_err;
uint8_t 	e_di_aps1_pg;
uint8_t 	e_di_aps2_pg;
uint8_t 	e_di_liquid_lvl_sensor;
uint8_t 	e_di_ps_pg;
uint8_t 	e_di_04;//04-12 is reserve
uint8_t 	e_di_05;
uint8_t 	e_di_06;
uint8_t 	e_di_07;
uint8_t 	e_di_08;
uint8_t 	e_di_09;
uint8_t 	e_di_10;
uint8_t 	e_di_11;
uint8_t 	e_di_12;

uint8_t 	e_do_12v_digital_pwr = 1;
uint8_t 	e_do_ps_en = 1;
uint8_t 	e_do_open_load_en;
uint8_t 	e_do_three_way_pwr;
uint8_t 	e_do_four_way_pwr;
uint8_t 	e_do_ari_cooling_pwr;
uint8_t 	e_do_compressor_pwr;
uint8_t 	e_do_diagnose_en;
uint8_t 	e_do_curr1_fb_chose0;
uint8_t 	e_do_curr1_fb_chose1;
uint8_t 	e_do_dcdc_en;
uint8_t 	e_do_bat_cooling_pwr;
uint8_t 	e_do_main_intake_grid_pwr;
uint8_t 	e_do_bat_ptc_pwr;
uint8_t 	e_do_curr2_fb_chose0;
uint8_t 	e_do_curr2_fb_chose1;
uint8_t 	e_do_bd8l700_reset1;
uint8_t 	e_do_bd8l700_reset2;
uint8_t 	e_do_body_can_standby = 1;
uint8_t 	e_do_can_en = 1;
uint8_t 	e_do_inter_can_standby = 1;
uint8_t 	e_do_lin_mode = 1;
uint8_t 	e_do_lin_wake = 1;
/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/
const hdioCfg_t  hdioCfgGrp[m_hdio_sigCnt] =
{
/*input signals*/
	{
		/*1*/
		&m_hdi_strCatRaw(hal_di_IGN_SW_HS),  /*signal*/
		SIUL_PA11,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_IGN_SW_HS),  /* readDeb*/
		&e_di_ign_sw,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*2*/
		&m_hdi_strCatRaw(hal_di_BRAKE1_SW_HS),  /*signal*/
		SIUL_PA12,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_BRAKE1_SW_HS),  /* readDeb*/
		&e_di_brake1_sw,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*3*/
		&m_hdi_strCatRaw(hal_di_BRAKE2_SW_HS),  /*signal*/
		SIUL_PD6,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_BRAKE2_SW_HS),  /* readDeb*/
		&e_di_brake2_sw,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*4*/
		&m_hdi_strCatRaw(hal_di_FS0B_LS),  /*signal*/
		SIUL_PF4,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_FS0B_LS),  /* readDeb*/
		&e_di_fs0b,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*5*/
		&m_hdi_strCatRaw(hal_di_05_HS),  /*signal*/
		SIUL_PD8,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_05_HS),  /* readDeb*/
		&e_di_05,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*6*/
		&m_hdi_strCatRaw(hal_di_06_HS),  /*signal*/
		SIUL_PE15,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_06_HS),  /* readDeb*/
		&e_di_06,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*7*/
		&m_hdi_strCatRaw(hal_di_07_HS),  /*signal*/
		SIUL_PA10,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_07_HS),  /* readDeb*/
		&e_di_07,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*8*/
		&m_hdi_strCatRaw(hal_di_08_HS),  /*signal*/
		SIUL_PD5,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_08_HS),  /* readDeb*/
		&e_di_08,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*9*/
		&m_hdi_strCatRaw(hal_di_09_LS),  /*signal*/
		SIUL_PD4,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_09_LS),  /* readDeb*/
		&e_di_09,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*10*/
		&m_hdi_strCatRaw(hal_di_10_LS),  /*signal*/
		SIUL_PE14,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_10_LS),  /* readDeb*/
		&e_di_10,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*11*/
		&m_hdi_strCatRaw(hal_di_11_LS),  /*signal*/
		SIUL_PB6,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_11_LS),  /* readDeb*/
		&e_di_11,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*12*/
		&m_hdi_strCatRaw(hal_di_12_LS),  /*signal*/
		SIUL_PD2,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_12_LS),  /* readDeb*/
		&e_di_12,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*13*/
		&m_hdi_strCatRaw(hal_di_LSD_SPARE10_FB_LS),  /*signal*/
		SIUL_PA2,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_LSD_SPARE10_FB_LS),  /* readDeb*/
		&hal_di_LSD_SPARE10_FB_LS,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*14*/
		&m_hdi_strCatRaw(hal_di_LSD_SPARE9_FB_LS),  /*signal*/
		SIUL_PF8,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_LSD_SPARE9_FB_LS),  /* readDeb*/
		&hal_di_LSD_SPARE9_FB_LS,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*15*/
		&m_hdi_strCatRaw(hal_di_APS1_PG_LS),  /*signal*/
		SIUL_PE11,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_APS1_PG_LS),  /* readDeb*/
		&e_di_aps1_pg,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*16*/
		&m_hdi_strCatRaw(hal_di_APS2_PG_LS),  /*signal*/
		SIUL_PC0,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_APS2_PG_LS),  /* readDeb*/
		&e_di_aps2_pg,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*17*/
		&m_hdi_strCatRaw(hal_di_LIQUID_LEVEL_SENSOR_PG_LS),  /*signal*/
		SIUL_PE12,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_LIQUID_LEVEL_SENSOR_PG_LS),  /* readDeb*/
		&e_di_liquid_lvl_sensor,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*18*/
		&m_hdi_strCatRaw(hal_di_PS_PG_LS),  /*signal*/
		SIUL_PE0,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_PS_PG_LS),  /* readDeb*/
		&e_di_ps_pg,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*19*/
		&m_hdi_strCatRaw(hal_di_CAN0_ERR_LS),  /*signal*/
		SIUL_PE13,  /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_CAN0_ERR_LS),  /* readDeb*/
		&e_di_can0_err,          /* debBuf*/
		NULL        /* fun*/
	},
	{
		/*20*/
		&m_hdi_strCatRaw(hal_di_CAN1_ERR_LS),  /*signal*/
		SIUL_PA9,       /* chanId*/
		m_dio_input,      /* dir*/
		m_dio_pullDisable,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		NULL,        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		0,         /* initVal*/
		NULL,        /* lockBuf*/
		&m_hdi_strCatFilter(hal_di_CAN1_ERR_LS),  /* readDeb*/
		&e_di_can1_err,          /* debBuf*/
		NULL        /* fun*/
	},
	
	/*output signals*/
	{
	    /*21*/
	    &e_do_12v_digital_pwr, /*signal*/
	    SIUL_PG6,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_12V_DIGITAL_POWER_HS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    TRUE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_12V_DIGITAL_POWER_HS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
		/*22*/
		&e_do_ps_en, /*signal*/
		SIUL_PF6,       /* chanId*/
		m_dio_output,      /* dir*/
		m_dio_pullUp,      /* pullType*/
		FALSE,        /* safeMode*/
		TRUE,        /* ioStd*/
		TRUE,        /* slewFast*/
		&m_hdo_strCatLock(hal_do_PS_EN_HS),        /* AppLock*/
		FALSE,        /* openDrain*/
		FALSE,        /* writeCheck*/
		TRUE,         /* initVal*/
		&m_hdo_strCatLockVal(hal_do_PS_EN_HS),        /* lockBuf*/
		NULL,  /* readDeb*/
		NULL,          /* debBuf*/
		NULL        /* fun*/
	},
	{
	    /*23*/
	    &hal_do_BD8LA700_RESET1_LS, /*signal*/
	    SIUL_PG8,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    FALSE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_BD8LA700_RESET1_LS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_BD8LA700_RESET1_LS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*24*/
	    &hal_do_BD8LA700_RESET2_LS, /*signal*/
	    SIUL_PF7,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    FALSE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_BD8LA700_RESET2_LS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_BD8LA700_RESET2_LS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*25*/
	    &hal_do_LSD_SPARE9_EN_HS, /*signal*/
	    SIUL_PD10,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_LSD_SPARE9_EN_HS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_LSD_SPARE9_EN_HS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*26*/
	    &hal_do_LSD_SPARE10_EN_HS, /*signal*/
	    SIUL_PA13,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_LSD_SPARE10_EN_HS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_LSD_SPARE10_EN_HS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*27*/
	    &e_do_open_load_en, /*signal*/
	    SIUL_PJ8,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_OPEN_LOAD_HS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_OPEN_LOAD_HS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*28*/
	    &e_do_diagnose_en, /*signal*/
	    SIUL_PA0,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_H0_DIAGNOSE_EN_HS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_H0_DIAGNOSE_EN_HS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*29*/
	    &e_do_dcdc_en, /*signal*/
	    SIUL_PG7,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_DCDC_ENABLE_HS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_DCDC_ENABLE_HS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*30*/
	    &e_do_bat_cooling_pwr, /*signal*/
	    SIUL_PG9,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_BAT_COOLING_POWER_HS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_BAT_COOLING_POWER_HS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*31*/
	    &e_do_main_intake_grid_pwr, /*signal*/
	    SIUL_PD11,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_MAIN_INTAKE_GRID_POWER_HS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_MAIN_INTAKE_GRID_POWER_HS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*32*/
	    &e_do_bat_ptc_pwr, /*signal*/
	    SIUL_PG10,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_BAT_PTC_POWER_HS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_BAT_PTC_POWER_HS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*33*/
	    &e_do_curr2_fb_chose0, /*signal*/
	    SIUL_PC4,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_HO_CURRENT2_FB_CHOSE0),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_HO_CURRENT2_FB_CHOSE0),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*34*/
	    &e_do_curr2_fb_chose1, /*signal*/
	    SIUL_PD7,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_HO_CURRENT2_FB_CHOSE1),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_HO_CURRENT2_FB_CHOSE1),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*35*/
	    &e_do_three_way_pwr, /*signal*/
	    SIUL_PF0,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_THREE_WAY_VALVE_POWER_HS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_THREE_WAY_VALVE_POWER_HS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*36*/
	    &e_do_four_way_pwr, /*signal*/
	    SIUL_PD0,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_FOUR_WAY_VALVE_POWER_HS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_FOUR_WAY_VALVE_POWER_HS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*37*/
	    &e_do_ari_cooling_pwr, /*signal*/
	    SIUL_PC15,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_ARI_COOLING_POWER_HS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_ARI_COOLING_POWER_HS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*38*/
	    &e_do_compressor_pwr, /*signal*/
	    SIUL_PF13,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_COMPRESSOR_POWER_HS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_COMPRESSOR_POWER_HS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*39*/
	    &e_do_curr1_fb_chose0, /*signal*/
	    SIUL_PA1,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_HO_CURRENT1_FB_CHOSE0),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_HO_CURRENT1_FB_CHOSE0),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*40*/
	    &e_do_curr1_fb_chose1, /*signal*/
	    SIUL_PG11,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_HO_CURRENT1_FB_CHOSE1),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_HO_CURRENT1_FB_CHOSE1),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*41*/
	    &e_do_body_can_standby, /*signal*/
	    SIUL_PC11,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_BODY_CAN_STANDBY_H),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_BODY_CAN_STANDBY_H),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*42*/
	    &e_do_can_en, /*signal*/
	    SIUL_PC10,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_CAN_EN),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_CAN_EN),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*43*/
	    &e_do_inter_can_standby, /*signal*/
	    SIUL_PA3,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_INTER_CAN_STANDBY_H),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_INTER_CAN_STANDBY_H),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*44*/
	    &hal_do_CAN_WAKE_HS, /*signal*/
	    SIUL_PD1,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_CAN_WAKE_HS),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    FALSE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_CAN_WAKE_HS),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*45*/
	    &e_do_lin_mode, /*signal*/
	    SIUL_PF10,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_LIN_MODE_H),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    TRUE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_LIN_MODE_H),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	},
	{
	    /*46*/
	    &e_do_lin_wake, /*signal*/
	    SIUL_PF9,       /* chanId*/
	    m_dio_output,      /* dir*/
	    m_dio_pullUp,      /* pullType*/
	    FALSE,        /* safeMode*/
	    TRUE,        /* ioStd*/
	    TRUE,        /* slewFast*/
	    &m_hdo_strCatLock(hal_do_LIN_WAKE_H),        /* AppLock*/
	    FALSE,        /* openDrain*/
	    FALSE,        /* writeCheck*/
	    TRUE,         /* initVal*/
	    &m_hdo_strCatLockVal(hal_do_LIN_WAKE_H),        /* lockBuf*/
	    NULL,  /* readDeb*/
	    NULL,          /* debBuf*/
	    NULL        /* fun*/
	}
};



/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
//static void hdio_ignUpdtHook(uint8_t st)
//{

//}

/* ---- Source switch off ---------------------------------------------------- */
#undef HDIO_CFG_C_
/*--- End of file ------------------------------------------------------------*/




