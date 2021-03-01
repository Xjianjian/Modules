/*
 * @Description:
 * @version: v1.0
 * @Author: lixiangping
 * @Date: 2019-10-11 10:55:34
 * @LastEditors: lixiangping
 * @LastEditTime: 2019-10-15 12:07:07
 */

/* ---- Sources witch on ------------------------------------------------------*/

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "defines.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "global_var.h"
#include "utils.h"
#include "PwSBC.h"
#include "spi.h"
#include "spiCfg.h"
#include "rv3149c3.h"
#include "hdioCfg.h"
#include "hdio.h"
#include "nvmBlockInfo.h"
#include "dio.h"
#include "exEE25LC256.h"
/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/
#define m_rv3149c3_chipEnPin SIUL_PF0

/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/
const char daysEachMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};


/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/
static rtcFreq_t rv3149c3_freqSet = em_RTC_32Hz;
static uint16_t rv3149c3_timerCountSet = m_rv3149c3_genCounts32Hz(1200);/*unit:s*/

static uint8_t RTC_OffTimeCalFinish = 0;
static uint32_t RTC_OffTime = 0xFFFFFFFE;
uint8_t RTC_startTime[7] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t RTC_diffTime[7] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00};

static regClock_t rv3149c3_dateT;
/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/
static uint8_t rv3149c3_BCD2DEC(uint8_t code_BCD);
static uint32_t rv3149c3_getSeconds(uint8_t BCD_year, uint8_t BCD_month, uint8_t BCD_day, uint8_t BCD_hour, uint8_t BCD_minute, uint8_t BCD_second);
static void rv3149c3_chipEnable(uint8_t en);
static void rv3149c3_pullDt(uint8_t *dt);
static void rv3149c3_pushDt(uint8_t dt, bool samePage);
static void rv3149c3_readCheckByte(uint8_t pageAddr,uint8_t regAddr, uint8_t *reg);
static bool rv3149c3_enableTempeture(void);
static bool rv3149c3_disableAutoReload(void);
static bool rv3149c3_setFreqCount(rtcFreq_t setFreq, uint16_t setCount);
static bool rv3149c3_enableTimerCount(void);
static bool rv3149c3_setHourType(void);
static bool rv3149c3_setWE(void);
void rv3149c3_readStatus(uint8_t *sts);
void rv3149c3_clearStatus(void);
/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
static uint8_t rv3149c3_BCD2DEC(uint8_t code_BCD)
{
	uint8_t code_DEC = 0;
	uint8_t tens = 0;
	uint8_t units = 0;

	units = code_BCD & 0x0F;
	tens = ((code_BCD & 0xF0) >> 4) & 0x0F;

	code_DEC = tens*10 + units;

	return code_DEC;
}

static uint8_t rv3149c3_DEC2BCD(uint8_t decDt)
{
	uint8_t code_BCD = 0;

	code_BCD = (((decDt/10)<<4) + (decDt%10));

	return code_BCD;
}

static uint32_t rv3149c3_getSeconds(uint8_t BCD_year, uint8_t BCD_month, uint8_t BCD_day, uint8_t BCD_hour, uint8_t BCD_minute, uint8_t BCD_second)
{
	uint8_t DEC_year = 0;
	uint8_t DEC_month = 0;
	uint8_t DEC_day = 0;
	uint8_t DEC_hour = 0;
	uint8_t DEC_minute = 0;
	uint8_t DEC_second = 0;

	uint16_t sum_days = 0;
	uint32_t sum_seconds = 0;
	uint8_t cnt;

	DEC_year = rv3149c3_BCD2DEC(BCD_year);
	DEC_month = rv3149c3_BCD2DEC(BCD_month);
	DEC_day = rv3149c3_BCD2DEC(BCD_day);
	DEC_hour = rv3149c3_BCD2DEC(BCD_hour);
	DEC_minute = rv3149c3_BCD2DEC(BCD_minute);
	DEC_second = rv3149c3_BCD2DEC(BCD_second);

	/*calculate each month's days*/
	if(1 < DEC_month)
	{
		for(cnt = 0; cnt < DEC_month-1; cnt++)
		{
			sum_days += daysEachMonth[cnt];
		}
	}
	else
	{}

	/*calculate days*/
	if(1 < DEC_day)
	{
		sum_days += DEC_day-1;
	}
	else
	{}


	/*in addition day，2.29*/
	if((0 == DEC_year % 4) && (2 < DEC_month))
	{
		sum_days += 1;
	}
	else
	{}

	sum_seconds = (sum_days * 24 * 60 * 60) + (DEC_hour * 60*60) + (DEC_minute * 60) + DEC_second;

	return sum_seconds;
}

