

/*
* Copyright (c) File ,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:autosar_dir_nm_if.c
* Description: autosar direct protocal interface file
* REQ IDs:
* History:
* Time:2018/05/04
* User:Devon
*/

/* ---- Source switch on ------------------------------------------------------*/

#define AUTOSAR_DIR_NM_IF_C_
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "autosar_dir_nm_types.h"
#include "dataLink.h"
#include "flexCan.h"
#include "autosar_dir_nm_if.h"
#include "autosar_dir_nm_cfg.h"
#include "autosar_dir_nm.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/

extern void autosar_dirNm_rec_dispose(nm_byte_t bus,nm_dWord_t id, nm_byte_t * buf);
/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/
nm_byte_t app_nm_st = 0;
uint32_t cycle_count = 0;
static uint8_t autosar_nm_txrxenable[AUTOSAR_DIRNM_BUS_CNT] = {1};
/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/
static volatile nm_dWord_t ptcan_wkupsource_repeatflg = 0;
static volatile nm_byte_t ptcan_repeatReq = 0;
static nm_byte_t NM_MCUR0_rxTimeoutcnt = 0;
static nm_byte_t NM_BMS_rxTimeoutcnt = 0;
static nm_byte_t NM_OBCM_rxTimeoutcnt = 0;
static nm_byte_t NM_GW_PT_rxTimeoutcnt = 0;

const nmIf_Rxpack_t	nmIf_Rxpack[nm_ptcan_nodeNum] =
{
	{0x632,&CAN_3_0x632_NM_MCUR0_rxTimeout,	&NM_MCUR0_rxTimeoutcnt,	0x1},
	{0x634,&CAN_3_0x634_NM_BMS_rxTimeout,	&NM_BMS_rxTimeoutcnt,	0x2},
	{0x637,&CAN_3_0x637_NM_OBCM_rxTimeout,	&NM_OBCM_rxTimeoutcnt,	0x4},
	{0x63E,&CAN_3_0x63E_NM_GW_PT_rxTimeout,	&NM_GW_PT_rxTimeoutcnt,	0x8}
};

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


/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/

nm_bool_t autosar_dirNm_init_ok(nm_byte_t  bus)
{
    nm_bool_t init_ok = TRUE;

    return init_ok;
}

nm_bool_t autosar_dirNm_getLocalWuFlag(nm_byte_t bus)
{
    nm_bool_t local_wu = FALSE;
    if(1 == e_di_ign_sw)
    {
        local_wu = TRUE;
    }
    return local_wu;
}

nm_bool_t autosar_dirNm_networkRequest(nm_byte_t bus)
{
    nm_bool_t local_wu = FALSE;
    if((1 == e_di_ign_sw) || (1 == autosar_Nm_appstatus()))
    {
        local_wu = TRUE;
    }
    return local_wu;
}

nm_byte_t autosar_dirNm_getLocalWuSoc(nm_byte_t bus)
{
    nm_byte_t local_wuSoc = 0;  
    
    if(e_di_ign_sw > 0)
    {
        local_wuSoc = 2; 
    }
    else if(TRUE == nm_rec_new_msg[bus])
    {
    	local_wuSoc = 1;/*PTCAN wkup*/
    }
    else
    {

    }
    return local_wuSoc;
}


void autosar_dirNm_send(nm_byte_t bus_i,nm_byte_t mode,nm_byte_t * buf)
{
	 
    switch(bus_i)
	{
		case 0:
			if(autosar_nm_txrxenable[bus_i] && \
					((NM_IMME_REPEAT_MODE == mode) || (NM_NORMAL_REPEAT_MODE == mode) || (NM_NORMAL_OPER_MODE == mode)))
		    {
		    	if(FALSE == hcan_PTcan_busoffFlag)
		    	{
		    		(void)can_txMsgFix(2,nmTxMsgIndx,0x631, buf, 8);
		    	}
		    }
			break;
		default:break;
	}
}

#if TEST_MODE_FORTEMP
void autosar_dirNm_testSend(nm_byte_t bus_i,nm_byte_t * buf)
{

    switch(bus_i)
	{
		case 0:
			(void)can_txMsgFix(2,nmTxMsgIndx,0x631, buf, 8);
		break;
		default:break;
	}
}
#endif

