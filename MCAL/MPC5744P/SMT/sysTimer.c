
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "MPC5744P.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "sysTimer.h"
#include "sysTimerCfg.h"
/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/
static uint32_t timeStamp[m_stm_stampMaxCnt] = {0,0};
static bool timeStampSet[m_stm_stampMaxCnt] = {FALSE,FALSE};

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
static void stm_cmd(functionSt_t st);
static void stm_setCntPrescaler(uint8_t prescaler);
static void stm_setCntFrz(functionSt_t st);
static void stm_channelCmd(uint8_t channelIndex,functionSt_t st);
static void stm_setCcr(uint8_t channelIndex,uint32_t cmpValue);
/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
/*
 * Function: stm_init
 * Param< void >:
 * Return< void>:
* Description: initialize the system general timer based on the configuration table which defined in sysGenTimerCfg.c
* Note: the system main loop can be sheduled normally only after this function executed normally
* REQ IDs: 
*/
void stm_init(bool start,uint8_t PreDiv,bool FrzDebug)
{
	int ChannelIndex = 0;
	m_InParaCheck(m_stm_isValiddiv(PreDiv),m_plat_stmModuleId,m_stm_InitFunId, m_plat_stmDivErr);
	stm_setCntPrescaler(PreDiv-1);
	if(TRUE == FrzDebug)
	{
		stm_setCntFrz(em_funSt_on);
	}
	else
	{
		stm_setCntFrz(em_funSt_off);
	}
	if(TRUE == start)
	{
		stm_cmd(em_funSt_on);
	}
	else
	{
		stm_cmd(em_funSt_off);
	}
	for(ChannelIndex = 0;ChannelIndex < m_stmChannelNum; ChannelIndex ++)
	{
		stm_setCcr(ChannelIndex,stmChanCfg[ChannelIndex].CmpValue);
		if(TRUE == stmChanCfg[ChannelIndex].start)
		{
			stm_channelCmd(ChannelIndex,em_funSt_on);
		}
		else
		{
			stm_channelCmd(ChannelIndex,em_funSt_off);
		}
	}
}
/*
 * Function: stm_setStamp
 * Param< void >:
 * Return< void>:
* Description: set the current timer counter as the time stamp
* Note: Only after the time stamp has been set, it can get the conclusion how long time passed.
* REQ IDs: 
*/
void stm_setStamp(uint8_t stampIndx)
{
	if(stampIndx < m_stm_stampMaxCnt)
	{
		timeStamp[stampIndx] = m_stm_FetchCnt;
		timeStampSet[stampIndx] = TRUE;
	}
	
}



/*
 * Function: stm_GetDiff
 * Param< uint32_t * elapseTime >:  the elapsed time since last time stamp
 * Return< bool>:TRUE:elapsed time has been fetched successfully, otherwise no.
* Description: get the time elapsed since last time stamp and stored in the parameter.
* Note:
* REQ IDs: 
*/
bool stm_getDiff(uint8_t stampIndx,uint32_t * elapseTime)
{
 	uint32_t timeDiff = 0;
	uint32_t currTimCnt = 0;
	bool retValue = FALSE;
	if((stampIndx < m_stm_stampMaxCnt) && (TRUE == timeStampSet[stampIndx]))
	{
		currTimCnt = m_stm_FetchCnt;
  		if(currTimCnt >= timeStamp[stampIndx])
		{
			timeDiff = currTimCnt - timeStamp[stampIndx];
		}
		else
		{
			timeDiff = 0xFFFFFFFFul - timeStamp[stampIndx] + currTimCnt + 1;
		}
		*elapseTime = timeDiff;
		retValue = TRUE;
	}
	return retValue;
}

void stm_delayUs(uint32_t us)
{
	uint32_t currCnt = m_stm_FetchCnt;
	uint32_t old = currCnt;
	while(currCnt < (us + old))
	{
		currCnt = m_stm_FetchCnt;
	}
}


