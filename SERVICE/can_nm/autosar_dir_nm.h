/*
* Copyright (c) File,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:autosar_dir_nm.h
* Description: Autosar direct network manager   header file.
* REQ IDs:
* History:
* Time:2018/5/4
* User:Devon
*/

/* ---- Source switch on ------------------------------------------------------*/
#ifndef AUTOSAR_DIR_NM_H_
#define AUTOSAR_DIR_NM_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "autosar_dir_nm.h"
#include "autosar_dir_nm_types.h"
#include "autosar_dir_nm_cfg.h"

/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
/* Network mode*/
#define NM_BAT_ON_MODE                                      0x00
#define NM_SLEEP_MODE                                       0x01
#define NM_NORMAL_REPEAT_MODE                               0x02
#define NM_IMME_REPEAT_MODE                                 0x03
#define NM_NORMAL_OPER_MODE                                 0x04
#define NM_READY_SLEEP_MODE                                 0x05
#define NM_PREPARE_SLEEP_MODE                               0x06

/*WAKE UP Source*/
#define AUTOSAR_DIRNM_NONEWU			0x00
#define AUTOSAR_DIRNM_LOCALWU			0x01
#define AUTOSAR_DIRNM_REMOTEWU			0x02

#define NM_TIMER_WAIT_SLEEP		(0u)
#define NM_TIMER_TIMEOUT		(1u)
#define NM_TIMER_REPEAT			(2u)
#define NM_TIMER_NORMAL		    (3u)
#define NM_TIMER_TO_SLEEP		(4u)
#define NM_TIMER_WAKE_OR_SLEEP	(5u)
#define NM_TIER_MTCOMM_DELAY	(6u)

#define NM_TIMER_CNT			(7u)

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef struct
{
	nm_bool_t	   On;
	nm_word_t      Tick;
	void           (*AlarmTask) (nm_byte_t);
}nm_timer_t;

typedef union
{
    nm_byte_t buf[8];
    struct
    {
        nm_byte_t  station_addr                     :8;

        nm_byte_t  resv1                            :3;
        nm_byte_t  local_wake                       :1;
        nm_byte_t  resv2                            :3;
        nm_byte_t  repeat_req                       :1;

        nm_byte_t  resv3                            :7;
        nm_byte_t  repeatSt                			:1;
        
        nm_byte_t  resv4                            :5;
        nm_byte_t  ignst                			:1;
        nm_byte_t  nmwkupst                			:1;
        nm_byte_t  appst                			:1;
        
        nm_byte_t  resv5                            :8;
		
        nm_byte_t  lastmode                            :4;
        nm_byte_t  currmode                            :4;

        nm_byte_t  state                 			:8;

        nm_byte_t  nmsleepst                         :3;
        nm_byte_t  sleepst                         :1;
        nm_byte_t  resv8                            :3;
        nm_byte_t  testflag                         :1;
    }b;

}nm_msg_t;

/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/
extern unsigned char hld_flg_nmBusSleep;
extern nm_bool_t nm_rec_new_msg[AUTOSAR_DIRNM_BUS_CNT];
extern nm_byte_t autosar_dirNm_mode[AUTOSAR_DIRNM_BUS_CNT];
/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/
extern void autosar_dirNm_init(void);
extern void autosar_dirNm_dispose(void);
extern void autosar_dirNm_RepeatMsgReq(nm_byte_t en);
extern void nm_start_timer(nm_byte_t bus,nm_byte_t timer, nm_word_t tick);
extern unsigned char autosar_dirNm_WakeupSource(unsigned char bus);
extern unsigned char autosar_dirNm_currMode(unsigned char bus);
#endif


