/*
* Copyright (c) File 2018-04-29,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:lin_com.c
* Description:
* REQ IDs:
* History:
* 2018-06-14, Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/


/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "lin_msr.h"
#include "linNM.h"
/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/
static uint8_t linNM_runMode[e_linmax];
static uint8_t linNM_frameslot_T[e_linmax];
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
static void linNM_mode_enter(uint8_t indx,uint8_t mode);
/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void linNM_init(void)
{
	uint8_t indx;
	for(indx = 0;indx < e_linmax;indx++)
	{
		linNM_mode_enter(indx,LINNM_MODE_INIT);
		linNM_frameslot_T[indx] = 0;
	}
}


void linNM_task(void)
{
	uint8_t indx;
	for(indx = 0;indx < e_linmax;indx++)
	{
		switch(linNM_runMode[indx])
		{
			case LINNM_MODE_INIT:
			{
				if(linNMCfg_istonormal())
				{
					linNM_mode_enter(indx,LINNM_MODE_NORMAL);
				}
			}
			break;
			case LINNM_MODE_NORMAL:
			{
				if(linNMCfg_istosleep())
				{
					linNM_mode_enter(indx,LINNM_MODE_SLEEP);
				}
			}
			break;
			case LINNM_MODE_SLEEP:
			{
				linNM_frameslot_T[indx]++;
				if(linNM_frameslot_T[indx] >= LINMSR_FRAMESLOT_T)
				{
					//send sleep request
					lin_msr_sendSleepRequest(indx);
					linNM_mode_enter(indx,LINNM_MODE_INIT);
				}
			}
			break;
			default:

			break;
		}
	}
}


static void linNM_mode_enter(uint8_t indx,uint8_t mode)
{
	switch(mode)
	{
		case LINNM_MODE_INIT:
		{
			lin_msr_setTxRxMode(indx,LINNM_TXRX_STOP);
		}
		break;
		case LINNM_MODE_NORMAL:
		{
			lin_msr_setTxRxMode(indx,LINNM_TXRX_START);
		}
		break;
		case LINNM_MODE_SLEEP:
		{
			linNM_frameslot_T[indx] = 0;
			lin_msr_setTxRxMode(indx,LINNM_TXRX_STOP);
		}
		break;
		default:
		break;
	}
	linNM_runMode[indx] = mode;
}

/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/