void autosar_dirNm_setTxRxMode(unsigned char bus,unsigned char enable)
{
	autosar_nm_txrxenable[bus] = enable;
}


void autosar_dirNm_comCtrl(nm_byte_t bus, nm_bool_t enable)
{
	switch(bus)
	{
		case 0:
			//if(FALSE == hcan_PTcan_busoffFlag)
			{
				if(enable)
				{
					hcan_setILMode(0,CAN_IL_REC_SEND,1);
					hcan_setILMode(2,CAN_IL_REC_SEND,1);
				}
				else
				{
					hcan_setILMode(0,CAN_IL_REC_SEND,0);
					hcan_setILMode(2,CAN_IL_REC_SEND,0);
				}
			}
			break;
		default:
			break;
	}
}


void  autosar_dirNm_rec_indication(nm_byte_t bus,nm_dWord_t id, nm_byte_t dlc, nm_byte_t * buf)
{
	uint8_t canbus2nmbus[3] = {0xff,0xff,0};

    if(dlc == 8    && NM_BASE_ADDR < id &&
       id <= 0x67F && (id == NM_BASE_ADDR + buf[0]))
    {
       autosar_dirNm_rec_dispose(canbus2nmbus[bus], id, buf);
    }
}


void autosar_dirNm_network_timeout_exception(nm_byte_t bus)
{
	(void)bus;

}

void autosar_dirNm_no_autosar_sleep()
{
    if(	
    	0 == hld_di_ChargeWuBatC && 0 == hld_di_ChargeWuBatB
    	&& 0 == hld_di_chgActive && 0 == e_di_ign_sw
    	//&& FALSE == nm_msg_recvFlg_realTime(1,0)
    	)
    {
    	#if !(NM_MODULE_ENABLE)
	    hld_flg_nmBusSleep = TRUE;
		#endif
    }
}


nm_bool_t autosar_dirNm_rec_realTime_timeout(nm_byte_t bus)
{
	nm_bool_t ret_sleep = FALSE;  
	
    if(
    	0 == hld_di_ChargeWuBatC && 0 == hld_di_ChargeWuBatB
    	&& 0 == hld_di_chgActive && 0 == e_di_ign_sw
    	&& FALSE == nm_nodeNotTimeout(bus)
      )
    {
        ret_sleep = TRUE; 
    }
    
    return ret_sleep;
}


nm_bool_t autosar_dirNm_rec_realTime(nm_byte_t bus)
{
	nm_bool_t ret_sleep = FALSE;  
	
    if(
    	0 == hld_di_ChargeWuBatC && 0 == hld_di_ChargeWuBatB
    	&& 0 == hld_di_chgActive && 0 == e_di_ign_sw
    	//&& FALSE == nm_msg_recvFlg_realTime(1,bus)
      )
    {
        ret_sleep = TRUE; 
    }
    
    return ret_sleep;
}


nm_bool_t autosar_dirNm_getLocalWuFlag_realTime(nm_byte_t bus)
{
    nm_bool_t local_wu = TRUE;  
    if(	
    	0 == hld_di_ChargeWuBatC && 0 == hld_di_ChargeWuBatB
    	&& 0 == hld_di_chgActive && 0 == e_di_ign_sw
    	//&& FALSE == nm_msg_recvFlg_realTime(1,bus)
      )
    {
        local_wu = FALSE; 
    }
    return local_wu;
}


#if 0
/*----------release to user----------*/
nm_bool_t nm_can0_msg_recvFlg_realTime(void)
{
	nm_bool_t recv_flg = TRUE;
#if 0
	if(
		hld_can0_0x401_received ||
	    hld_can0_0x403_received ||
	    hld_can0_0x404_received ||
	    hld_can0_0x400_received 
	 )
	{
		recv_flg = TRUE;
	}
#endif
	return recv_flg;
}

