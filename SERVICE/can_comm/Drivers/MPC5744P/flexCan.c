
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "MPC5744P.h"
#include "SIUL.h"
#include "defines.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "flexCan.h"
#include "flexCanCfg.h"
#include "INTC.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/
//#define m_can_module	(can_moduleGrp[module])
#define XOC16M  0
#define XOC40M  !XOC16M
/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/


static volatile struct CAN_tag  * const can_moduleGrp[m_canModCnt] =
{
	#if m_canModCnt > 0
	&CAN_0,
	#endif
	#if m_canModCnt > 1
	&CAN_1,
	#endif
	#if m_canModCnt > 2
	&CAN_2,
	#endif
};

/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/

/*Class CAN    -> CAN0 	250KPS  TJA1043*/
/*Reserved CAN ->CAN1	500KPS  TJA1021*/
/*PTCAN        ->CAN2	500KPS  MC33907*/
static const canModCfg_t canModCfgGrp[m_canModCnt] =
{
#if m_canModCnt > 0
	{
		&CAN_0,
		SIUL_PB1,
		SIUL_IN_PB1,
		SIUL_ALT2,
		SIUL_PB0,
		SIUL_ALT1,
	},
#endif
#if m_canModCnt > 1
	{
		&CAN_1,
		SIUL_PA15,
		SIUL_IN_PA15,
		SIUL_ALT1,
		SIUL_PA14,
		SIUL_ALT1,
	},
#endif
#if m_canModCnt > 2
	{
		&CAN_2,
		SIUL_PF15,
		SIUL_IN_PF15,
		SIUL_ALT1,
		SIUL_PF14,
		SIUL_ALT2,
	}
#endif
};

#if XOC16M
static const canBuadCfg_t canBuadCfgGrp[m_can_baudCnt] =
{
	{
		3,	/*prediv = 3, fq = 16M/4 = 4M*/
		7,	/*proPseg*/
		3,	/*PSEG1*/
		2	/*PSEG2, Sampling time = (proPseg + PSEG1 +3)/(proPseg + PSEG1 +PSEG2 +4) = 81.25%*/
			/*Can Buad = 4M/(proPseg + PSEG1 + PSEG2 +4) = 250k*/
	},
	{
		1,	/*prediv = 1, fq = 16M/2 = 8M*/
		7,	/*proPseg*/
		3,	/*PSEG1*/
		2	/*PSEG2, Sampling time = (proPseg + PSEG1 +3)/(proPseg + PSEG1 +PSEG2 +4) = 81.25%*/
			/*Can Buad = 8M/(proPseg + PSEG1 + PSEG2 +4) = 500k*/
	},
	{
		0,	/*prediv = 1, fq = 16M/1= 16M*/
		7,	/*proPseg*/
		3,	/*PSEG1*/
		2	/*PSEG2, Sampling time = (proPseg + PSEG1 +3)/(proPseg + PSEG1 +PSEG2 +4) = 81.25%*/
			/*Can Buad = 16M/(proPseg + PSEG1 + PSEG2 +4) = 1M*/
	}
};

#elif XOC40M
static const canBuadCfg_t canBuadCfgGrp[m_can_baudCnt] =
{
	{
		7,	/*prediv = 7, fq = 40M/8 = 5M*/
		7,	/*proPseg*/
		6,	/*PSEG1*/
		3	/*PSEG2, Sampling time = (proPseg + PSEG1 +3)/(proPseg + PSEG1 +PSEG2 +4) = 80%*/
			/*Can Buad = 5M/(proPseg + PSEG1 + PSEG2 +4) = 250k*/
	},
	{
		3,	/*prediv = 3, fq = 40M/4 = 10M*/
		7,	/*proPseg*/
		6,	/*PSEG1*/
		3	/*PSEG2, Sampling time = (proPseg + PSEG1 +3)/(proPseg + PSEG1 +PSEG2 +4) = 80%*/
			/*Can Buad = 10M/(proPseg + PSEG1 + PSEG2 +4) = 500k*/
	},
	{
		1,	/*prediv = 1, fq = 40M/2= 20M*/
		7,	/*proPseg*/
		6,	/*PSEG1*/
		3	/*PSEG2, Sampling time = (proPseg + PSEG1 +3)/(proPseg + PSEG1 +PSEG2 +4) = 80%*/
			/*Can Buad = 20M/(proPseg + PSEG1 + PSEG2 +4) = 1M*/
	}
};
#endif


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
//extern void INTC_InstallINTCInterruptHandler(INTCInterruptFn handlerFn, unsigned short vectorNum, unsigned char psrPriority);
/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/

void can_isr0Mb00_03(void);
void can_isr0Mb04_07(void);
void can_isr0Mb08_11(void);
void can_isr0Mb12_15(void);
void can_isr0Mb16_31(void);
void can_isr0Mb32_39(void);
void can_isr0Mb40_47(void);
void can_isr0Mb48_55(void);
void can_isr0Mb56_63(void);
void can_isr0BusOff(void);
void can_isr0BusErr(void);



void can_isr1Mb00_03(void);
void can_isr1Mb04_07(void);
void can_isr1Mb08_11(void);
void can_isr1Mb12_15(void);
void can_isr1Mb16_31(void);
void can_isr1Mb32_39(void);
void can_isr1Mb40_47(void);
void can_isr1Mb48_55(void);
void can_isr1Mb56_63(void);
void can_isr1BusOff(void);
void can_isr1BusErr(void);


