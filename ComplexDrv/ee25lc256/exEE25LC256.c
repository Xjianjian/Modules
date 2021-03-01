/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:exEE25LC256.c
* Description:
* REQ IDs:
* History:
* 2016-11-11, Jimmy Draft version
*/

/* ---- Sources witch on ------------------------------------------------------*/
#define EXEE25LC526_C_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "global_var.h"
#include "spi.h"
#include "spiCfg.h"
#include "hdio.h"
#include "hdioCfg.h"
#include "exEE25LC256.h"
#include "exEE25LC256_cfg.h"


/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/
#define m_2256_initWriteProtected  FALSE

/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/
static uint8_t ee256Op = m_ee256_opNone;
static ee256StReg_t  ee256StReg = {0};
static bool ee256StRegUpdt = FALSE;


static uint8_t ee256CurrJob = m_ee256_jobNone;
static uint8_t ee256CurrSt = m_ee256_idle;
static uint8_t ee256CurrJobSt = m_ee256_jobOk;
static ee256Job_t ee256Job = {0,0,0,0};
/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/
bool ee256_spiCommStatus = m_spi_sendNoErr;
/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/
static void ee256_readStatusReg(void);
static void ee256_recMsgHandle(uint32_t rsp);
static bool ee256_writeEna(bool ena,bool verify);
static uint8_t ee256_taskDispose(void);


/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/

bool ee256_init(void)
{
	bool initOk = FALSE;
	uint16_t timeTick = 6000;
	#if 0
	uint8_t comRslt = 0;
	uint16_t addr = 40;
	uint16_t byteIndx = 0;
	uint8_t readBuf[40] = {0};
	uint16_t timeTick1 = 100;
	uint32_t tempDt = 0;
	#endif
	
	hld_do_EEP_WP = FALSE;
	hdio_doUpdtIndx(m_hdo_EEP_WP);



	if(TRUE == ee256_writeEna(TRUE,TRUE))
	{
		initOk = TRUE;
	}

	#if 0
	for(byteIndx = 0;byteIndx < 40;byteIndx ++)
	{
		timeTick1 = 100;
		comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,m_ee256_writeDtCmd,TRUE,TRUE);
		comRslt |= spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,
							 (((addr + byteIndx) >>8) & 0xFF) ,TRUE,TRUE);
		comRslt |= spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,
							 ((addr + byteIndx) & 0xFF) ,TRUE,TRUE);
		if(m_spi_sendNoErr == comRslt)
		{
			comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, byteIndx ,FALSE,TRUE);
		}
		while(timeTick1 >0)
		{
			timeTick1 -- ;
		}
	}
	for(byteIndx = 0;byteIndx < 40;byteIndx ++)
	{
		timeTick1 = 100;
		comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,m_ee256_readDtCmd,TRUE,TRUE);
		comRslt |= spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, (((byteIndx + addr) >>8) & 0xFF) ,TRUE,TRUE);
		comRslt |= spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, ((byteIndx + addr)  & 0xFF) ,TRUE,TRUE);
		if(m_spi_sendNoErr == comRslt)
		{
			comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, 0 ,FALSE,TRUE);
			if(m_spi_sendNoErr == comRslt)
			{
				comRslt = spi_readDt(m_ee256_spiModule,&tempDt);
				if(m_spi_readNoErr == comRslt)
				{
					readBuf[byteIndx] = (uint8_t)(tempDt & 0xFF);
				}
				while(timeTick1 >0)
				{
					timeTick1 -- ;
				}
			}
		}
	}
	#endif

	while(timeTick >0)
	{
		timeTick -- ;
	}
	
