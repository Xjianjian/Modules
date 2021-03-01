/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:eepromBlockCfg.h
* Description:
* REQ IDs:
* History:
* 2016-10-06, Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef EEPROM_BLOCK_CFG_H_
#define EEPROM_BLOCK_CFG_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/

typedef struct
{
	uint8_t dtSt;	/*must be in the range[m_eehBlkSt_Ok,		m_eehBlkSt_writeErr]*/
	uint8_t opSt;	/*must be in the range:[m_eeBlkOp_Idle,		m_eeBlkOp_err]*/
}eeblkSt_t;




/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/
extern bool nvmBlockDirty[];

/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
extern const uint8_t nvmBlockCnt;

/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/
extern void anv_defDtInit(void);

#endif





