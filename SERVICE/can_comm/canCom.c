

 /*------------------------------------------------------------------------------
					 Include files
 ------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "global_var.h"
#include "flexCan.h"
#include "canCfg_export.h"
#include "canCom.h"

 /*-----------------------------------------------------------------------------
						 Module local defines and constants
 ------------------------------------------------------------------------------*/
 #define m_dataLinkMatchNds06   FALSE
#define m_dtLink_dynRxMb				(0)
#define m_dtLink_dynTxMb				(1)

#define m_dtLink_DyntxCycleFifo				(FALSE)

#if m_dtLink_DyntxCycleFifo
static bool dynTxisrEnabled = FALSE;
#endif

 /*------------------------------------------------------------------------------
								  Module local types
 ------------------------------------------------------------------------------*/




 /*------------------------------------------------------------------------------
								 Local data at RAM
 ------------------------------------------------------------------------------*/
static hcanMboxAssign_t hcanMboxAssignGrp[m_canMod_cnt] =
{
	{
		FALSE, m_hcan_mboxFix
	},
	{
		FALSE, m_hcan_mboxFix,
	},
	{
		FALSE, m_hcan_mboxFix
	}
};

static dtLinkMsgTypeCollect_t dtLinkMsgTypeCollectGrp[m_canMod_cnt] = 
{
	{0,0,0,0,0},
	{0,0,0,0,0},
	{0,0,0,0,0}
};

static volatile dtLinkTxDynMsgMagr_t dtLinkTxDynMsgMagrGrp[m_canMod_cnt] =
{
	{0,0},
	{0,0},
	{0,0}
};

static uint8_t CANILModeGrp[m_canMod_cnt] = 
{
	CAN_IL_REC_SEND,CAN_IL_REC_SEND,CAN_IL_REC_SEND
};

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
static void hcan_mboxOrderSort(uint8_t module);
static void hcan_mboxInit(uint8_t module);
static void hcan_getCanMsgNum(uint8_t module,uint16_t * pRxMsgNum,uint16_t * pTxMsgNum);
static void hcan_RxMsgUnpack(uint8_t module);
static void hcan_txPack(uint8_t module);
static void hcan_txScanFinish(uint8_t module);


 /*------------------------------------------------------------------------------
					   Implementation of global functions
 ------------------------------------------------------------------------------*/
void hcan_init(void)
{
	uint8_t bus_i = 0;
	hcan_initModule(DATALINK_CLASS_CAN);
	hcan_initModule(DATALINK_RESERVED_CAN);
	hcan_initModule(DATALINK_PTCAN);

	for(;bus_i < m_canMod_cnt; bus_i ++)
	{
		CANILModeGrp[bus_i] = CAN_IL_REC_SEND;
	}
	/*the other two can buses will be initialized in network manager module*/
}

void hcan_initModule(uint8_t module)
{
	if(module < m_canMod_cnt)
	{
		if(canMsgCfgInfoGrp[module].msgNum > m_can_mboxCnt)
		{
			(void)can_init(module,canBaudCfg[module],em_canMsgBind_Dyn);
		}
		else
		{
			(void)can_init(module,canBaudCfg[module],em_canMsgBind_Fix);
		}			
		hcan_mboxInit(module);
	}
	else
	{
		/*wrong CAN Module*/
	}
}

void hcan_deInitModule(uint8_t module)
{
	if(module < m_canMod_cnt)
	{
		//can_reset(module);
		can_cmd(module,em_funSt_off);
	}
	else
	{
		/*wrong CAN Module*/
	}
}

bool hcan_getBusOffFlag(uint8_t module)
{
	bool  busOffFlag = FALSE;
	if(module < m_canMod_cnt)
	{
		busOffFlag = can_getBusOffFlag(module);
	}
	return busOffFlag;
}

void hcan_clearBusOffFlag(uint8_t module)
{
	can_clrBusOffFlag(module);
}

bool hcan_getBusErrFlag(uint8_t module)
{
	bool  busErrFlag = FALSE;
	if(module < m_canMod_cnt)
	{
		busErrFlag = can_getErrFlag(module);
	}
	return busErrFlag;
}

void hcan_clearBusErrFlag(uint8_t module)
{
	can_clrErrFlag(module);
}



void hcan_setILMode(uint8_t module,uint8_t mode)
{
	if(module < m_canMod_cnt)
	{
		if(CAN_IL_REC_SEND == mode)
		{
			CANILModeGrp[module] = CAN_IL_REC_SEND;
		}
		else
		{
			CANILModeGrp[module] = CAN_IL_REC_ONLY;
		}
	}
}

void hcan_canScanRecMsgDispose(uint8_t module,uint8_t mbIndx,uint32_t id,uint8_t * buf,uint8_t dlc)
{
	/*here at first need to find and determine which  message have been received correctly*/
	canMboxCfg_t   cfg;
	uint32_t extern_id = 0;
	/*at first, verify the validity of the input parameters*/
	if((module < m_canMod_cnt) && (m_hcan_mboxFix == hcanMboxAssignGrp[module].MboxAssignWay) && 
		(TRUE == hcanMboxAssignGrp[module].mboxAssignedOk) && (mbIndx < canMsgCfgInfoGrp[module].msgNum))
	{
		cfg = canMsgCfgInfoGrp[module].pCfg[mbIndx];
		if(TRUE == canMsgCfgInfoGrp[module].pCfg[mbIndx].extFrame)
		{
			extern_id = m_can_exFrameSymMask;
		}
		if((FALSE == cfg.isrEn) && (m_can_dirRx == cfg.dir) && \
			(id == (cfg.id  | extern_id)))
		{
			canMsgScheCfgInfoGrp[module][mbIndx].timeRemain = cfg.period;
			/*modified based on NDS 06 rules, interaction-presentation layer*/
			if((NULL != buf) && (NULL != cfg.buf) && (dlc
			#if m_dataLinkMatchNds06
			>=
			#else
			==
			#endif
			cfg.dlc))
			{
				if(cfg.received != NULL && TRUE ==  *(cfg.received))
				{
					if((NULL != cfg.rxOverlap) && (TRUE == *(cfg.rxOverlap)))
					{
						util_memoryCpy8(buf,cfg.buf,cfg.dlc);
						if(NULL != cfg.pack_unpcak)
						{
							cfg.pack_unpcak();
						}
						if(NULL != cfg.cbHook)
						{
							cfg.cbHook();
						}
						canMsgScheCfgInfoGrp[module][mbIndx].msgRec = TRUE;
					}
				}
				else
				{
					if(cfg.received != NULL)
					{
						*(cfg.received ) = TRUE;
					}
					util_memoryCpy8(buf,cfg.buf,cfg.dlc);
					if(NULL != cfg.pack_unpcak)
					{
						cfg.pack_unpcak();
					}
					if(NULL != cfg.cbHook)
					{
						cfg.cbHook();
					}
					canMsgScheCfgInfoGrp[module][mbIndx].msgRec = TRUE;
				}
			}
		}
	}
}


