/*
* Copyright (c) 2015,SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.:ACEC
* File:nt_iso15765.c
* Description:
* REQ IDs:
* History:
* 2015-4-14, 1007040, original
*/
#include "platformTypes.h"
#include "nt_iso15765.h"
#include "nt_import.h"

/* NT private global variables define */
static volatile nt_status_t  nt_state = NTS_IDLE;     /* NT request, response flow status */
static volatile nt_tm_type_t nt_tm_type = NT_TM_IDLE; /* NT timing type */
static volatile nt_word_t    nt_tm_tick = 0;          /* NT timming tick */

static nt_word_t             nt_stmin_tick = 0; /* timer tick for CF received */
static nt_byte_t             nt_bs         = 0; /* received FC block size */
static nt_word_t             nt_stmin      = 0; /* received FC STmin */

static nt_byte_t             nt_tx_cf_sn   = 0; /* transmit CF block SN */

static nt_bool_t             nt_is_functional_req = FALSE;

/* NT PDU (N_PCI + N_Data, not include N_AI) buffers, buffer length must always NT_PDU_BUFFER_SIZE (8) bytes */
static nt_byte_t pdu_send_buf[NT_PDU_BUFFER_SIZE];
/* Application layer buffer and indicator variables */
static nt_word_t app_receive_pos = 0; /* receive buffer data position */
static nt_word_t app_received_dl = 0; /* received total data length (not include N_PCI) for current request */
static nt_word_t app_rx_remain_dl = 0; /* receive remain data length */
static nt_byte_t app_rx_cf_sn = 0; /* Next receive CF_SN */
static nt_word_t app_send_pos = 0; /* data push position for transmit */
static nt_word_t app_send_dl  = 0; /* transmit length (not include NPCI) */ 
/*--->>region Devon modify it base on NDS08 standard*/
static nt_word_t pdu_recCfIndx = 0;
/*if the first FC following the FF tx frame has been received or not, the STMIN can only adopt the st value of the first FC received */
static nt_word_t nt_recFCFrameCnt = 0;
static nt_word_t nt_txCFFrameCntAfterFC = 0;	/*the CF frame count has been trasmitted after receiving the FC frame*/
static nt_word_t nt_sendErrRepTick = 0;
/*just now we have send out an send an frame,but not sure whetehr will cause bus error,if yes, we need to re-send it*/
static nt_word_t nt_sendFrameErrJuding = FALSE;	
/*--->>End region Devon modify it base on NDS08 standard*/

/* for UDS upper layer notice response pending by negtive response code 0x78 */
static nt_byte_t uds_rsp_pd_serv_id = 0; /* response pending service ID */
static nt_word_t uds_rsp_pd_tick = 0; /* repeat send response pending message time tick */

/* NT private function declare */
static nt_bool_t nt_pdu_send(void);
static nt_bool_t nt_pdu_push(nt_byte_t nt_pdu_buf[], nt_byte_t start, nt_byte_t length);
static void nt_tm_timeout_cb(nt_tm_type_t tm_type);
static void nt_pre_send_fc(nt_byte_t flow_status);
static void nt_send_fc(void);
static nt_bool_t nt_tm_set(nt_tm_type_t tm_type);
static void nt_tm_task(void);
static void nt_tm_reset(void);

/* UDS layer (Upper layer) function declare */
static void uds_pd_rsp_send(void);
static void uds_pd_rsp_task(void);

/*
 * Function:nt_init
 * Param<void>:
 * Return<void>:
 * REQ IDs:
 * Discription: NT initialize.
 * Note: You need call this function before nt_task.
*/
void nt_init(void)
{
    /* reset NT status */
    nt_state   = NTS_IDLE;
    /* reset NT timing type and parameters */
    nt_tm_type = NT_TM_IDLE;
    nt_tm_tick = 0;
    /* reset NT timer controls (for receive) */
    nt_stmin_tick = 0;
    nt_bs         = 0;
    nt_stmin      = 0;
    /* reset for transmit */
    nt_tx_cf_sn   = 0;
    /* application layer initialize */
    app_receive_pos  = 0;
    app_received_dl  = 0;
    app_rx_remain_dl = 0;
    app_rx_cf_sn = 0;
    app_send_pos = 0;
    app_send_dl  = 0;
    /* UDS application layer response pending variables initialize */
    uds_rsp_pd_serv_id = 0;
    uds_rsp_pd_tick    = 0;
    /* Set default request type to phisical addressing */
    nt_is_functional_req = FALSE;

	/*Devon modify it base on NDS08 standard*/
	nt_recFCFrameCnt = 0;
	nt_txCFFrameCntAfterFC = 0;
	nt_sendErrRepTick = 0;
	pdu_recCfIndx = 0;
	nt_sendFrameErrJuding = FALSE;
}

/*
 * Function:nt_task
 * Param<void>:
 * Return<void>:
 * REQ IDs:
 * Discription: Task process of NT. Include NT timer task, UDS pending response task
 * Note: 
*/
void nt_task(void)
{
    /* NT timer task */
    nt_tm_task();
    /* UDS response pending task */
    uds_pd_rsp_task();
}

/*
 * Function:nt_get_cur_state
 * Param<void>:
 * Return<nt_status_t>: current NT state
 * REQ IDs:
 * Discription: return current NT state
 * Note: 
*/
nt_status_t nt_get_cur_state(void)
{
    return nt_state;
}