void stm_clrStamp(uint8_t stampIndx)
{
	if(stampIndx < m_stm_stampMaxCnt)
	{
		timeStamp[stampIndx] = 0;
		timeStampSet[stampIndx] = FALSE;
	}
}
/*
 * Function: stm_getIsrFlag
 * Param<uint8_t channelIndex>:Channel number,valid range:0~3
 * Return< bool>:TRUE: isr request occurs,otherwise not occur
* Description: get the flag which indicates whetHer the channel ISR request occurs.
* Note:
* REQ IDs:
*/
bool stm_getIsrFlag(uint8_t channelIndex)
{
	bool IsrOccured = FALSE;
	m_InParaCheck(m_stm_isValidTimChannel(channelIndex),3,0,m_plat_stmChannelNoErr);
	if(STM_0.CHANNEL[channelIndex].CIR.B.CIF >0)
	{
		IsrOccured = TRUE;
	}
	return IsrOccured;
}



/*
 * Function: stm_genRandomNumber
 * Param< void>
 * Return< uint16_t>:return the random number based on the timer counter
* Description: generate the random number based on the timer counter
* Note: this function can only work well after the timer has been started normally
* REQ IDs: 
*/
uint32_t stm_genRandomNumber(void)
{
	uint32_t randomNum = 0;
	randomNum = (m_stm_FetchCnt >> 8) * 17;
	return randomNum;
}

/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
/*
 * Function: stm_cmd
 * Param<functionSt_t st >:
 *						em_funSt_off :diable the timer
 *						em_funSt_on:enable the timer
 * Return< void>:
* Description: enable or disable the System general timer
* Note: 
* REQ IDs: 
*/
static void stm_cmd(functionSt_t st)
{
	m_InParaCheck(m_plat_isFunctionState(st),m_plat_stmModuleId,0,m_plat_funStErr);
	if(em_funSt_off == st)
	{
		STM_0.CR.B.TEN = 0u;
	}
	else
	{
		STM_0.CR.B.TEN = 1u;
	}
}

/*
 * Function: stm_setCntPrescaler
 * Param<uint8_t prescaler >: The counter prescaler,a 8-bit symbolic macro will be prefered 
 * Return< void>:
* Description: set the timer counter prescaler
* Note: 
* REQ IDs: 
*/
static void stm_setCntPrescaler(uint8_t prescaler)
{
	STM_0.CR.R = (prescaler << 8);	
}

/*
 * Function: stm_setCntFrz
 * Param<functionSt_t st >: The counter prescaler,a 8-bit symbolic macro will be prefered 
 * Return< void>:
* Description: set the timer counter prescaler
* Note: 
* REQ IDs: 
*/
static void stm_setCntFrz(functionSt_t st)
{
	m_InParaCheck(m_plat_isFunctionState(st),3,0,m_plat_funStErr);
	if(em_funSt_off == st)
	{
		STM_0.CR.B.FRZ = 0u;
	}
	else
	{
		STM_0.CR.B.FRZ = 1u;
	}	
}



/*
 * Function: stm_fetchCnt
 * Param<uint8_t channelIndex>:Channel number,valid range:0~3
 * Param<functionSt_t st>:em_funSt_off :diable the timer cannel
 *					     em_funSt_on :enable the timer cannel
 * Return< void>
* Description: fetch the current timer counter
* Note: 
* REQ IDs: 
*/
static void stm_channelCmd(uint8_t channelIndex,functionSt_t st)
{
	m_InParaCheck(m_stm_isValidTimChannel(channelIndex),3,0,m_plat_stmChannelNoErr);
	m_InParaCheck(m_plat_isFunctionState(st),3,0,m_plat_stmChannelNoErr);
	if(em_funSt_off == st)
	{
		STM_0.CHANNEL[channelIndex].CCR.B.CEN = 0u;
	}
	else
	{
		STM_0.CHANNEL[channelIndex].CCR.B.CEN = 1u;
	}
}


/*
 * Function: stm_setCcr
 * Param<uint8_t channelIndex>:Channel number,valid range:0~3
 * Param<uint32_t cmpValue>:timer channel compare value
 * Return< void>
* Description: set the timer channel compare value which is contained in the second parameter
* Note: 
* REQ IDs: 
*/
static void stm_setCcr(uint8_t channelIndex,uint32_t cmpValue)
{
	m_InParaCheck(m_stm_isValidTimChannel(channelIndex),3,0,m_plat_stmChannelNoErr);
	STM_0.CHANNEL[channelIndex].CMP.R = cmpValue;
}