void hcan_canIsrRecMsgDispose(uint8_t module,uint8_t mbIndx,uint32_t id,uint8_t  * buf,uint8_t dlc)
{
	canMboxCfg_t   cfg;
	uint32_t extern_id  = 0;
	uint16_t dichotomyStart = 0;
	uint16_t searchLen = 0;
	uint16_t search_i = 0;
	bool     find = FALSE;
	if((module < m_canMod_cnt) && (TRUE == hcanMboxAssignGrp[module].mboxAssignedOk))
	{
		if(m_hcan_mboxFix == hcanMboxAssignGrp[module].MboxAssignWay)
		{
			if(mbIndx < canMsgCfgInfoGrp[module].msgNum)
			{
				cfg = canMsgCfgInfoGrp[module].pCfg[mbIndx];
				if(TRUE == canMsgCfgInfoGrp[module].pCfg[mbIndx].extFrame)
				{
					extern_id = m_can_exFrameSymMask;
				}
				if((TRUE == cfg.isrEn) && (m_can_dirRx == cfg.dir) && (id == (cfg.id  | extern_id)))
				{
					if((NULL != buf) && (NULL != cfg.buf) && (dlc
					#if m_dataLinkMatchNds06
					>=
					#else
					==
					#endif
					cfg.dlc))
					{
						if(cfg.received != NULL && TRUE ==  *(cfg.received))
						{
							if((NULL != cfg.rxOverlap) && (TRUE == *(cfg.rxOverlap)))
							{
								util_memoryCpy8(buf,cfg.buf,cfg.dlc);
								if(NULL != cfg.pack_unpcak)
								{
									cfg.pack_unpcak();
								}
								if(NULL != cfg.cbHook)
								{
									cfg.cbHook();
								}
								canMsgScheCfgInfoGrp[module][mbIndx].msgRec = TRUE;
							}
						}
						else
						{
							util_memoryCpy8(buf,cfg.buf,cfg.dlc);
							if(cfg.received != NULL)
							{
								*(cfg.received ) = TRUE;
							}
							if(NULL != cfg.pack_unpcak)
							{
								cfg.pack_unpcak();
							}
							if(NULL != cfg.cbHook)
							{
								cfg.cbHook();
							}
							canMsgScheCfgInfoGrp[module][mbIndx].msgRec = TRUE;
						}
					}
				}
			}
		}
		else if(NULL != canMsgSortInfoGrp[module])
		{
			/*dynamically way*/
			/*try to use Dichotomy query method to find the message index*/
			/*at first, judge the validity of the input precondition parameters*/
			if((mbIndx == m_dtLink_dynRxMb) && (dtLinkMsgTypeCollectGrp[module].recMsgCnt > 0))
			{
				if((id & m_can_exFrameSymMask) > 0)
				{
					id &= m_can_exIdMask;
					if((dtLinkMsgTypeCollectGrp[module].recExtMsgCnt > 0) && 
					   (dtLinkMsgTypeCollectGrp[module].recExtMsgCnt <= dtLinkMsgTypeCollectGrp[module].recMsgCnt))
					{
						dichotomyStart = dtLinkMsgTypeCollectGrp[module].recMsgCnt - dtLinkMsgTypeCollectGrp[module].recExtMsgCnt;
						searchLen = dtLinkMsgTypeCollectGrp[module].recExtMsgCnt;
					}
				}
				else
				{
					id &= m_can_stdIdMask;
					if(dtLinkMsgTypeCollectGrp[module].recMsgCnt > dtLinkMsgTypeCollectGrp[module].recExtMsgCnt)
					{
						dichotomyStart = 0;
						searchLen = dtLinkMsgTypeCollectGrp[module].recMsgCnt - dtLinkMsgTypeCollectGrp[module].recExtMsgCnt;	
					}
				}
				while(searchLen > 0)
				{
					if(searchLen > 4)
					{
						/*Dichotomy way*/
						if(id < canMsgSortInfoGrp[module][dichotomyStart + (searchLen >> 1)].id)
						{
							/*dichotomyStart not change,decrease the searchLen*/
							/*dichotomyStart = dichotomyStart;	*/
							searchLen = (searchLen >> 1);
						}
						else if(id > canMsgSortInfoGrp[module][dichotomyStart + (searchLen >> 1)].id)
						{
							/*move forward the dichotomyStart,decrease the searchLen*/
							dichotomyStart = dichotomyStart + (searchLen >> 1) + 1;
							searchLen =  searchLen - (searchLen >> 1) - 1;
						}
						else
						{
							search_i = dichotomyStart + (searchLen >> 1);
							find = TRUE;
							searchLen = 0;
						}
						
					}
					else
					{
						/*directly way*/
						for(search_i = dichotomyStart; search_i < dichotomyStart + searchLen; search_i ++)
						{
							if(id == canMsgSortInfoGrp[module][search_i].id)
							{
								searchLen = 0;
								find = TRUE;
								break;
							}
							else if(id < canMsgSortInfoGrp[module][search_i].id)
							{
								searchLen = 0;
								search_i = dichotomyStart + searchLen;
								break;
							}
							else
							{
								/*continue searching*/
							}
						}
						searchLen = 0;
					}
				}
				if(TRUE == find)
				{
					/*have found the id*/
					search_i = canMsgSortInfoGrp[module][search_i].orgIndx;
					if(search_i < canMsgCfgInfoGrp[module].msgNum)
					{
						cfg = canMsgCfgInfoGrp[module].pCfg[search_i];
						if((NULL != buf) && (NULL != cfg.buf) && (dlc
						#if m_dataLinkMatchNds06
						>=
						#else
						==
						#endif
						cfg.dlc))
						{
							if((NULL != cfg.received) && (TRUE == *(cfg.received)))
							{
								if((NULL != cfg.rxOverlap) && (TRUE == *(cfg.rxOverlap)))
								{
									util_memoryCpy8(buf,cfg.buf,cfg.dlc);
									if(NULL != cfg.pack_unpcak)
									{
										cfg.pack_unpcak();
									}
									if(NULL != cfg.cbHook)
									{
										cfg.cbHook();
									}
									canMsgScheCfgInfoGrp[module][search_i].msgRec = TRUE;
								}
							}
							else
							{
								util_memoryCpy8(buf,cfg.buf,cfg.dlc);
								if(NULL != cfg.pack_unpcak)
								{
									cfg.pack_unpcak();
								}
								if(NULL != cfg.cbHook)
								{
									cfg.cbHook();
								}
								if(NULL != cfg.received)
								{
									*(cfg.received) = TRUE;
								}
								canMsgScheCfgInfoGrp[module][search_i].msgRec = TRUE;
							}
						}
					}
				}
				else
				{
					/*not found*/
				}
				
			}
			
			
		}
		else
		{
			/*nothing to do*/
		}
	}

}

