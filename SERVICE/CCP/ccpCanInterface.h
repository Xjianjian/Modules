/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:ccpCanInterface.h
* Description:
* REQ IDs:
* History:
* 2016-12-29, Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef CCP_CAN_INTERFACE_H_
#define CCP_CAN_INTERFACE_H_
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_ccp_seedCAL			(0x63635063ul)			/*the ASCII FOR CCPC*/
#define m_ccp_seedDaq			(0x6363506dul)			/*the ASCII FOR CCPD*/
#define m_ccp_seedPgm			(0x63635050ul)			/*the ASCII FOR CCPP*/

#define m_ccp_PLCAL 0x01
#define m_ccp_PLDAQ 0x02
#define m_ccp_PLPGM 0x40


/* Return values for ccpWriteMTA and ccpCheckWriteEEPROM */
#define m_ccp_writeDenied  			0
#define m_ccp_writeOk      			1
#define m_ccp_writePending 			2
#define m_ccp_writeErr   			3
/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
extern bool ccp_storeFlg;
/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/


/*interaction */
extern void ccp_task(void);
extern bool hcan_ccpSendReq(uint8_t * msg);
extern void ccpCommand(uint8_t * msg);
extern uint8_t ccpSendCallBack( void );
extern void ccpSetMTAAddr(uint8_t mta, uint8_t * addr);

extern void ccp_tx_timeout_cb(void);

extern uint8_t nvm_getCCPBlockNum(void);
#endif