/*
 * Function:nt_pdu_receive_cb
 * Param<nt_byte_t nt_pdu_buf>:
 * Param<nt_byte_t buf_len>:
 * Return<void>:
 * REQ IDs:
 * Discription: This function used for the lower layer (eg. CAN driver) to call back when PDU (a frame) was be received
 * Note: 
*/
void nt_pdu_receive_cb(nt_bool_t func_addr, nt_byte_t nt_pdu_buf[], nt_byte_t buf_len)
{
    nt_byte_t npci_type = 0xFFu;
    nt_byte_t fc_fs = 0xFFu;
    nt_byte_t dt_len = 0;

    /* get N_PCI type */
    npci_type = (nt_byte_t)(nt_pdu_buf[NT_NPCI_POS] & NT_NPCI_MASK);

    /* check NT state and PDU buffer size */
    if ((buf_len == NT_PDU_BUFFER_SIZE)
        && (FALSE == func_addr || NT_NPCI_SF == npci_type)) /*Jason 20160519, add || NT_NPCI_FC == npci_type condition */
    {														/* liujian 20210104, delete NT_NPCI_FC == npci_type condition */
        nt_sendErrRepTick = 0;/*todo, if needed or not*/
        switch (npci_type)
        {
            case NT_NPCI_SF:
                /* single frame */
                /* Half-duplex: Segmented transmit in progress check */
                if((NTS_APPL_LOCKED > nt_state) && (NTS_WAIT_CF != nt_state))
                {
                    /* Not in transmit process */
                    /* get SF data length */
                    dt_len = (nt_byte_t)(nt_pdu_buf[NT_NPCI_POS] & NT_NPCI_INV_MASK);
                    /* data length valid check (data length invalid will ignore the received N_PDU) */
                    if (dt_len > 0 && dt_len <= NT_SF_MAX_DL)
                    {
                        /* reset timer */
                        nt_tm_reset();
                        /* set NT status to NTS_RX_SF */
                        nt_state = NTS_RX_SF;
                        /* store received data length */
                        app_received_dl = (nt_word_t)dt_len;
                        /* set remain receive data length to 0 */
                        app_rx_remain_dl = app_received_dl;
                        /* reset receive buffer */
                        app_receive_pos = 0;
                        /* push data */
                        if (TRUE == nt_pdu_push(nt_pdu_buf, 1, dt_len))
                        {
                            /* change status to application layer locked state */
                            nt_state = NTS_APPL_LOCKED;
                            /* reset transmit buffer position */
                            app_send_pos = 0;
                            /* reset receive buffer point position for received data read */
                            app_receive_pos = 0;
                            /* Set request type */
                            nt_is_functional_req = func_addr;
                            /* notice request */
                            nt_request_cb();
                        }
                        else
                        {
                            nt_init();
                        }
                    }
                }
                break;
                
            case NT_NPCI_FF:
                /* first frame */
                /* Half-duplex: Segmented transmit in progress check */
                if (NTS_APPL_LOCKED > nt_state)
                {
                    /* Not in transmit process */
                    /* get frame length (12 bit length) */
                    app_received_dl = (nt_word_t)(nt_pdu_buf[NT_NPCI_POS] & NT_NPCI_INV_MASK);
                    app_received_dl = app_received_dl << 8;
                    app_received_dl = app_received_dl + nt_pdu_buf[NT_NPCI_POS + 1];
                    /* set remain length to need received length */
                    app_rx_remain_dl = app_received_dl;
                    /* FF length valid check */
                    if ((app_received_dl >= NT_FF_MIN_DL) && (app_received_dl <= APP_RECEIVE_BUF_SIZE))
                    {
                        /* reset Timer */
                        nt_tm_reset();
                        /* set NT status */
                        nt_state = NTS_RX_FF;
                        /* reset receive buffer */
                        app_receive_pos = 0;
                        /* set next receive CF SN */
                        app_rx_cf_sn = 1;
                        /* push data (will auto dec remain length) */
                        if (TRUE == nt_pdu_push(nt_pdu_buf, (nt_byte_t)NT_FF_DT_START_POS, (nt_byte_t)NT_FF_DT_LEN))
                        {
                            /* Push success, pre-send FC */
                            nt_pre_send_fc(NT_FC_FS_CTS);
                        }
                        else
                        {
                            /* Push fail, reset NT */
                            nt_init();
                        }
                    }
                    else if (app_received_dl > APP_RECEIVE_BUF_SIZE)
                    {
                        /* reset Timer */
                        nt_tm_reset();
                        /* response error */
                        nt_pre_send_fc(NT_FC_FS_OVFLW);
                    }
                    else
                    {
                        /* ignore the received N_PDU */
                    }
                }
                break;
            case NT_NPCI_FC:
                /* flow control */
                /* NT status match check, if not in wait FC status, ignore received N_PDU */
                if (NTS_WAIT_FC == nt_state)
                {
                    /* get FC flow state parameter */
					nt_txCFFrameCntAfterFC = 0;
                    fc_fs = (nt_byte_t)(nt_pdu_buf[NT_NPCI_POS] & NT_NPCI_INV_MASK);
                    switch(fc_fs)
                    {
                        case NT_FC_FS_CTS:
                            /* continue or start to send CF */
                            nt_state = NTS_SEND_CF; /* set status to send CF */
							if(nt_recFCFrameCnt < 0xFFFF)
							{
								nt_recFCFrameCnt ++;
							}
							if(1 == nt_recFCFrameCnt)
							{
								nt_bs = nt_pdu_buf[NT_NPCI_POS + 1]; /* get block size */
								nt_stmin = nt_pdu_buf[NT_NPCI_POS + 2]; /* get STmin */
	                            /* transfer STmin parameter to task ticks */
	                            if (nt_stmin < 0x80)
	                            {
	                                /* unit: 1MS */
	                                nt_stmin = (nt_word_t)((nt_stmin) / nt_task_cycle);
	                            }
	                            else if ((nt_stmin >= 0xF1) && (nt_stmin <= 0xF9))
	                            {
	                                /* unit: 100us */
	                                nt_stmin = (nt_word_t)((nt_stmin & 0x0F) / nt_task_cycle);
	                            }
	                            else
	                            {
	                                /* undefined value, as max delay time 127ms */
	                                nt_stmin = 1270 / nt_task_cycle;
	                            }
							}
                            /* set send CF delay */
                            (void)nt_tm_set(NT_TM_Cs);
                            break;

                        case NT_FC_FS_WT:
                            /* wait next FC */
                            /* set wait FC delay */
							/*Devon modify it base NDS08 standard*/
							if(nt_recFCFrameCnt < 0xFFFF)
							{
								nt_recFCFrameCnt ++;
							}
							if(1 == nt_recFCFrameCnt)
							{
								nt_bs = nt_pdu_buf[NT_NPCI_POS + 1]; /* get block size */
								nt_stmin = nt_pdu_buf[NT_NPCI_POS + 2]; /* get STmin */
	                            /* transfer STmin parameter to task ticks */
	                            if (nt_stmin < 0x80)
	                            {
	                                /* unit: 1MS */
	                                nt_stmin = (nt_word_t)((nt_stmin * 10) / nt_task_cycle);
	                            }
	                            else if ((nt_stmin >= 0xF1) && (nt_stmin <= 0xF9))
	                            {
	                                /* unit: 100us */
	                                nt_stmin = (nt_word_t)((nt_stmin & 0x0F) / nt_task_cycle);
	                            }
	                            else
	                            {
	                                /* undefined value, as max delay time 127ms */
	                                nt_stmin = 1270 / nt_task_cycle;
	                            }
							}
                            nt_tm_set(NT_TM_Bs);
                            break;

                        case NT_FC_FS_OVFLW:
						default:/*Todo Devon modify it base on NDS08 standard*/
                            /* over flow */
                            /* reset NT */
                            nt_init();
                            /* Hear can make a callback to the adjacent upper layer */
                            break;
                    }
                }
                break;
            case NT_NPCI_CF:
                /* consecutive frame */
                /* NT status match check, if not in wait CF status, ignore received N_PDU */
                //if ((NTS_WAIT_CF == nt_state) && (0 == nt_stmin_tick))
                if(NTS_WAIT_CF == nt_state)
                {
                    if ((nt_pdu_buf[NT_NPCI_POS] & NT_NPCI_INV_MASK) == app_rx_cf_sn)
                    {
                        /* SN ++ */
						pdu_recCfIndx ++;/*<dodo Devon modify it base on NDS08 standard>*/
                        app_rx_cf_sn++;
                        app_rx_cf_sn = (nt_byte_t)(app_rx_cf_sn & 0x000Fu); /* SN is 4 bit length */
                        /* remain length check (if below or equal NT_CF_MAX_DL, is last CF block) */
                        if (app_rx_remain_dl > NT_CF_MAX_DL)
                        {
                            /* Not last block */
                            /* push data */
                            if (TRUE == nt_pdu_push(nt_pdu_buf, (nt_byte_t)NT_CF_DT_START_POS, (nt_byte_t)NT_CF_MAX_DL))
                            {
                            	/*<todo>Devon modify it base on NDS08 standard*/
                            	if((pdu_fc_bs > 0) && (pdu_recCfIndx >= pdu_fc_bs))
                            	{
									/* Push success, pre-send FC */
			                         nt_pre_send_fc(NT_FC_FS_CTS);
								}
								else
								{
									/* push success, set Next receive CF timer */
                                	(void)nt_tm_set(NT_TM_Cr);
								}
                                
                            }
                            else
                            {
                                /* push fail, reset NT */
                                nt_init();
                            }
                        }
                        else
                        {
                            /* Last block */
                            /* push data */
                            if (TRUE == nt_pdu_push(nt_pdu_buf, (nt_byte_t)NT_CF_DT_START_POS, (nt_byte_t)app_rx_remain_dl))
                            {
                                /* push success */
                                /* reset Timer (Stop all NT timer) */
                                nt_tm_reset();
                                /* change status to application layer locked state */
                                nt_state = NTS_APPL_LOCKED;
                                /* reset transmit buffer position */
                                app_send_pos = 0;
                                /* reset receive buffer point position for received data read */
                                app_receive_pos = 0;
                                /* Set request type */
                                nt_is_functional_req = func_addr;
                                /* notice request */
                                nt_request_cb();
                            }
                            else
                            {
                                /* push fail, reset NT */
                                nt_init();
                            }
                        }
                    }
                    else
                    {
                        /* reset NT */
                        nt_init();
                        /* Hear can make a callback to the adjacent upper layer */
                    }
                }
                break;
            default:
                break;
        }
    }
}

