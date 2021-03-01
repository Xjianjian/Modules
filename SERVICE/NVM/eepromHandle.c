 /*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:eepromHandle.c
* Description:	diagnose module with leaky bucket algorithm
* REQ IDs:
* History:
* 2016-10-06,  Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#define EEPROM_HANDLE_C_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "nvmBlockInfo.h"
#include "exEE25LC256.h"
#include "eepromBlockCfg.h"
#include "eepromHandle.h"
/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/
static eeblkSt_t eeblkStGrp[m_nvmBlockCnt] =
{
    {m_eehBlkSt_Ok,m_eeBlkOp_Idle},{m_eehBlkSt_Ok,m_eeBlkOp_Idle},{m_eehBlkSt_Ok,m_eeBlkOp_Idle},{m_eehBlkSt_Ok,m_eeBlkOp_Idle},{m_eehBlkSt_Ok,m_eeBlkOp_Idle},
    {m_eehBlkSt_Ok,m_eeBlkOp_Idle},{m_eehBlkSt_Ok,m_eeBlkOp_Idle}
};


static eeJob_t eeJobList[m_eeJob_maxCnt + 1] = 
{
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None},
	{0xFF,m_eeJob_None}
};

static uint8_t eeJobListHead = 0;
static uint8_t eeJobListTail = 0;

#if TRUE == m_eeh_failRetry
static eeRetry_t eeRetry = {0,{0xFF,m_eeJob_None}};
#endif

static uint8_t eeh_lastwrStatus[m_nvmBlockCnt] = {0};

#if TRUE == m_eeh_waitTimeOut
static uint16_t eehWaitTimeCnt = m_eeh_waitTimeMaxTick;
#endif

static uint8_t eehOpStep = m_eeh_opStep_idle;

static eeJob_t currJob = {0xFF,m_eeJob_None};
/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/
static uint8_t eeh_getJobCnt(void);
static bool eeh_readJob(eeJob_t * pjob);

/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void eeh_init(void)
{
	uint8_t blockIndx = 0;
	uint8_t bkUpBlkIndx = 0;
	ee256_init();
	for(; blockIndx < nvmBlockCnt; blockIndx ++)
	{
		if(TRUE == nvmBlockInfoGrp[blockIndx].major)
		{
			if(TRUE == eeh_addJob(blockIndx,m_eeJob_read,TRUE))
			{
				while(m_eehSt_idle != eeh_getSt())
				{
					eeh_cyclic();
					ee256_cyclic();
				}
				if((m_eehBlkSt_Ok != eeh_getBlkSt(blockIndx)) &&
				  (TRUE == nvmBlockInfoGrp[blockIndx].backUp))
				{
					/*try the backup data*/
					bkUpBlkIndx = nvmBlockInfoGrp[blockIndx].bkUpBlock;
					if(TRUE == eeh_addJob(bkUpBlkIndx ,m_eeJob_read,TRUE))
					{
						while(m_eehSt_idle != eeh_getSt())
						{
							eeh_cyclic();
							ee256_cyclic();
						}
					}
				}
			}
			else
			{
				
			}
		}
		
	}
}

bool eeh_addJob(uint8_t eeBlk,uint8_t op,bool forceAdd)
{
	bool addOk = FALSE;
	uint8_t jobCnt = 0;
	bool jobExist = FALSE;
	if((eeBlk < nvmBlockCnt) && ((m_eeJob_read == op) || (m_eeJob_Write == op)))
	{
		/*at first to find if the given job has been added to eeJobList*/
		
		for(jobCnt = eeJobListHead;jobCnt != eeJobListTail;)
		{
			if((eeJobList[jobCnt].block == eeBlk) && (eeJobList[jobCnt].job == op))	
			{
				jobExist = TRUE;
				break;
			}
			jobCnt ++;
			if(jobCnt > m_eeJob_maxCnt)
			{
				jobCnt = 0;
			}
		}
		if(FALSE == jobExist)
		{
			jobCnt = eeh_getJobCnt();
			if(m_eeJob_maxCnt > jobCnt)
			{
				/*the job list not full*/
				eeJobList[eeJobListTail].block = eeBlk;
				eeJobList[eeJobListTail].job = op;
				eeJobListTail ++;
				if(eeJobListTail > m_eeJob_maxCnt)
				{
					eeJobListTail = 0;
				}
				addOk = TRUE;
				
			}
			else if(TRUE == forceAdd)
			{
				/*the job list is full, but force add is required, so just delete the oldest job and add this new one*/
				eeJobList[eeJobListTail].block = eeBlk;
				eeJobList[eeJobListTail].job = op;
				eeJobListTail ++;
				if(eeJobListTail > m_eeJob_maxCnt)
				{
					eeJobListTail = 0;
					eeJobListHead = 1;
				}
				else
				{
					eeJobListHead = eeJobListTail + 1;
					if(eeJobListHead > m_eeJob_maxCnt)
					{
						eeJobListHead = 0;		
					}
				}
				addOk = TRUE;	
			}
			else
			{
				/*the job list full and not force add required, so nothing to do*/
			}
		}
		else
		{
			/*the given job has alrady been accepted,so nothing more need to be done*/
		}
	}
	else
	{
		/*the given job parameters are not welcomed*/
	}
	return addOk;
}