void can_isr2Mb00_03(void);
void can_isr2Mb04_07(void);
void can_isr2Mb08_11(void);
void can_isr2Mb12_15(void);
void can_isr2Mb16_31(void);
void can_isr2Mb32_39(void);
void can_isr2Mb40_47(void);
void can_isr2Mb48_55(void);
void can_isr2Mb56_63(void);
void can_isr2BusOff(void);
void can_isr2BusErr(void);


static void can_pinInit(uint8_t module);
static void can_clkSocSel(uint8_t module,uint8_t soc);
static void can_frzCmd(uint8_t module ,functionSt_t st);
static bool can_getRdySt(uint8_t module);
static bool can_getLpmAck(uint8_t module);
static bool can_getFrzAck(uint8_t module);
static void can_setBaudRate(uint8_t module,uint8_t baud);

/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
bool can_init(uint8_t module,uint8_t baud,canMsgBindWay_t msgBindWay)
{

	bool canInitRslt = TRUE;
	uint8_t mboxIndx = 0;
	uint16_t initTimeout = 65000;
	uint32_t RegShadowVal =0;
	volatile struct CAN_tag * canModule;
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_initFunId,m_canModuleErr);
	m_InParaCheck(m_can_isValidBaud(baud),m_plat_canModuleId,m_can_initFunId,m_canBaudRateErr);
	canModule = can_moduleGrp[module];
	can_pinInit(module);
	can_cmd(module,em_funSt_off);
	can_clkSocSel(module,em_canClkSoc_pll);/*200M*/
	can_cmd(module,em_funSt_on);
	can_reset(module);
	while((1 == canModule->MCR.B.SOFTRST) && (initTimeout > 0))
	{
		initTimeout --;
	}
	if(initTimeout == 0)
	{
		canInitRslt = FALSE;
	}
	else
	{
		initTimeout = 65000;
		can_frzCmd(module,em_funSt_on);
		while((FALSE == can_getFrzAck(module)) && (initTimeout >0))
		{
			initTimeout --;
		}
		if(initTimeout <= 0)
		{
			canInitRslt = FALSE;
		}
		else
		{
			while((0 == can_moduleGrp[module]->MCR.B.FRZ ) && (initTimeout >0))
			{
				initTimeout --;
			}
			if(initTimeout <= 0)
			{
				canInitRslt = FALSE;
			}

//			while(initTimeout >0)
//			{
//				initTimeout --;
//			}
			else
			{
				initTimeout = 65000;
				RegShadowVal = 0;
				can_setBaudRate(module,baud);
				can_cmd(module,em_funSt_off);
				can_clkSocSel(module,em_canClkSoc_pll);/*200M*/
				can_cmd(module,em_funSt_on);
				canModule->CTRL1.B.SMP = 0;	/* 0: one sample to determine the bit value, 1: three sample */
				canModule->CTRL1.B.LBUF = 1;/* lowest buffer transmit first */
				canModule->CTRL1.B.BOFFREC = 0;/*Automatic recovering from Bus Off state enabled, according to CAN Spec 2.0 part B*/
				/* Disable MBox interrupt */
				canModule->IMASK1.R = 0;
				canModule->IMASK2.R = 0;/*during initialization,disable all the mbox interrupt*/
				canModule->IFLAG1.R = 0xFFFFFFFFul;
				canModule->IFLAG2.R = 0xFFFFFFFFul;
				canModule->CTRL1.B.RJW = 2;
				//canModule->CTRL1.B.BOFFMSK = 0;
				//canModule->CTRL1.B.ERRMSK = 0;
				canModule->CTRL1.B.BOFFMSK = ((TRUE == m_can_busOffInt)?(1):(0));
				canModule->CTRL1.B.ERRMSK = ((TRUE == m_can_busErrInt)?(1):(0));
				canModule->CTRL2.B.EACEN = 1;/*Enables the comparison of both Rx Mailbox filter's IDE and RTR bit with their corresponding bits within the incoming frame. Mask bits do apply.*/
				canModule->MCR.B.RFEN = 0;            /* RX FIFO not needed*/
				canModule->RXMGMASK.R = (em_canMsgBind_Fix == msgBindWay)?(0xFFFFFFFF):(0x00000000);
				(TRUE == m_can_enable)?(RegShadowVal |= 0):(RegShadowVal |= m_can_mcrMdisMask);
				(TRUE == m_can_selfRec)?(RegShadowVal |= 0):(RegShadowVal |= m_can_mcrSelfRecMask);
				(FALSE == m_can_IRMQ)?(RegShadowVal |= 0):(RegShadowVal |= m_can_mcrIrmqMask);
				(FALSE == m_can_txAbortEna)?(RegShadowVal |= 0):(RegShadowVal |= m_can_mcrTxAbortMask);
				RegShadowVal |= m_can_mbNum -1;
				RegShadowVal |= m_can_mcrHaltMask;
				canModule->MCR.R = RegShadowVal;
				canModule->IFLAG1.R = 0xFFFFFFFFul;
				canModule->IFLAG2.R = 0xFFFFFFFFul;
				for(mboxIndx = 0;mboxIndx < m_can_mboxCnt;mboxIndx ++)
				{
					canModule->RXIMR[mboxIndx].R =((em_canMsgBind_Fix == msgBindWay)? (0xFFFFFFFF):(0x00000000)); /* set mask registers - all ID bits must match */
					canModule->MB[mboxIndx].CS.B.CODE = m_can_txInactive;
				}
				can_frzCmd(module,em_funSt_off);
				while(((TRUE == can_getFrzAck(module)) || (FALSE == can_getRdySt(module))) && (initTimeout > 0))
				{
					initTimeout -- ;
					if(3000 == initTimeout)
					{
						can_frzCmd(module,em_funSt_off);
					}
				}
				if(initTimeout >0)
				{
					canInitRslt = TRUE;
				}
			}

		}
	}
	switch(module)
	{
		case 0:
			INTC_InstallINTCInterruptHandler(&can_isr0Mb00_03,522,9);
			INTC_InstallINTCInterruptHandler(&can_isr0Mb04_07,523,8);
			INTC_InstallINTCInterruptHandler(&can_isr0Mb08_11,524,7);
			INTC_InstallINTCInterruptHandler(&can_isr0Mb12_15,525,6);
			INTC_InstallINTCInterruptHandler(&can_isr0Mb16_31,526,5);
			INTC_InstallINTCInterruptHandler(&can_isr0Mb32_39,527,4);
			INTC_InstallINTCInterruptHandler(&can_isr0Mb40_47,528,3);
			INTC_InstallINTCInterruptHandler(&can_isr0Mb48_55,529,2);
			INTC_InstallINTCInterruptHandler(&can_isr0Mb56_63,530,1);
			INTC_InstallINTCInterruptHandler(&can_isr0BusOff, 521,10);
			INTC_InstallINTCInterruptHandler(&can_isr0BusErr, 520,11);
			
			break;
		case 1:
			INTC_InstallINTCInterruptHandler(&can_isr1Mb00_03,535,9);
			INTC_InstallINTCInterruptHandler(&can_isr1Mb04_07,536,8);
			INTC_InstallINTCInterruptHandler(&can_isr1Mb08_11,537,7);
			INTC_InstallINTCInterruptHandler(&can_isr1Mb12_15,538,6);
			INTC_InstallINTCInterruptHandler(&can_isr1Mb16_31,539,5);
			INTC_InstallINTCInterruptHandler(&can_isr1Mb32_39,540,4);
			INTC_InstallINTCInterruptHandler(&can_isr1Mb40_47,541,3);
			INTC_InstallINTCInterruptHandler(&can_isr1Mb48_55,542,2);
			INTC_InstallINTCInterruptHandler(&can_isr1Mb56_63,543,1);
			INTC_InstallINTCInterruptHandler(&can_isr1BusOff, 534,10);
			INTC_InstallINTCInterruptHandler(&can_isr1BusErr, 533,11);
			break;
		case 2:
			INTC_InstallINTCInterruptHandler(&can_isr2Mb00_03,548,9);
			INTC_InstallINTCInterruptHandler(&can_isr2Mb04_07,549,8);
			INTC_InstallINTCInterruptHandler(&can_isr2Mb08_11,550,7);
			INTC_InstallINTCInterruptHandler(&can_isr2Mb12_15,551,6);
			INTC_InstallINTCInterruptHandler(&can_isr2Mb16_31,552,5);
			INTC_InstallINTCInterruptHandler(&can_isr2Mb32_39,553,4);
			INTC_InstallINTCInterruptHandler(&can_isr2Mb40_47,554,3);
			INTC_InstallINTCInterruptHandler(&can_isr2Mb48_55,555,2);
			INTC_InstallINTCInterruptHandler(&can_isr2Mb56_63,556,1);
			INTC_InstallINTCInterruptHandler(&can_isr2BusOff, 547,10);
			INTC_InstallINTCInterruptHandler(&can_isr2BusErr, 546,11);
			break;
		default:
			break;
	}
	return canInitRslt;
}