void hcan_canIsrTxConfirmCb(uint8_t module,uint8_t mbIndx,uint32_t id)
{
	const volatile canMboxCfg_t * pCfg = NULL;
	uint16_t msgIndx = 0;
	bool txFail = FALSE;
	if(module < m_canMod_cnt)
	{
		pCfg = canMsgCfgInfoGrp[module].pCfg;
		if(NULL != pCfg)
		{
			if(m_hcan_mboxFix == hcanMboxAssignGrp[module].MboxAssignWay)
			{
				if(mbIndx < canMsgCfgInfoGrp[module].msgNum)
				{
					#define CFG pCfg[mbIndx]
					canMsgScheCfgInfoGrp[module][msgIndx].txToCnt = 0;
					if((NULL != CFG.cbHook) && (TRUE == CFG.isrEn))
					{
						CFG.cbHook();
					}
					#undef CFG
				}	
			}
			else
			{
				if((m_dtLink_dynTxMb == mbIndx) && (NULL != canTxDynMsgIndxBuf[module]))
				{
#if m_dtLink_DyntxCycleFifo
					msgIndx =canTxDynMsgIndxBuf[module][dtLinkTxDynMsgMagrGrp[module].head];
					canMsgScheCfgInfoGrp[module][msgIndx].txToCnt = 0;
					#define CFG pCfg[msgIndx]
					if((NULL != CFG.cbHook) && (TRUE == CFG.isrEn))
					{
						CFG.cbHook();
					}
					dtLinkTxDynMsgMagrGrp[module].head  += 1;
					if(dtLinkTxDynMsgMagrGrp[module].head  >= dtLinkMsgTypeCollectGrp[module].memCnt)
					{
						dtLinkTxDynMsgMagrGrp[module].head = 0;
					}
					if(dtLinkTxDynMsgMagrGrp[module].head != dtLinkTxDynMsgMagrGrp[module].tail)
					{
						msgIndx = canTxDynMsgIndxBuf[module][dtLinkTxDynMsgMagrGrp[module].head];
						txFail = !can_txMsgFix(module,m_dtLink_dynTxMb,CFG.id, CFG.buf, CFG.dlc);
						if(NULL != CFG.txFailed)
						{
							*(CFG.txFailed) = txFail;
						}

					}
					else
					{
						can_mbIntCmd(module,m_dtLink_dynTxMb,em_funSt_off);
						dynTxisrEnabled = FALSE;
					}
					#undef CFG

#else
					msgIndx =canTxDynMsgIndxBuf[module][dtLinkTxDynMsgMagrGrp[module].head];
					canMsgScheCfgInfoGrp[module][msgIndx].txToCnt = 0;
					#define CFG pCfg[msgIndx]
					if((NULL != CFG.cbHook) && (TRUE == CFG.isrEn))
					{
						CFG.cbHook();
					}
					

					dtLinkTxDynMsgMagrGrp[module].head  += 1;
					can_mbIntCmd(module,m_dtLink_dynTxMb,em_funSt_off);
					if(dtLinkTxDynMsgMagrGrp[module].head < dtLinkTxDynMsgMagrGrp[module].tail)
					{
						msgIndx = canTxDynMsgIndxBuf[module][dtLinkTxDynMsgMagrGrp[module].head];
						txFail = !can_txMsgFix(module,m_dtLink_dynTxMb,CFG.id, CFG.buf, CFG.dlc);
						can_mbIntCmd(module,m_dtLink_dynTxMb,em_funSt_on);
						if(NULL != CFG.txFailed)
						{
							*(CFG.txFailed) = txFail;
						}
					}
					else
					{
						dtLinkTxDynMsgMagrGrp[module].head = dtLinkTxDynMsgMagrGrp[module].tail;
					}
					#undef CFG
#endif
					
				}	
			}
		}
		
	}

}



void hcan_canRecRemoteFrameDispose(uint8_t module,uint8_t mbIndx,uint32_t id)
{
 	return;
}


void hcan_canIsrRecRemoteFrameDispose(uint8_t module,uint8_t mbIndx,uint32_t id)
{
 	return;
}

void hcan_canBusOffConfirmCb(uint8_t module)
{

}

void hcan_canBusOffRemoveCb(uint8_t module)
{

}




void hcan_canBusErrConfrimCb(uint8_t module)
{
	switch(module)
	{
		case m_can_ClassCan:
			can_init(m_can_ClassCan,canBaudCfg[m_can_ClassCan],(canMsgCfgInfoGrp[m_can_ClassCan].msgNum > m_can_mboxCnt)?(em_canMsgBind_Dyn):(em_canMsgBind_Fix));
			hcan_mboxInit(m_can_ClassCan);
			break;
		case m_can_ReservedCan:
			can_init(m_can_ReservedCan,canBaudCfg[m_can_ReservedCan],(canMsgCfgInfoGrp[m_can_ReservedCan].msgNum > m_can_mboxCnt)?(em_canMsgBind_Dyn):(em_canMsgBind_Fix));
			hcan_mboxInit(m_can_ReservedCan);
			break;
		case m_can_PTCan:

			break;
		default:
			break;
	}
}

void hcan_canBusErrRemoveCb(uint8_t module)
{
	switch(module)
	{
		case m_can_ClassCan:
			
			break;
		case m_can_ReservedCan:
			break;
		case m_can_PTCan:
			break;
		default:
			break;
	}
}

void hcan_canTxToCb(uint8_t module, uint8_t msgIndx)
{
	can_clearIFlag(module,msgIndx);
#if 0
	if((module < m_canMod_cnt) && (msgIndx < canMsgCfgInfoGrp[module].msgNum))
	{
		switch(canMsgCfgInfoGrp[module].pCfg[msgIndx].msgType)
		{
			case m_can_ccpMsg:
				break;
			case m_can_udsMsg:

				break;
			case m_can_norMsg:
				break;
			default:
				break;

		}

	}
#endif

}





void hcan_canRxIndiCb(uint8_t module, uint8_t msgIndx)
{
	if((module < m_canMod_cnt) && (msgIndx < canMsgCfgInfoGrp[module].msgNum))
	{
		canMboxCfg_t cfg = canMsgCfgInfoGrp[module].pCfg[msgIndx];
		if((m_can_udsMsg == cfg.msgType) &&
			   (m_can_dirRx == cfg.dir))
		{
			nt_recIndication(cfg.id,cfg.dlc,cfg.buf);
			if(NULL != cfg.received)
			{
				*(cfg.received) = FALSE;
			}
			

		}
		else if((m_can_ccpMsg == cfg.msgType) &&
				   (m_can_dirRx == cfg.dir))
		{
			
		}
	   else if((m_can_nmMsg == cfg.msgType) &&
	   			(m_can_dirRx == cfg.dir))
		{
			if(8 == cfg.dlc)
			{
				//NM_RxIndication(module, cfg.id, cfg.buf);
			}
			if(NULL != cfg.received)
			{
				*(cfg.received) = FALSE;
			}
		}
		else
		{

		}
	}



}


void hcan_canRxToCb(uint8_t module, uint8_t msgIndx)
{

}



void hcan_canBusInitReq(uint8_t module)
{
	if(module < m_canMod_cnt)
	{
		if(canMsgCfgInfoGrp[module].msgNum > m_can_mboxCnt)
		{
			(void)can_init(module,canBaudCfg[module],em_canMsgBind_Dyn);
		}
		else
		{
			(void)can_init(module,canBaudCfg[module],em_canMsgBind_Fix);
		}
		hcan_mboxInit(module);
	}
}

bool hcan_ccpSendReq(uint8_t * msg)
{
	uint8_t ccpLenInByte = 0;
	const volatile canMboxCfg_t *pCfg = NULL;
	bool txOk = FALSE;
	if((ccpTxCanBus < m_canMod_cnt)  && (CAN_IL_REC_SEND == CANILModeGrp[ccpTxCanBus]) && (ccpTxMsgIndx < canMsgCfgInfoGrp[ccpTxCanBus].msgNum) &&
	  (m_can_dirTx == canMsgCfgInfoGrp[ccpTxCanBus].pCfg[ccpTxMsgIndx].dir) && (m_can_ccpMsg == canMsgCfgInfoGrp[ccpTxCanBus].pCfg[ccpTxMsgIndx].msgType))
	{
		pCfg = canMsgCfgInfoGrp[ccpTxCanBus].pCfg;
		if(NULL != pCfg[ccpTxMsgIndx].txRdy)
		{
			*(pCfg[ccpTxMsgIndx].txRdy) = TRUE;
			for(;ccpLenInByte < 8; ccpLenInByte ++)
			{
				canMsgCfgInfoGrp[ccpTxCanBus].pCfg[ccpTxMsgIndx].buf[ccpLenInByte] = msg[ccpLenInByte];
			}

			txOk = can_txMsgFix(ccpTxCanBus,ccpTxMsgIndx,pCfg[ccpTxMsgIndx].id, pCfg[ccpTxMsgIndx].buf, pCfg[ccpTxMsgIndx].dlc);

			if(0 == canMsgScheCfgInfoGrp[ccpTxCanBus][ccpTxMsgIndx].txToCnt)
			{
				/*the previous message has been transmitted successfully, or timeout event has happened*/
				canMsgScheCfgInfoGrp[ccpTxCanBus][ccpTxMsgIndx].txToCnt	= m_dLink_txTimeOut;
			}
			*(canMsgCfgInfoGrp[ccpTxCanBus].pCfg[ccpTxMsgIndx].txRdy) = FALSE;

		}
	}
	return txOk;
}


