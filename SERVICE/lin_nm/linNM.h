/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:lin_com.h
* Description:
* REQ IDs:
* History:
* 2018-06-14, Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef LIN_NM_H_
#define LIN_NM_H_
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "linNMCfg.h"
/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define LINNM_MODE_INIT		0
#define LINNM_MODE_NORMAL	1
#define LINNM_MODE_SLEEP	3

#define LINNM_TXRX_STOP		0
#define LINNM_TXRX_START	1
/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/

typedef enum
{
	e_lin0 = 0,
	e_lin1,
	e_linmax
}linNM_obj;




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
extern void linNM_init(void);
extern void linNM_task(void);
#endif