void can_mbTxCodeSet(uint8_t module,uint8_t mbIndx,uint8_t code)
{
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_mbTxCodeSetFunId,m_canModuleErr);
	m_InParaCheck(m_can_isValidMbox(mbIndx),m_plat_canModuleId,m_can_mbTxCodeSetFunId,m_canMbErr);
	m_InParaCheck(m_can_isValidTxCode(code),m_plat_canModuleId,m_can_mbTxCodeSetFunId,m_canTxCodeErr);
	can_moduleGrp[module]->MB[mbIndx].CS.B.CODE = code;
}


void can_mbRxCodeSet(uint8_t module,uint8_t mbIndx,uint8_t code)
{
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_mbRxCodeSetFunId,m_canModuleErr);
	m_InParaCheck(m_can_isValidMbox(mbIndx),m_plat_canModuleId,m_can_mbRxCodeSetFunId,m_canMbErr);
	m_InParaCheck(m_can_isValidRxCode(code),m_plat_canModuleId,m_can_mbRxCodeSetFunId,m_canRxCodeErr);
	can_moduleGrp[module]->MB[mbIndx].CS.B.CODE = code;
}

uint8_t can_getCsCode(uint8_t module,uint8_t mbIndx)
{
	uint8_t code = 0;
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_getCsCodeFunId,m_canModuleErr);
	m_InParaCheck(m_can_isValidMbox(mbIndx),m_plat_canModuleId,m_can_getCsCodeFunId,m_canMbErr);
	code  = can_moduleGrp[module]->MB[mbIndx].CS.B.CODE;
	return code;
}

void can_mbIntCmd(uint8_t module,uint8_t mbIndx,functionSt_t st)
{
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_mbIntCmdFunId,m_canModuleErr);
	m_InParaCheck(m_can_isValidMbox(mbIndx),m_plat_canModuleId,m_can_mbIntCmdFunId,m_canMbErr);
	m_InParaCheck(m_plat_isFunctionState(st),m_plat_canModuleId,m_can_mbIntCmdFunId,m_plat_funStErr);
	can_clearIFlag(module,mbIndx);
	if(em_funSt_on == st)
	{
		if(mbIndx < 32)
		{
			can_moduleGrp[module]->IMASK1.R |= (0x00000001ul << mbIndx);
		}
		else
		{
			can_moduleGrp[module]->IMASK2.R |= (0x00000001ul << (mbIndx - 32));
		}

	}
	else
	{
		if(mbIndx < 32)
		{
			can_moduleGrp[module]->IMASK1.R &= (~(0x00000001ul << mbIndx));
		}
		else
		{
			can_moduleGrp[module]->IMASK2.R &= (~(0x00000001ul << (mbIndx - 32)));
		}
	}
}