bool hcan_ntTransmit(uint8_t * msg)
{
	uint8_t udsLenInByte = 0;
	bool txOk = FALSE;
	const volatile canMboxCfg_t *pCfg = NULL;
	if((udsTxCanBus < m_canMod_cnt) && (CAN_IL_REC_SEND == CANILModeGrp[udsTxCanBus]) && (udsTxMsgIndx < canMsgCfgInfoGrp[udsTxCanBus].msgNum) &&
	  (m_can_dirTx == canMsgCfgInfoGrp[udsTxCanBus].pCfg[udsTxMsgIndx].dir) && (m_can_udsMsg == canMsgCfgInfoGrp[udsTxCanBus].pCfg[udsTxMsgIndx].msgType))
	{
		pCfg = canMsgCfgInfoGrp[udsTxCanBus].pCfg;
		if(NULL != pCfg[udsTxMsgIndx].txRdy)
		{
			*(pCfg[udsTxMsgIndx].txRdy) = TRUE;
			for(;udsLenInByte < 8; udsLenInByte ++)
			{
				canMsgCfgInfoGrp[udsTxCanBus].pCfg[udsTxMsgIndx].buf[udsLenInByte] = msg[udsLenInByte];
			}

			txOk = can_txMsgFix(udsTxCanBus,udsTxMsgIndx,pCfg[udsTxMsgIndx].id, pCfg[udsTxMsgIndx].buf, pCfg[udsTxMsgIndx].dlc);

			if(0 == canMsgScheCfgInfoGrp[udsTxCanBus][udsTxMsgIndx].txToCnt)
			{
				/*the previous message has been transmitted successfully, or timeout event has happened*/
				canMsgScheCfgInfoGrp[udsTxCanBus][udsTxMsgIndx].txToCnt	= m_dLink_txTimeOut;
			}
			*(canMsgCfgInfoGrp[udsTxCanBus].pCfg[udsTxMsgIndx].txRdy) = FALSE;
		}
	}
	return txOk;
}

void hcan_ntTransmitErrInit(void)
{
	hcan_canBusInitReq(udsTxCanBus);
	/*
	if((udsTxCanBus < m_canMod_cnt) && (udsTxMsgIndx < canMsgCfgInfoGrp[udsTxCanBus].msgNum) &&
	  (m_can_dirTx == canMsgCfgInfoGrp[udsTxCanBus].pCfg[udsTxMsgIndx].dir) && (m_can_udsMsg == canMsgCfgInfoGrp[udsTxCanBus].pCfg[udsTxMsgIndx].msgType))
	{
		can_mbTxCodeSet(udsTxCanBus,udsTxMsgIndx,m_can_txAbort);
	}*/
}


void hcan_recUnpackMain(void)
{
	hcan_RxMsgUnpack(m_can_ClassCan);
	hcan_RxMsgUnpack(m_can_ReservedCan);
	hcan_RxMsgUnpack(m_can_PTCan);
	
}

 void hcan_txPackMain(void)
 {
	 hcan_txPack(m_can_ClassCan);
	 hcan_txPack(m_can_ReservedCan);
	 hcan_txPack(m_can_PTCan);
 }

 void hcan_classCan_txPackMain(void)
 {
	 hcan_txPack(m_can_ClassCan);
 }

 void hcan_reservedCan_txPackMain(void)
 {
	 hcan_txPack(m_can_ReservedCan);
 }

 void hcan_ptCan_txPackMain(void)
 {
	 hcan_txPack(m_can_PTCan);
 }

void hcan_txFinishMain(void)
{
	hcan_txScanFinish(m_can_ClassCan);
	hcan_txScanFinish(m_can_ReservedCan);
	hcan_txScanFinish(m_can_PTCan);
}

