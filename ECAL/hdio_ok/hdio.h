/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:hdio.h
* Description:
* REQ IDs:
* History:
* 2016-08-11, Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef HDIO_H_
#define HDIO_H_
#include "typedefs.h"
#include "platformTypes.h"
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_hdio_writeSigFunId		0
#define m_hdio_readDebInitFunId		1
#define m_hdio_toggleSigFunId		0


#define m_hdio_DebObjIndexInRange(indx) (((indx) >= 0) && ((indx) < m_hdio_diSigCnt))
#define m_hdio_outSigIndxInRange(indx) (((indx) >= m_hdio_firstDoIndex) && ((indx) <= m_hdio_LastDoIndex))
#define m_hdio_wtValueInRange(val)		((m_hdio_stLow == (val)) || (m_hdio_stHigh == (val)))
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
extern void hdio_init(void);
extern void hdio_diSampleCyclic(void);
extern void hdio_doUpdtCycle(void);
extern void hdio_doUpdtIndx(uint8_t indx);
extern bool hdio_writeSig(uint8_t indx,uint8_t val);
extern bool hdio_toggleSig(uint8_t indx);
#endif