#if 0
	timeTick = 60000;
	comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,m_ee256_readDtCmd,TRUE,TRUE);
	comRslt |= spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, 0,TRUE,TRUE);
	comRslt |= spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, 0 ,TRUE,TRUE);
	if(m_spi_sendNoErr == comRslt)
	{
		for(;byteIndx <10;byteIndx ++)
		{
			if(9 == byteIndx)
			{
				comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, 0 ,FALSE,TRUE);
			}
			else
			{
				comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, 0 ,TRUE,TRUE);
			}
			if(m_spi_sendNoErr == comRslt)
			{
				comRslt = spi_readDt(m_ee256_spiModule,&tempData);
				readDt[byteIndx] = (uint8_t)(tempData & 0xFF);
			}
			else
			{
				break;
			}
		}

	}


	while(timeTick >0)
	{
		timeTick -- ;
	}
	if(byteIndx >= 10)
	{
		byteIndx = 0;
		comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,m_ee256_writeDtCmd,TRUE,TRUE);
		comRslt |= spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, 0,TRUE,TRUE);
		comRslt |= spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, 0 ,TRUE,TRUE);
		if(m_spi_sendNoErr == comRslt)
		{
			for(;byteIndx <10;byteIndx ++)
			{
				if(9 == byteIndx)
				{
					comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, (uint32_t)writeDt[byteIndx] ,FALSE,TRUE);
				}
				else
				{
					comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, (uint32_t)writeDt[byteIndx] ,TRUE,TRUE);
				}
				if(m_spi_sendNoErr != comRslt)
				{
					break;
				}
			}
		}
		
		
	}
#endif
	return initOk;
}

void ee256_cyclic(void)
{
	uint16_t handleTimes = 0;
	while((m_ee256_busy == ee256CurrSt) && (ee256Job.remainLen > 0) && (handleTimes < m_ee256_handleFreqTimes))
	{
		ee256_spiCommStatus = ee256_taskDispose();
		handleTimes ++;
	}
}




void ee256_read(uint32_t eeAddr, uint32_t ramAddr,uint16_t len)
{
	
	uint8_t jobStShadow = m_ee256_jobOk;
	jobStShadow = (m_ee256_idle == ee256CurrSt)?(jobStShadow):(m_ee256_jobBusy);
	if(m_ee256_jobOk != jobStShadow)
	{
		if((ee256Job.eeAddr == eeAddr) && 
		  (ee256Job.ramAddr == ramAddr) && 
		  (ee256Job.len == len)) 
		{
			ee256CurrJobSt = m_ee256_jobBusy;	
		}
		else
		{
			ee256CurrJobSt = m_ee256_sysBusy;	
		}	
	}
	
	if(m_ee256_jobOk == jobStShadow)
	{
		jobStShadow = ((len > 0) && (len < 0x8000))?(jobStShadow):(m_ee256_dtLenErr);
		if(m_ee256_jobOk != jobStShadow)
		{
			ee256CurrJobSt = m_ee256_dtLenErr;		
		}
	}
	
	if(m_ee256_jobOk == jobStShadow)
	{
		jobStShadow = ((eeAddr >=0) && ((len + eeAddr) <= 0x8000))?(jobStShadow):(m_ee256_eeAddrErr);
		if(m_ee256_jobOk != jobStShadow)
		{
			ee256CurrJobSt = m_ee256_eeAddrErr;		
		}
	}
	
	if(m_ee256_jobOk == jobStShadow)
	{
		jobStShadow = ((ramAddr >=0x40000000) && ((ramAddr + len) <= 0x40060000))?(jobStShadow):(m_ee256_ramAddrErr);
		if(m_ee256_jobOk != jobStShadow)
		{
			ee256CurrJobSt = m_ee256_ramAddrErr;		
		}
	}

	
	if(m_ee256_jobOk == jobStShadow)
	{
		ee256CurrSt = m_ee256_busy;
		ee256CurrJob = m_ee256_jobRead;
		ee256Job.eeAddr = eeAddr;
		ee256Job.ramAddr = ramAddr;
		ee256Job.len = len;
		ee256Job.remainLen = len;
	}
}

