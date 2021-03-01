/*
 * Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
 * All Rights Reserved.
 * Dept.: Software Department
 * File:stationManage.c
 * Description:
 * REQ IDs:
 * History:
 * 2016-12-21, Devon Draft version
 */

 /* ---- Source switch on ------------------------------------------------------*/

 /*------------------------------------------------------------------------------
					 Include files
 ------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "global_var.h"
#include "canCfg_export.h"
#include "hdioCfg.h"
#include "dataLink.h"
//#include "networkManageFault.h"
//#include "networkManageFaultCfg.h"
#include "stationManage.h"
#include "stationManageCfg.h"

 /*-----------------------------------------------------------------------------
						 Module local defines and constants
 ------------------------------------------------------------------------------*/



 /*------------------------------------------------------------------------------
								  Module local types
 ------------------------------------------------------------------------------*/
static uint8_t sm_busoffEventCnt[m_canMod_cnt];
static uint16_t sm_busoff_tQuick[m_canMod_cnt];
static uint16_t sm_busoff_tQTimes[m_canMod_cnt];
static uint16_t sm_busoff_tSlow[m_canMod_cnt];
static uint8_t	sm_mode[m_canMod_cnt];
static uint8_t * sm_busoff_dtc_st[m_canMod_cnt] = {&hcan_Ccan_busoffFlag,&hcan_can1_busoffFlag,&hcan_PTcan_busoffFlag};

static uint8_t sm_busoff_firstFlag[m_canMod_cnt] = {0};
 /*------------------------------------------------------------------------------
								 Local data at RAM
 ------------------------------------------------------------------------------*/


 /*------------------------------------------------------------------------------
								 Local data at ROM
 ------------------------------------------------------------------------------*/




 /*------------------------------------------------------------------------------
							  Local data at EEPROM
 ------------------------------------------------------------------------------*/

 /*------------------------------------------------------------------------------
								 Global data at RAM
 ------------------------------------------------------------------------------*/

 /*------------------------------------------------------------------------------
								 Global data at ROM
 ------------------------------------------------------------------------------*/

 /*------------------------------------------------------------------------------
							  Global data at EEPROM
 ------------------------------------------------------------------------------*/

 /*------------------------------------------------------------------------------
						  Declaration of local functions
 ------------------------------------------------------------------------------*/
static void sm_busoffHandle(uint8_t bus);
static void sm_busoff_txEnable(uint8_t bus,uint8_t en);
 /*------------------------------------------------------------------------------
					   Implementation of global functions
 ------------------------------------------------------------------------------*/
 void sm_init(void)
 {
	uint8_t bus_i;

	for(bus_i = 0;bus_i <m_canMod_cnt;bus_i++)
	{
		sm_busoff_tQTimes[bus_i] = 0;
		sm_busoffEventCnt[bus_i] = 0;
		sm_busoff_tQuick[bus_i] = 0;
		sm_busoff_tSlow[bus_i] = 0;
		sm_mode[bus_i] = 0;
	}

 }





void sm_handleCyclic(void)
{
	uint8_t bus_i;

	for(bus_i = 0;bus_i <m_canMod_cnt;bus_i++)
	{
		if(bus_i == 1) continue;//can1 预留

		sm_busoffHandle(bus_i);

		if(NULL != sm_busoff_dtc_st[bus_i])
		{
			if(sm_busoffEventCnt[bus_i] >= 8)
			{
				*sm_busoff_dtc_st[bus_i] = 1;
			}
		}
	}
}


static void sm_busoffHandle(uint8_t bus)
{
	switch(sm_mode[bus])
	{
		case 0://check idle
		{

		}
		break;
		case 1://quick recover
		{
			sm_busoff_tQuick[bus]++;
			if(sm_busoff_tQuick[bus] >= m_sm_tQuickTime)
			{
				sm_busoff_tQuick[bus] = 0;
				sm_busoff_txEnable(bus,1);
				sm_busoff_tQTimes[bus]--;
				if(0 == sm_busoff_tQTimes[bus])
				{
					sm_busoff_tSlow[bus] = 0;
					sm_mode[bus] = 2;
				}
			}
		}
		break;
		case 2://slow recover
		{
			sm_busoff_tSlow[bus]++;
			if(sm_busoff_tSlow[bus] >= m_sm_tSlowTime)
			{
				sm_busoff_tSlow[bus] = 0;
				sm_busoff_txEnable(bus,1);
			}
		}
		break;
		default:
			sm_mode[bus] = 0;
			sm_busoffEventCnt[bus] = 0;
		break;
	}
}


void sm_busoffCb(uint8_t bus)
{
	sm_busoff_txEnable(bus,0);
	sm_busoffEventCnt[bus]++;
	if(0 == sm_busoff_firstFlag[bus])
	{
		sm_busoff_firstFlag[bus] = 1;
		sm_busoff_tQTimes[bus] = m_sm_tQTimes;
		sm_busoff_tQuick[bus] = 0;
		sm_mode[bus] = 1;
	}
}

void sm_busoffRecoverCb(uint8_t bus)
{
	sm_mode[bus] = 0;
	sm_busoffEventCnt[bus] = 0;
	sm_busoff_firstFlag[bus] = 0;
	if(NULL != sm_busoff_dtc_st[bus])
	{
		*sm_busoff_dtc_st[bus] = 0;
	}
}

static void sm_busoff_txEnable(uint8_t bus,uint8_t en)
{
	if(en)
	{
		hcan_setILMode(bus,CAN_IL_APP_SEND,1);
		if(2 == bus)
		{
			autosar_dirNm_setTxRxMode(0, 1);
		}
	}
	else
	{
		hcan_setILMode(bus,CAN_IL_APP_SEND,0);
		if(2 == bus)
		{
			autosar_dirNm_setTxRxMode(0, 0);
		}
	}
}
