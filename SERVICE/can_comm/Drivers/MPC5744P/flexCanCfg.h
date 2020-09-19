
#ifndef FLEX_CAN_CFG_H_
#define FLEX_CAN_CFG_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_canModCnt 						 3
#define m_can_enable						 TRUE
#define m_can_selfRec						 FALSE
#define m_can_IRMQ							 FALSE
#define m_can_busOffInt					     FALSE
#define m_can_busErrInt					     FALSE
#define m_can_txAbortEna					 TRUE


#define m_canScanRecMsgCb(module,mbIndx,id,buf,dlc)	hcan_canScanRecMsgDispose(module,mbIndx,id,buf,dlc)
#define m_canRecRemoteFrameCb(module,mbIndx,id)		hcan_canRecRemoteFrameDispose(module,mbIndx,id)
#define m_canIsrRecMsgCb(module,mbIndx,id,buf,dlc) 	hcan_canIsrRecMsgDispose(module,mbIndx,id,buf,dlc)
#define m_canIsrRecRemoteFrameCb(module,mbIndx,id) 	hcan_canIsrRecRemoteFrameDispose(module,mbIndx,id)
#define m_canIsrTxFinshCb(module,mbIndx,id);		hcan_canIsrTxConfirmCb(module,mbIndx,id)
#define m_canIsrBusOffCb(module)			 		hcan_canBusOffConfirmCb(module)
#define m_canIsrBusErrCb(module)			 		hcan_canBusErrConfrimCb(module)

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/



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

extern void hcan_canScanRecMsgDispose(uint8_t module,uint8_t mbIndx,uint32_t id,uint8_t  * buf,uint8_t dlc);
extern void hcan_canRecRemoteFrameDispose(uint8_t module,uint8_t mbIndx,uint32_t id);
extern void hcan_canBusOffConfirmCb(uint8_t module);
extern void hcan_canBusErrConfrimCb(uint8_t module);
extern void hcan_canIsrRecMsgDispose(uint8_t module,uint8_t mbIndx,uint32_t id,uint8_t  * buf,uint8_t dlc);
extern void hcan_canIsrRecRemoteFrameDispose(uint8_t module,uint8_t mbIndx,uint32_t id);
extern void hcan_canIsrTxConfirmCb(uint8_t module,uint8_t mbIndx,uint32_t id);
#endif