void ee256_write(uint32_t eeAddr, uint32_t ramAddr,uint16_t len)
{
	uint8_t jobStShadow = m_ee256_jobOk;
	jobStShadow = (m_ee256_idle == ee256CurrSt)?(jobStShadow):(m_ee256_jobBusy);
	if(m_ee256_jobOk != jobStShadow)
	{
		if((ee256Job.eeAddr == eeAddr) && 
		  (ee256Job.ramAddr == ramAddr) && 
		  (ee256Job.len == len)) 
		{
			ee256CurrJobSt = m_ee256_jobBusy;	
		}
		else
		{
			ee256CurrJobSt = m_ee256_sysBusy;	
		}	
	}
	
	if(m_ee256_jobOk == jobStShadow)
	{
		jobStShadow = ((len > 0) && (len < 0x8000))?(jobStShadow):(m_ee256_dtLenErr);
		if(m_ee256_jobOk != jobStShadow)
		{
			ee256CurrJobSt = m_ee256_dtLenErr;		
		}
	}
	
	if(m_ee256_jobOk == jobStShadow)
	{
		jobStShadow = ((eeAddr >=0) && ((len + eeAddr) <= 0x8000))?(jobStShadow):(m_ee256_eeAddrErr);
		if(m_ee256_jobOk != jobStShadow)
		{
			ee256CurrJobSt = m_ee256_eeAddrErr;		
		}
	}
	
	if(m_ee256_jobOk == jobStShadow)
	{
		jobStShadow = ((ramAddr >=0x40000000) && ((ramAddr + len) <= 0x40060000))?(jobStShadow):(m_ee256_ramAddrErr);
		if(m_ee256_jobOk != jobStShadow)
		{
			ee256CurrJobSt = m_ee256_ramAddrErr;		
		}
	}

	
	if(m_ee256_jobOk == jobStShadow)
	{
		ee256CurrSt = m_ee256_busy;
		ee256CurrJob = m_ee256_jobWrite;
		ee256Job.eeAddr = eeAddr;
		ee256Job.ramAddr = ramAddr;
		ee256Job.len = len;
		ee256Job.remainLen = len;
	}		
}


uint8_t ee256_getSt(void)
{
	return ee256CurrSt;
}

uint8_t ee256_getJobRslt(void)
{
	return ee256CurrJobSt;
}



