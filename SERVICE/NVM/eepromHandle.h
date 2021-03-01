/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:eepromHandle.h
* Description:
* REQ IDs:
* History:
* 2016-10-06, Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef EEPROM_HANDLE_H_
#define EEPROM_HANDLE_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_eeJob_read		1
#define m_eeJob_Write		2
#define m_eeJob_None		0

#define m_eehSt_idle		0
#define m_eehSt_busy		1

#define m_eeJob_maxCnt		20


#define m_eeh_failRetry	FALSE
#if TRUE == m_eeh_failRetry
#define m_eeh_RetryMaxCnt	3
#endif

#define m_eeh_opStep_idle			0
#define m_eeh_opStep_waitEeOver	1
#define m_eeh_waitTimeOut			TRUE
#if TRUE == m_eeh_waitTimeOut
#define m_eeh_waitTimeMaxTick		1000/10		/*tick timebase is 10ms*/
#endif


#define m_eeBlkOp_Idle				0
#define m_eeBlkOp_reading			1
#define m_eeBlkOp_writing			2
#define m_eeBlkOp_readErr			3
#define m_eeBlkOp_writeErr			4


#define m_eehBlkSt_Ok				0
#define m_eehBlkSt_readFail			1
#define m_eehBlkSt_IdErr			2
#define m_eehBlkSt_csumErr			3
#define m_eehBlkSt_writeOk			4
#define m_eehBlkSt_writeFail		5


/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/


typedef struct
{
	uint8_t block;
	uint8_t	job;
}eeJob_t;


typedef struct
{
	uint8_t cnt;
	eeJob_t job;
}eeRetry_t;

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
extern void eeh_init(void);
extern bool eeh_addJob(uint8_t eeBlk,uint8_t op,bool forceAdd);
extern void eeh_cyclic(void);
extern uint8_t eeh_getSt(void);
extern uint8_t eeh_getBlkSt(uint8_t blk);
extern uint8_t NvM_GetErrorStatus(uint8_t BlockId, uint8_t* RequestResultPtr);
#endif