void can_clearIFlag(uint8_t module, uint8_t mbIndx)
{
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_clearIFlag,m_canModuleErr);
	m_InParaCheck(m_can_isValidMbox(mbIndx),m_plat_canModuleId,m_can_clearIFlag,m_canMbErr);
	if(mbIndx < 32) 
	{
		if(0 < (can_moduleGrp[module]->IFLAG1.R & (0x00000001ul << mbIndx)))
		{
			can_moduleGrp[module]->IFLAG1.R = (0x00000001ul << mbIndx);	
		}
	}
	else
	{
		if(0 < (can_moduleGrp[module]->IFLAG2.R & (0x00000001ul << (mbIndx - 32))))
		{
			can_moduleGrp[module]->IFLAG2.R = (0x00000001ul << (mbIndx - 32));
		}
	}
}

uint32_t can_getIFlag1(uint8_t module)
{
	uint32_t IFlag = 0;
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_getIFlag1,m_canModuleErr);
	IFlag = can_moduleGrp[module]->IFLAG1.R;
	return IFlag;
}


uint32_t can_getIFlag2(uint8_t module)
{
	uint32_t IFlag = 0;
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_getIFlag2,m_canModuleErr);
	IFlag = can_moduleGrp[module]->IFLAG2.R;
	return IFlag;
}


void can_mbAllIntCmd(uint8_t module,functionSt_t st)
{
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_mbAllIntCmdFunId,m_canModuleErr);
	m_InParaCheck(m_plat_isFunctionState(st),m_plat_canModuleId,m_can_mbAllIntCmdFunId,m_plat_funStErr);
	if(em_funSt_on == st)
	{
		can_moduleGrp[module]->IMASK1.R = 0xFFFFFFFFul;
		can_moduleGrp[module]->IMASK2.R = 0xFFFFFFFFul;
	}
	else
	{
		can_moduleGrp[module]->IMASK1.R = 0;
		can_moduleGrp[module]->IMASK2.R = 0;
	}
}



void can_txAllowAbort(uint8_t module,functionSt_t st)
{
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_txAllowAbortFunId,m_canModuleErr);
	m_InParaCheck(m_plat_isFunctionState(st),m_plat_canModuleId,m_can_txAllowAbortFunId,m_plat_funStErr);
	if(em_funSt_on == st)
	{
		can_moduleGrp[module]->MCR.B.AEN = 1;
	}
	else
	{
		can_moduleGrp[module]->MCR.B.AEN = 0;
	}
}



void can_rxMbSet(uint8_t module,uint8_t mbIndx,uint32_t id,uint8_t dlc,uint8_t rxCode)
{
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_rxMbSetFunId,m_canModuleErr);
	m_InParaCheck(m_can_isValidMbox(mbIndx),m_plat_canModuleId,m_can_rxMbSetFunId,m_canMbErr);
	can_mbRxCodeSet(module,mbIndx,rxCode);
	if(0 != (id & m_can_exFrameSymMask))
	{
		/*extend frame*/
		can_moduleGrp[module]->MB[mbIndx].CS.B.SRR = 1; /* SRR Flag */
		can_moduleGrp[module]->MB[mbIndx].CS.B.IDE = 1;
		can_moduleGrp[module]->MB[mbIndx].ID.R = id & m_can_exIdMask;
	}
	else
	{
		/*standard frame*/
		can_moduleGrp[module]->MB[mbIndx].CS.B.SRR = 0; /* SRR Flag */
		can_moduleGrp[module]->MB[mbIndx].CS.B.IDE = 0;
		can_moduleGrp[module]->MB[mbIndx].ID.B.ID_STD = id & m_can_stdIdMask;
	}
}



void can_txMbSet(uint8_t module,uint8_t mbIndx,uint32_t id,uint8_t dlc,uint8_t txCode)
{
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_txMbSetFunId,m_canModuleErr);
	m_InParaCheck(m_can_isValidMbox(mbIndx),m_plat_canModuleId,m_can_txMbSetFunId,m_canMbErr);
	can_mbTxCodeSet(module,mbIndx,txCode);
	can_moduleGrp[module]->MB[mbIndx].CS.B.DLC = (dlc & 0x0F);
	if(0 != (id & m_can_exFrameSymMask))
	{
		/*extend frame*/
		can_moduleGrp[module]->MB[mbIndx].CS.B.SRR = 1;
		can_moduleGrp[module]->MB[mbIndx].CS.B.IDE = 1;
		can_moduleGrp[module]->MB[mbIndx].ID.R = id & m_can_exIdMask;
	}
	else
	{
		/*standard frame*/
		can_moduleGrp[module]->MB[mbIndx].CS.B.SRR = 0; /* SRR Flag */
		can_moduleGrp[module]->MB[mbIndx].CS.B.IDE = 0;
		can_moduleGrp[module]->MB[mbIndx].ID.B.ID_STD = id & m_can_stdIdMask;
	}
	can_mbTxCodeSet(module,mbIndx,txCode);	
}