/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static uint8_t ee256_taskDispose(void)
{
	uint16_t finishLen = 0;
	uint32_t eeData = 0;
#if !m_ee256_readAllOneTime
	uint16_t readLenRemainThisCycle = 0;
#endif
	uint16_t writeLenRemainThisCycle = 0;
	uint8_t comRslt = m_spi_sendNoErr;
	if((m_ee256_busy == ee256CurrSt) && (ee256Job.remainLen > 0))	
	{
		switch(ee256CurrJob)
		{
			case m_ee256_jobRead:
				finishLen = ee256Job.len - ee256Job.remainLen;
				
				if((finishLen >= 0) && (finishLen < ee256Job.len)) 
				{
					#if m_ee256_readAllOneTime
					comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,m_ee256_readDtCmd,TRUE,TRUE);
					comRslt |= spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, ((ee256Job.eeAddr >>8) & 0xFF) ,TRUE,TRUE);
					comRslt |= spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, (ee256Job.eeAddr  & 0xFF) ,TRUE,TRUE);
					if(m_spi_sendNoErr == comRslt)
					{
						while(ee256Job.remainLen >0)
						{
							if(1 == ee256Job.remainLen)
							{
								comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, 0 ,FALSE,TRUE);
								
							}
							else
							{
								comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, 0 ,TRUE,TRUE);
							}
							if(m_spi_sendNoErr == comRslt)
							{
								comRslt = spi_readDt(m_ee256_spiModule,(uint32_t *)(&eeData));
								/*清空了ram 数据*/
								if(m_spi_readNoErr == comRslt)
								{
									*(uint8_t *)(ee256Job.ramAddr + finishLen) = (uint8_t)(eeData & 0xFF);
									ee256Job.remainLen --;
									finishLen ++;
								}
								else
								{
									ee256CurrJobSt = m_ee256_jobFail;
									ee256CurrSt = m_ee256_idle;
									break;
								}
							}
							else
							{
								ee256CurrJobSt = m_ee256_jobFail;
								ee256CurrSt = m_ee256_idle;
								break;
							}
						}
					}
					else
					{
						ee256CurrJobSt = m_ee256_jobFail;
						ee256CurrSt = m_ee256_idle;	
					}
					#else
					if(ee256Job.remainLen > m_ee256_readSize)
					{
						readLenRemainThisCycle = ee256Job.remainLen ;
					}
					else
					{
						readLenRemainThisCycle = 	ee256Job.remainLen;	
					}
					comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,m_ee256_readDtCmd,TRUE,TRUE);
					comRslt |= spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, (((ee256Job.eeAddr + finishLen) >>8) & 0xFF) ,TRUE,TRUE);
					comRslt |= spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, ((ee256Job.eeAddr + finishLen) & 0xFF) ,TRUE,TRUE);
					if(m_spi_sendNoErr == comRslt)
					{
						while(readLenRemainThisCycle > 0)
						{
							if(1 == readLenRemainThisCycle)
							{
								comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, 0 ,FALSE,TRUE);
							}
							else
							{
								comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, 0 ,TRUE,TRUE);
							}
							if(m_spi_sendNoErr == comRslt)
							{
								comRslt = spi_readDt(m_ee256_spiModule,(uint32_t *)(&eeData));
								if(m_spi_readNoErr == comRslt)
								{
									*(uint8_t *)(ee256Job.ramAddr + finishLen) = (uint8_t)(eeData & 0xFF);
									ee256Job.remainLen --;
									finishLen ++;
									readLenRemainThisCycle --;
								}
								else
								{
									ee256CurrJobSt = m_ee256_jobFail;
									ee256CurrSt = m_ee256_idle;
									break;
								}
							}
							else
							{
								ee256CurrJobSt = m_ee256_jobFail;
								ee256CurrSt = m_ee256_idle;
								break;
							}
						}
					}
					else
					{
						ee256CurrJobSt = m_ee256_jobFail;
						ee256CurrSt = m_ee256_idle;	
					}
					#endif
					
				}
				else
				{
					ee256CurrJobSt = m_ee256_jobOk;
					ee256CurrSt = m_ee256_idle;
				}
				/*if the ee256CurrSt has been set to m_ee256_idle before here, one fail must have happened*/
				if((0 == ee256Job.remainLen) && (m_ee256_idle != ee256CurrSt))
				{
					ee256CurrJobSt = m_ee256_jobOk;
					ee256CurrSt = m_ee256_idle;
				}
				
				break;
			case m_ee256_jobWrite:
				finishLen = ee256Job.len - ee256Job.remainLen;
 				writeLenRemainThisCycle = m_ee256_writePageSize - ((ee256Job.eeAddr + finishLen) % m_ee256_writePageSize);
				if(writeLenRemainThisCycle > ee256Job.remainLen)
				{
					writeLenRemainThisCycle = ee256Job.remainLen;
				}
				if((finishLen >= 0) && (finishLen < ee256Job.len)) 
				{
					if(TRUE == ee256_writeEna(TRUE,TRUE))
					{
						comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,m_ee256_writeDtCmd,TRUE,TRUE);
						comRslt |= spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,
											 (((ee256Job.eeAddr + finishLen) >>8) & 0xFF) ,TRUE,TRUE);
						comRslt |= spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,
											 ((ee256Job.eeAddr + finishLen) & 0xFF) ,TRUE,TRUE);
						if(m_spi_sendNoErr == comRslt)
						{
							while(writeLenRemainThisCycle > 0)
							{
								if(1 == writeLenRemainThisCycle)
								{
									comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, *((uint8_t *)(ee256Job.ramAddr + finishLen)) ,FALSE,TRUE);
									
								}
								else
								{
									comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, *((uint8_t *)(ee256Job.ramAddr + finishLen)) ,TRUE,TRUE);
								}
								if(m_spi_sendNoErr == comRslt)
								{
									ee256Job.remainLen --;
									finishLen ++;
									writeLenRemainThisCycle --;
								}
								else
								{
									ee256CurrJobSt = m_ee256_jobFail;
									ee256CurrSt = m_ee256_idle;
									break;
								}
							}
						}
						else
						{
							ee256CurrJobSt = m_ee256_jobFail;
							ee256CurrSt = m_ee256_idle;
						}
					}
					else
					{
						ee256CurrJobSt = m_ee256_jobFail;
						ee256CurrSt = m_ee256_idle;
					}
					
				}
				else
				{
					ee256CurrJobSt = m_ee256_jobOk;
					ee256CurrSt = m_ee256_idle;
				}
				/*if the ee256CurrSt has been set to m_ee256_idle before here, one fail must have happened*/
				if((0 == ee256Job.remainLen) && (m_ee256_idle != ee256CurrSt))
				{
					ee256CurrJobSt = m_ee256_jobOk;
					ee256CurrSt = m_ee256_idle;
				}
				break;
			default:
				break;
		}
	}
	return comRslt;
}



