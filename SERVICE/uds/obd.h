/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:obd.h
* Description: OBD diagnose module header file
* REQ IDs:
* History:
* 2017-02-20,  Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef OBD_H_
#define OBD_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "dtcInfo.h"
#include "obd.h"

/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/


#define m_ff_orderMask		(0x7FFF)
#define m_ff_emptyMask		(0x8000)

#define m_ff_setEmpty(foIndx)		(obdFFOrder[foIndx] |= 	m_ff_emptyMask)	
#define m_ff_setFull(foIndx)		(obdFFOrder[foIndx] &= 	(~m_ff_emptyMask))
#define m_ff_getOrder(foIndx)		(obdFFOrder[foIndx] & m_ff_orderMask)
#define m_ff_getEmptyFlag(foIndx)	(((obdFFOrder[foIndx] & m_ff_emptyMask) > 0)?(TRUE):(FALSE))
#define m_ff_setOrder(foIndx,val)	(obdFFOrder[foIndx] = (val))


#define m_frameOrderKeepFreshFirst			TRUE

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
extern void obd_defDtInit(void);
extern void obd_init(void);
extern void obd_updateFault(uint16_t fault_i,bool occured);
extern void obd_shutdown(void);
extern void obd_clearAll(void);
extern void obd_clearFault(uint16_t fault_i, bool udsClear);
extern void obd_diag_update(void);

#endif

