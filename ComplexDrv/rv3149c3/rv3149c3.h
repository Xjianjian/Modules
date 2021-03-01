/*
 * @Descripttion: 
 * @version: v1.0
 * @Author: lixiangping
 * @Date: 2019-10-11 10:55:34
 * @LastEditors: lixiangping
 * @LastEditTime: 2019-10-15 11:56:56
 */
/* ---- Source switch on ------------------------------------------------------*/
#ifndef RV3149C3_H_
#define RV3149C3_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_rv3149c3_read     (0x1)
#define m_rv3149c3_write    (0x0)

/*page addr*/
#define m_rv3149c3_controlPage 		(0x00)
#define m_rv3149c3_clockPage 		(0x01)
#define m_rv3149c3_alramPage 		(0x02)
#define m_rv3149c3_timerPage 		(0x03)
#define m_rv3149c3_temperaturePage 	(0x04)
#define m_rv3149c3_eepromPage 		(0x05)
#define m_rv3149c3_eepromCtrlPage 	(0x06)
#define m_rv3149c3_ramPage 			(0x07)

/*control page register addr*/
#define m_rv3149c3_control1Reg              (0x0)
#define m_rv3149c3_controlInitReg           (0x1)
#define m_rv3149c3_controlInitFlgReg        (0x2)
#define m_rv3149c3_controlStsReg            (0x3)
#define m_rv3149c3_controlResetReg          (0x4)

/*clock page register addr*/
#define m_rv3149c3_secondsReg               (0x0)
#define m_rv3149c3_minutesReg               (0x1)
#define m_rv3149c3_hoursReg                 (0x2)
#define m_rv3149c3_daysReg                  (0x3)
#define m_rv3149c3_weekdaysReg              (0x4)
#define m_rv3149c3_monthsReg                (0x5)
#define m_rv3149c3_yearsReg                 (0x6)

/*alarm page register addr*/
#define m_rv3149c3_secondsAlarmReg          (0x0)
#define m_rv3149c3_minutesAlarmReg          (0x1)
#define m_rv3149c3_hoursAlarmReg            (0x2)
#define m_rv3149c3_daysAlarmReg             (0x3)
#define m_rv3149c3_weekdaysAlarmReg         (0x4)
#define m_rv3149c3_monthsAlarmReg           (0x5)
#define m_rv3149c3_yearsAlarmReg            (0x6)

/*timer page register addr*/
#define m_rv3149c3_timerLowReg              (0x0)
#define m_rv3149c3_timerHighReg             (0x1)

/*temperature page register addr*/
#define m_rv3149c3_temperatureReg           (0x0)

/*eeprom user page register addr*/
#define m_rv3149c3_eepromUserLowReg         (0x0)
#define m_rv3149c3_eepromUserHighReg        (0x1)

/*eeprom control page register addr*/
#define m_rv3149c3_eepromCtrlReg            (0x0)
#define m_rv3149c3_xtalOffesetReg           (0x1)
#define m_rv3149c3_xtalCoefReg              (0x2)
#define m_rv3149c3_xtalT0Reg                (0x3)

#define m_rv3149c3_genCounts32Hz(x)		(uint16_t)(x*32)
#define m_rv3149c3_genCounts8Hz(x)		(uint16_t)(x*8)
#define m_rv3149c3_genCounts1Hz(x)		(uint16_t)(x*1)
#define m_rv3149c3_genCountsHalfHz(x)	(uint16_t)(x/2)

#define m_rv3149c3_24HourType	(0x00)
#define m_rv3149c3_12HourType	(0x40)

#define m_rtc_wkup	(0)

#define rtc_currentYear 	(RTC_startTime[6])
#define rtc_currentMonth 	(RTC_startTime[5])
#define rtc_currentDay 		(RTC_startTime[3])
#define rtc_currentHour 	(RTC_startTime[2])
#define rtc_currentMinute 	(RTC_startTime[1])
#define rtc_currentSecond 	(RTC_startTime[0])