void eeh_cyclic(void)
{
	/*at first to find if there is any pending job exists*/
	bool readJobOk = FALSE;
	uint8_t jobRslt = m_ee256_jobOk;
	uint32_t blockId = 0;
	uint32_t cSum	= 0;
	#if TRUE == m_eeh_failRetry
	bool eeReadRetrying = FALSE;
	#endif
	switch (eehOpStep)
	{
		case m_eeh_opStep_idle:
			if(m_ee256_idle == ee256_getSt())
			{
				#if TRUE == m_eeh_failRetry
				if(eeRetry.cnt > 0)
				{
					currJob = eeRetry.job;
					readJobOk = TRUE;
					eeReadRetrying = TRUE;
				}
				else
				#endif
				{
					while(eeh_getJobCnt() >0)
					{
						readJobOk = eeh_readJob(&currJob);
						if(TRUE == readJobOk)
						{
							break;
						}
					}
				}
				if(TRUE == readJobOk)
				{
					if(currJob.block < nvmBlockCnt)
					{
						switch(currJob.job)
						{
							case m_eeJob_read:
								eeh_lastwrStatus[currJob.block] = 2;
								eeblkStGrp[currJob.block].opSt = m_eeBlkOp_reading;
								ee256_read(nvmBlockInfoGrp[currJob.block].eeAddr,nvmBlockInfoGrp[currJob.block].ramAddrCpy,nvmBlockInfoGrp[currJob.block].dtLen + 8);
								eehWaitTimeCnt = m_eeh_waitTimeMaxTick;
								eehOpStep = m_eeh_opStep_waitEeOver;
								break;
							case m_eeJob_Write:
								eeh_lastwrStatus[currJob.block] = 2;
								eeblkStGrp[currJob.block].opSt = m_eeBlkOp_writing;
								util_memoryCpy32((uint32_t *)(&nvmBlockInfoGrp[currJob.block].id),(uint32_t *)(nvmBlockInfoGrp[currJob.block].ramAddrCpy),1);
								util_memoryCpy8((uint8_t *)nvmBlockInfoGrp[currJob.block].ramAddr,(uint8_t *)(nvmBlockInfoGrp[currJob.block].ramAddrCpy + 4),nvmBlockInfoGrp[currJob.block].dtLen);
								cSum = util_checksum((uint8_t *)(nvmBlockInfoGrp[currJob.block].ramAddr),nvmBlockInfoGrp[currJob.block].dtLen);
								util_memoryCpy32(&cSum,(uint32_t *)(nvmBlockInfoGrp[currJob.block].ramAddrCpy + nvmBlockInfoGrp[currJob.block].dtLen + 4),1);
								ee256_write(nvmBlockInfoGrp[currJob.block].eeAddr,nvmBlockInfoGrp[currJob.block].ramAddrCpy,nvmBlockInfoGrp[currJob.block].dtLen + 8);
								eehWaitTimeCnt = m_eeh_waitTimeMaxTick;
								eehOpStep = m_eeh_opStep_waitEeOver;
								break;
							default:
								break;
						}
					}
					
				}
			}
			break;
		case m_eeh_opStep_waitEeOver:
			if(m_ee256_idle == ee256_getSt())
			{
				jobRslt = ee256_getJobRslt();
				switch(jobRslt)
				{
					case m_ee256_jobOk:
						#if TRUE == m_eeh_failRetry
						if(TRUE == eeReadRetrying)
						{
							eeRetry.cnt = 0;
							eeRetry.job.block = nvmBlockCnt;
							eeRetry.job.job = m_eeJob_None;
						}
						#endif
						if(m_eeJob_read == currJob.job)
						{
							/*verify the block id is correct or not*/
							util_memoryCpy32((uint32_t *)nvmBlockInfoGrp[currJob.block].ramAddrCpy,&blockId,1);
							if((nvmBlockInfoGrp[currJob.block].id & 0x80000000) || (blockId == nvmBlockInfoGrp[currJob.block].id))
							{
								/*verify the checksum is correct or not*/
								if(*(uint32_t *)(nvmBlockInfoGrp[currJob.block].ramAddrCpy + nvmBlockInfoGrp[currJob.block].dtLen + 4) == 	\
								util_checksum((uint8_t *)(nvmBlockInfoGrp[currJob.block].ramAddrCpy + 4),nvmBlockInfoGrp[currJob.block].dtLen))
								{
									util_memoryCpy8((uint8_t *)nvmBlockInfoGrp[currJob.block].ramAddrCpy + 4,(uint8_t *)(nvmBlockInfoGrp[currJob.block].ramAddr),
													nvmBlockInfoGrp[currJob.block].dtLen);
									eeblkStGrp[currJob.block].dtSt = m_eehBlkSt_Ok;
									eeh_lastwrStatus[currJob.block] = 0;
								}
								else
								{
									eeblkStGrp[currJob.block].dtSt = m_eehBlkSt_csumErr;
									eeh_lastwrStatus[currJob.block] = 1;
								}

							}
							else
							{
								eeblkStGrp[currJob.block].dtSt = m_eehBlkSt_IdErr;
								eeh_lastwrStatus[currJob.block] = 1;
							}
						}
						else if(m_eeJob_Write == currJob.job)
						{
							/*nothing more need to be done*/
							eeblkStGrp[currJob.block].dtSt = m_eehBlkSt_writeOk;
							eeh_lastwrStatus[currJob.block] = 0;
						}
						else
						{
							/*nothing more need to be done*/
						}
						eeblkStGrp[currJob.block].opSt = m_eeBlkOp_Idle;
						eehOpStep = m_eeh_opStep_idle;
						break;
					case m_ee256_jobBusy:
						/*current the  job is being handled*/
						/*Timeout handled*/
						#if m_eeh_waitTimeOut
						if(eehWaitTimeCnt >0)
						{
							eehWaitTimeCnt --;
						}
						else
						{
							/*here need to log an error of this block*/
							if(m_eeJob_read == currJob.job)
							{
								eeblkStGrp[currJob.block].opSt = m_eeBlkOp_readErr;
								eeblkStGrp[currJob.block].dtSt = m_eehBlkSt_readFail;
								eeh_lastwrStatus[currJob.block] = 1;
							}
							else if(m_eeJob_Write == currJob.job)
							{
								eeblkStGrp[currJob.block].opSt = m_eeBlkOp_writeErr;
								eeblkStGrp[currJob.block].dtSt = m_eehBlkSt_writeFail;
								eeh_lastwrStatus[currJob.block] = 1;
							}
							eehOpStep = m_eeh_opStep_idle;
						}
						#endif
						break;
					case m_ee256_eeAddrErr:
					case m_ee256_ramAddrErr:
					case m_ee256_dtLenErr:
						if(m_eeJob_read == currJob.job)
						{
							eeblkStGrp[currJob.block].opSt = m_eeBlkOp_readErr;
							eeblkStGrp[currJob.block].dtSt = m_eehBlkSt_readFail;
							eeh_lastwrStatus[currJob.block] = 1;
						}
						else if(m_eeJob_Write == currJob.job)
						{
							eeblkStGrp[currJob.block].opSt = m_eeBlkOp_writeErr;
							eeblkStGrp[currJob.block].dtSt = m_eehBlkSt_writeFail;
							eeh_lastwrStatus[currJob.block] = 1;
						}
						else
						{
							/*nothing more need to be done*/
						}
						eehOpStep = m_eeh_opStep_idle;
						break;
					case m_ee256_jobFail:
						#if TRUE == m_eeh_failRetry
						if(TRUE == eeReadRetrying)
						{
							eeRetry.cnt --;
							if(eeRetry.cnt <= 0)
							{
								/*here need to log an error of this block*/
								if(m_eeJob_read == currJob.job)
								{
									eeblkStGrp[currJob.block].opSt = m_eeBlkOp_readErr;
									eeblkStGrp[currJob.block].dtSt = m_eehBlkSt_readFail;
								}
								else if(m_eeJob_Write == currJob.job)
								{
									eeblkStGrp[currJob.block].opSt = m_eeBlkOp_writeErr;
									eeblkStGrp[currJob.block].dtSt = m_eehBlkSt_writeFail;
								}
							}
							else
							{
							}
						}
						else
						{
							eeRetry.cnt = m_eeh_RetryMaxCnt;
							eeRetry.job.block = currJob.block;
							eeRetry.job.job = currJob.job;
						}
						#else
						if(m_eeJob_read == currJob.job)
						{
							eeblkStGrp[currJob.block].opSt = m_eeBlkOp_readErr;
							eeblkStGrp[currJob.block].dtSt = m_eehBlkSt_readFail;
							eeh_lastwrStatus[currJob.block] = 1;
						}
						else if(m_eeJob_Write == currJob.job)
						{
							eeblkStGrp[currJob.block].opSt = m_eeBlkOp_writeErr;
							eeblkStGrp[currJob.block].dtSt = m_eehBlkSt_writeFail;
							eeh_lastwrStatus[currJob.block] = 1;
						}
						else
						{
							/*nothing more need to be done*/
						}
						#endif
						eehOpStep = m_eeh_opStep_idle;
						break;
					default:
						eehOpStep = m_eeh_opStep_idle;
						break;
				}
			}
			else
			{
				/*ee256 is still busy , just continue waiting*/
			}
			break;
		default:
			break;
	}

}