/*
 * Function:nt_pdu_send_ok_cb
 * Param<void>:
 * Return<void>:
 * REQ IDs:
 * Discription: call back function to notice NT the message (N_PDU) was be send success.
 * Note: You only need call this function when As, Ar parameter not zero (0)
*/
void nt_pdu_send_ok_cb(void)
{
    switch (nt_state)
    {
        case NTS_SEND_SF:
            /* SF send compelete, reset NT */
            nt_init();
            break;
            
        case NTS_SEND_CF:
			if(nt_txCFFrameCntAfterFC < 0xFFFF)
			{
				nt_txCFFrameCntAfterFC ++;
			}
            /* send CF compelete, check remain data */
            if (app_send_pos < app_send_dl)
            {
            	if((nt_bs >0) && (nt_txCFFrameCntAfterFC >= nt_bs))
				{
					nt_state = NTS_WAIT_FC;
                    /* set wait FC timeout */
                    (void)nt_tm_set(NT_TM_Bs);
				}
				else
				{
					/* not compelete, continue send CF */
                	(void)nt_tm_set(NT_TM_Cs); /* for send next CF */
				}
                /* need transmit next block, set transmit CF time delay */
            }
            else
            {
                /* all data was be send, reset NT */
                nt_init();
            }
            break;

        case NTS_SEND_FF:
            /* send FF complete, wait FC */
            nt_state = NTS_WAIT_FC; /* change status to wait FC */
			nt_recFCFrameCnt = 0;
            (void)nt_tm_set(NT_TM_Bs); /* set wait FC timer */
            break;
            
        case NTS_SEND_FC:
            /* send FC compelete, wait CF block */
            nt_state = NTS_WAIT_CF; /* change status to wait CF */
            (void)nt_tm_set(NT_TM_Cr); /* set wait CF timer */
            break;
        default:
            /* don't care */
            break;
    }
}

