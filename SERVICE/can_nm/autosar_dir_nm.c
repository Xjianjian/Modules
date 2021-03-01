

/*
* Copyright (c) File ,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:autosar_dir_nm.c
* Description: autosar direct protocal implementaion files
* REQ IDs:
* History:
* Time:2018/05/04
* User:Devon
*/

/* ---- Source switch on ------------------------------------------------------*/

#define AUTOSAR_DIR_NM_C_
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "autosar_dir_nm_types.h"
#include "autosar_dir_nm_cfg.h"
#include "autosar_dir_nm_if.h"
#include "autosar_dir_nm.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/
static nm_byte_t autosar_dirNm_imme_remain_times[AUTOSAR_DIRNM_BUS_CNT];
static nm_byte_t autosar_dirNm_wait_sleep[AUTOSAR_DIRNM_BUS_CNT];

static nm_timer_t nm_timer_grp[AUTOSAR_DIRNM_BUS_CNT][NM_TIMER_CNT] = 
{
	{
		{FALSE,0,NULL_PTR},{FALSE,0,NULL_PTR},{FALSE,0,NULL_PTR},{FALSE,0,NULL_PTR},
		{FALSE,0,NULL_PTR},{FALSE,0,NULL_PTR},{FALSE,0,NULL_PTR}
	}
};
#if TEST_MODE_FORTEMP
static nm_msg_t nm_send_testmsg[AUTOSAR_DIRNM_BUS_CNT];
#endif
static nm_msg_t nm_send_msg[AUTOSAR_DIRNM_BUS_CNT];
static nm_msg_t nm_rec_msg[AUTOSAR_DIRNM_BUS_CNT];
//static nm_bool_t nm_can_init_fail[AUTOSAR_DIRNM_BUS_CNT] = {FALSE};
static char nm_repeatMsgReqFlg = 0;
/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/
static unsigned char nm_platform_ready_sleep = 0;
unsigned char hld_flg_nmBusSleep = FALSE;
nm_bool_t nm_rec_new_msg[AUTOSAR_DIRNM_BUS_CNT] = {FALSE};
nm_byte_t nm_wkup_source_can[AUTOSAR_DIRNM_BUS_CNT] = {0};

nm_byte_t autosar_dirNm_mode[AUTOSAR_DIRNM_BUS_CNT];
static nm_bool_t nm_recv_realTimeFlg[AUTOSAR_DIRNM_BUS_CNT] = {FALSE};//????



/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Local data at EEPROM
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
//void nm_start_timer(nm_byte_t bus,nm_byte_t timer, nm_word_t tick);
static void nm_stop_timer(nm_byte_t bus,nm_byte_t timer);
static void nm_timer_install_process(nm_byte_t bus_i,nm_byte_t timer,void (*p_fun)(nm_byte_t));
static void nm_timer_task(void);
static void nm_mode_enter(nm_byte_t bus,nm_byte_t mode);
static void nm_timer_nm_timeout_expired(nm_byte_t bus_i);
static void nm_timer_normal_expired(nm_byte_t bus_i);

/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void autosar_dirNm_init(void)
{
	nm_byte_t bus_i = 0;
	nm_byte_t byte_i = 0;
	nm_platform_ready_sleep = 0;
    for(bus_i = 0; bus_i < AUTOSAR_DIRNM_BUS_CNT; bus_i ++)
    {
		autosar_dirNm_wait_sleep[bus_i] = FALSE;
		nm_timer_install_process(bus_i,NM_TIMER_TIMEOUT,nm_timer_nm_timeout_expired);
		nm_timer_install_process(bus_i,NM_TIMER_NORMAL,nm_timer_normal_expired);
		for(byte_i =0; byte_i < 8; byte_i ++)
		{
			nm_send_msg[bus_i].buf[byte_i] = 0;
			nm_rec_msg[bus_i].buf[byte_i] = 0;
		}
		nm_send_msg[bus_i].buf[0] = NM_STATION_ADDR;

		hld_flg_nmBusSleep = FALSE;
		nm_rec_new_msg[bus_i] = FALSE;
		autosar_dirNm_setTxRxMode(bus_i,1);
		autosar_dirNm_mode[bus_i] = NM_SLEEP_MODE;
		nm_mode_enter(bus_i,NM_SLEEP_MODE);
    }
}