static uint32_t rv3149c3_BCDToDateTime(uint8_t *srcT,regClock_t* desT)
{
	desT->years = rv3149c3_BCD2DEC(srcT[6]);
	desT->months = rv3149c3_BCD2DEC(srcT[5]);
	desT->weekdays = rv3149c3_BCD2DEC(srcT[4]);
	desT->days = rv3149c3_BCD2DEC(srcT[3]);
	desT->hours = rv3149c3_BCD2DEC(srcT[2]);
	desT->minutes = rv3149c3_BCD2DEC(srcT[1]);
	desT->seconds = rv3149c3_BCD2DEC(srcT[0]);
	return 0;
}


static void rv3149c3_chipEnable(uint8_t en)
{
	dio_writeVal(m_rv3149c3_chipEnPin,en,FALSE);
}

/**
 * @name: 
 * @test: test font
 * @msg: 
 * @param {type} 
 * @return: 
 */
void rv3149c3_init(void)
{
	uint8_t cnt = 0;
	uint32_t diffSeconds = 0;
	uint32_t lastOffSeconds = 0;
	uint32_t currentStartSeconds = 0;
	uint8_t DEC_lastYear = 0;
	uint8_t DEC_currentYear = 0;

	rv3149c3_unInitTimer();
#if m_rtc_wkup
	rv3149c3_enableTempeture();
	rv3149c3_setFreqCount(rv3149c3_freqSet,rv3149c3_timerCountSet);
	rv3149c3_enableTimerCount();
#endif
	rv3149c3_setWE();
	rv3149c3_readWatchClock(RTC_startTime);

	/*12 Hours Type*/
	if((TRUE == (rtc_currentHour & 0x40)) && (TRUE == (rtc_lastHour & 0x40)))
	{
		/*PM*/
		if(TRUE == (rtc_currentHour & 0x20))
		{
			rtc_currentHour = (rtc_currentHour & 0x1F) + 0x12 ;
		}
		else
		{}

		if(TRUE == (rtc_lastHour & 0x20))
		{
			rtc_lastHour = (rtc_lastHour & 0x1F) + 0x12;
		}
		else
		{}
	}
	lastOffSeconds = rv3149c3_getSeconds(rtc_lastYear,rtc_lastMonth,rtc_lastDay,rtc_lastHour,rtc_lastMinute,rtc_lastSecond);
	currentStartSeconds = rv3149c3_getSeconds(rtc_currentYear,rtc_currentMonth,rtc_currentDay,rtc_currentHour,rtc_currentMinute,rtc_currentSecond);

	DEC_lastYear = rv3149c3_BCD2DEC(rtc_lastYear);
	DEC_currentYear = rv3149c3_BCD2DEC(rtc_currentYear);

	if((DEC_currentYear < DEC_lastYear) || (currentStartSeconds < lastOffSeconds))
	{
		RTC_OffTimeCalFinish = FALSE;
	}
	else
	{
		/*diffTime = (currentYear - lastYear) * period - lastTime + currentTime*/
		diffSeconds = (DEC_currentYear - DEC_lastYear) * secondsPerYear  + currentStartSeconds - lastOffSeconds;

		/*判断中间的年份是否有闰年 leap years ，add 1 day*/
		for(cnt = 1; cnt < (DEC_currentYear - DEC_lastYear); cnt++)
		{
			if(0 == (DEC_lastYear + cnt) % 4)
			{
				diffSeconds += 24*60*60;
			}
			else
			{}
		}

		RTC_OffTime = diffSeconds;
		RTC_OffTimeCalFinish = TRUE;
	}
}