/*cyclic query*/
void can_rxMsg(uint8_t module,uint8_t mbIndx)
{
	uint8_t rxCode =0;
	uint32_t id = 0;
	uint8_t dlc=0;
	uint8_t rxBuf[m_can_stdFrameLen];
	uint8_t byteIndx = 0;
	volatile uint8_t * mbBuf = NULL;
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_rxMsgFunId,m_canModuleErr);
	m_InParaCheck(m_can_isValidMbox(mbIndx),m_plat_canModuleId,m_can_rxMsgFunId,m_canMbErr);
	rxCode = can_moduleGrp[module]->MB[mbIndx].CS.B.CODE;
	if(m_can_isRecDtCode(rxCode))
	{
		if(1 == can_moduleGrp[module]->MB[mbIndx].CS.B.IDE)
		{
			id = can_moduleGrp[module]->MB[mbIndx].ID.R & m_can_exIdMask;
			id |= m_can_exFrameSymMask;
		}
		else
		{
			id = can_moduleGrp[module]->MB[mbIndx].ID.B.ID_STD & m_can_stdIdMask;
		}
		if(1 == can_moduleGrp[module]->MB[mbIndx].CS.B.RTR)
		{
			m_canRecRemoteFrameCb(module,mbIndx,id);
		}
		else
		{
			dlc = can_moduleGrp[module]->MB[mbIndx].CS.B.DLC;
			if(m_can_isStdFrameLen(dlc))
			{
				mbBuf = can_moduleGrp[module]->MB[mbIndx].DATA.B;
				for(;byteIndx < dlc;byteIndx ++)
				{
					rxBuf[byteIndx] = mbBuf[byteIndx];
				}
				m_canScanRecMsgCb(module,mbIndx,id,rxBuf,dlc);
			}
		}
		/*Clear rx flags*/
		can_mbRxCodeSet(module,mbIndx,m_can_rxEmpty);
		if(mbIndx < 32)
		{
			can_moduleGrp[module]->IFLAG1.R = (0x00000001ul << mbIndx);
		}
		else
		{
			can_moduleGrp[module]->IFLAG2.R = (0x00000001ul << (mbIndx - 32));
		}


	}
	else
	{
		
	}
}

bool can_txMsgFix(uint8_t module,uint8_t mbIndx,uint32_t id, uint8_t * txBuf, uint8_t dlc)
{
	bool txOk = FALSE;
	uint8_t byteIndx = 0;
	volatile uint8_t * mbBuf = NULL;
	uint16_t waitAbortFinsihTick = 6000;
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_txMsgFixFunId,m_canModuleErr);
	m_InParaCheck(m_can_isValidMbox(mbIndx),m_plat_canModuleId,m_can_txMsgFixFunId,m_canMbErr);
	if(FALSE == m_can_istxEmptyCode(can_moduleGrp[module]->MB[mbIndx].CS.B.CODE))
	{
		can_mbTxCodeSet(module,mbIndx,m_can_txAbort);
	}
	if(mbIndx < 32)
	{
		if(0 != (can_moduleGrp[module]->IFLAG1.R & m_util_bitMask32(mbIndx)))
		{
			can_moduleGrp[module]->IFLAG1.R = m_util_bitMask32(mbIndx);
		}
	}
	else
	{
		if(0 != (can_moduleGrp[module]->IFLAG2.R & m_util_bitMask32(mbIndx-32)))
		{
			can_moduleGrp[module]->IFLAG2.R = m_util_bitMask32(mbIndx-32);
		}
	}
	while((FALSE == m_can_istxEmptyCode(can_moduleGrp[module]->MB[mbIndx].CS.B.CODE)) &&
		(waitAbortFinsihTick > 0))
	{
		waitAbortFinsihTick --;
	}

	if(0 == (id & m_can_exFrameSymMask))
	{
		/*standard frame*/
		can_moduleGrp[module]->MB[mbIndx].CS.B.IDE = 0;
		can_moduleGrp[module]->MB[mbIndx].CS.B.SRR = 0;
		can_moduleGrp[module]->MB[mbIndx].ID.B.ID_STD = (id & m_can_stdIdMask);
	}
	else
	{
		/*extended frame*/
		can_moduleGrp[module]->MB[mbIndx].CS.B.IDE = 1;
		can_moduleGrp[module]->MB[mbIndx].CS.B.SRR = 1;
		can_moduleGrp[module]->MB[mbIndx].ID.R = (id & m_can_exIdMask);
	}
	if(m_can_isStdFrameLen(dlc))
	{
		can_moduleGrp[module]->MB[mbIndx].CS.B.DLC = (dlc & 0x0F);
		mbBuf = can_moduleGrp[module]->MB[mbIndx].DATA.B;
		for(;byteIndx < dlc;byteIndx ++)
		{
			mbBuf[byteIndx] = txBuf[byteIndx];
		}
		can_mbTxCodeSet(module,mbIndx,m_can_txDt);
		
		txOk = TRUE;
	}
	return txOk;
}
bool can_txMsgDyn(uint8_t module,uint32_t id, uint8_t * txBuf, uint8_t dlc)
{
	bool txOk = FALSE;
	uint8_t inacTxMbIndx = 0;
	uint8_t byteIndx = 0;
	volatile uint8_t * mbBuf = NULL;
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_txMsgDynFunId,m_canModuleErr);
	for(;inacTxMbIndx < m_can_mboxCnt;inacTxMbIndx ++)
	{
		if(m_can_istxEmptyCode(can_moduleGrp[module]->MB[inacTxMbIndx].CS.B.CODE))
		{
			break;
		}
	}
	if(inacTxMbIndx < m_can_mboxCnt)
	{
		if(0 == (id & m_can_exFrameSymMask))
		{
			/*standard frame*/
			can_moduleGrp[module]->MB[inacTxMbIndx].CS.B.IDE = 0;
			can_moduleGrp[module]->MB[inacTxMbIndx].CS.B.SRR = 0;
			can_moduleGrp[module]->MB[inacTxMbIndx].ID.B.ID_STD = (id & m_can_stdIdMask);
		}
		else
		{
			/*extended frame*/
			can_moduleGrp[module]->MB[inacTxMbIndx].CS.B.IDE = 1;
			can_moduleGrp[module]->MB[inacTxMbIndx].CS.B.SRR = 1;
			can_moduleGrp[module]->MB[inacTxMbIndx].ID.R = (id & m_can_exIdMask);
		}
		if(m_can_isStdFrameLen(dlc))
		{
			can_moduleGrp[module]->MB[inacTxMbIndx].CS.B.DLC = (dlc & 0x0F);
			mbBuf = can_moduleGrp[module]->MB[inacTxMbIndx].DATA.B;
			for(;byteIndx < dlc;byteIndx ++)
			{
				mbBuf[byteIndx] = txBuf[byteIndx];
			}
			can_mbTxCodeSet(module,inacTxMbIndx,m_can_txDt);
			txOk = TRUE;
		}
	}
	return txOk;
}