void autosar_dirNm_dispose(void)
{
	nm_byte_t bus_i = 0;
	nm_byte_t nmMode;
	nm_bool_t local_wu[AUTOSAR_DIRNM_BUS_CNT] = {FALSE};

	nm_timer_task();
	
	for(bus_i = 0; bus_i < AUTOSAR_DIRNM_BUS_CNT; bus_i ++)
	{
		autosar_Nm_clrRepeatFlag(0);
		nmMode = autosar_dirNm_mode[bus_i];
		if(NM_BAT_ON_MODE == nmMode)
		{
		    nm_mode_enter(bus_i,NM_BAT_ON_MODE);
		}
		
		/*non gateway node does not need to beconfigured--not_expired_alive*/
		switch(nmMode)
		{
#if 0
			case NM_BAT_ON_MODE:
				/*add Timer: NM_TIMER_WAKE_OR_SLEEP to distinguish BAT wakeup and CAN wakeup*/
				if(nm_timer_grp[bus_i][NM_TIMER_WAKE_OR_SLEEP].On <= 0)
				{
				    nm_start_timer(bus_i,NM_TIMER_WAKE_OR_SLEEP,T_NM_WAKE_OR_SLEEP);
				}
				if((nm_timer_grp[bus_i][NM_TIMER_WAKE_OR_SLEEP].On > 0) && (0 == nm_timer_grp[bus_i][NM_TIMER_WAKE_OR_SLEEP].Tick))
				{
					nm_can_init_fail[bus_i] = TRUE;
				    nm_mode_enter(bus_i,NM_SLEEP_MODE);
				}
				else if(TRUE == autosar_dirNm_init_ok(bus_i))
				{
					nm_can_init_fail[bus_i] = FALSE;
					autosar_dirNm_wait_sleep[bus_i] = FALSE;
					autosar_dirNm_mode[bus_i] = NM_SLEEP_MODE;
					nm_mode_enter(bus_i,NM_SLEEP_MODE);
					#if TEST_MODE_FORTEMP
					nm_send_testmsg[bus_i].b.state = 0;
					#endif
				}
				else
				{

				}
				break;
#endif
			case NM_SLEEP_MODE:
				{
					local_wu[bus_i] = autosar_dirNm_getLocalWuFlag(bus_i);//
					if(TRUE == local_wu[bus_i])
					{
						/*NM_03*/
						#if TEST_MODE_FORTEMP
							nm_send_testmsg[bus_i].b.state = busslp2repeatmsg;
						#endif
						autosar_dirNm_wait_sleep[bus_i] = FALSE;
						//nm_wkup_source_can[bus_i] = AUTOSAR_DIRNM_LOCALWU;
						nm_send_msg[bus_i].b.local_wake  = 1;
						nm_mode_enter(bus_i,NM_IMME_REPEAT_MODE);
					}
					else if(TRUE == nm_rec_new_msg[bus_i])
					{
						#if TEST_MODE_FORTEMP
						nm_send_testmsg[bus_i].b.state = busslp2repeatmsg;
						#endif
						autosar_dirNm_wait_sleep[bus_i] = FALSE;
						//nm_wkup_source_can[bus_i] = AUTOSAR_DIRNM_REMOTEWU;
					    nm_mode_enter(bus_i,NM_NORMAL_REPEAT_MODE);
					}
					else
					{
						autosar_dirNm_wait_sleep[bus_i] = TRUE;
					}
				}
				
				break;
			case NM_IMME_REPEAT_MODE:
				if((nm_timer_grp[bus_i][NM_TIMER_TIMEOUT].On > 0 ) && (0 == nm_timer_grp[bus_i][NM_TIMER_TIMEOUT].Tick))
				{
					/*NM_05*/
					nm_start_timer(bus_i,NM_TIMER_TIMEOUT,T_NM_TIMEOUT);
				}
			
				if(nm_timer_grp[bus_i][NM_TIMER_NORMAL].On > 0 )
				{
					if(0 == autosar_dirNm_imme_remain_times[bus_i])
					{
						/*NM_04*/
						nm_mode_enter(bus_i,NM_NORMAL_REPEAT_MODE);
					}
				}
				
				break;
			case NM_NORMAL_REPEAT_MODE:
				if((nm_timer_grp[bus_i][NM_TIMER_TIMEOUT].On > 0 ) && (0 == nm_timer_grp[bus_i][NM_TIMER_TIMEOUT].Tick))
				{
					/*NM_05*/
					nm_start_timer(bus_i,NM_TIMER_TIMEOUT,T_NM_TIMEOUT);
				}

				if((nm_timer_grp[bus_i][NM_TIMER_REPEAT].On > 0 ) && (0 == nm_timer_grp[bus_i][NM_TIMER_REPEAT].Tick))	/*normal mode*/
				{
					nm_send_msg[bus_i].b.repeat_req = 0;
					local_wu[bus_i] = autosar_dirNm_networkRequest(bus_i);//
					if(TRUE == local_wu[bus_i])
					{
						/*NM_06*/
						#if TEST_MODE_FORTEMP
						nm_send_testmsg[bus_i].b.state = repeatmsg2normal;
						#endif
						//nm_wkup_source_can[bus_i] = AUTOSAR_DIRNM_LOCALWU;
						nm_mode_enter(bus_i,NM_NORMAL_OPER_MODE);
					}
					else
					{
						#if TEST_MODE_FORTEMP
						nm_send_testmsg[bus_i].b.state = repeatmsg2readyslp;
						#endif
						if(1 == autosar_Nm_networkRelease(bus_i))
						{
							nm_mode_enter(bus_i,NM_READY_SLEEP_MODE);
						}
					}
				}
				break;
			case NM_NORMAL_OPER_MODE:

				if(1 == autosar_Nm_networkRelease(bus_i))
				{
					#if TEST_MODE_FORTEMP
					nm_send_testmsg[bus_i].b.state = normal2readyslp;
					#endif
					nm_mode_enter(bus_i,NM_READY_SLEEP_MODE);
				}
				else
				{
					if((nm_timer_grp[bus_i][NM_TIMER_TIMEOUT].On > 0 ) && (0 == nm_timer_grp[bus_i][NM_TIMER_TIMEOUT].Tick))
					{
						nm_start_timer(bus_i,NM_TIMER_TIMEOUT,T_NM_TIMEOUT);
					}

					if(TRUE == nm_recv_realTimeFlg[bus_i] && nm_rec_msg[bus_i].b.repeat_req > 0)
					{
						/*NM_07_1*/
						#if TEST_MODE_FORTEMP
						nm_send_testmsg[bus_i].b.state = normal2repeatmsg;
						#endif
						//nm_wkup_source_can[bus_i] = AUTOSAR_DIRNM_REMOTEWU;
						nm_recv_realTimeFlg[bus_i] = FALSE;
						nm_rec_msg[bus_i].b.repeat_req = 0;
						nm_mode_enter(bus_i,NM_NORMAL_REPEAT_MODE);
					}
					else if(1==nm_repeatMsgReqFlg)
					{
						/*NM_07_2*/
						#if TEST_MODE_FORTEMP
						nm_send_testmsg[bus_i].b.state = normal2repeatmsg;
						#endif
						nm_send_msg[bus_i].b.repeat_req = 1;
						nm_mode_enter(bus_i,NM_IMME_REPEAT_MODE);
					}
					else
					{}
				}
				break;
			case NM_READY_SLEEP_MODE:
				if((nm_timer_grp[bus_i][NM_TIMER_TIMEOUT].On > 0 ) && (0 < nm_timer_grp[bus_i][NM_TIMER_TIMEOUT].Tick))
				{
					local_wu[bus_i] = autosar_dirNm_networkRequest(bus_i);//
					if(TRUE == local_wu[bus_i])
					{
						#if TEST_MODE_FORTEMP
						nm_send_testmsg[bus_i].b.state = readyslp2normal;
						#endif
						//nm_wkup_source_can[bus_i] = AUTOSAR_DIRNM_LOCALWU;
						nm_send_msg[bus_i].b.repeat_req = 0;
						nm_mode_enter(bus_i,NM_NORMAL_OPER_MODE);
					}
					else if(1==nm_repeatMsgReqFlg)
					{
						/*NM_11_1*/
						#if TEST_MODE_FORTEMP
						nm_send_testmsg[bus_i].b.state = readyslp2repeatmsg;
						#endif
						//nm_wkup_source_can[bus_i] = AUTOSAR_DIRNM_LOCALWU;
						nm_send_msg[bus_i].b.repeat_req = 1;
						nm_mode_enter(bus_i,NM_IMME_REPEAT_MODE);
					}
					else if(TRUE == nm_recv_realTimeFlg[bus_i] && (1 == autosar_Nm_RepeatReqStatus(bus_i)))
					{
						/*NM_11_2*/
						#if TEST_MODE_FORTEMP
						nm_send_testmsg[bus_i].b.state = readyslp2repeatmsg;
						#endif
						//nm_wkup_source_can[bus_i] = AUTOSAR_DIRNM_REMOTEWU;
						nm_recv_realTimeFlg[bus_i] = FALSE;
						nm_rec_msg[bus_i].b.repeat_req = 0;
						nm_mode_enter(bus_i,NM_NORMAL_REPEAT_MODE);
					}
					else
					{

					}
				}
				else
				{
					#if TEST_MODE_FORTEMP
					nm_send_testmsg[bus_i].b.state = readyslp2prebusslp;
					#endif
					nm_mode_enter(bus_i,NM_PREPARE_SLEEP_MODE);
				}

				break;
			case NM_PREPARE_SLEEP_MODE:

				if((nm_timer_grp[bus_i][NM_TIMER_WAIT_SLEEP].On > 0 ) && (0 < nm_timer_grp[bus_i][NM_TIMER_WAIT_SLEEP].Tick))
				{
					local_wu[bus_i] = autosar_dirNm_getLocalWuFlag(bus_i);//
					if(TRUE == local_wu[bus_i])
					{
						/*NM_16*/
						#if TEST_MODE_FORTEMP
						nm_send_testmsg[bus_i].b.state = prebusslp2repeatmsg;
						#endif
						//nm_wkup_source_can[bus_i] = AUTOSAR_DIRNM_LOCALWU;
						nm_send_msg[bus_i].b.local_wake  = 1;
						nm_mode_enter(bus_i,NM_IMME_REPEAT_MODE);
					}
					else if(TRUE == nm_rec_new_msg[bus_i])
					{
						/*NM_15*/
						#if TEST_MODE_FORTEMP
						nm_send_testmsg[bus_i].b.state = prebusslp2repeatmsg;
						#endif
						//nm_wkup_source_can[bus_i] = AUTOSAR_DIRNM_REMOTEWU;
						nm_mode_enter(bus_i,NM_NORMAL_REPEAT_MODE);
					}
					else
					{

					}
				}
				else
				{
					/*NM_17*/
					#if TEST_MODE_FORTEMP
					nm_send_testmsg[bus_i].b.state = prebusslp2busslp;
					#endif
					nm_mode_enter(bus_i,NM_SLEEP_MODE);
				}

				break;
		}

		nm_platform_ready_sleep &= ~(1<<bus_i);
		nm_platform_ready_sleep |= autosar_dirNm_wait_sleep[bus_i]?(1<<bus_i):0;
		hld_flg_nmBusSleep = (nm_platform_ready_sleep == NM_ENTERSLEEP_MASK)?1:0;

#if TEST_MODE_FORTEMP
/*-------------------------------------------TEST---------------------------------------*/
		nm_send_testmsg[bus_i].b.station_addr = nm_send_msg[bus_i].b.station_addr;
		nm_send_testmsg[bus_i].b.repeatSt = nm_send_msg[bus_i].b.repeatSt;
		nm_send_testmsg[bus_i].b.local_wake = nm_send_msg[bus_i].b.local_wake;
		nm_send_testmsg[bus_i].b.repeat_req = nm_send_msg[bus_i].b.repeat_req;
		nm_send_testmsg[bus_i].b.currmode = autosar_dirNm_mode[bus_i];
		nm_send_testmsg[bus_i].b.nmsleepst = nm_platform_ready_sleep;
		nm_send_testmsg[bus_i].b.sleepst = hld_flg_nmBusSleep;
		nm_send_testmsg[bus_i].b.testflag = 1;
		nm_send_testmsg[bus_i].b.resv5 = CANILModeGrp[2];
		autosar_dirNm_testSend(bus_i,nm_send_testmsg[bus_i].buf);
/*--------------------------------------END--TEST---------------------------------------*/
#endif
	}	

}