static void rv3149c3_pushDt(uint8_t dt, bool samePage)
{
	spi_sendDt(m_spi_module3,m_spi3_rv3149c3Ctas,m_spi3_rv3149c3CsMask,dt,samePage,TRUE);
}

static void rv3149c3_pullDt(uint8_t *dt)
{
	spi_readDtBtye(m_spi_module3,dt);
}


void rv3149c3_sendData(uint8_t cmd, uint8_t *sendData, uint8_t length)
{
	uint8_t cyclCnt = 0;

	rv3149c3_chipEnable(TRUE);
	rv3149c3_pushDt(cmd,TRUE);

	for(cyclCnt = 0; cyclCnt < length; cyclCnt++)
	{
		if((length - 1 )== cyclCnt)
		{
			rv3149c3_pushDt(sendData[cyclCnt],FALSE);
		}
		else
		{
			rv3149c3_pushDt(sendData[cyclCnt],TRUE);
		}
	}
	rv3149c3_chipEnable(FALSE);
}


void rv3149c3_recvData(uint8_t cmd, uint8_t *sendData, uint8_t *recvData, uint8_t length)
{
	uint8_t cyclCnt = 0;

	rv3149c3_chipEnable(TRUE);
	rv3149c3_pushDt(cmd,TRUE);

	for(cyclCnt = 0; cyclCnt < length; cyclCnt++)
	{
		if((length - 1 ) == cyclCnt)
		{
			rv3149c3_pushDt(sendData[cyclCnt],FALSE);
		}
		else
		{
			rv3149c3_pushDt(sendData[cyclCnt],TRUE);
		}
		rv3149c3_pullDt(recvData+cyclCnt);
	}

	rv3149c3_chipEnable(FALSE);
}


static void rv3149c3_readCheckByte(uint8_t pageAddr,uint8_t regAddr, uint8_t *reg)
{
	cmd_t cmd;
	cmd.R = 0;
	uint8_t reg_tmp = 0;

	cmd.B.execute = m_rv3149c3_read;
	cmd.B.page_addr = pageAddr;
	cmd.B.reg_addr = regAddr;

	rv3149c3_chipEnable(TRUE);

	rv3149c3_pushDt(cmd.R,TRUE);
	rv3149c3_pushDt(0xAA,FALSE);/*to produce clock for RTC output*/
	rv3149c3_pullDt(&reg_tmp);

	rv3149c3_chipEnable(FALSE);

	*reg = reg_tmp;
}


static bool rv3149c3_enableTempeture(void)
{
	bool ret_value = FALSE;

	cmd_t cmd;
	eeCtrlReg_t	eectrlReg_3149Send;
	eeCtrlReg_t	eectrlReg_3149Recv;
	cmd.R = 0x00;
	eectrlReg_3149Send.R = 0;
	eectrlReg_3149Recv.R = 0;

	cmd.B.execute = m_rv3149c3_write;
	cmd.B.page_addr = m_rv3149c3_eepromCtrlPage;
	cmd.B.reg_addr = m_rv3149c3_eepromCtrlReg;
	eectrlReg_3149Send.B.ThP = 0;
	eectrlReg_3149Send.B.ThE = 1;
	rv3149c3_sendData(cmd.R,(uint8_t*)&eectrlReg_3149Send.R,1);

	/*Need to re-read to confirm last writing action is correct*/
	rv3149c3_readCheckByte(m_rv3149c3_eepromCtrlPage,m_rv3149c3_eepromCtrlReg,(uint8_t*)&eectrlReg_3149Recv);
	if(0x0 == eectrlReg_3149Recv.B.ThP && 0x1 == eectrlReg_3149Recv.B.ThE)
	{
		ret_value = TRUE;
	}
	else
	{
		ret_value = FALSE;
	}
	return ret_value;
}