void can_isrRxTxMsgHandle(uint8_t module,uint16_t mboxFrom,uint16_t mboxTo)
{
	uint32_t id = 0;
	uint8_t dlc =0;
	bool remoteFrame = FALSE;
	uint8_t rxBuf[m_can_stdFrameLen]={0,0,0,0,0,0,0,0};
	uint8_t mbCode = m_can_rxEmpty;
	uint8_t mbIndx = mboxFrom;
	uint8_t byteIndx = 0;
	volatile uint8_t * mbBuf = NULL;
	bool recMbFind = FALSE;
	for(;mbIndx <= mboxTo;mbIndx ++)
	{
		recMbFind = FALSE;
		if(mbIndx <32)
		{
			if((0 != (can_moduleGrp[module]->IFLAG1.R & m_util_bitMask32(mbIndx))) &&
		  	  (0 != (can_moduleGrp[module]->IMASK1.R & m_util_bitMask32(mbIndx))))
			{
				recMbFind = TRUE;
			}
		}
		else
		{
			if((0 != (can_moduleGrp[module]->IFLAG2.R & m_util_bitMask32(mbIndx - 32))) &&
		  	  (0 != (can_moduleGrp[module]->IMASK2.R & m_util_bitMask32(mbIndx -32))))
			{
				recMbFind = TRUE;
			}
		}
		if(TRUE == recMbFind)
		{
			dlc = can_moduleGrp[module]->MB[mbIndx].CS.B.DLC;
			mbCode = can_moduleGrp[module]->MB[mbIndx].CS.B.CODE;
			remoteFrame = (can_moduleGrp[module]->MB[mbIndx].CS.B.RTR >0)?(TRUE):(FALSE);
			if(1 == can_moduleGrp[module]->MB[mbIndx].CS.B.IDE)
			{
				id = can_moduleGrp[module]->MB[mbIndx].ID.R & m_can_exIdMask;
				id |= m_can_exFrameSymMask;
			}
			else
			{
				id = can_moduleGrp[module]->MB[mbIndx].ID.B.ID_STD & m_can_stdIdMask;
			}
			if(mbIndx <32)
			{
				can_moduleGrp[module]->IFLAG1.R = m_util_bitMask32(mbIndx);/*write 1 to clear the corresponding interrupt flag*/
			}
			else
			{
				can_moduleGrp[module]->IFLAG2.R = m_util_bitMask32(mbIndx - 32);/*write 1 to clear the corresponding interrupt flag*/
			}
			if(m_can_isRecDtCode(mbCode))
			{
				if(TRUE == remoteFrame)
				{
					m_canIsrRecRemoteFrameCb(module,mbIndx,id);
				}
				else
				{

					if(m_can_isStdFrameLen(dlc))
					{
						(void)can_moduleGrp[module]->MB[mbIndx].CS.B.TIMESTAMP;
						mbBuf = can_moduleGrp[module]->MB[mbIndx].DATA.B;
						for(;byteIndx < dlc;byteIndx ++)
						{
							rxBuf[byteIndx] = mbBuf[byteIndx];
						}
						m_canIsrRecMsgCb(module,mbIndx,id,rxBuf,dlc);
						can_mbRxCodeSet(module,mbIndx,m_can_rxEmpty);
					}
				}
			}
			else if(m_can_istxDtFinshCode(mbCode))
			{
				m_canIsrTxFinshCb(module,mbIndx,id);
			}
			else
			{

			}

			if(mbIndx < 32)
			{
				if(mboxTo < 32)
				{
					if(0 == (can_moduleGrp[module]->IFLAG1.R & m_util_regionMask32(mbIndx,mboxTo)))
					{
						break;
					}
				}
				else
				{
					if((0 == (can_moduleGrp[module]->IFLAG1.R & m_util_regionMask32(mbIndx,31))) &&
					  (0 == (can_moduleGrp[module]->IFLAG2.R & m_util_regionMask32(0,mboxTo - 32))))
					{
						break;
					}	
				}
			}
			else
			{
				if(mboxTo < 32)
				{
					/*mboxTo should never be less than mbIndx, error happened,just Exist*/
					break;
				}
				else
				{
					if(0 == (can_moduleGrp[module]->IFLAG2.R & m_util_regionMask32(mbIndx - 32,mboxTo - 32)))
					{
						break;
					}
				}
			}

		}
	}
}