/*
 * Function:nt_pdu_send_fail_cb
 * Param<void>:
 * Return<void>:
 * REQ IDs:
 * Discription:call back function to notice NT the message (N_PDU) was be send fail.
 * Note: You only need call this function when As, Ar parameter not zero (0)
*/
void nt_pdu_send_fail_cb(void)
{
    switch (nt_state)
    {
        case NTS_SEND_SF:
        case NTS_SEND_CF:
        case NTS_SEND_FF:
        case NTS_SEND_FC:
            /* send N_PDU fail, reset NT */
            nt_init();
            break;
        default:
            /* not send N_PDU, don't care */
            break;
    }
}


/*devon modify it todo 2017-03-01*/
void nt_pdu_busErrStCb(nt_bool_t setOrClr)
{
	if(TRUE == setOrClr)
	{
		if((0 == nt_sendErrRepTick) && (TRUE == nt_sendFrameErrJuding))
		{
			nt_busErrInit();
			nt_sendErrRepTick = nt_busErrRepMaxTime;
		}
	}
	else
	{
		nt_sendErrRepTick = 0;
		/*we can confirm last frame has been sent without  bus error*/
		nt_sendFrameErrJuding = FALSE;
	}
	return;
}


/*
 * Function:nt_req_len
 * Param<void>:
 * Return<nt_word_t>: The total received request length.
 * REQ IDs:
 * Discription:
 * Note: 
*/
nt_word_t nt_req_len(void)
{
    return app_received_dl; /* return received data length */
}


/*
 * Function:nt_req_read
 * Param<nt_byte_t def_val>: Default value when no more data
 * Return<nt_byte_t>: The current pop data of received buffer. if no more data, will return def_val
 * REQ IDs:
 * Discription:
 * Note: 
*/
nt_byte_t nt_req_pop_dt(nt_byte_t def_val)
{
    nt_byte_t data = def_val; /* preset return value to default value */

    if (NTS_APPL_LOCKED == nt_state) /* <-- Jason 20160602, add conditon. Forbide read data when received request by NT layer */
    {
        /* check receive buffer */
        if (app_receive_pos < app_received_dl)
        {
            /* have data can be pop */
            data = app_receive_buf[app_receive_pos]; /* set return value */
            app_receive_pos++; /* receive pop position point to next value */
        }
    }
    
    return data;
}

/*
 * Function:nt_req_pos_reset
 * Param<void>:
 * Return<void>:
 * REQ IDs:
 * Discription: reset request pop data position for read received data from position 0
 * Note: This function only actived when receive buffer was be locked by upper layer
*/
void nt_req_pos_reset(void)
{
    /* check NT state */
    if (NTS_APPL_LOCKED == nt_state)
    {
        /* NT in application locked buffer status */
        app_receive_pos = 0; /* reset pop position */
    }
}

/*
 * Function:nt_req_pos_set
 * Param<nt_word_t pos>: The pop point position for read received data
 * Return<void>:
 * REQ IDs:
 * Discription: set request pop data position for read received data
 * Note: This function only actived when receive buffer was be locked by upper layer
*/
void nt_req_pos_set(nt_word_t pos)
{
    /* check NT state */
    if (NTS_APPL_LOCKED == nt_state)
    {
        /* NT in application locked buffer status */
        app_receive_pos = pos; /* set pop position */
    }
}

/*
 * Function:nt_get_req_pos
 * Param<void>:
 * Return<nt_word_t>: current receive position
 * REQ IDs:
 * Discription: get current receive buffer point position
 * Note: 
*/
nt_word_t nt_get_req_pos(void)
{
    return app_receive_pos;
}

/*
 * Function:nt_req_remain_len
 * Param<void>:
 * Return<nt_word_t>: request data (received buffer) remain pop data length
 * REQ IDs:
 * Discription:
 * Note: 
*/
nt_word_t nt_req_remain_len(void)
{
    nt_word_t remain_len = 0;

    /* check remain length */
    if (app_received_dl > app_receive_pos)
    {
        /* received Dl above current pop position */
        remain_len = (nt_word_t)(app_received_dl - app_receive_pos); /* get remain pop length */
    }
    
    return remain_len;
}

nt_bool_t nt_req_is_functional_check(void)
{
	return nt_is_functional_req;
}

/*
 * Function:nt_unlock
 * Param<void>:
 * Return<void>:
 * REQ IDs:
 * Discription: for upper layer unlock the NT receive buffer
 * Note: 
*/
void nt_unlock(void)
{
    /* NT state check */
    if (NTS_APPL_LOCKED == nt_state)
    {
        /* in application locked state */
        nt_init(); /* reset NT */
    }
}

/*
 * Function:nt_is_locked
 * Param<void>:
 * Return<nt_bool_t>: NT unlock state. TRUE for is locked, FALSE for is unlock
 * Discription:
 * Note: 
*/
nt_bool_t nt_is_locked(void)
{
    return (nt_bool_t)(NTS_APPL_LOCKED == nt_state);
}

/*
 * Function:nt_rsp_push_dt
 * Param<nt_byte_t val>: the data value (byte) need push to response (send) buffer
 * Return<nt_bool_t>: push result, TRUE: push success, FALSE: push fail, the buffer is full
 * REQ IDs:
 * Discription:
 * Note: This function will auto clear all push data if push fail (buffer full)
*/
nt_bool_t nt_rsp_push_dt(nt_byte_t val)
{
    nt_bool_t rst = FALSE; /* pre-set push result to FALSE */

    /* buffer full check */
    if (app_send_pos < APP_SEND_BUF_SIZE)
    {
        /* not full, can push at least 1 byte */
        app_send_buf[app_send_pos] = val; /* set value */
        app_send_pos++; /* push position inc 1 */
        rst = TRUE; /* set push result to TRUE */
    }

    return rst; /* return push result */
}

