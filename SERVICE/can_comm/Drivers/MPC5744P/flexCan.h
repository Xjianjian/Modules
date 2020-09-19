
#ifndef FLEX_CAN_H_
#define FLEX_CAN_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_can_initFunId				0
#define m_can_mbTxCodeSetFunId			1
#define m_can_mbRxCodeSetFunId			2
#define m_can_getCsCodeFunId			3
#define m_can_mbIntCmdFunId			4
#define m_can_clearIFlag				5
#define m_can_getIFlag1					6
#define m_can_getIFlag2					7
#define m_can_mbAllIntCmdFunId			8
#define m_can_txAllowAbortFunId		9
#define m_can_rxMbSetFunId				10
#define m_can_txMbSetFunId				11
#define m_can_rxMsgFunId				12
#define m_can_txMsgDynFunId			13
#define m_can_txMsgFixFunId			14
#define m_can_scanTxFinshFunId			15
#define m_can_pinInitFunId				16
#define m_can_cmdFunId					17
#define m_can_clkSocSelFunId			18
#define m_can_frzCmdFunId				19
#define m_can_getRdyStFunId			20
#define m_can_getLpmAckFunId			21
#define m_can_getFrzAckFunId			22
#define m_can_setBaudRateFunId			23
#define m_can_resetFunId				24






#define m_can_isModule(module)	(((module) >= 0) && ((module) < m_canModCnt))
#define m_can_isStdFrameLen(len) ((m_can_stdFrameLen >= (len)) && (0 < (len)))
#define m_can_isValidBaud(baud)	(((baud) == em_canBaud_250kbps) || \
								((baud) == em_canBaud_500kbps) ||	\
								((baud) == em_canBaud_1Mbps))
#define m_can_isValidClkSoc(soc) ((em_canClkSoc_pll == (soc)) || (em_canClkSoc_xOsc == (soc)))
#define m_can_isValidMbox(mbIndx)	(((mbIndx) >=0) && ((mbIndx) < m_can_mboxCnt))
#define m_can_isValidRxCode(rxCode)((m_can_rxInactive == (rxCode))	|| 	(m_can_rxEmpty == (rxCode)) ||	\
								(m_can_rxFull == (rxCode))	|| 	(m_can_rxOverRun == (rxCode)) ||	\
								(m_can_rxBusy == (rxCode)))
#define m_can_isValidTxCode(txCode)((m_can_txInactive == (txCode))	|| 	(m_can_txAbort == (txCode)) ||	\
								(m_can_txDt == (txCode))	||	(m_can_txRemote == (txCode)) ||	\
								(m_can_txRAnswer == (txCode)))

#define m_can_isRecDtCode(rxCode)	((m_can_rxFull == (rxCode))	|| 	(m_can_rxOverRun == (rxCode)))
#define m_can_istxDtFinshCode(txCode)	(m_can_txInactive == (txCode))
#define m_can_istxEmptyCode(txCode)	((m_can_txInactive == (txCode)) || (m_can_txAbort == (txCode)))
#define m_can_ismcuModule(moduIndx)	(((moduIndx) >= 0) &&((moduIndx) <= 2))

#define m_can_stdFrameLen		8
#define m_can_baudCnt			em_canBaud_cnt
#define m_can_mboxCnt			64

#define m_can_exFrameSymMask		0x80000000ul
#define m_can_exIdMask				0x1FFFFFFFul
#define m_can_stdIdMask				0x000007FFul



/*CAN RX MB CODE*/
#define m_can_rxInactive 0x00
#define m_can_rxEmpty    0x04
#define m_can_rxFull    0x02
#define m_can_rxOverRun	0x06
#define m_can_rxBusy	0x01

/*CAN TX MB CODE*/
#define m_can_txInactive	0x08
#define m_can_txAbort		0x09
/*MB RTR : 0*/
#define m_can_txDt			0x0c
/*MB RTR : 1*/
#define m_can_txRemote		0x0c
#define m_can_txRAnswer		0x0E

/*module register bit mask definition*/
#define m_can_mcrResetMask 		0x02000000
#define m_can_mcrMdisMask		0x80000000
#define m_can_mcrSelfRecMask	0x00020000
#define m_can_mcrIrmqMask		0x00010000
#define m_can_mcrHaltMask		0x10000000
#define m_can_mcrTxAbortMask	0x00001000
#define m_can_mbNum				64

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef struct
{
	volatile struct CAN_tag * module;
	uint8_t			   rxMscrPin;
	uint8_t			   rxIscrPin;
	uint8_t			   rxAfLv;
	uint8_t			   txMscrPin;
	uint8_t			   txAfLv;
}canModCfg_t;

typedef struct
{
	uint8_t preDiv;
	uint8_t	proPseg;	/*[0,7]*/
	uint8_t	pseg1;		/*[0,7]*/
	uint8_t	pseg2;		/*[0,7]*/
}canBuadCfg_t;

typedef enum
{
	em_canMod_0 = 0,
	em_canMod_1,
	em_canMod_2
	//em_canMod_cnt
}canMod_t;

typedef enum
{
	em_canBaud_250kbps = 0,
	em_canBaud_500kbps,
	em_canBaud_1Mbps,
	em_canBaud_cnt
}canBaud_t;

typedef enum
{
	em_canClkSoc_xOsc =0,
	em_canClkSoc_pll,
}canClkSoc_t;


typedef enum
{
	em_canMsgBind_Fix = 0,
	em_canMsgBind_Dyn	
}canMsgBindWay_t;
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
extern bool can_init(uint8_t module,uint8_t baud,canMsgBindWay_t msgBindWay);
extern bool can_txMsgFix(uint8_t module,uint8_t mbox,uint32_t id, uint8_t * txBuf, uint8_t dlc); 
extern bool can_txMsgDyn(uint8_t module,uint32_t id, uint8_t * txBuf, uint8_t dlc);
extern void can_rxMbSet(uint8_t module,uint8_t mbIndx,uint32_t id,uint8_t dlc,uint8_t rxCode);
extern void can_txMbSet(uint8_t module,uint8_t mbIndx,uint32_t id,uint8_t dlc,uint8_t txCode);
extern void can_rxMsg(uint8_t module,uint8_t mbIndx);
extern uint8_t can_getCsCode(uint8_t module,uint8_t mbIndx);
extern void can_mbIntCmd(uint8_t module,uint8_t mbIndx,functionSt_t st);
extern void can_clearIFlag(uint8_t module, uint8_t mbIndx);
extern uint32_t can_getIFlag1(uint8_t module);
extern uint32_t can_getIFlag2(uint8_t module);
extern bool can_getErrFlag(uint8_t module);
extern bool can_getBusOffFlag(uint8_t module);
extern void can_clrBusOffFlag(uint8_t module);
extern void can_clrErrFlag(uint8_t module);
extern void can_mbAllIntCmd(uint8_t module,functionSt_t st); 
extern void can_reset(uint8_t module);
extern void can_cmd(uint8_t module,functionSt_t st);

#endif