void can_isr0Mb00_03(void)
{
	#if m_canModCnt >0
	can_isrRxTxMsgHandle((uint8_t)em_canMod_0,0,3);
	#endif
}

void can_isr0Mb04_07(void)
{
	#if m_canModCnt >0
	can_isrRxTxMsgHandle((uint8_t)em_canMod_0,4,7);
	#endif
}

void can_isr0Mb08_11(void)
{
	#if m_canModCnt >0
	can_isrRxTxMsgHandle((uint8_t)em_canMod_0,8,11);
	#endif
}

void can_isr0Mb12_15(void)
{
	#if m_canModCnt >0
	can_isrRxTxMsgHandle((uint8_t)em_canMod_0,12,15);
	#endif
}

void can_isr0Mb16_31(void)
{
	#if m_canModCnt >0
	can_isrRxTxMsgHandle((uint8_t)em_canMod_0,16,31);
	#endif
}

void can_isr0Mb32_39(void)
{
	#if m_canModCnt >0
	can_isrRxTxMsgHandle((uint8_t)em_canMod_0,32,39);
	#endif
}

void can_isr0Mb40_47(void)
{
	#if m_canModCnt >0
	can_isrRxTxMsgHandle((uint8_t)em_canMod_0,40,47);
	#endif
}
void can_isr0Mb48_55(void)
{
	#if m_canModCnt >0
	can_isrRxTxMsgHandle((uint8_t)em_canMod_0,48,55);
	#endif
}

void can_isr0Mb56_63(void)
{
	#if m_canModCnt >0
	can_isrRxTxMsgHandle((uint8_t)em_canMod_0,56,63);
	#endif
}

void can_isr0BusOff(void)
{
	m_canIsrBusOffCb(0);
	can_moduleGrp[0]->ESR1.B.BOFFINT = 1;
}

void can_isr0BusErr(void)
{
	m_canIsrBusErrCb(0);
	can_moduleGrp[0]->ESR1.B.ERRINT = 1;
}


void can_isr1Mb00_03(void)
{
	#if m_canModCnt >1
	can_isrRxTxMsgHandle((uint8_t)em_canMod_1,0,3);
	#endif
}

void can_isr1Mb04_07(void)
{
	#if m_canModCnt >1
	can_isrRxTxMsgHandle((uint8_t)em_canMod_1,4,7);
	#endif
}

void can_isr1Mb08_11(void)
{
	#if m_canModCnt >1
	can_isrRxTxMsgHandle((uint8_t)em_canMod_1,8,11);
	#endif
}

void can_isr1Mb12_15(void)
{
	#if m_canModCnt >1
	can_isrRxTxMsgHandle((uint8_t)em_canMod_1,12,15);
	#endif
}

void can_isr1Mb16_31(void)
{
	#if m_canModCnt >1
	can_isrRxTxMsgHandle((uint8_t)em_canMod_1,16,31);
	#endif
}

void can_isr1Mb32_39(void)
{
	#if m_canModCnt >1
	can_isrRxTxMsgHandle((uint8_t)em_canMod_1,32,39);
	#endif
}

void can_isr1Mb40_47(void)
{
	#if m_canModCnt >1
	can_isrRxTxMsgHandle((uint8_t)em_canMod_1,40,47);
	#endif
}

void can_isr1Mb48_55(void)
{
	#if m_canModCnt >1
	can_isrRxTxMsgHandle((uint8_t)em_canMod_1,48,55);
	#endif
}

void can_isr1Mb56_63(void)
{
	#if m_canModCnt >1
	can_isrRxTxMsgHandle((uint8_t)em_canMod_1,56,63);
	#endif
}

void can_isr1BusOff(void)
{
	m_canIsrBusOffCb(1);
	can_moduleGrp[1]->ESR1.B.BOFFINT = 1;
}

void can_isr1BusErr(void)
{
	m_canIsrBusErrCb(1);
	can_moduleGrp[1]->ESR1.B.ERRINT = 1;
}

bool can_getErrFlag(uint8_t module)
{
	bool errFlag = FALSE;
	if(module < m_canModCnt)
	{
		if(can_moduleGrp[module]->ESR1.B.ERRINT >0)
		{
			//can_moduleGrp[module]->ESR1.B.ERRINT = 1;
			errFlag = TRUE;
		}
	}
	return errFlag;
}


void can_clrErrFlag(uint8_t module)
{
	if(module < m_canModCnt)
	{
		can_moduleGrp[module]->ESR1.B.ERRINT = 1;
	}
}

bool can_getBusOffFlag(uint8_t module)
{
	bool busOffFlag = FALSE;
	if(module < m_canModCnt)
	{
		if(can_moduleGrp[module]->ESR1.B.BOFFINT >0)
		{
			//can_moduleGrp[module]->ESR1.B.BOFFINT = 1;
			busOffFlag = TRUE;
		}
	}
	return busOffFlag;
	
}

void can_clrBusOffFlag(uint8_t module)
{
	if(module < m_canModCnt)
	{
		can_moduleGrp[module]->ESR1.B.BOFFINT = 1;
	}
}




void can_isr2Mb00_03(void)
{
	#if m_canModCnt >2
	can_isrRxTxMsgHandle((uint8_t)em_canMod_2,0,3);
	#endif
}

void can_isr2Mb04_07(void)
{
	#if m_canModCnt >2
	can_isrRxTxMsgHandle((uint8_t)em_canMod_2,04,7);
	#endif
}

void can_isr2Mb08_11(void)
{
	#if m_canModCnt >2
	can_isrRxTxMsgHandle((uint8_t)em_canMod_2,8,11);
	#endif
}