static bool rv3149c3_disableAutoReload(void)
{
	bool ret_value = FALSE;
	cmd_t cmd;
	contrl1_t ctrl1Send;
	contrl1_t ctrl1Recv;
	cmd.R = 0x00;
	ctrl1Send.R = 0x00;
	ctrl1Recv.R = 0x00;

	/*Disables Countdown Timer auto-reload mode,Disables Countdown Timer*/
	cmd.B.execute = m_rv3149c3_write;
	cmd.B.page_addr = m_rv3149c3_controlPage;
	cmd.B.reg_addr = m_rv3149c3_control1Reg;
	ctrl1Send.B.TAR = 0x0;
	ctrl1Send.B.TE = 0x0;
	rv3149c3_sendData(cmd.R,(uint8_t*)&ctrl1Send.R,1);

	/*Need to re-read to confirm last writing action is correct*/
	rv3149c3_readCheckByte(m_rv3149c3_controlPage,m_rv3149c3_control1Reg,(uint8_t*)&ctrl1Recv);
	if(0x0 == ctrl1Recv.B.TAR && 0x0 == ctrl1Recv.B.TE)
	{
		ret_value = TRUE;
	}
	else
	{
		ret_value = FALSE;
	}
	return ret_value;
}

static bool rv3149c3_setWE(void)
{
	bool ret_value = FALSE;
	cmd_t cmd;
	contrl1_t ctrl1Send;
	contrl1_t ctrl1Recv;
	cmd.R = 0x00;

	/* Enables 1Hz Clock Source for Watch */
	ctrl1Send.B.WE = 0x01;

	cmd.B.execute = m_rv3149c3_write;
	cmd.B.page_addr = m_rv3149c3_controlPage;
	cmd.B.reg_addr = m_rv3149c3_control1Reg;
	rv3149c3_sendData(cmd.R,(uint8_t*)&ctrl1Send.R,1);

	/*Need to re-read to confirm last writing action is correct*/
	rv3149c3_readCheckByte(m_rv3149c3_controlPage,m_rv3149c3_control1Reg,(uint8_t*)&ctrl1Recv);
	if(0x01 == ctrl1Recv.B.WE)
	{
		ret_value = TRUE;
	}
	else
	{}

	return ret_value;
}


static bool rv3149c3_setFreqCount(rtcFreq_t setFreq, uint16_t setCount)
{
	bool ret_value = FALSE;
	cmd_t cmd;
	contrl1_t ctrl1Send;
	contrl1_t ctrl1Recv;
	timerPage_t timer_3149Send;
	timerPage_t timer_3149Recv;
	uint8_t sendData[2] = {0x00,0x00};

	uint8_t currentFreq = 0x00;
	uint16_t currentCount = 0x00;
	cmd.R = 0x00;
	ctrl1Send.R = 0x00;
	ctrl1Recv.R = 0x00;
	timer_3149Send.R = 0x00;
	timer_3149Recv.R = 0x00;

	/* Select Source Clock for internal Countdown Timer */
	if(TRUE == rv3149c3_disableAutoReload())
	{
		switch(setFreq)
		{
			case em_RTC_32Hz:
				ctrl1Send.B.TD0 = 0x0;
				ctrl1Send.B.TD1 = 0x0;
				break;
			case em_RTC_8Hz:
				ctrl1Send.B.TD0 = 0x1;
				ctrl1Send.B.TD1 = 0x0;
				break;
			case em_RTC_1Hz:
				ctrl1Send.B.TD0 = 0x0;
				ctrl1Send.B.TD1 = 0x1;
				break;
			case em_RTC_halfHz:
				ctrl1Send.B.TD0 = 0x1;
				ctrl1Send.B.TD1 = 0x1;
				break;
			default:
				ret_value = FALSE;
				break;
		}
		/*set RTC Countdown Timer frequency*/
		cmd.B.execute = m_rv3149c3_write;
		cmd.B.page_addr = m_rv3149c3_controlPage;
		cmd.B.reg_addr = m_rv3149c3_control1Reg;
		rv3149c3_sendData(cmd.R,(uint8_t*)&ctrl1Send.R,1);

		/*Need to re-read to confirm last writing action is correct*/
		rv3149c3_readCheckByte(m_rv3149c3_controlPage,m_rv3149c3_control1Reg,(uint8_t*)&ctrl1Recv);
		currentFreq = ctrl1Recv.B.TD1;
		currentFreq = currentFreq << 1;
		currentFreq |= ctrl1Recv.B.TD0;
		if(setFreq == (rtcFreq_t)currentFreq)
		{
			/*set RTC Countdown Timer count*/
			cmd.R = 0;
			cmd.B.execute = m_rv3149c3_write;
			cmd.B.page_addr = m_rv3149c3_timerPage;
			cmd.B.reg_addr = m_rv3149c3_timerLowReg;

			timer_3149Send.B.timerLow  = (uint8_t)(setCount & 0x00FF);
			timer_3149Send.B.timerHigh = (uint8_t)((setCount >> 8) & 0x00FF);
			sendData[0] = (uint8_t)(setCount & 0x00FF);
			sendData[1] = (uint8_t)((setCount >> 8) & 0x00FF);
			rv3149c3_sendData(cmd.R,sendData,2);

			/*Need to re-read to confirm last writing action is correct*/
			rv3149c3_readCheckByte(m_rv3149c3_timerPage,m_rv3149c3_timerLowReg,(uint8_t*)&timer_3149Recv.B.timerLow);
			rv3149c3_readCheckByte(m_rv3149c3_timerPage,m_rv3149c3_timerHighReg,(uint8_t*)&timer_3149Recv.B.timerHigh);
			currentCount = timer_3149Recv.B.timerHigh;
			currentCount = currentCount << 8;
			currentCount |= timer_3149Recv.B.timerLow;
			if(setCount == (rtcFreq_t)currentCount)
			{
				ret_value = TRUE;
			}
			else
			{
				ret_value = FALSE;
			}
		}
		else
		{
			ret_value = FALSE;
		}
	}
	else
	{
		ret_value = FALSE;
	}

	return ret_value;
}

