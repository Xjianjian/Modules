/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:dataLink.h
* Description:
* REQ IDs:
* History:
* 2016-08-11,Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef DATA_LINK_H_
#define DATA_LINK_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define DATALINK_CLASS_CAN			0
#define DATALINK_RESERVED_CAN		1
#define DATALINK_PTCAN				2


#define CAN_IL_REC_SEND_STOP 0x0

#define CAN_IL_APP_REC		0x1
#define CAN_IL_APP_SEND		0x2

#define CAN_IL_UDS_REC		0x4
#define CAN_IL_UDS_SEND		0x8

#define CAN_IL_CCP_REC		0x10
#define CAN_IL_CCP_SEND		0x20

#define CAN_IL_REC_SEND 	0xFF

#define DTLINK_UDS_TX_MB           (62)
#define DTLINK_CCP_TX_MB           (63)
/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef struct
{
	uint16_t memCnt;
	uint16_t memIndx;
	uint16_t recMsgCnt;
	uint16_t recExtMsgCnt;
	uint16_t txExtMsgCnt;
}dtLinkMsgTypeCollect_t;

typedef struct
{
	uint16_t head;
	uint16_t tail;
}dtLinkTxDynMsgMagr_t;
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
extern void hcan_init(void);
extern void hcan_initModule(uint8_t module);
extern void hcan_deInitModule(uint8_t module);
extern bool hcan_getBusOffFlag(uint8_t module);
extern void hcan_clearBusOffFlag(uint8_t module);
extern bool hcan_getBusErrFlag(uint8_t module);
extern void hcan_clearBusErrFlag(uint8_t module);
extern void hcan_setILMode(uint8_t module,uint8_t mode,uint8_t set);
extern void hcan_recUnpackMain(void);
extern void hcan_txFinishMain(void);
extern void hcan_txPackMain(void);
extern void hcan_canBusInitReq(uint8_t module);

extern void nm_busOffIndi(uint8_t module);
extern void nm_nodeRecTo(uint8_t bus, uint8_t msgIndx);
extern void nm_nodeRecIndi(uint8_t bus, uint8_t msgIndx);
extern void nm_nodeTranFail(uint8_t bus, uint8_t msgIndx);
extern void nm_nodeTranConfirm(uint8_t bus, uint8_t msgIndx);

extern void hcan_busScanErrHandle(void);
extern void hcan_busScanOffHandle(void);

extern void hcan_canBusOffRemoveCb(uint8_t module);
extern void hcan_canBusErrRemoveCb(uint8_t module);


extern void ccpRxIndication(uint8_t * msg);
extern void ccpTxConfirmCb(void);

extern void nt_txToCb(void);
extern void nt_txConfirmCb(void);

extern void hcan_ntTransmitErrInit(void);


extern void hcan_classCan_txPackMain(void);
extern void hcan_reservedCan_txPackMain(void);
extern void hcan_ptCan_txPackMain(void);

#endif