void hcan_busScanErrHandle(void)
{
	uint8_t canIndx = 0;
	for(;canIndx < m_canMod_cnt;canIndx++)
	{
		if(TRUE == can_getErrFlag(canIndx))
		{
			hcan_canBusErrConfrimCb(canIndx);
		}
		else
		{
			hcan_canBusErrRemoveCb(canIndx);
		}
	}

}


 void hcan_busScanOffHandle(void)
 {
 
 }

 

 /*------------------------------------------------------------------------------
					   Implementation of local functions
 ------------------------------------------------------------------------------*/
 /* make a statistics about the number of the messages
   *sort the message, and notity Down  the id, the message index in the orginal buffer, 
   * this function  is only be used in the can-bus assigned more than 64 message,containing Rx messages and Tx messages
   *
|------------------  |------------------|-------------------|-----------------|
|Rx Standard Messages| Rx Extend Message|Tx Standard message|Tx Extend Message|
|------------------  |------------------|-------------------|-----------------|

If in the same range, for example[Rx Standard Messages], the message with a lower ID will be put fronter 
 *
 */

 static void hcan_mboxOrderSort(uint8_t module)
 {
	canMboxCfg_t cfg;
	uint16_t msg_i = 0;
	uint16_t sort_i = 0;
	uint16_t temp_i = 0;
	/*the message with a bigger prioVal, put it more ahead
	  * same prioVal, the less ID put more ahead	
	*/
	uint32_t prioVal = 0;
	if(module < m_canMod_cnt)
	{
		dtLinkMsgTypeCollectGrp[module].memCnt = canMsgCfgInfoGrp[module].msgNum;
		dtLinkMsgTypeCollectGrp[module].memIndx = 0;
		dtLinkMsgTypeCollectGrp[module].recMsgCnt = 0;
		dtLinkMsgTypeCollectGrp[module].recExtMsgCnt = 0;
		dtLinkMsgTypeCollectGrp[module].txExtMsgCnt = 0;

	
		
		for(;msg_i < dtLinkMsgTypeCollectGrp[module].memCnt; msg_i ++)
		{
			cfg = canMsgCfgInfoGrp[module].pCfg[msg_i];
			if(m_can_dirRx == cfg.dir)
			{
				dtLinkMsgTypeCollectGrp[module].recMsgCnt += 1;
				if(TRUE == cfg.extFrame)
				{
					dtLinkMsgTypeCollectGrp[module].recExtMsgCnt += 1;
				}
			}
			else
			{
				if(TRUE == cfg.extFrame)
				{
					dtLinkMsgTypeCollectGrp[module].txExtMsgCnt += 1;
				}
			}
		}

		if((NULL != canMsgCfgInfoGrp[module].pCfg) && (NULL !=  canMsgSortInfoGrp[module]))
		{
			for(msg_i = 0; msg_i < canMsgCfgInfoGrp[module].msgNum;msg_i ++)
			{
				cfg = canMsgCfgInfoGrp[module].pCfg[msg_i];
				prioVal = 0;
				/*calculate the prioVal */
				if(m_can_dirRx == cfg.dir)
				{
					prioVal += 3;
					if(FALSE == cfg.extFrame)
					{
						prioVal += 2;
					}
				}
				else
				{
					if(FALSE == cfg.extFrame)
					{
						prioVal += 1;
					}
					else
					{
						prioVal += 0;
					}
				}
				for(sort_i = 0;  sort_i < dtLinkMsgTypeCollectGrp[module].memIndx; sort_i ++)
				{
					if((prioVal > canMsgSortInfoGrp[module][sort_i].prioVal) ||
					  ((prioVal ==  canMsgSortInfoGrp[module][sort_i].prioVal) && (cfg.id < canMsgSortInfoGrp[module][sort_i].id)))
					{
						
						break;	
					}
				}
				if(sort_i < dtLinkMsgTypeCollectGrp[module].memIndx)
				{
					for(temp_i = dtLinkMsgTypeCollectGrp[module].memIndx; temp_i > sort_i; temp_i -= 1)
					{
						canMsgSortInfoGrp[module][temp_i].id = canMsgSortInfoGrp[module][temp_i -1].id;
						canMsgSortInfoGrp[module][temp_i].orgIndx = canMsgSortInfoGrp[module][temp_i -1].orgIndx;
						canMsgSortInfoGrp[module][temp_i].prioVal = canMsgSortInfoGrp[module][temp_i -1].prioVal;
					}
					canMsgSortInfoGrp[module][sort_i].id = cfg.id;
					canMsgSortInfoGrp[module][sort_i].orgIndx = msg_i;
					canMsgSortInfoGrp[module][sort_i].prioVal = prioVal;
				}
				else
				{
					canMsgSortInfoGrp[module][dtLinkMsgTypeCollectGrp[module].memIndx].id = cfg.id;
					canMsgSortInfoGrp[module][dtLinkMsgTypeCollectGrp[module].memIndx].orgIndx = msg_i;
					canMsgSortInfoGrp[module][dtLinkMsgTypeCollectGrp[module].memIndx].prioVal = prioVal;	
				}
				dtLinkMsgTypeCollectGrp[module].memIndx += 1;
					
			}
		}
		
		
	}
 }


 static void hcan_mboxInit(uint8_t module)
 {
	 uint16_t rxMsgCnt = 0;
	 uint16_t txMsgCnt = 0;
	 uint16_t msgIndx = 0;
	 uint8_t mbIndx = 0;
	 uint32_t id = 0;
	 uint8_t dlc = 8;
	 hcan_mboxOrderSort(module);
	 if(module < m_canMod_cnt)
	 {			 
		hcan_getCanMsgNum(module,&rxMsgCnt,&txMsgCnt);
		if(canMsgCfgInfoGrp[module].msgNum != (rxMsgCnt + txMsgCnt))
		{
			hcanMboxAssignGrp[module].mboxAssignedOk = FALSE;
		}
		else if(canMsgCfgInfoGrp[module].msgNum <= m_can_mbNum)
		{
			hcanMboxAssignGrp[module].mboxAssignedOk = TRUE;
			hcanMboxAssignGrp[module].MboxAssignWay = m_hcan_mboxFix;	
		}
		else
		{
			hcanMboxAssignGrp[module].mboxAssignedOk = TRUE;
			hcanMboxAssignGrp[module].MboxAssignWay = m_hcan_mboxDyna;	
		}



		if(TRUE == hcanMboxAssignGrp[module].mboxAssignedOk)
		{
			/*enough space for assigning the rx & tx messages*/
			if(m_hcan_mboxFix == hcanMboxAssignGrp[module].MboxAssignWay)
			{
				/*each message occupies one mailbox permanently, and the mailbox number equals to the msgIndx*/
				
				for(msgIndx = 0;msgIndx < canMsgCfgInfoGrp[module].msgNum;msgIndx ++)
				{
					mbIndx = msgIndx;
					id = canMsgCfgInfoGrp[module].pCfg[msgIndx].id;
					if(TRUE == canMsgCfgInfoGrp[module].pCfg[msgIndx].extFrame)
					{
						id = canMsgCfgInfoGrp[module].pCfg[msgIndx].id | m_can_exFrameSymMask;
					}
					dlc = canMsgCfgInfoGrp[module].pCfg[msgIndx].dlc;
					if(m_can_dirRx == canMsgCfgInfoGrp[module].pCfg[msgIndx].dir)
					{
						/*mark the rx Mbox empty*/
						can_rxMbSet(module,mbIndx,id,dlc,m_can_rxEmpty);
						if(TRUE == canMsgCfgInfoGrp[module].pCfg[msgIndx].isrEn)
						{
							can_mbIntCmd(module,mbIndx,em_funSt_on);
						}
						else
						{
							can_mbIntCmd(module,mbIndx,em_funSt_off);
						}
						
						canMsgScheCfgInfoGrp[module][msgIndx].msgRec = FALSE;
						canMsgScheCfgInfoGrp[module][msgIndx].timeRemain = canMsgCfgInfoGrp[module].pCfg[msgIndx].period;
					}
					else
					{
						/*mark the tx Mbox idle*/
						can_txMbSet(module,mbIndx,id,dlc,m_can_txInactive);
						if(TRUE == canMsgCfgInfoGrp[module].pCfg[msgIndx].isrEn)
						{
							can_mbIntCmd(module,mbIndx,em_funSt_on);
						}
						else
						{
							can_mbIntCmd(module,mbIndx,em_funSt_off);
						}
						canMsgScheCfgInfoGrp[module][msgIndx].timeRemain = canMsgCfgInfoGrp[module].pCfg[msgIndx].txFirstDelay;
					}
				}
			}
			else
			{

				for(msgIndx = 0;msgIndx < canMsgCfgInfoGrp[module].msgNum;msgIndx ++)
				{
					if(m_can_dirRx == canMsgCfgInfoGrp[module].pCfg[msgIndx].dir)
					{
						canMsgScheCfgInfoGrp[module][msgIndx].msgRec = FALSE;
						canMsgScheCfgInfoGrp[module][msgIndx].timeRemain = canMsgCfgInfoGrp[module].pCfg[msgIndx].period;
					}
					else
					{
						canMsgScheCfgInfoGrp[module][msgIndx].timeRemain = canMsgCfgInfoGrp[module].pCfg[msgIndx].txFirstDelay;
					}
				}
#if m_dtLink_DyntxCycleFifo
				dynTxisrEnabled = FALSE;
#else
				dtLinkTxDynMsgMagrGrp[module].tail = 0;
				dtLinkTxDynMsgMagrGrp[module].head = 0;
#endif

				 /*use the Mb0 to Rec Message and enable the Rx ISR*/
				mbIndx = m_dtLink_dynRxMb;
				can_rxMbSet(module,mbIndx,id,dlc,m_can_rxEmpty);
				can_mbIntCmd(module,mbIndx,em_funSt_on);

				/*use the Mb1 to Tx all the messages and currently disable the Tx Isr.
				Before the CAN message being transimitted via CAN-BUS, the TX ISR should be enabled*/
				mbIndx = m_dtLink_dynTxMb;
				can_txMbSet(module,mbIndx,id,dlc,m_can_txInactive);
				can_mbIntCmd(module,mbIndx,em_funSt_off);
				
				
			}
		}
	}
}

static void hcan_getCanMsgNum(uint8_t module,uint16_t * pRxMsgNum,uint16_t * pTxMsgNum)
{
	uint16_t rxMsgCnt = 0;
	uint16_t txMsgCnt = 0;
	uint16_t msgIndx = 0;
	uint16_t msgNum =0;
	if((NULL != pRxMsgNum) && (NULL != pTxMsgNum) && (module < canModuleNum))
	{
		msgNum = canMsgCfgInfoGrp[module].msgNum;
		for(;msgIndx < msgNum;msgIndx ++)
		{
			if(m_can_dirTx == canMsgCfgInfoGrp[module].pCfg[msgIndx].dir)
			{
				txMsgCnt ++;
			}
			else
			{
				rxMsgCnt ++;
			}
		}
		*pRxMsgNum = rxMsgCnt;
		*pTxMsgNum = txMsgCnt;
	}
	else
	{
		/*wrong input parameters,just exit*/
	}
}



