/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:appNvm.h
* Description:
* REQ IDs:
* History:
* 2016-10-08, Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef APP_NVM_H_
#define APP_NVM_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_appNvm_waitMaxTick				10 
#define m_appNvm_shutDownWaitMaxTick		10		
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
extern void anv_init(void);
extern void anv_cyclic(void);
extern void anv_shutdownCyclic(void);
extern void anv_modifyReq(uint8_t block);
extern bool anv_noDirtyBlock(void);
extern bool anv_blockIsDirty(uint16_t blk);

#endif