/*
 * Function:nt_rsp_push_buf
 * Param<nt_byte_t buf[]>: the buffer need push to response (send) buffer
 * Param<nt_word_t len>: push data length
 * Return<nt_bool_t>: push result, TRUE: push success, FALSE: push fail, the send buffer can not push so many datas
 * REQ IDs:
 * Discription:
 * Note: This function will auto clear all push data if push fail (buffer not enough)
*/
nt_bool_t nt_rsp_push_buf(nt_byte_t buf[], nt_word_t len)
{
    nt_word_t index; /* push data buffer index */
    nt_bool_t rst = FALSE; /* push result, pre-set push fail (FALSE) */

    /* buffer remain size enough check */
    if ((APP_SEND_BUF_SIZE - app_send_pos) > len)
    {
        /* buffer enough, push data to buffer */
        for (index = 0; index < len; index++)
        {
            app_send_buf[app_send_pos] = buf[index];
            app_send_pos++;
        }
        /* push success */
        rst = TRUE;
    }
    
    return rst;
}

/*
 * Function: nt_rsp_get_data
 * Param<nt_word_t rsp_pos>: The data position of send buffer for read
 * Return<nt_byte_t>: The data of send buffer by rsp_pos point.
 * Discription: 
 * Note: If out of buffer or push data range, return zero
*/
nt_byte_t nt_rsp_get_data(nt_word_t rsp_pos)
{
    if ((APP_SEND_BUF_SIZE > rsp_pos) && (rsp_pos < app_send_pos))
    {
        return app_send_buf[rsp_pos];
    }
    else
    {
        return (nt_byte_t)(0u);
    }
}


/*
 * Function:nt_rsp_set
 * Param<nt_byte_t buf[]>: buffer need set to response (send) buffer.
 * Param<nt_word_t len>: buffer length
 * Return<nt_bool_t>: push result, TRUE: push success, FALSE: push fail, the set buffer length above send buffer length
 * REQ IDs:
 * Discription: Push a buffer to response(send) buffer from position 0
 * Note: This function will auto clear all push data if push fail (buffer not enough)
*/
nt_bool_t nt_rsp_set(nt_byte_t buf[], nt_word_t len)
{
    nt_bool_t rst = FALSE; /* push result, pre-set push fail (FALSE) */

    /* set buffer length not zero check and send buffer enough check */
    if (len > 0 && APP_SEND_BUF_SIZE >= len)
    {
        /* set buffer to send buffer */
        for (app_send_pos = 0; app_send_pos < len; app_send_pos++)
        {
            app_send_buf[app_send_pos] = buf[app_send_pos];
        }
        /* set success */
        rst = TRUE;
    }
    else
    {
        /* buffer not enough to push so many datas, clear all push data (take as an error) */
        app_send_pos = 0; /* set posh position to 0 for clear all data */
    }
    
    return rst;
}

/*
 * Function:nt_rsp_len
 * Param<void>:
 * Return<nt_word_t>: current push buffer length to response (send) buffer
 * REQ IDs:
 * Discription: Get the current response buffer length
 * Note: It is current push position also
*/
nt_word_t nt_rsp_len(void)
{
    return app_send_pos;
}

/*
 * Function:nt_rsp_remain_len
 * Param<void>:
 * Return<nt_word_t>: send buffer remain length for push
 * REQ IDs:
 * Discription:
 * Note: 
*/
nt_word_t nt_rsp_remain_len(void)
{
    return (nt_word_t)((nt_word_t)APP_SEND_BUF_SIZE - app_send_pos);
}

/*
 * Function:nt_rsp_reset
 * Param<void>:
 * Return<void>:
 * REQ IDs:
 * Discription: reset send buffer (clear all push datas)
 * Note: 
*/
void nt_rsp_reset(void)
{
    app_send_pos = 0;
    app_send_dl = 0;
}

/*
 * Function:nt_rsp_start
 * Param<void>:
 * Return<void>:
 * REQ IDs:
 * Discription: start send response
 * Note: 
*/
void nt_rsp_start(void)
{
    nt_byte_t pdu_index;

    /* store send data length (lash push data position) */
    app_send_dl = app_send_pos;
    /* check NT status */
    if (NTS_APPL_LOCKED == nt_state)
    {
        /* cancel UDS response pending */
        uds_rsp_pd_serv_id = 0;
        uds_rsp_pd_tick = 0;
        /* set send data position to 0 (start send data position) */
        app_send_pos = 0;
        /* check send length */
        if (app_send_dl > 0)
        {
            
            if(1 == NT_NPCI_POS)
            {
                /* extended addressing mode check */
                pdu_send_buf[0] = nt_ext_rsp_addr; /* set externed response address */
            }

            /* start send */
            if (app_send_dl <= NT_SF_MAX_DL)
            {
                /* single frame transmit */
                nt_state = NTS_SEND_SF;
                /* set N_PCI */
                pdu_send_buf[NT_NPCI_POS] = (nt_byte_t)(NT_NPCI_SF | (app_send_dl & 0x0Fu));
                /* set Datas */
                for (pdu_index = NT_NPCI_POS + 1; pdu_index < NT_PDU_BUFFER_SIZE; pdu_index++)
                {
                    /* data pop compelete check */
                    if (app_send_pos < app_send_dl)
                    {
                        /* pop data to send buffer from send buffer */
                        pdu_send_buf[pdu_index] = app_send_buf[app_send_pos];
                        app_send_pos++;
                    }
                    else
                    {
                        /* fill blank */
                        pdu_send_buf[pdu_index] = pdu_fill_byte;
                    }
                }
            }
            else
            {
                /* multi-frame transmit by consecutive segment frame */
                /* first frame transmit */
                nt_state = NTS_SEND_FF;
                /* set start block SN (FF for block 0) */
                nt_tx_cf_sn   = 0;
                /* set N_PCI */
                pdu_send_buf[NT_NPCI_POS] = (nt_byte_t)(NT_NPCI_FF | ((app_send_dl >> 8) & 0x0Fu));
                pdu_send_buf[NT_NPCI_POS + 1] = (nt_byte_t)(app_send_dl & 0x00FFu);
                /* set Datas */
                for (pdu_index = NT_NPCI_POS + 2; pdu_index < NT_PDU_BUFFER_SIZE; pdu_index++)
                {
                    /* data pop compelete check */
                    if (app_send_pos < app_send_dl)
                    {
                        /* pop data to send buffer from send buffer */
                        pdu_send_buf[pdu_index] = app_send_buf[app_send_pos];
                        app_send_pos++;
                    }
                    else
                    {
                        /* make sure never reach hear */
                        pdu_send_buf[pdu_index] = pdu_fill_byte;
                    }
                }
            }
            (void)nt_pdu_send();
        }
        else
        {
            /* reset NT status */
            nt_init();
        }
    }
}

