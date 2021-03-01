/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:stationManage.h
* Description:
* REQ IDs:
* History:
* 2016-12-21,Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef STATION_MANAGE_H_
#define STATION_MANAGE_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
		


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_sm_diagMuxStoff			0
#define m_sm_diagMuxStOn			1



/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
/*fault storing byte*/
typedef union
{
	uint8_t R;
	struct
	{
		uint8_t currFaultSt			:1;
		uint8_t cfirmFaultSt		:1;
		uint8_t faultCnt			:6;
		
	}B;
}stationManageDiagByte_t;




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
extern  void sm_init(void);
extern void sm_handleCyclic(void);
extern void sm_busoffCb(uint8_t bus);
extern void sm_busoffRecoverCb(uint8_t bus);
#endif