static void hcan_RxMsgUnpack(uint8_t module)
{
	uint16_t msgIndx = 0;
	uint16_t sort_i = 0;
	if((TRUE == hcanMboxAssignGrp[module].mboxAssignedOk) && (module < m_canMod_cnt) && 
	   (NULL != canMsgSortInfoGrp[module]))
	{
		for(sort_i = 0;sort_i < dtLinkMsgTypeCollectGrp[module].recMsgCnt; sort_i ++)
		{
			msgIndx = canMsgSortInfoGrp[module][sort_i].orgIndx;
			if((TRUE == canMsgCfgInfoGrp[module].pCfg[msgIndx].isrEn) || 
			   (m_hcan_mboxDyna == hcanMboxAssignGrp[module].MboxAssignWay))
			{
				if(TRUE == canMsgScheCfgInfoGrp[module][msgIndx].msgRec)
				{
					canMsgScheCfgInfoGrp[module][msgIndx].msgRec = FALSE;
					if(NULL != canMsgCfgInfoGrp[module].pCfg[msgIndx].rxTimeout)
					{
						*(canMsgCfgInfoGrp[module].pCfg[msgIndx].rxTimeout) = FALSE;
					}
					canMsgScheCfgInfoGrp[module][msgIndx].timeRemain = canMsgCfgInfoGrp[module].pCfg[msgIndx].period;
					hcan_canRxIndiCb(module,msgIndx);
				}
				else
				{
					if(NULL != canMsgCfgInfoGrp[module].pCfg[msgIndx].received)
					{
						(*canMsgCfgInfoGrp[module].pCfg[msgIndx].received) = FALSE;
					}
					if(canMsgCfgInfoGrp[module].pCfg[msgIndx].period > 0)
					{
						if(canMsgScheCfgInfoGrp[module][msgIndx].timeRemain > 0)
						{
							canMsgScheCfgInfoGrp[module][msgIndx].timeRemain --;
							if(0 == canMsgScheCfgInfoGrp[module][msgIndx].timeRemain)
							{
								if(NULL != canMsgCfgInfoGrp[module].pCfg[msgIndx].rxTimeout)
								{
									*(canMsgCfgInfoGrp[module].pCfg[msgIndx].rxTimeout) = TRUE;
								}
								canMsgScheCfgInfoGrp[module][msgIndx].timeRemain = canMsgCfgInfoGrp[module].pCfg[msgIndx].period;
								hcan_canRxToCb(module,msgIndx);
							}
						}
					}
				}
			}
			else
			{
				can_rxMsg(module,msgIndx);
				if(TRUE == canMsgScheCfgInfoGrp[module][msgIndx].msgRec)
				{
					canMsgScheCfgInfoGrp[module][msgIndx].msgRec = FALSE;
					if(NULL != canMsgCfgInfoGrp[module].pCfg[msgIndx].rxTimeout)
					{
						*(canMsgCfgInfoGrp[module].pCfg[msgIndx].rxTimeout) = FALSE;
					}
					canMsgScheCfgInfoGrp[module][msgIndx].timeRemain = canMsgCfgInfoGrp[module].pCfg[msgIndx].period;
					hcan_canRxIndiCb(module,msgIndx);
				}
				else
				{
					if(NULL != canMsgCfgInfoGrp[module].pCfg[msgIndx].received)
					{
						(*canMsgCfgInfoGrp[module].pCfg[msgIndx].received) = FALSE;
					}
					if(canMsgCfgInfoGrp[module].pCfg[msgIndx].period > 0)
					{
						if(canMsgScheCfgInfoGrp[module][msgIndx].timeRemain >0)
						{
							canMsgScheCfgInfoGrp[module][msgIndx].timeRemain --;
							if(0 == canMsgScheCfgInfoGrp[module][msgIndx].timeRemain)
							{
								if(NULL != canMsgCfgInfoGrp[module].pCfg[msgIndx].rxTimeout)
								{
									*(canMsgCfgInfoGrp[module].pCfg[msgIndx].rxTimeout) = TRUE;
								}
								canMsgScheCfgInfoGrp[module][msgIndx].timeRemain = canMsgCfgInfoGrp[module].pCfg[msgIndx].period;
								hcan_canRxToCb(module,msgIndx);
							}
						}
						
					}
				}
			}
			
		}
	}
}