/*
 * Function:uds_rsp_pending
 * Param<nt_byte_t serv_id>: service ID of notice response pending message
 * Return<void>:
 * REQ IDs:
 * Discription: UDS notice response sending
 * Note: This function only send UDS respnose pending negtive code when NT in application locked buffer state
*/
void uds_rsp_pending(nt_byte_t serv_id)
{
    /* check NT condition */
    if (NTS_APPL_LOCKED == nt_state)
    {
        /* save service ID */
        uds_rsp_pd_serv_id = serv_id;
        /* start time tick for repeat response */
        uds_rsp_pd_tick    = uds_rsp_pd_cycle;
        /* send response pending message immediately */
        uds_pd_rsp_send();
    }
}

nt_bool_t uds_rsp_is_pending(void)
{
    nt_bool_t rsp_is_pending = FALSE;

    if ((NTS_APPL_LOCKED == nt_state) && (uds_rsp_pd_serv_id != 0))
    {
        rsp_is_pending = TRUE;
    }

    return rsp_is_pending;
}

/*
 * Function:uds_pd_rsp_send
 * Param<void>:
 * Return<static void>:
 * REQ IDs:
 * Discription: send or re-send PDU response pending message
 * Note: This function only send UDS respnose pending negtive code when NT in application locked buffer state
 *       You must call uds_rsp_pending function
*/
static void uds_pd_rsp_send(void)
{
    nt_byte_t pdu_index;

    if (NTS_APPL_LOCKED == nt_state)
    {
        /* set N_PCI for UDS negtive response pending */
        pdu_send_buf[NT_NPCI_POS] = (nt_byte_t)(NT_NPCI_SF | 0x03);
        pdu_send_buf[NT_NPCI_POS + 1] = (nt_byte_t)0x7F; /* UDS negtive response ID */
        pdu_send_buf[NT_NPCI_POS + 2] = uds_rsp_pd_serv_id;
        pdu_send_buf[NT_NPCI_POS + 3] = 0x78; /* NRSP code, for notice response pending */
        /* fill blank */
        for (pdu_index = NT_NPCI_POS + 4; pdu_index < NT_PDU_BUFFER_SIZE; pdu_index++)
        {
            pdu_send_buf[pdu_index] = pdu_fill_byte;
        }
        (void)nt_pdu_transmit(pdu_send_buf, NT_PDU_BUFFER_SIZE);
		nt_sendFrameErrJuding = TRUE;
    }
}

/*
 * Function:uds_pd_rsp_task
 * Param<void>:
 * Return<static void>:
 * REQ IDs:
 * Discription: UDS repeat notice response pending task
 * Note: 
*/
static void uds_pd_rsp_task(void)
{
    /* NT state valid and response service ID valid check */
    if ((NTS_APPL_LOCKED == nt_state) && (uds_rsp_pd_serv_id != 0) && (0 == nt_sendErrRepTick))
    {
        /* timeout check */
        if (uds_rsp_pd_tick > 0)
        {
            uds_rsp_pd_tick--; /* time tick dec 1 */
            /* timeout check */
            if (0 == uds_rsp_pd_tick)
            {
                /* reset timer */
                uds_rsp_pd_tick = uds_rsp_pd_cycle;
                /* repeat send */
                (void)uds_pd_rsp_send();
            }
        }
    }
    else
    {
        /* reset UDS response pending send */
        uds_rsp_pd_serv_id = 0;
        uds_rsp_pd_tick = 0;
    }
}