static bool rv3149c3_enableTimerCount(void)
{
	bool ret_value = FALSE;
	cmd_t cmd;
	contrl1_t ctrl1;
	contrlInit_t ctrlInit;
	contrlIntFlag_t ctrlInitFlg;
	contrl1_t ctrl1Recv;
	contrlInit_t ctrlInitRecv;
	contrlIntFlag_t ctrlInitFlgRecv;

	cmd.R = 0x00;
	ctrl1.R = 0x00;
	ctrlInit.R = 0x00;
	ctrlInitFlg.R = 0x00;

	ctrl1Recv.R = 0x00;
	ctrlInitRecv.R = 0x00;
	ctrlInitFlgRecv.R = 0x00;

	/*Enables Countdown Timer INT*/
	cmd.B.execute = m_rv3149c3_write;
	cmd.B.page_addr = m_rv3149c3_controlPage;
	cmd.B.reg_addr = m_rv3149c3_controlInitReg;
	ctrlInit.B.TIE = 0x1;
	rv3149c3_sendData(cmd.R,(uint8_t*)&ctrlInit.R,1);

	/*Enables Countdown Timer auto-reload mode,Enables Countdown Timer*/
	cmd.B.reg_addr = m_rv3149c3_control1Reg;
	ctrl1.R = 0;
	ctrl1.B.TAR = 0x1;
	ctrl1.B.TE = 0x1;
	rv3149c3_sendData(cmd.R,(uint8_t*)&ctrl1.R,1);

	/*clear INT flag*/
	cmd.B.reg_addr = m_rv3149c3_controlInitFlgReg;
	ctrlInitFlg.R = 0x0;
	ctrlInitFlg.B.TF = 0x0;
	rv3149c3_sendData(cmd.R,(uint8_t*)&ctrlInitFlg.R,1);

	/*Need to re-read to confirm last writing action is correct*/
	rv3149c3_readCheckByte(m_rv3149c3_controlPage,m_rv3149c3_control1Reg,(uint8_t*)&ctrl1Recv);
	rv3149c3_readCheckByte(m_rv3149c3_controlPage,m_rv3149c3_controlInitReg,(uint8_t*)&ctrlInitRecv);
	rv3149c3_readCheckByte(m_rv3149c3_controlPage,m_rv3149c3_controlInitFlgReg,(uint8_t*)&ctrlInitFlgRecv);

	if(0x1 == ctrlInitRecv.B.TIE &&
	   0x1 == ctrl1Recv.B.TAR && 0x1 == ctrl1Recv.B.TE &&
	   0x0 ==ctrlInitFlgRecv.B.TF)
	{
		   ret_value = TRUE;
	}
	else
	{
		ret_value = FALSE;
	}

	return ret_value;
}