void autosar_dirNm_RepeatMsgReq(nm_byte_t en)
{
	nm_repeatMsgReqFlg = en;
}


void autosar_dirNm_rec_dispose(nm_byte_t bus,nm_dWord_t id, nm_byte_t * buf)
{
	nm_byte_t byte_i = 0;
	if((bus < AUTOSAR_DIRNM_BUS_CNT) && (NM_BASE_ADDR == (id & NM_BASE_ADDR)))
	{
		for(byte_i = 0; byte_i < 8; byte_i ++)
		{
			nm_rec_msg[bus].buf[byte_i] = buf[byte_i];
		}
		nm_rec_new_msg[bus] = TRUE;
		nm_recv_realTimeFlg[bus] = TRUE;
		nm_wkup_source_can[bus] = 1;
		if( NM_SLEEP_MODE != autosar_dirNm_mode[bus] && 
			NM_BAT_ON_MODE != autosar_dirNm_mode[bus] && 
			NM_PREPARE_SLEEP_MODE != autosar_dirNm_mode[bus])
		{
			/*NM_14*/
			nm_timer_timeout_recvNMreset(bus);	
		}

		autosar_Nm_msgRepeatBitSt( bus, id, buf[1]);
	}

}

/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
void nm_start_timer(nm_byte_t bus_i,nm_byte_t timer, nm_word_t tick)
{
	if(timer < NM_TIMER_CNT && bus_i < AUTOSAR_DIRNM_BUS_CNT)
	{
	       nm_timer_grp[bus_i][timer].On = TRUE;
		   nm_timer_grp[bus_i][timer].Tick = tick; 
	}
}