static nm_bool_t nm_can2_msg_recvFlg_realTime(void)
{
	nm_bool_t recv_flg = TRUE;
#if 0
	if(hld_can2_0x402_received)
	{
		recv_flg = TRUE;
	}
#endif
	return recv_flg;
}
//#endif


nm_bool_t nm_msg_recvFlg_realTime(nm_byte_t num, nm_byte_t channel)
{
	nm_bool_t recv_flg = FALSE;
	
	switch(channel)
	{
		case 0:
			if(nm_can2_msg_recvFlg_realTime())
			{
				recv_flg = TRUE;
			}
			break;
		default:
			break;
	}
			
	return recv_flg;
}
#endif

void nm_rec_new_msg_clear(nm_byte_t num, nm_byte_t channel)
{
	uint8_t chl;
	if(AUTOSAR_DIRNM_BUS_CNT == num)
	{
		for(chl = 0;chl < num; chl++)
		{
			nm_rec_new_msg[chl] = FALSE;
		}
	}
	else
	{
		nm_rec_new_msg[channel] = FALSE;
	}
}

void nm_sendSuccessCb(unsigned char bus)
{
	nm_start_timer(bus,NM_TIMER_TIMEOUT,T_NM_TIMEOUT);
}

void nm_timer_timeout_recvNMreset(nm_byte_t bus) 
{
	nm_start_timer(bus,NM_TIMER_TIMEOUT,T_NM_TIMEOUT);
}

uint8_t autosar_Nm_networkRelease(nm_byte_t bus)
{
	if((FALSE == e_di_ign_sw) && \
			(1 == app_req_sleep_flag))
	{
		return 1;
	}

	return 0;
}

uint8_t autosar_Nm_appstatus(void)
{
	return (app_req_sleep_flag == 0)?1:0;
}


void autosar_Nm_clrRepeatFlag(uint8_t bus)
{
	uint8_t indx;
	if(0 == bus)
	{
		for(indx = 0;indx<nm_ptcan_nodeNum;indx++)
		{
			if(NULL_PTR != nmIf_Rxpack[indx].timeoutcnt)
			{
				if(*nmIf_Rxpack[indx].timeoutcnt >= 55)//10ms period
				{
					if(NULL_PTR != nmIf_Rxpack[indx].timeoutFlg)
					{
						*nmIf_Rxpack[indx].timeoutFlg = 1;
						ptcan_wkupsource_repeatflg &= ~nmIf_Rxpack[indx].mask;
					}
				}
				else
				{
					*nmIf_Rxpack[indx].timeoutcnt += 1;
				}
			}
		}
	}
}

void autosar_Nm_msgRepeatBitSt(nm_byte_t bus,nm_dWord_t id,nm_byte_t repeatByte)
{
	uint8_t indx;
	switch(bus)
	{
		case 0:
		{
			for(indx = 0;indx<nm_ptcan_nodeNum;indx++)
			{
				if(nmIf_Rxpack[indx].id == id)
				{
					if(repeatByte & 0x1)
					{
						if(0 == ptcan_wkupsource_repeatflg)
						{
							ptcan_wkupsource_repeatflg |= nmIf_Rxpack[indx].mask;
							ptcan_repeatReq = 1;
						}
					}
					else
					{
						ptcan_wkupsource_repeatflg &= ~nmIf_Rxpack[indx].mask;
					}

					if(NULL_PTR != nmIf_Rxpack[indx].timeoutFlg)
					{
						*nmIf_Rxpack[indx].timeoutFlg = 0;
						if(NULL_PTR != nmIf_Rxpack[indx].timeoutcnt)
						{
							*nmIf_Rxpack[indx].timeoutcnt = 0;
						}
					}
				}
			}
		}
		break;
		default:
		break;
	}
}

uint8_t autosar_Nm_RepeatReqStatus(uint8_t bus)
{
	uint8_t res = 0;

	if(0 == bus)
	{
		if(1 == ptcan_repeatReq)
		{
			ptcan_repeatReq = 0;
			res = 1;
		}
	}

	return res;
}

/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/







/* ---- Source switch off ---------------------------------------------------- */
#undef AUTOSAR_DIR_NM_IF_C_

/*--- End of file ------------------------------------------------------------*/