static bool rv3149c3_setHourType(void)
{
	bool ret_value = FALSE;
	cmd_t cmd;
	uint8_t hourTypeSend = m_rv3149c3_24HourType;
	uint8_t hourTypeRecv = 0x00;

	cmd.B.execute = m_rv3149c3_write;
	cmd.B.page_addr = m_rv3149c3_clockPage;
	cmd.B.reg_addr = m_rv3149c3_hoursReg;

	rv3149c3_sendData(cmd.R,&hourTypeSend,1);

	/*Need to re-read to confirm last writing action is correct*/
	rv3149c3_readCheckByte(m_rv3149c3_clockPage,m_rv3149c3_hoursReg,(uint8_t*)&hourTypeRecv);

	if(hourTypeSend == hourTypeRecv)
	{
		ret_value = TRUE;
	}
	else
	{}

	return ret_value;
}

void rv3149c3_set_rtcTime(regClock_t *dateT)
{
	regClock_t wr_clockRegSet;
	cmd_t cmd;

	wr_clockRegSet.years = rv3149c3_DEC2BCD(dateT->years);
	wr_clockRegSet.months = rv3149c3_DEC2BCD(dateT->months);
	wr_clockRegSet.weekdays = rv3149c3_DEC2BCD(dateT->weekdays);
	wr_clockRegSet.days = rv3149c3_DEC2BCD(dateT->days);
	wr_clockRegSet.hours = rv3149c3_DEC2BCD(dateT->hours);
	wr_clockRegSet.minutes = rv3149c3_DEC2BCD(dateT->minutes);
	wr_clockRegSet.seconds = rv3149c3_DEC2BCD(dateT->seconds);

	cmd.B.execute = m_rv3149c3_write;
	cmd.B.page_addr = m_rv3149c3_clockPage;
	cmd.B.reg_addr = m_rv3149c3_secondsReg;

	rv3149c3_sendData(cmd.R,&wr_clockRegSet.seconds,7);
}

void rv3149c3_get_rtcTime(regClock_t *dateT)
{
	uint8_t clockReg2Send[7] = {0,0,0,0,0,0,0};
	uint8_t clockReg2Recv[7] = {0,0,0,0,0,0,0};

	cmd_t cmd;
	cmd.B.execute = m_rv3149c3_read;
	cmd.B.page_addr = m_rv3149c3_clockPage;
	cmd.B.reg_addr = m_rv3149c3_secondsReg;

	rv3149c3_recvData(cmd.R,clockReg2Send,clockReg2Recv,7);

	dateT->years = rv3149c3_BCD2DEC(clockReg2Recv[6]);
	dateT->months = rv3149c3_BCD2DEC(clockReg2Recv[5]);
	dateT->weekdays = rv3149c3_BCD2DEC(clockReg2Recv[4]);
	dateT->days = rv3149c3_BCD2DEC(clockReg2Recv[3]);
	dateT->hours = rv3149c3_BCD2DEC(clockReg2Recv[2]);
	dateT->minutes = rv3149c3_BCD2DEC(clockReg2Recv[1]);
	dateT->seconds = rv3149c3_BCD2DEC(clockReg2Recv[0]);
}

void rv3149c3_readWatchClock(uint8_t *timeRecv)
{
	uint8_t read_clockReg2Send[7] = {0,0,0,0,0,0,0};
	uint8_t read_clockReg2Recv[7] = {0,0,0,0,0,0,0};
	
	cmd_t cmd;
	cmd.B.execute = m_rv3149c3_read;
	cmd.B.page_addr = m_rv3149c3_clockPage;
	cmd.B.reg_addr = m_rv3149c3_secondsReg;

	rv3149c3_recvData(cmd.R,read_clockReg2Send,read_clockReg2Recv,7);

	timeRecv[0] = read_clockReg2Recv[0];
	timeRecv[1] = read_clockReg2Recv[1];
	timeRecv[2] = read_clockReg2Recv[2];
	timeRecv[3] = read_clockReg2Recv[3];
	timeRecv[4] = read_clockReg2Recv[4];
	timeRecv[5] = read_clockReg2Recv[5];
	timeRecv[6] = read_clockReg2Recv[6];
}

