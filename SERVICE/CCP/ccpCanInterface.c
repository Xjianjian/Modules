 /*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:ccpCanInterface.c
* Description:
* REQ IDs:
* History:
* 2016-12-29,  Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#define CCP_CAN_INTERFACE_C_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "sysTimer.h"
#include "fee.h"
#include "feeInfo.h"
#include "dataLink.h"
#include "ccpCanInterface.h"
#include "ccp.h"
#include "appNvm.h"

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

bool ccp_storeFlg = FALSE;/*TRUE,for store ccpData; FALSE,for NOT store ccpData*/

/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void ccp_task(void)
{
	char i;
	for(i = 0;i< CCP_MAX_DAQ;i++)
	{
		ccpDaq(i+1);
	}

	ccpBackground();
}


void ccpSend(uint8_t * msg)
{
	hcan_ccpSendReq(msg);
}

uint8_t * ccpGetPointer( uint8_t  addr_ext, uint32_t  addr )        // get Pointer into
{                                                            // normal C
  return (uint8_t *) (addr);
}

void ccpUserBackground( void )
{

}

void ccpRxIndication(uint8_t * msg)
{
	uint8_t ccp_blockNum = 0;
	ccpCommand(msg);
	if(TRUE == ccp_storeFlg)
	{
		ccp_blockNum = nvm_getCCPBlockNum();
		anv_modifyReq(ccp_blockNum);
	}
	else
	{}
}


void ccpTxConfirmCb(void)
{
	(void)ccpSendCallBack();
}

void ccp_tx_timeout_cb(void)
{
	(void)ccpSendCallBack();
}

void ccpDisableInterrupt(void)
{
	 __asm__(" wrteei 0");
}


void ccpEnableInterrupt(void)
{
	 __asm__(" wrteei 1");
}



uint32_t ccpGetSeed(uint8_t seedLv)
{
	uint32_t seed = 0;
	switch(seedLv)
	{
		case m_ccp_PLCAL:
			seed = m_ccp_seedCAL;
			break;
		case m_ccp_PLDAQ:
			seed = m_ccp_seedDaq;
			break;
		case m_ccp_PLPGM:
			seed = m_ccp_seedPgm;
			break;
		default:
			break;
	}
	return seed;
}

uint8_t ccpUnlock(uint8_t * keyBuf)
{
	uint8_t PlLv = 0;
	uint32_t key = *((uint32_t *)keyBuf);
	if((key + m_ccp_seedCAL) == 0xFFFFFFFFul)
	{
		PlLv = m_ccp_PLCAL;
	}
	else if((key + m_ccp_seedDaq) == 0xFFFFFFFFul)
	{
		PlLv = m_ccp_PLDAQ;
	}
	if((key + m_ccp_seedPgm) == 0xFFFFFFFFul)
	{
		PlLv = m_ccp_PLPGM;
	}
	return PlLv;
}





void ccpSetCalPage(uint32_t addr)
{
	uint8_t blkIndx = 0;
	blkIndx = fee_getBlkIndxByAddr(addr);
	if(m_feeInfo_blkNone != blkIndx)
	{
		ccpSetMTAAddr(0,(uint8_t *)(feeBlkInfoGrp[blkIndx].startAddr));
	}
}


uint32_t ccpGetCalPage(void)
{
#if 0
	uint32_t addr = 0x1234;
	uint8_t blkIndx = 0;
	blkIndx = fee_getBlkIndxByAddr(ccp_getMTAAddr(0));
	if(m_feeInfo_blkNone != blkIndx)
	{
		addr = feeBlkInfoGrp[blkIndx].startAddr;
	}
#endif
	return 0x40000000;
}

void ccpFlashClear( uint8_t * addr, uint32_t size)
{
	uint8_t feeBlk = m_feeInfo_blkNone;
	feeBlcokSel_t feeBlcokSel = {0,0,0,0};
	uint32_t hvOp = m_fee_ok;
	uint32_t waitOverTick = 1000;
	feeBlk = fee_getBlkIndxByAddr((uint32_t)addr);
	feeBlcokSel = fee_getLowMidHighSelByBlockIndx(feeBlk);
	if((m_feeInfo_blkNone != feeBlk) && ((0 != feeBlcokSel.low) ||
		(0 != feeBlcokSel.mid) || (0 != feeBlcokSel.high)))
	{
		fee_blockUnLock(feeBlcokSel.low,feeBlcokSel.mid,feeBlcokSel.high);			
		hvOp = fee_EraseStart((uint32_t)addr,feeBlcokSel.low,feeBlcokSel.mid,feeBlcokSel.high);
		if(m_fee_ok == hvOp)
		{
			hvOp = fee_flashCheckSt();
			while((m_fee_hvOpBusy == hvOp) && (waitOverTick > 0))
			{
				hvOp = fee_flashCheckSt();
				waitOverTick --;
			}
		}
		fee_blockLockAll();
	}
	
}


uint8_t ccpFlashProgramm(uint8_t * soc, uint8_t *  dest, uint8_t size)
{
	uint8_t retSt = m_ccp_writeOk;
	uint8_t feeBlk = m_feeInfo_blkNone;
	uint32_t hvOp = m_fee_ok;
	uint32_t waitOverTick = 1000;
	feeBlcokSel_t feeBlcokSel = {0,0,0,0};
	feeBlk = fee_getBlkIndxByAddr((uint32_t)dest);
	feeBlcokSel = fee_getLowMidHighSelByBlockIndx(feeBlk);
	if((m_feeInfo_blkNone != feeBlk) && ((0 != feeBlcokSel.low) ||
		(0 != feeBlcokSel.mid) || (0 != feeBlcokSel.high)))
	{
		fee_blockUnLock(feeBlcokSel.low,feeBlcokSel.mid,feeBlcokSel.high);
		hvOp = fee_programStart((uint32_t)dest,size,(uint16_t)(uint32_t)soc);
		if(m_fee_ok == hvOp)
		{
			hvOp = fee_flashCheckSt();
			while((m_fee_hvOpBusy == hvOp) && (waitOverTick > 0))
			{
				hvOp = fee_flashCheckSt();
				waitOverTick --;
			}
			fee_blockLockAll();
			switch(hvOp)
			{
				case m_fee_ok:
					retSt = m_ccp_writeOk;
					break;
				case m_fee_hvOpBusy:
					retSt = m_ccp_writePending;
					break;
				case m_fee_hvOpFail:
					retSt = m_ccp_writeErr;
					break;
			}
		}
		else
		{
			retSt = m_ccp_writeDenied;
		}
	}
	return retSt;
}

uint8_t ccpCheckWriteAccess( uint8_t * addr, uint8_t size)
{
	uint8_t write_access = m_ccp_writeDenied;
	if(((uint32_t)addr >= 0x40000000) && ((uint32_t)addr + size < 0x40008000))
	{
		write_access = m_ccp_writeOk;
	}
	return write_access;

}

/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/



/* ---- Source switch off ---------------------------------------------------- */
#undef CCP_CAN_INTERFACE_C_
/*--- End of file ------------------------------------------------------------*/






