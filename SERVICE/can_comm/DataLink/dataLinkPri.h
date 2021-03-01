/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:dataLinkPri.h
* Description:
* REQ IDs:
* History:
* 2016-08-11,Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef DATA_LINK_PRI_H_
#define DATA_LINK_PRI_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_hcan_mboxFix   0		/*one message fixed map to one mbox,and never change,both rx and tx*/
#define m_hcan_mboxDyna  1		/*all the rx messages share the Mb0, all the tx messages share the Mb1, and both Rx isr and Tx Isr must be enabled*/

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/

typedef struct
{
	bool 	 mboxAssignedOk;
	uint8_t	 MboxAssignWay;
}hcanMboxAssign_t;



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