/*
 * Function:nt_pdu_send
 * Param<void>:
 * Return<static nt_bool_t>: TRUE send success or in process
 * REQ IDs: 
 * Discription:
 * Note: 
*/
static nt_bool_t nt_pdu_send(void)
{
    nt_bool_t rst = FALSE; /* pre-set send fail */

    /* Try to transmit PDU */
    if (TRUE == nt_pdu_transmit(pdu_send_buf, NT_PDU_BUFFER_SIZE))
    {
        /* send in process */
        /* set send timeout timer */
		nt_sendFrameErrJuding = TRUE;
        if (NTS_SEND_FC == nt_state)
        {
            (void)nt_tm_set(NT_TM_Cr); /* state change based FC_FS type */
        }
        else
        {
            if (FALSE == nt_tm_set(NT_TM_As))
            {
                switch (nt_state)
                {
                    case NTS_SEND_SF:
                        nt_init();
                        break;
                    case NTS_SEND_FF:
                        /* set to Next status */
                        nt_state = NTS_WAIT_FC;
						nt_recFCFrameCnt = 0;
                        /* set wait FC timeout */
                        (void)nt_tm_set(NT_TM_Bs);
                        break;
                    case NTS_SEND_CF:
						if(nt_txCFFrameCntAfterFC < 0xFFFF)
						{
							nt_txCFFrameCntAfterFC ++;
						}
                        /* CF send compelete check */
                        if (app_send_pos < app_send_dl)
                        {
                        	if((nt_bs >0) && (nt_txCFFrameCntAfterFC >= nt_bs))
							{
								nt_state = NTS_WAIT_FC;
		                        /* set wait FC timeout */
		                        (void)nt_tm_set(NT_TM_Bs);
							}
							else
							{
								/* not compelete, continue send CF */
                            	(void)nt_tm_set(NT_TM_Cs); /* for send next CF */
							}
                        }
                        else
                        {
                            /* complete, reset NT */
                            nt_init();
                        }
                        break;
                    default:  /* Jason 20160519, miss default, add it */
                        break;
                }
            }
        }
        rst = TRUE; /* a message(frame) is sending */
    }
    else
    {
        /* send fail */
        nt_init();
    }

    return rst;
}

/*
 * Function:nt_pdu_push
 * Param<nt_byte_t nt_pdu_buf[]>: the buffer need push to NT received buffer
 * Param<nt_byte_t start>: push start position of nt_pdu_buf (the input buffer)
 * Param<nt_byte_t length>: push length of nt_pdu_buf (the input buffer)
 * Return<static nt_bool_t>: push result
 * REQ IDs:
 * Discription:
 * Note: 
*/
static nt_bool_t nt_pdu_push(nt_byte_t nt_pdu_buf[], nt_byte_t start, nt_byte_t length)
{
    nt_bool_t rst = FALSE; /* pre-set push Fail */
    nt_byte_t index = 0; /* nt_pdu_buf push data index */

    /* input parameter valid, buffer enough and received data length push valid check */
    if (((start + length) <= NT_PDU_BUFFER_SIZE)
        && ((APP_RECEIVE_BUF_SIZE - app_receive_pos) >= length)
        && (app_rx_remain_dl >= length))
    {
        /* push data */
        for (index = 0; index < length; index++)
        {
            app_receive_buf[app_receive_pos] = nt_pdu_buf[start]; /* set data */
            app_receive_pos++; /* receive push position inc */
            start++; /* push source buffer position inc */
            app_rx_remain_dl--; /* received remain data length-- */
        }
        rst = TRUE;
    }
    return rst;
}

/*
 * Function:nt_tm_timeout_cb
 * Param<nt_tm_type_t tm_type>: timeout timer type
 * Return<static void>:
 * REQ IDs:
 * Discription: timer timeout callback
 * Note: 
*/
static void nt_tm_timeout_cb(nt_tm_type_t tm_type)
{
    nt_byte_t pdu_index; /* N_PDU data index */
	/*<dodo> Devon modify it base on NDS08 standard*/
	if(nt_sendErrRepTick > 0)
	{
		nt_sendErrRepTick --;
		if(0 == nt_sendErrRepTick)
		{
			nt_init();
		}
	}
	else
	{
		switch (tm_type)
	    {
	        case NT_TM_Br:
	            /* Send FC */
	            nt_send_fc();
	            break;
	        case NT_TM_Cs:
	            /* Send CF */
	            /* set N_PCI */
				nt_tx_cf_sn++; /* block SN inc 1 */
	            nt_tx_cf_sn = (nt_byte_t)(nt_tx_cf_sn & 0x0F); /* block SN bit length fit (blosk SN is 4 bit length) */
	            pdu_send_buf[NT_NPCI_POS] = (nt_byte_t)(NT_NPCI_CF | nt_tx_cf_sn); /* set N_PCI */
	            /* set Datas */
	            for (pdu_index = NT_NPCI_POS + 1; pdu_index < NT_PDU_BUFFER_SIZE; pdu_index++)
	            {
	                /* last data check */
	                if (app_send_pos < app_send_dl)
	                {
	                    /* not last data, fill all */
	                    pdu_send_buf[pdu_index] = app_send_buf[app_send_pos];
	                    app_send_pos++;
	                }
	                else
	                {
	                    /* last data set compelete, fill not used data */
	                    pdu_send_buf[pdu_index] = pdu_fill_byte;
	                }
	            }
	            (void)nt_pdu_send();
	            break;
	        default:
	            /* As(Send Frame for response) Ar(Send Frame for request) Bs(Wait FC) Cr(Wait CF) */
	            /* You can throw a fault to Uper Layer here */
	            nt_init();
	            break;
	    }
	}
    
}

/*
 * Function:nt_pre_send_fc
 * Param<nt_byte_t flow_status>: flow control frame flow status type
 * Return<static void>:
 * REQ IDs:
 * Discription: pre-send FC
 * Note: if Br is 0, will send FC imeadiately
*/
static void nt_pre_send_fc(nt_byte_t flow_status)
{
    nt_word_t index;
    /* flow status valid check */
    if (flow_status < NT_FC_FS_COUNT)
    {
    	pdu_recCfIndx = 0;/*Devon modify it base on NDS08 standard*/
        nt_state = NTS_SEND_FC;
        /* set N_PCI type and flow status */
        pdu_send_buf[NT_NPCI_POS] = (nt_byte_t)(NT_NPCI_FC | flow_status);
        /* set block size */
        pdu_send_buf[NT_NPCI_POS + 1] = pdu_fc_bs;
        /* set STmin */
        pdu_send_buf[NT_NPCI_POS + 2] = pdu_fc_stmin;
        for (index = NT_NPCI_POS + 3; index < NT_PDU_BUFFER_SIZE; index++)
        {
            pdu_send_buf[index] = pdu_fill_byte;
        }
        
        /* set time delay */
        if (0 == nt_tm_set(NT_TM_Br))
        {
            nt_send_fc();
        }
    }
}

