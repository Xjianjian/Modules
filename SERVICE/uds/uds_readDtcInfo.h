/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:uds_readDtcInfo.h
* Description: the header files for the sub-functions of the 0x19[readDtcInfomation] service
* REQ IDs:
* History:
* 2017-02-22,  Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef UDS_READ_DTC_INFO_H_
#define UDS_READ_DTC_INFO_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define uds_supprotDtcStatusMask		(0xFF)
#define uds_iso14229DtcFormat			(0x01)

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
extern uint8_t serv_reportNumberOfDtcByStatusMask(void);
extern uint8_t serv_reportDtcByStatusMask(void);
extern uint8_t serv_reportDtcSnapshotId(void);
extern uint8_t serv_reportDtcSnapshotByDtcNo(void);
extern uint8_t serv_reportSupportDtc(void);
#endif

