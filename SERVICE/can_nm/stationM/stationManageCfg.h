/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:stationManageCfg.h
* Description:
* REQ IDs:
* History:
* 2016-12-21,Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef STATION_MANAGE_CFG_H_
#define STATION_MANAGE_CFG_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_sm_faultMaskCategoryC					1
#define m_sm_faultMaskCategoryD					2
#define m_sm_faultMaskCategory					m_sm_faultMaskCategoryC		

#if m_sm_faultMaskCategory == m_sm_faultMaskCategoryC
/*only availbale in category C mode*/
#define m_sm_diagMuxBus						1
#define m_sm_diagMuxMsg						0
#define m_sm_diagMuxByteInFrame				4
#if m_sm_diagMuxByteInFrame >= 8
#error "wrong byte index of diagMux state allocation"
#endif



#define m_sm_igStViaCanBus					FALSE
#if TRUE == m_sm_igStViaCanBus
#define m_sm_igStBus						1
#define m_sm_igStMsg						0
#define m_sm_IgStByteInFrame				3
#if m_sm_IgStByteInFrame >= 8
#error "wrong byte index of ignition state allocation"
#endif



#endif

#else
/*only availbale in category D mode*/


#endif

#define m_sm_decThres				(1)
#define m_sm_decM					(1)
#define m_sm_valMax					(40)


#define m_sm_callPeriod				(10)					/*unit:ms*/
#define m_sm_tQuickTime				(5)	/*unit:m_sm_callPeriod ms*/
#define m_sm_tSlowTime				(20)
#define m_sm_tQTimes			5
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

#endif