static void nm_stop_timer(nm_byte_t bus_i,nm_byte_t timer)
{
	if(timer < NM_TIMER_CNT && bus_i < AUTOSAR_DIRNM_BUS_CNT)
	{
		nm_timer_grp[bus_i][timer].On = FALSE;
		nm_timer_grp[bus_i][timer].Tick = 0;
	}
}

static void nm_timer_install_process(nm_byte_t bus_i,nm_byte_t timer,void(*p_fun)(nm_byte_t))
{
	if(timer < NM_TIMER_CNT && bus_i < AUTOSAR_DIRNM_BUS_CNT)
	{
		nm_timer_grp[bus_i][timer].AlarmTask = p_fun;
	}
}

static void nm_timer_nm_timeout_expired(nm_byte_t bus_i)
{
	if(bus_i < AUTOSAR_DIRNM_BUS_CNT)
	{
		if((TRUE == nm_timer_grp[bus_i][NM_TIMER_TIMEOUT].On) &&(0 == nm_timer_grp[bus_i][NM_TIMER_TIMEOUT].Tick))
		{
			//nm_start_timer(bus_i,NM_TIMER_TIMEOUT,T_NM_TIMEOUT);
		}
	}
}

static void nm_timer_normal_expired(nm_byte_t bus_i)
{
	if(bus_i < AUTOSAR_DIRNM_BUS_CNT)
	{
		if((NM_IMME_REPEAT_MODE == autosar_dirNm_mode[bus_i])
				&& (autosar_dirNm_imme_remain_times[bus_i] > 0))
		{
			autosar_dirNm_imme_remain_times[bus_i]--;
			if(0 == autosar_dirNm_imme_remain_times[bus_i])
			{
				nm_start_timer(bus_i,NM_TIMER_NORMAL,T_NM_MSG_CYCLE);
			}
			else
			{
				nm_start_timer(bus_i,NM_TIMER_NORMAL,T_NM_IMME_CYCLE);
			}
		}
		else
		{
			nm_start_timer(bus_i,NM_TIMER_NORMAL,T_NM_MSG_CYCLE);
		}

		//nm_send_msg[bus_i].b.ignst	= autosar_dirNm_getLocalWuFlag(bus_i);
		//nm_send_msg[bus_i].b.nmwkupst = autosar_dirNm_WakeupSource(bus_i);
		//nm_send_msg[bus_i].b.appst	= autosar_Nm_appstatus();
		autosar_dirNm_send(bus_i,autosar_dirNm_mode[bus_i],nm_send_msg[bus_i].buf);
	}
}