#define rtc_lastYear 		(RTC_OffTimeLast[6])
#define rtc_lastMonth 		(RTC_OffTimeLast[5])
#define rtc_lastDay 		(RTC_OffTimeLast[3])
#define rtc_lastHour 		(RTC_OffTimeLast[2])
#define rtc_lastMinute 		(RTC_OffTimeLast[1])
#define rtc_lastSecond 		(RTC_OffTimeLast[0])

/*一年的秒数*/
#define secondsPerYear (365*24*60*60)

#define m_rv3149c3_spiCommTryCnt (3u)

/*control status*/
#define m_rv3149c3_PON	(0x20)
#define m_rv3149c3_SR	(0x10)
#define m_rv3149c3_V2F	(0x08)
#define m_rv3149c3_V1F	(0x04)
/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef enum{
	em_RTC_32Hz = 0,
	em_RTC_8Hz,
	em_RTC_1Hz,
	em_RTC_halfHz
}rtcFreq_t;


typedef union { ///CMD
    uint8_t R;
    struct {
        uint8_t execute	    :1;
        uint8_t page_addr	:4;
        uint8_t reg_addr	:3;
    } B;
}cmd_t;

typedef union { 
    uint8_t R;
    struct {
        uint8_t ClkInt	:1;
        uint8_t TD1	    :1;
        uint8_t TD0	    :1;
        uint8_t SROn	:1;
        uint8_t EERE	:1;
        uint8_t TAR	    :1;
        uint8_t TE	    :1;
        uint8_t WE	    :1;
    } B;
}contrl1_t;

typedef union { 
    uint8_t R;
    struct {
        uint8_t reserved	:3;
        uint8_t SRIE	    :1;
        uint8_t V2IE	    :1;
        uint8_t V1IE	    :1;
        uint8_t TIE	        :1;
        uint8_t AIE	        :1;
    } B;
}contrlInit_t;

typedef union { 
    uint8_t R;
    struct {
        uint8_t reserved	:3;
        uint8_t SRF	        :1;
        uint8_t V2IF	    :1;
        uint8_t V1IF	    :1;
        uint8_t TF	        :1;
        uint8_t AF	        :1;
    } B;
}contrlIntFlag_t;

typedef union { 
    uint8_t R;
    struct {
        uint8_t EEbusy      :1;
        uint8_t reserved1	:1;
        uint8_t PON	        :1;
        uint8_t SR	        :1;
        uint8_t V2F	        :1;
        uint8_t V1F	        :1;
        uint8_t reserved2	:2;
    } B;
}contrlSts_t;

typedef union { 
    uint8_t R;
    struct {
        uint8_t reserved1	:3;
        uint8_t SysR	    :1;
        uint8_t reserved2	:4;
    } B;
}contrlReset_t;

typedef struct
{
	uint8_t 		seconds;
	uint8_t 		minutes;
	uint8_t 		hours;
	uint8_t 		days;
	uint8_t 	   	weekdays;
	uint8_t 	   	months;
	uint8_t 	   	years;	
}regClock_t;

typedef union { 
    uint16_t R;
    struct {
        uint8_t timerLow;
        uint8_t timerHigh;
    } B;
}timerPage_t;


typedef union {
	uint8_t R;
	struct {
		uint8_t r80k	:1;
		uint8_t r20k	:1;
		uint8_t r5k		:1;
		uint8_t r1l		:1;
		uint8_t FD1		:1;
		uint8_t FD0		:1;
		uint8_t ThE		:1;
		uint8_t ThP		:1;
	}B;
}eeCtrlReg_t;

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
extern void rv3149c3_init(void);
extern void rv3149c3_unInitTimer(void);
extern void rv3149c3_cleanINT(void);
extern void rv3149c3_set_rtcTime(regClock_t *dateT);
extern void rv3149c3_get_rtcTime(regClock_t *dateT);
extern void rv3149c3_readWatchClock(uint8_t*timeRecv);
extern void rv3149c3_clearStatus(void);
extern void rt_get_time(uint8_t *time);
#endif