void can_isr2Mb12_15(void)
{
	#if m_canModCnt >2
	can_isrRxTxMsgHandle((uint8_t)em_canMod_2,12,15);
	#endif
}

void can_isr2Mb16_31(void)
{
	#if m_canModCnt >2
	can_isrRxTxMsgHandle((uint8_t)em_canMod_2,16,31);
	#endif
}

void can_isr2Mb32_39(void)
{
	#if m_canModCnt >2
	can_isrRxTxMsgHandle((uint8_t)em_canMod_2,32,39);
	#endif
}

void can_isr2Mb40_47(void)
{
	#if m_canModCnt >2
	can_isrRxTxMsgHandle((uint8_t)em_canMod_2,40,47);
	#endif
}

void can_isr2Mb48_55(void)
{
	#if m_canModCnt >2
	can_isrRxTxMsgHandle((uint8_t)em_canMod_2,48,55);
	#endif
}

void can_isr2Mb56_63(void)
{
	#if m_canModCnt >2
	can_isrRxTxMsgHandle((uint8_t)em_canMod_2,56,63);
	#endif
}

void can_isr2BusOff(void)
{
	m_canIsrBusOffCb(2);
	can_moduleGrp[2]->ESR1.B.BOFFINT = 1;
}

void can_isr2BusErr(void)
{
	m_canIsrBusErrCb(2);
	can_moduleGrp[2]->ESR1.B.ERRINT = 1;
}



void can_cmd(uint8_t module,functionSt_t st)
{
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_cmdFunId,m_canModuleErr);
	m_InParaCheck(m_plat_isFunctionState(st),m_plat_canModuleId,m_can_cmdFunId,m_plat_funStErr);
	
	if(em_funSt_on == st)
	{
		can_moduleGrp[module]->MCR.R &= (~m_can_mcrMdisMask);
	}
	else
	{
		can_moduleGrp[module]->MCR.R |= m_can_mcrMdisMask;
	}
}

void can_reset(uint8_t module)
{
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_resetFunId,m_canModuleErr);
	can_moduleGrp[module]->MCR.R = m_can_mcrResetMask;
}

/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static void can_pinInit(uint8_t module)
{
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_pinInitFunId,m_canModuleErr);
	SIUL_DigitalInput(canModCfgGrp[module].rxMscrPin,canModCfgGrp[module].rxIscrPin, 0, canModCfgGrp[module].rxAfLv);
    SIUL_DigitalOutput(canModCfgGrp[module].txMscrPin,canModCfgGrp[module].txAfLv);
}




static void can_clkSocSel(uint8_t module,uint8_t soc)
{
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_clkSocSelFunId,m_canModuleErr);
	m_InParaCheck(m_can_isValidClkSoc(soc),m_plat_canModuleId,m_can_clkSocSelFunId,m_canClkSocErr);
	/*the can clock source can only be configured in disable mode*/
	if(0 != (can_moduleGrp[module]->MCR.B.MDIS))
	{
		can_moduleGrp[module]->CTRL1.B.CLKSRC = ((0 == soc)?(0):(1));
	}
}

static void can_frzCmd(uint8_t module ,functionSt_t st)
{
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_frzCmdFunId,m_canModuleErr);
	m_InParaCheck(m_plat_isFunctionState(st),m_plat_canModuleId,m_can_frzCmdFunId,m_plat_funStErr);
	if(em_funSt_on == st)
	{
		can_moduleGrp[module]->MCR.B.FRZ = 1;
		can_moduleGrp[module]->MCR.B.HALT = 1;
	}
	else
	{
		can_moduleGrp[module]->MCR.B.FRZ = 0;
		can_moduleGrp[module]->MCR.B.HALT = 0;
	}
}

 static bool can_getRdySt(uint8_t module)
 {
 	bool rdySt = FALSE;
 	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_getRdyStFunId,m_canModuleErr);
	rdySt = ((0 == can_moduleGrp[module]->MCR.B.NOTRDY)?(TRUE):(FALSE));
	return rdySt;
 }

static bool can_getLpmAck(uint8_t module)
{
   bool lpm = FALSE;
   m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_getLpmAckFunId,m_canModuleErr);
   lpm = ((0 == can_moduleGrp[module]->MCR.B.LPMACK)?(FALSE):(TRUE));
   return lpm;
}

static bool can_getFrzAck(uint8_t module)
{
	bool frz = FALSE;
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_getFrzAckFunId,m_canModuleErr);
	frz = ((0 == can_moduleGrp[module]->MCR.B.FRZACK)?(FALSE):(TRUE));
	return frz;
}


/*only can work well when in freeze mode*/
static void can_setBaudRate(uint8_t module,uint8_t baud)
{
	uint32_t ShadowValue = 0;
	m_InParaCheck(m_can_isModule(module),m_plat_canModuleId,m_can_setBaudRateFunId,m_canModuleErr);
	m_InParaCheck(m_can_isValidBaud(baud),m_plat_canModuleId,m_can_setBaudRateFunId,m_canBaudRateErr);
	ShadowValue = can_moduleGrp[module]->CTRL1.R;
	ShadowValue &= 0x00FFFFFF;
	ShadowValue &= 0XFFC0FFFF;
	ShadowValue &= 0xFFFFFFF8;
	ShadowValue |= (canBuadCfgGrp[baud].preDiv << 24);
	ShadowValue |= (canBuadCfgGrp[baud].pseg1 << 19);
	ShadowValue |= (canBuadCfgGrp[baud].pseg2 << 16);
	ShadowValue |= (canBuadCfgGrp[baud].proPseg);
	can_moduleGrp[module]->CTRL1.R = ShadowValue;
}





