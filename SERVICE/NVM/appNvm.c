 /*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:appNvm.c
* Description:
* REQ IDs:
* History:
* 2016-10-06,  Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#define APP_NVM_C_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "exEE25LC256.h"
#include "eepromBlockCfg.h"
#include "eepromHandle.h"
#include "appNvm.h"
#include "obd.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/

static uint8_t immeBlockCnt = 0;

static uint16_t appNvmWaitTick = 0;

static uint8_t appNvmCurBlock = 0xFF;		/*0xFF means App Nvm is not handled any block*/

static uint8_t appNvmWaitAddBlock = 0xFF;

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

/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void anv_init(void)
{
	uint8_t blockIndx = 0;
	anv_defDtInit();
	eeh_init();
	immeBlockCnt = 0;
	for(;blockIndx < nvmBlockCnt; blockIndx ++)
	{
		if(TRUE == nvmBlockInfoGrp[blockIndx].major)
		{
			if(m_eehBlkSt_Ok != eeh_getBlkSt(blockIndx))
			{
				nvmBlockDirty[blockIndx] = TRUE;
			}
			else if(TRUE == nvmBlockInfoGrp[blockIndx].backUp)
			{
				if(TRUE == eeh_addJob(nvmBlockInfoGrp[blockIndx].bkUpBlock,m_eeJob_Write,TRUE))
				{
					while(m_eehSt_idle != eeh_getSt())
					{
						eeh_cyclic();
						ee256_cyclic();
					}
				}
			}
		}
	}
	appNvmWaitTick = 0;
}


void anv_cyclic(void)
{
	uint8_t blockIndx = 0;
	if(appNvmCurBlock < nvmBlockCnt)
	{
		if((m_eehBlkSt_writeOk == eeh_getBlkSt(appNvmCurBlock)) || (m_eehBlkSt_writeFail == eeh_getBlkSt(appNvmCurBlock)))
		{
			/*the ram data has been successfully written into the Eeprom or write fail*/
			nvmBlockDirty[appNvmCurBlock] = FALSE;	
			appNvmCurBlock = 0xFF;
			appNvmWaitTick = 0;
		}
	}
	else if(0xFF == appNvmWaitAddBlock)
	{
		for(;blockIndx < nvmBlockCnt; blockIndx ++)
		{
			if((TRUE == nvmBlockInfoGrp[blockIndx].imdt) 
				&& (TRUE == nvmBlockDirty[blockIndx]))
			{
				if(TRUE == eeh_addJob(blockIndx,m_eeJob_Write,FALSE))
				{
					appNvmCurBlock = blockIndx;
					appNvmWaitAddBlock = 0xFF;
					break;
				}
				else
				{
					appNvmWaitAddBlock = blockIndx;
					appNvmWaitTick = m_appNvm_waitMaxTick;
				}
			}
		}
	}
	else
	{
		if(TRUE == eeh_addJob(appNvmWaitAddBlock,m_eeJob_Write,FALSE))
		{
			appNvmCurBlock = blockIndx;
			appNvmWaitAddBlock = 0xFF;
		}
		else
		{
			/*waiting here until timeout*/
			if(appNvmWaitTick > 0)
			{
				appNvmWaitTick --;
			}
			else
			{
				appNvmWaitTick = m_appNvm_shutDownWaitMaxTick;
				/*timeout event occurs, so have to abandon this write request*/
				nvmBlockDirty[appNvmWaitAddBlock] = FALSE;
				appNvmWaitAddBlock = 0xFF;
			}
		}
		
	}	
}

void anv_modifyReq(uint8_t block)
{
	if((block < nvmBlockCnt) && (TRUE == nvmBlockInfoGrp[block].major))
	{
		nvmBlockDirty[block] = TRUE;
		if(TRUE == nvmBlockInfoGrp[block].backUp && nvmBlockInfoGrp[block].bkUpBlock < nvmBlockCnt)
		{
			nvmBlockDirty[nvmBlockInfoGrp[block].bkUpBlock] = TRUE;
		}
	}
	else
	{

	}
}


void anv_shutdownCyclic(void)
{
	uint8_t blockIndx = 0;
	if(appNvmCurBlock < nvmBlockCnt)
	{
		if((m_eehBlkSt_writeOk == eeh_getBlkSt(appNvmCurBlock)) ||  (m_eehBlkSt_writeFail == eeh_getBlkSt(appNvmCurBlock)))
		{
			/*the ram data has been successfully written into the Eeprom or write fail*/
			nvmBlockDirty[appNvmCurBlock] = FALSE;	
			appNvmCurBlock = 0xFF;
		}
	}
	else if(0xFF == appNvmWaitAddBlock)
	{
		for(;blockIndx < nvmBlockCnt; blockIndx ++)
		{
			if((TRUE == nvmBlockDirty[blockIndx]))
			{
				if(TRUE == eeh_addJob(blockIndx,m_eeJob_Write,FALSE))
				{
					appNvmCurBlock = blockIndx;
					appNvmWaitAddBlock = 0xFF;
					break;
				}
				else
				{
					appNvmWaitAddBlock = blockIndx;
					appNvmWaitTick = m_appNvm_shutDownWaitMaxTick;
				}
			}
		}
	}
	else
	{
		if(TRUE == eeh_addJob(appNvmWaitAddBlock,m_eeJob_Write,FALSE))
		{
			appNvmCurBlock = blockIndx;
			appNvmWaitAddBlock = 0xFF;
		}
		else
		{
			/*waiting here until timeout*/
			if(appNvmWaitTick > 0)
			{
				appNvmWaitTick --;
			}
			else
			{
				appNvmWaitTick = m_appNvm_shutDownWaitMaxTick;
				/*timeout event occurs, so have to abandon this write request*/
				nvmBlockDirty[appNvmWaitAddBlock] = FALSE;
				appNvmWaitAddBlock = 0xFF;
			}
		}
	}
}

/*only be efficient for major block*/
bool anv_noDirtyBlock(void)
{
	bool noDirtyBlock = TRUE;
	uint8_t blockIndx = 0;
	for(;blockIndx < nvmBlockCnt; blockIndx ++)
	{
		if(TRUE == nvmBlockDirty[blockIndx])
		{
			noDirtyBlock = FALSE;
			break;
		}
	}
	return noDirtyBlock;
}

/*only be efficient for major block*/
bool anv_blockIsDirty(uint16_t blk)
{
	bool isDirty = FALSE;
	if(blk < nvmBlockCnt)
	{
		isDirty = nvmBlockDirty[blk];
	}
	return isDirty;
}

/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/


/* ---- Source switch off ---------------------------------------------------- */
#undef APP_NVM_C_
/*--- End of file ------------------------------------------------------------*/