static void nm_mode_enter(nm_byte_t bus,nm_byte_t mode)
{
    static nm_byte_t last_mode[AUTOSAR_DIRNM_BUS_CNT] = {NM_BAT_ON_MODE};

	if(bus <AUTOSAR_DIRNM_BUS_CNT)
	{
		last_mode[bus] = autosar_dirNm_mode[bus];
		#if TEST_MODE_FORTEMP
		nm_send_testmsg[bus].b.lastmode = last_mode[bus];
		#endif
		//nm_send_msg[bus].b.repeat_req = 0;
		switch(mode)
		{
			case NM_BAT_ON_MODE:
				autosar_dirNm_mode[bus] = mode;
				autosar_dirNm_comCtrl(bus,FALSE);
				break;
			case NM_SLEEP_MODE:
				//nm_stop_timer(bus,NM_TIMER_WAKE_OR_SLEEP);
				autosar_dirNm_mode[bus] = mode;
				autosar_dirNm_comCtrl(bus,FALSE);
				break;
			case NM_NORMAL_REPEAT_MODE:
				autosar_dirNm_mode[bus] = mode;
				autosar_dirNm_comCtrl(bus,TRUE);
				nm_stop_timer(bus,NM_TIMER_WAIT_SLEEP);
				nm_send_msg[bus].b.repeatSt = 1;
				if((NM_SLEEP_MODE == last_mode[bus]) 	   	|| \
				   (NM_READY_SLEEP_MODE == last_mode[bus]) 	|| \
				   (NM_PREPARE_SLEEP_MODE == last_mode[bus])|| \
				   (NM_NORMAL_OPER_MODE == last_mode[bus]))
				{
					nm_start_timer(bus,NM_TIMER_TIMEOUT,T_NM_TIMEOUT);
					nm_start_timer(bus,NM_TIMER_NORMAL,T_NM_MSG_CYCLE);
					autosar_dirNm_send(bus,autosar_dirNm_mode[bus],nm_send_msg[bus].buf);
				}
				if(NM_IMME_REPEAT_MODE != last_mode[bus])
				{
					nm_start_timer(bus,NM_TIMER_REPEAT,T_NM_REPEAT_MSG);
				}
				break;
			case NM_IMME_REPEAT_MODE:
				autosar_dirNm_mode[bus] = mode;
			    autosar_dirNm_comCtrl(bus,TRUE);
				nm_stop_timer(bus,NM_TIMER_WAIT_SLEEP);
				if(NM_SLEEP_MODE == last_mode[bus] || NM_READY_SLEEP_MODE == last_mode[bus] || NM_PREPARE_SLEEP_MODE == last_mode[bus])
				{
					nm_start_timer(bus,NM_TIMER_TIMEOUT,T_NM_TIMEOUT);
					nm_start_timer(bus,NM_TIMER_NORMAL,T_NM_IMME_CYCLE);
				}
				nm_start_timer(bus,NM_TIMER_REPEAT,T_NM_REPEAT_MSG);
				autosar_dirNm_imme_remain_times[bus] = N_NM_IMME_TIMES;
				nm_send_msg[bus].b.repeatSt = 1;
				autosar_dirNm_send(bus,autosar_dirNm_mode[bus],nm_send_msg[bus].buf);
				break;
			case NM_NORMAL_OPER_MODE:
				autosar_dirNm_mode[bus] = mode;
				autosar_dirNm_comCtrl(bus,TRUE);
				nm_stop_timer(bus,NM_TIMER_REPEAT);
				nm_stop_timer(bus,NM_TIMER_WAIT_SLEEP);
				nm_stop_timer(bus,NM_TIER_MTCOMM_DELAY);
				if(NM_READY_SLEEP_MODE == last_mode[bus])
				{
					nm_start_timer(bus,NM_TIMER_NORMAL,T_NM_MSG_CYCLE);
				}
				nm_send_msg[bus].b.repeatSt = 0;
				break;
			case NM_READY_SLEEP_MODE:
				nm_send_msg[bus].b.local_wake  = 0;
				nm_send_msg[bus].b.repeatSt = 0;
				autosar_dirNm_mode[bus] = mode;
				autosar_dirNm_comCtrl(bus,TRUE);
				nm_stop_timer(bus,NM_TIMER_NORMAL);
				nm_stop_timer(bus,NM_TIMER_REPEAT);
				nm_stop_timer(bus,NM_TIMER_WAIT_SLEEP);
				nm_stop_timer(bus,NM_TIMER_TO_SLEEP);
				nm_stop_timer(bus,NM_TIER_MTCOMM_DELAY);
				break;
			case NM_PREPARE_SLEEP_MODE:
				nm_wkup_source_can[bus] = 0;
				nm_rec_new_msg[bus] = FALSE;
				autosar_dirNm_mode[bus] = mode;
				autosar_dirNm_comCtrl(bus,FALSE);
				
				nm_stop_timer(bus,NM_TIMER_TIMEOUT);
				//nm_stop_timer(bus,NM_TIMER_WAKE_OR_SLEEP);
				nm_start_timer(bus,NM_TIMER_WAIT_SLEEP,T_WAIT_BUS_SLEEP);
				break;
			default:
				break;
		}
	}

#if TEST_MODE_FORTEMP
/*-------------------------------------------TEST---------------------------------------*/
		nm_send_testmsg[bus].b.station_addr = nm_send_msg[bus].b.station_addr;
		nm_send_testmsg[bus].b.repeatSt = nm_send_msg[bus].b.repeatSt;
		nm_send_testmsg[bus].b.local_wake = nm_send_msg[bus].b.local_wake;
		nm_send_testmsg[bus].b.repeat_req = nm_send_msg[bus].b.repeat_req;
		nm_send_testmsg[bus].b.currmode = autosar_dirNm_mode[bus];
		nm_send_testmsg[bus].b.lastmode = last_mode[bus];
		nm_send_testmsg[bus].b.nmsleepst = nm_platform_ready_sleep;
		nm_send_testmsg[bus].b.sleepst = hld_flg_nmBusSleep;
		nm_send_testmsg[bus].b.testflag = 1;
		nm_send_testmsg[bus].b.resv5 = CANILModeGrp[2];
		autosar_dirNm_testSend(bus,nm_send_testmsg[bus].buf);
/*--------------------------------------END--TEST---------------------------------------*/
#endif
}