static void ee256_readStatusReg(void)
{	
	ee256Op = m_ee256_opReadStReg;
	uint32_t data = 0;
	uint8_t comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,m_ee256_readStRegCmd,TRUE,TRUE);
	comRslt |= spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,0,FALSE,TRUE);
	if(m_spi_sendNoErr == comRslt)
	{
		ee256StRegUpdt = FALSE;
		comRslt = spi_readDt(m_ee256_spiModule,&data);
		if(m_spi_readNoErr == comRslt)
		{
			ee256_recMsgHandle(data);		
		}
	}
}
#if FALSE
static void ee256_WriteStatusReg(uint8_t cmd)
{
	ee256StReg_t stReg = {0};
	stReg.all = cmd;
	(void)spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,m_ee256_writeStRegCmd,TRUE,TRUE);
	spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask, stReg.all,FALSE,TRUE);
}
#endif

static bool ee256_writeEna(bool ena,bool verify)
{
	bool writeEnaRslt = TRUE;
	uint8_t comRslt = m_spi_sendNoErr;
	uint8_t comDt = 0;
	if(TRUE == ena)
	{
		comDt = m_ee256_writeEnaCmd;
	}
	else
	{
		comDt = m_ee256_writeDisCmd;
	}
	
	if(TRUE == verify)
	{
		comRslt = spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,comDt,FALSE,TRUE);
		if(m_spi_sendNoErr == comRslt)
		{
			ee256_readStatusReg();
			if(TRUE == ee256StRegUpdt)
			{
				if(((TRUE == ena)?(1):(0)) != ee256StReg.b.WEL)
				{
					writeEnaRslt = FALSE; 		
				}
			}
			else
			{
				writeEnaRslt = FALSE; 
			}

		}
		else
		{
			writeEnaRslt = FALSE; 
		}
	}
	else
	{
		(void)spi_sendDt(m_ee256_spiModule,m_spi3_eeCtas,m_spi3_eeCsMask,comDt,FALSE,TRUE);
	}
	return writeEnaRslt;
}


static void ee256_recMsgHandle(uint32_t rsp)
{
	switch(ee256Op)
	{
		case m_ee256_opReadStReg:
			ee256StReg.all = (uint8_t)(rsp & 0xFF);	
			ee256StRegUpdt = TRUE;
			
			break;
		case m_ee256_opWriteStReg:

			break;
		case m_ee256_opReadDt:

			break;
		case m_ee256_opWriteDt:

			break;
		default:
			break;
	}
}

/* ---- Sourceswitch off ---------------------------------------------------- */
#undef EXEE25LC526_C_
/*--- End of file ------------------------------------------------------------*/