/*
 * Function:nt_send_fc
 * Param<void>:
 * Return<static void>:
 * REQ IDs:
 * Discription: send FC imediately
 * Note: You must call nt_pre_send_fc first before call this function
*/
static void nt_send_fc(void)
{
    /* This function not need Br time delay */
    /* get FC flow status */
    
    nt_byte_t flow_status = (nt_byte_t)(pdu_send_buf[NT_NPCI_POS] & NT_NPCI_INV_MASK); /* get FC flow status */

    if ((nt_byte_t)(pdu_send_buf[NT_NPCI_POS] & NT_NPCI_MASK) == (nt_byte_t)(NT_NPCI_FC)) /* check the send buffer N_PCI type is set to FC */
    {
        /* send FC immediately, send result check */
        if (TRUE == nt_pdu_send())
        {
            switch (flow_status)
            {
                case NT_FC_FS_CTS:
                    /* send continue transmit FC */
                    /* set NT state to Next (wait CF) */
                    nt_state = NTS_WAIT_CF;
                    /* check Ar timer */
                    if (0u == nt_tm_tick)
                    {
                        /* No Ar timer running, set wait CF timer */
                        (void)nt_tm_set(NT_TM_Cr);
						/* First CF after FC(Flow control) not need STmin Limit */
						nt_stmin_tick = 0;
                    }
                    break;
                case NT_FC_FS_WT:
                    /* Notused */
                    break;
                case NT_FC_FS_OVFLW:
                    /* over flow, reset NT */
                    nt_init();
                    break;
                default:
                    break;
            }
        }
    }
}

/*
 * Function:nt_tm_set
 * Param<nt_tm_type_t tm_type>: Set timer type
 * Return<static nt_bool_t>: TRUE, set timer success, timer is in running
 *               FALSE, set timer fault, or current tm_type timing parameter is 0. no timer running
 * REQ IDs:
 * Discription:
 * Note: Call this function will stop current running timer.
*/
static nt_bool_t nt_tm_set(nt_tm_type_t tm_type)
{
    nt_word_t tm_ticks = 0;
    nt_bool_t set_result = FALSE;

    /* array access range check */
    if (tm_type < NT_TM_TYPES_COUNT)
    {
        /* pre-load valid parameter from network layer timing parameters array */
        tm_ticks = nt_tm_params[tm_type];
		/*<todo Devon modify it base on NDS08 standard>*/
		/*after receiving the FC frame, the first CF should be transmitted as fast as possible*/
		
        if (NT_TM_Cs == tm_type)
        {
        	if(0 == nt_txCFFrameCntAfterFC)
        	{
				tm_ticks = 1;
			}
			else
			{
				 /* real ticks set to parameter plus STmin */
	            tm_ticks = tm_ticks + nt_stmin;
	            if (0 == tm_ticks)
	            {
	                tm_ticks = 1; /* make sure Cs not be zero */
	            }
			}
        }
    }

    /* load timing parameter to tick */
    nt_tm_tick = tm_ticks;
    
    /* check timer ticks */
    if (nt_tm_tick > 0)
    {
        /* valid, means a timer will running */
        set_result = TRUE;
    }
    /* else invalid, means no timer runnning */

    /* renew timer type */
    nt_tm_type = tm_type;

    /* Cr timer type check */
    if (NT_TM_Cr == nt_tm_type)
    {
        /* if is waiting CF, set a STmin timer */
        /* 20151222 Jason --> original: nt_stmin_tick = pdu_fc_stmin; */
		/* transfer STmin parameter to task ticks */
		if (pdu_fc_stmin < 0x80)
		{
			/* unit: 1MS */
			nt_stmin_tick = (nt_word_t)((pdu_fc_stmin * 10) / nt_task_cycle);
		}
		else if ((pdu_fc_stmin >= 0xF1) && (pdu_fc_stmin <= 0xF9))
		{
			/* unit: 100us */
			nt_stmin_tick = (nt_word_t)((pdu_fc_stmin & 0x0F) / nt_task_cycle);
		}
		else
		{
			/* undefined value, as max delay time 127ms */
			nt_stmin_tick = 1270 / pdu_fc_stmin;
		}
		/* <-- 20151222 Jason */
    }
    
    return set_result;
}

/*
 * Function:nt_tm_task
 * Param<void>:
 * Return<static void>:
 * REQ IDs:
 * Discription: NT timer task
 * Note: 
*/
static void nt_tm_task(void)
{
    /* tick over check */
	if(nt_sendErrRepTick  > 0)
	{
		nt_sendErrRepTick --;
		if(0 == nt_sendErrRepTick)
		{
			/*abort the last one*/
			nt_busErrInit();
			nt_init();
		}
		else
		{
			//hcan_ntTransmitErrInit();
			(void)nt_pdu_transmit(pdu_send_buf, NT_PDU_BUFFER_SIZE);
			nt_sendFrameErrJuding = TRUE;
		}	
	}
	else
	{
		if (nt_tm_tick > 0)
	    {
	        /* dec tick */
	        nt_tm_tick--;
	        /* timeout check */
	        if (0 == nt_tm_tick)
	        {
	            /* timeout call back */
	            nt_tm_timeout_cb(nt_tm_type);
	        }
	    }
	    if (nt_stmin_tick > 0)
	    {
	        nt_stmin_tick--;
	    }
	}
}

/*
 * Function:nt_tm_reset
 * Param<void>:
 * Return<static void>:
 * REQ IDs:
 * Discription: Reset all NT timer
 * Note: 
*/
static void nt_tm_reset(void)
{
    nt_tm_tick = 0; /* reset NT regular timer */
    nt_stmin_tick = 0; /* set special timer (STmin) for receive CF */
    nt_tm_type = NT_TM_IDLE; /* reset timer type to IDLE (No timer running type) */
}

