/*
* Copyright (c) File,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:autosar_dir_nm_cfg.h
* Description: Autosar direct network manager configuration header file.
* REQ IDs:
* History:
* Time:2018/5/4
* User:Devon
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef AUTOSAR_DIR_NM_CFG_H_
#define AUTOSAR_DIR_NM_CFG_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "OS_Types.h"
#include "global_var.h"
#include "canCfg_export.h"
#include "autosar_dir_nm_types.h"
/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define TEST_MODE_FORTEMP	0

#if TEST_MODE_FORTEMP
#define  busslp2repeatmsg    		1
#define  repeatmsg2normal    		2
#define  normal2repeatmsg    		3
#define  normal2readyslp    		4
#define  repeatmsg2readyslp    		5
#define  readyslp2repeatmsg    		6
#define  readyslp2normal    		7
#define  readyslp2prebusslp    		8
#define  prebusslp2repeatmsg    	9
#define  prebusslp2busslp    		10
#endif

/*should never be bigger than 255*/
#define AUTOSAR_DIRNM_BUS_CNT       1

#define NM_ENTERSLEEP_MASK				0x1


#define T_NM_CALL_CYCLE			(10u)
#define T_NM_REPEAT_MSG			(1500u)
#define T_NM_TIMEOUT			(2000u)
#define T_WAIT_BUS_SLEEP		(2000u)
#define T_START_APP_FRAME		(20u)
#define T_NM_IMME_CYCLE			(20u)
#define T_NM_MSG_CYCLE			(500u)
#define T_NM_NORMAL_TO_SLEEP    (2000u)
#define T_NM_WAKE_OR_SLEEP      (2000u)
#define T_NM_UDS_DELAY			(5000u)
#define T_NM_EXPIRED_DELAY		(600u)

#define N_NM_IMME_TIMES			(4u)/*因在enter模式后马上发出NM帧，所以剩余4次*/
#define T_NM_WAKE_UP			(200u)

#define NM_STATION_ADDR         (0x31)
#define NM_BASE_ADDR            (0x600)

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