uint8_t eeh_getSt(void)
{
	uint8_t eehSt = m_eehSt_busy;
	if((0 == eeh_getJobCnt()) && (m_ee256_idle == ee256_getSt()) && (m_eeh_opStep_idle == eehOpStep))
	{
		eehSt = m_eehSt_idle;
	}
	return eehSt;
}

uint8_t eeh_getBlkSt(uint8_t blk)
{
	uint8_t blkSt = 0xFF;/*0xFF means invalid block state*/
	blkSt = ((blk < nvmBlockCnt)?(eeblkStGrp[blk].dtSt):(0xFF));
	return blkSt;
}

uint8_t NvM_GetErrorStatus(uint8_t BlockId, uint8_t* RequestResultPtr)
{
	if(BlockId < nvmBlockCnt)
	{
		if(NULL != RequestResultPtr)
		{
			*RequestResultPtr = eeh_lastwrStatus[BlockId];
		}
	}
	else
	{}
	return 0;
}

/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static uint8_t eeh_getJobCnt(void)
{
	uint8_t jobCnt = 0;
	if((eeJobListHead > m_eeJob_maxCnt) || (eeJobListTail > m_eeJob_maxCnt))
	{
		/*unexpected error happens here, need to reset the head and tail*/
		eeJobListHead = 0;
		eeJobListTail = 0;
	}
	if(eeJobListHead <= eeJobListTail)
	{
		jobCnt = eeJobListTail - eeJobListHead;	
	}
	else
	{
		jobCnt = eeJobListTail + m_eeJob_maxCnt + 1 - eeJobListHead;
	}
	if(jobCnt > m_eeJob_maxCnt)
	{
		/*unexpected error happens here, need to reset the head and tail*/
		eeJobListHead = 0;
		eeJobListTail = 0;
		jobCnt = 0;
	}
	return jobCnt;
}



static bool eeh_readJob(eeJob_t * pjob)
{
	bool readOk = FALSE;
	if(eeh_getJobCnt() >0)
	{
		*pjob = eeJobList[eeJobListHead];
		eeJobListHead ++;
		if(eeJobListHead > m_eeJob_maxCnt)
		{
			eeJobListHead = 0;
		}
		readOk = TRUE;
	}
	return readOk;
}






/* ---- Source switch off ---------------------------------------------------- */
#undef EEPROM_HANDLE_C_
/*--- End of file ------------------------------------------------------------*/





