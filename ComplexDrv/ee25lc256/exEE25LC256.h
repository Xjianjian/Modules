/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:exEE25LC256.h
* Description:
* REQ IDs:
* History:
* 2016-10-06,Jimmy Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef EXEE_25LC256_H_
#define EXEE_25LC256_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_ee256_readDtCmd				(0b00000011)	
#define m_ee256_writeDtCmd				(0b00000010)
#define m_ee256_writeDisCmd			(0b00000100)
#define m_ee256_writeEnaCmd			(0b00000110)
#define m_ee256_readStRegCmd			(0b00000101)	
#define m_ee256_writeStRegCmd			(0b00000001)	


#define m_ee256_opNone					0
#define m_ee256_opReadStReg			1
#define m_ee256_opWriteStReg			2
#define m_ee256_opReadDt				3
#define m_ee256_opWriteDt				4


#define m_ee256_idle 			0
#define m_ee256_busy			1

#define m_ee256_jobOk			0	/*the job has been excuted successfully*/
#define m_ee256_sysBusy		1	/*the system is busy*/
#define m_ee256_jobBusy		2	/*the  job is being handled*/
#define m_ee256_eeAddrErr		3	/*the eeprom addr was wrong*/
#define m_ee256_ramAddrErr		4	/*the ram addr was wrong*/
#define m_ee256_dtLenErr		5	/*the data length was wrong*/
#define m_ee256_jobFail		6	/*job has been executed failed*/


#define m_ee256_jobNone  		0
#define m_ee256_jobRead		1
#define m_ee256_jobWrite		2




/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef union
{
	uint8_t all;
	struct
	{
		uint8_t WPEN	:1;				/*write protection enable*/
		uint8_t Rsv		:3;
		uint8_t BP		:2;
		uint8_t	WEL		:1;
		uint8_t WIP		:1;
	}b;
}ee256StReg_t;

typedef struct
{
	uint16_t eeAddr;
	uint32_t ramAddr;
	uint16_t len;
	uint16_t remainLen;
}ee256Job_t;

/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/
extern bool ee256_spiCommStatus;



/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/
extern bool ee256_init(void);

extern void ee256_cyclic(void);

extern void ee256_read(uint32_t eeAddr, uint32_t ramAddr,uint16_t len);

extern void ee256_write(uint32_t eeAddr, uint32_t ramAddr,uint16_t len);

extern uint8_t ee256_getSt(void);

extern uint8_t ee256_getJobRslt(void);

#endif