static void hcan_txPack(uint8_t module)
{
	uint16_t msgIndx;
	uint16_t sort_i = 0;
	uint32_t id = 0;
	uint8_t dlc = 0;
	bool txFail = FALSE;
	const volatile canMboxCfg_t *pCfg = NULL;
	
	if((module < m_canMod_cnt) && (NULL != canMsgCfgInfoGrp[module].pCfg))
	{
		
		pCfg = canMsgCfgInfoGrp[module].pCfg;
		if(m_hcan_mboxFix == hcanMboxAssignGrp[module].MboxAssignWay)
		{
			for(sort_i = dtLinkMsgTypeCollectGrp[module].recMsgCnt;sort_i < dtLinkMsgTypeCollectGrp[module].memCnt; sort_i ++)
			{
				msgIndx = canMsgSortInfoGrp[module][sort_i].orgIndx;

				if((m_can_nmMsg == pCfg[msgIndx].msgType) || (CAN_IL_REC_SEND == CANILModeGrp[module]))
				{
					/*at first, check if the period message has been reached its lauching time*/
					if((m_can_periodMsg == pCfg[msgIndx].msgMode) || (m_can_peMsg == pCfg[msgIndx].msgMode))
					{
						if((0 < pCfg[msgIndx].period) && (canMsgScheCfgInfoGrp[module][msgIndx].timeRemain <= 0))
						{
							id = pCfg[msgIndx].id;
							id |= ((TRUE == pCfg[msgIndx].extFrame)?(m_can_exFrameSymMask):(0));
							dlc = pCfg[msgIndx].dlc;
							if(NULL != pCfg[msgIndx].pack_unpcak)
							{
								pCfg[msgIndx].pack_unpcak();
							}
							txFail = !can_txMsgFix(module,msgIndx,id, pCfg[msgIndx].buf, dlc);
							if(m_can_peMsg == pCfg[msgIndx].msgMode)
							{
								if(NULL != pCfg[msgIndx].txRdy)
								{
									/*protect the periodEvent message from being transmitted the second time in Event message handle region*/
									*(pCfg[msgIndx].txRdy) = FALSE;
								}
							}
							if(0 == canMsgScheCfgInfoGrp[module][msgIndx].txToCnt)
							{
								/*the previous message has been transmited successfully, or timeout event has happened*/
								canMsgScheCfgInfoGrp[module][msgIndx].txToCnt	= m_dLink_txTimeOut;
							}
							else
							{
								/*the tx timeout counter is decreasing, just wait here*/
							}

							if(NULL != pCfg[msgIndx].txFailed)
							{
								*(pCfg[msgIndx].txFailed) = txFail;
							}
							canMsgScheCfgInfoGrp[module][msgIndx].timeRemain = pCfg[msgIndx].period;
						}
						if(canMsgScheCfgInfoGrp[module][msgIndx].timeRemain > 0)
						{
							canMsgScheCfgInfoGrp[module][msgIndx].timeRemain --;
						}

					}
					/*Event message handle region*/
					if((m_can_eventMsg == pCfg[msgIndx].msgMode) || (m_can_peMsg == pCfg[msgIndx].msgMode))
					{
						if((NULL != pCfg[msgIndx].txRdy) &&
						(TRUE == *(pCfg[msgIndx].txRdy)))
						{
							*(pCfg[msgIndx].txRdy) = FALSE;
							id = pCfg[msgIndx].id;
							id |= ((TRUE == pCfg[msgIndx].extFrame)?(m_can_exFrameSymMask):(0));
							dlc = pCfg[msgIndx].dlc;
							if(NULL != pCfg[msgIndx].pack_unpcak)
							{
								pCfg[msgIndx].pack_unpcak();
							}
							txFail = !can_txMsgFix(module,msgIndx,id, pCfg[msgIndx].buf, dlc);
							if(0 == canMsgScheCfgInfoGrp[module][msgIndx].txToCnt)
							{
								/*the previous message has been transmitted successfully, or timeout event has happened*/
								canMsgScheCfgInfoGrp[module][msgIndx].txToCnt	= m_dLink_txTimeOut;
							}
							if(NULL != pCfg[msgIndx].txFailed)
							{
								*(pCfg[msgIndx].txFailed) = txFail;
							}
							canMsgScheCfgInfoGrp[module][msgIndx].timeRemain = pCfg[msgIndx].period;
						}
						if(canMsgScheCfgInfoGrp[module][msgIndx].timeRemain > 0)
						{
							canMsgScheCfgInfoGrp[module][msgIndx].timeRemain --;
						}
					}

				}
			}
		}
		else 
		{
#if m_dtLink_DyntxCycleFifo

			for(sort_i = dtLinkMsgTypeCollectGrp[module].recMsgCnt;sort_i < dtLinkMsgTypeCollectGrp[module].memCnt; sort_i ++)
			{

				if((dtLinkTxDynMsgMagrGrp[module].tail + 1) % dtLinkMsgTypeCollectGrp[module].memCnt == dtLinkTxDynMsgMagrGrp[module].head)
				{
					break;
				}
				msgIndx = canMsgSortInfoGrp[module][sort_i].orgIndx;
				/*collect all the messages which need to be sent in this cycle, and store the message index into a buffer*/
				/*at first, check if the period message has been reached its lauching time*/

				if((m_can_periodMsg == pCfg[msgIndx].msgMode) || (m_can_peMsg == pCfg[msgIndx].msgMode))
				{
					if((0 < pCfg[msgIndx].period) && (canMsgScheCfgInfoGrp[module][msgIndx].timeRemain <= 0))
					{
						canTxDynMsgIndxBuf[module][dtLinkTxDynMsgMagrGrp[module].tail] = msgIndx;
						dtLinkTxDynMsgMagrGrp[module].tail += 1;
						if(dtLinkTxDynMsgMagrGrp[module].tail >= dtLinkMsgTypeCollectGrp[module].memCnt)
						{
							dtLinkTxDynMsgMagrGrp[module].tail = 0;

						}
						if(dtLinkTxDynMsgMagrGrp[module].tail == dtLinkTxDynMsgMagrGrp[module].head)
						{
							dtLinkTxDynMsgMagrGrp[module].head += 1;
							if(dtLinkTxDynMsgMagrGrp[module].head >= dtLinkMsgTypeCollectGrp[module].memCnt)
							{
								dtLinkTxDynMsgMagrGrp[module].head = 0;
							}
						}
						if(m_can_peMsg == pCfg[msgIndx].msgMode)
						{
							if(NULL != pCfg[msgIndx].txRdy)
							{
								/*protect the periodEvent message from being transmitted the second time in Event message handle region*/
								*(pCfg[msgIndx].txRdy) = FALSE;
							}
						}
						if(0 == canMsgScheCfgInfoGrp[module][msgIndx].txToCnt)
						{
							/*the previous message has been transmited successfully, or timeout event has happened*/
							canMsgScheCfgInfoGrp[module][msgIndx].txToCnt	= m_dLink_txTimeOut;
						}
						else
						{
							/*the tx timeout counter is decreasing, just wait here*/
						}
						canMsgScheCfgInfoGrp[module][msgIndx].timeRemain = pCfg[msgIndx].period;


					}
					if(canMsgScheCfgInfoGrp[module][msgIndx].timeRemain > 0)
					{
						canMsgScheCfgInfoGrp[module][msgIndx].timeRemain --;
					}

				}
				/*Event message handle region*/
				if((m_can_eventMsg == pCfg[msgIndx].msgMode) || (m_can_peMsg == pCfg[msgIndx].msgMode))
				{
					if((NULL != pCfg[msgIndx].txRdy) &&
					(TRUE == *(pCfg[msgIndx].txRdy)))
					{
						*(pCfg[msgIndx].txRdy) = FALSE;
						canTxDynMsgIndxBuf[module][dtLinkTxDynMsgMagrGrp[module].tail] = msgIndx;
						dtLinkTxDynMsgMagrGrp[module].tail += 1;
						if(dtLinkTxDynMsgMagrGrp[module].tail >= dtLinkMsgTypeCollectGrp[module].memCnt)
						{
							dtLinkTxDynMsgMagrGrp[module].tail = 0;

						}
						if(dtLinkTxDynMsgMagrGrp[module].tail == dtLinkTxDynMsgMagrGrp[module].head)
						{
							dtLinkTxDynMsgMagrGrp[module].head += 1;
							if(dtLinkTxDynMsgMagrGrp[module].head >= dtLinkMsgTypeCollectGrp[module].memCnt)
							{
								dtLinkTxDynMsgMagrGrp[module].head = 0;
							}
						}
						if(0 == canMsgScheCfgInfoGrp[module][msgIndx].txToCnt)
						{
							/*the previous message has been transmitted successfully, or timeout event has happened*/
							canMsgScheCfgInfoGrp[module][msgIndx].txToCnt	= m_dLink_txTimeOut;
						}
						canMsgScheCfgInfoGrp[module][msgIndx].timeRemain = pCfg[msgIndx].period;

					}
					if(canMsgScheCfgInfoGrp[module][msgIndx].timeRemain > 0)
					{
						canMsgScheCfgInfoGrp[module][msgIndx].timeRemain --;
					}
				}
			}
			if(dtLinkTxDynMsgMagrGrp[module].head != dtLinkTxDynMsgMagrGrp[module].tail)
			{
				if(FALSE == dynTxisrEnabled)
				{
					msgIndx = canTxDynMsgIndxBuf[module][dtLinkTxDynMsgMagrGrp[module].head];
					txFail = !can_txMsgFix(module,m_dtLink_dynTxMb,pCfg[msgIndx].id, pCfg[msgIndx].buf, pCfg[msgIndx].dlc);
					dynTxisrEnabled = TRUE;
					can_mbIntCmd(module,m_dtLink_dynTxMb,em_funSt_on);
					if(NULL != pCfg[msgIndx].txFailed)
					{
						*(pCfg[msgIndx].txFailed) = txFail;
					}
				}
			}
#else
			if((dtLinkTxDynMsgMagrGrp[module].tail == dtLinkTxDynMsgMagrGrp[module].head ) && (NULL != canTxDynMsgIndxBuf[module]))
			{
				dtLinkTxDynMsgMagrGrp[module].head = 0;
				dtLinkTxDynMsgMagrGrp[module].tail = 0;
				can_mbIntCmd(module,m_dtLink_dynTxMb,em_funSt_off);
				for(sort_i = dtLinkMsgTypeCollectGrp[module].recMsgCnt;sort_i < dtLinkMsgTypeCollectGrp[module].memCnt; sort_i ++)
				{

					msgIndx = canMsgSortInfoGrp[module][sort_i].orgIndx;
					if((m_can_nmMsg == pCfg[msgIndx].msgType) || (CAN_IL_REC_SEND == CANILModeGrp[module]))
					{
						/*collect all the messages which need to be sent in this cycle, and store the message index into a buffer*/
						/*at first, check if the period message has been reached its lauching time*/
						if((m_can_periodMsg == pCfg[msgIndx].msgMode) || (m_can_peMsg == pCfg[msgIndx].msgMode))
						{
							if((0 < pCfg[msgIndx].period) && (canMsgScheCfgInfoGrp[module][msgIndx].timeRemain <= 0))
							{

								if(dtLinkTxDynMsgMagrGrp[module].tail < dtLinkMsgTypeCollectGrp[module].memCnt - dtLinkMsgTypeCollectGrp[module].recMsgCnt)
								{
									canTxDynMsgIndxBuf[module][dtLinkTxDynMsgMagrGrp[module].tail] = msgIndx;
									dtLinkTxDynMsgMagrGrp[module].tail += 1;
								}
								if(m_can_peMsg == pCfg[msgIndx].msgMode)
								{
									if(NULL != pCfg[msgIndx].txRdy)
									{
										/*protect the periodEvent message from being transmitted the second time in Event message handle region*/
										*(pCfg[msgIndx].txRdy) = FALSE;
									}
								}
								if(0 == canMsgScheCfgInfoGrp[module][msgIndx].txToCnt)
								{
									/*the previous message has been transmited successfully, or timeout event has happened*/
									canMsgScheCfgInfoGrp[module][msgIndx].txToCnt	= m_dLink_txTimeOut;
								}
								else
								{
									/*the tx timeout counter is decreasing, just wait here*/
								}
								canMsgScheCfgInfoGrp[module][msgIndx].timeRemain = pCfg[msgIndx].period;
							}
							if(canMsgScheCfgInfoGrp[module][msgIndx].timeRemain > 0)
							{
								canMsgScheCfgInfoGrp[module][msgIndx].timeRemain --;
							}

						}
						/*Event message handle region*/
						if((m_can_eventMsg == pCfg[msgIndx].msgMode) || (m_can_peMsg == pCfg[msgIndx].msgMode))
						{
							if((NULL != pCfg[msgIndx].txRdy) &&
							(TRUE == *(pCfg[msgIndx].txRdy)))
							{
								*(pCfg[msgIndx].txRdy) = FALSE;
								if(dtLinkTxDynMsgMagrGrp[module].tail < dtLinkMsgTypeCollectGrp[module].memCnt - dtLinkMsgTypeCollectGrp[module].recMsgCnt)
								{
									canTxDynMsgIndxBuf[module][dtLinkTxDynMsgMagrGrp[module].tail] = msgIndx;
									dtLinkTxDynMsgMagrGrp[module].tail += 1;
								}
								if(0 == canMsgScheCfgInfoGrp[module][msgIndx].txToCnt)
								{
									/*the previous message has been transmitted successfully, or timeout event has happened*/
									canMsgScheCfgInfoGrp[module][msgIndx].txToCnt	= m_dLink_txTimeOut;
								}

								canMsgScheCfgInfoGrp[module][msgIndx].timeRemain = pCfg[msgIndx].period;
							}
							if(canMsgScheCfgInfoGrp[module][msgIndx].timeRemain > 0)
							{
								canMsgScheCfgInfoGrp[module][msgIndx].timeRemain --;
							}
						}
					}
				}
				if(dtLinkTxDynMsgMagrGrp[module].tail > 0)
				{
					msgIndx = canTxDynMsgIndxBuf[module][0];
					txFail = !can_txMsgFix(module,m_dtLink_dynTxMb,pCfg[msgIndx].id, pCfg[msgIndx].buf, pCfg[msgIndx].dlc);
					can_mbIntCmd(module,m_dtLink_dynTxMb,em_funSt_on);
					if(NULL != pCfg[msgIndx].txFailed)
					{
						*(pCfg[msgIndx].txFailed) = txFail;
					}
				}
			}
			else
			{
				for(sort_i = dtLinkMsgTypeCollectGrp[module].recMsgCnt;sort_i < dtLinkMsgTypeCollectGrp[module].memCnt; sort_i ++)
				{
					msgIndx = canMsgSortInfoGrp[module][sort_i].orgIndx;
					if(canMsgScheCfgInfoGrp[module][msgIndx].timeRemain > 0)
					{
						canMsgScheCfgInfoGrp[module][msgIndx].timeRemain --;
					}
				}
			}
#endif
			
			
		}
	}
}