void rv3149c3_cleanINT(void)
{
	cmd_t cmd ;
	contrlIntFlag_t ctrlInitFlg;
	contrlIntFlag_t ctrlInitFlgRecv;

	cmd.R = 0;
	ctrlInitFlg.R = 0;
	rv3149c3_readCheckByte(m_rv3149c3_controlPage,m_rv3149c3_controlInitFlgReg,(uint8_t*)&ctrlInitFlgRecv);
	/*the Timer Flag “TF” will be set = “1” at the end of every Timer countdown*/
	if(1 == ctrlInitFlgRecv.B.TF)
	{
		/*INT cleared by software writing “TF” = “0”*/
		cmd.B.execute = m_rv3149c3_write;
		cmd.B.page_addr = m_rv3149c3_controlPage;
		cmd.B.reg_addr = m_rv3149c3_controlInitFlgReg;
		ctrlInitFlg.B.TF = 0;
		rv3149c3_sendData(cmd.R, (uint8_t*)&ctrlInitFlg.R, 1);
	}
	else
	{
		/*keep waiting*/
	}

}

void rv3149c3_unInitTimer(void)
{
	cmd_t cmd;
	uint8_t sendData[4] = {0x00,0x00,0x00,0x00};

	cmd.R = 0x00;
	cmd.B.execute = m_rv3149c3_write;
	cmd.B.page_addr = m_rv3149c3_controlPage;
	cmd.B.reg_addr = m_rv3149c3_control1Reg;
	rv3149c3_sendData(cmd.R,sendData,4);
}


void rv3149c3_readStatus(uint8_t *sts)
{
	cmd_t cmd;
	uint8_t read_stsReg2Send = 0;

	cmd.B.execute = m_rv3149c3_read;
	cmd.B.page_addr = m_rv3149c3_controlPage;
	cmd.B.reg_addr = m_rv3149c3_controlStsReg;
	rv3149c3_recvData(cmd.R,&read_stsReg2Send,sts,1);
}

void rv3149c3_clearStatus(void)
{
	cmd_t cmd;
	uint8_t read_stsReg2Send = 0;
	uint8_t ctrl_sts = 0;

	cmd.B.execute = m_rv3149c3_write;
	cmd.B.page_addr = m_rv3149c3_controlPage;
	cmd.B.reg_addr = m_rv3149c3_controlStsReg;

	rv3149c3_readStatus(&ctrl_sts);
	if(0 != (ctrl_sts & (m_rv3149c3_PON | m_rv3149c3_SR | m_rv3149c3_V2F | m_rv3149c3_V1F)))
	{
		rv3149c3_sendData(cmd.R,&read_stsReg2Send,1);
	}
	else
	{
		/*NO ERROR*/
	}
}

void rtc_test(void)
{
	static uint8_t rtc_test_step = 0;
	regClock_t dateT;

	switch(rtc_test_step)
	{
		case 0:
			dateT.years = 20;
			dateT.months = 8;
			dateT.weekdays = 1;
			dateT.days = 24;
			dateT.hours = 12;
			dateT.minutes = 2;
			dateT.seconds = 0;

			rv3149c3_set_rtcTime(&dateT);
			rtc_test_step = 1;
		break;
		case 1:
			rv3149c3_get_rtcTime(&rv3149c3_dateT);
		break;
		default:
		break;
	}
}

void rt_get_time(uint8_t *time)
{
	time[0] = rv3149c3_dateT.years;
	time[1] = rv3149c3_dateT.months;
	time[2] = rv3149c3_dateT.days;
	time[3] = rv3149c3_dateT.hours;
	time[4] = rv3149c3_dateT.minutes;
	time[5] = rv3149c3_dateT.seconds;
	time[6] = rv3149c3_dateT.weekdays;
}

/*--- End of file ------------------------------------------------------------*/