static void nm_timer_task(void)
{
	nm_byte_t bus_i;
	nm_byte_t timer_i;
	for(bus_i = 0; bus_i < AUTOSAR_DIRNM_BUS_CNT; bus_i ++)
	{
		for(timer_i = 0; timer_i < NM_TIMER_CNT; timer_i ++)
		{
			if(TRUE == nm_timer_grp[bus_i][timer_i].On)
			{
				if(nm_timer_grp[bus_i][timer_i].Tick > 0)
				{
					if(nm_timer_grp[bus_i][timer_i].Tick > T_NM_CALL_CYCLE)
					{
						nm_timer_grp[bus_i][timer_i].Tick -= T_NM_CALL_CYCLE;
					}
					else
					{
						nm_timer_grp[bus_i][timer_i].Tick  = 0;
					}
					if(0 == nm_timer_grp[bus_i][timer_i].Tick)
					{
						if(NULL_PTR != nm_timer_grp[bus_i][timer_i].AlarmTask)
						{
							nm_timer_grp[bus_i][timer_i].AlarmTask(bus_i);
						}
					}	
				}
				
			}
		}

	}

}

unsigned char autosar_dirNm_WakeupSource(unsigned char bus)
{
	return nm_wkup_source_can[bus];
}

unsigned char autosar_dirNm_currMode(unsigned char bus)
{
	return autosar_dirNm_mode[bus];
}


/* ---- Source switch off ---------------------------------------------------- */
#undef AUTOSAR_DIR_NM_C_

/*--- End of file ------------------------------------------------------------*/