static void hcan_txScanFinish(uint8_t module)
{
	uint16_t msgIndx = 0;
	uint16_t sort_i = 0;
	uint8_t mbCode = m_can_txInactive;
	if(module < m_canMod_cnt)
	{
		if(m_hcan_mboxFix == hcanMboxAssignGrp[module].MboxAssignWay)
		{
			for(sort_i = dtLinkMsgTypeCollectGrp[module].recMsgCnt; sort_i < dtLinkMsgTypeCollectGrp[module].memCnt; sort_i ++)
			{
				msgIndx = canMsgSortInfoGrp[module][sort_i].orgIndx;
				if(FALSE == canMsgCfgInfoGrp[module].pCfg[msgIndx].isrEn)
				{
					mbCode = can_getCsCode(module,msgIndx);
					if(canMsgScheCfgInfoGrp[module][msgIndx].txToCnt > 0)
					{
						if(m_can_istxDtFinshCode(mbCode))
						{
							canMsgScheCfgInfoGrp[module][msgIndx].txToCnt = 0;
							if(NULL != canMsgCfgInfoGrp[module].pCfg[msgIndx].cbHook)
							{
								canMsgCfgInfoGrp[module].pCfg[msgIndx].cbHook();
							}
						}
						else
						{
							canMsgScheCfgInfoGrp[module][msgIndx].txToCnt --;
							if(0 == canMsgScheCfgInfoGrp[module][msgIndx].txToCnt)
							{
								hcan_canTxToCb(module,msgIndx);
							}
						}
					}
					else
					{
						/*has been transmited successfully already or wait Tx finish timeout event has been confirmed*/
						if(m_can_istxDtFinshCode(mbCode))
						{
							can_clearIFlag(module,msgIndx);
						}
					}
				}
				else
				{
					/*the call-back actions will be done in Tx isr function*/
					if(canMsgScheCfgInfoGrp[module][msgIndx].txToCnt > 0)
					{
						canMsgScheCfgInfoGrp[module][msgIndx].txToCnt --;
						if(0 == canMsgScheCfgInfoGrp[module][msgIndx].txToCnt)
						{
							hcan_canTxToCb(module,msgIndx);
						}
					}
				}
			}
		}
		else if(NULL != canTxDynMsgIndxBuf[module])
		{
			msgIndx = canTxDynMsgIndxBuf[module][dtLinkTxDynMsgMagrGrp[module].head];
			if(canMsgScheCfgInfoGrp[module][msgIndx].txToCnt > 0)
			{
				canMsgScheCfgInfoGrp[module][msgIndx].txToCnt --;
				if(0 == canMsgScheCfgInfoGrp[module][msgIndx].txToCnt)
				{
					hcan_canTxToCb(module,msgIndx);
				}
			}
		}
		else
		{

		}
	}
}




