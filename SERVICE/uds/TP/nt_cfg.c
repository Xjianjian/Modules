/*
* Copyright (c) 2015,SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.:ACEC
* File:nt_cfg.c
* Description:
* REQ IDs:
* History:
* 2015-4-22, 1007040, original
*/

#include "nt_cfg.h"
/* User file include */
//#include "can_cfg.h"
//#include "can.h"
//#include "uds_cfg.h"
//#include "uds.h"

/* ------------------ NT Layer parameter config ------------------- */
/* Config paramters
 * Those parameters is NT necessary.
 * You must not change those parameters(include type, name)
 * But you can change the parameters set value */

/* NT task call cycle (Based 100us) */
const nt_word_t       nt_task_cycle = PARAM_NT_TASK_CYCLE;

/* external addressing mode parameters */
const nt_byte_t nt_npci_start_pos  = PARAM_NT_NPDU_START_POS;
const nt_byte_t nt_ext_rsp_addr    = PARAM_NT_RSP_EXT_ADDR;

const nt_word_t nt_busErrRepMaxTime = PARAM_NT_AUTO_REP_TIME;
/* network layer timing parameters
 * Note: This const array can used volatile array to allowed up layer modify.
*/
const nt_word_t       nt_tm_params[NT_TM_TYPES_COUNT] = 
{
    0, /* NT_TM_IDLE */
    PARAM_NT_TM_Br,     /* NT_TM_Br */
    PARAM_NT_TM_Cr,     /* NT_TM_Cr */
    PARAM_NT_TM_Bs,     /* NT_TM_Bs */
    PARAM_NT_TM_Cs_Red, /* NT_TM_Cs: STmin + this value */
    PARAM_NT_TM_Ar,     /* NT_TM_Ar */
    PARAM_NT_TM_As      /* NT_TM_As */
};
/* N_PDU un-used data byte fill */
const nt_byte_t      pdu_fill_byte = PARAM_PDU_FILL_BYTE;
/* FC parameter when receive segmented frame (send FC after receive FF) */
const nt_byte_t      pdu_fc_bs     = PARAM_PDU_FC_BS; /* FC Blocksize */
const nt_byte_t      pdu_fc_stmin  = PARAM_PDU_FC_STMIN; /* FC STmin */
/* for UDS upper layer */
const nt_word_t       uds_rsp_pd_cycle = PRAM_UDS_RSP_PD_CYCLE; /* UDS application layer response pending message repeat send cycle */
/* NT layer buffer */
nt_byte_t app_receive_buf[PARAM_APP_RECEIVE_BUF_SIZE]; /* receive buffer */
const nt_word_t app_receive_buf_len = PARAM_APP_RECEIVE_BUF_SIZE;
nt_byte_t app_send_buf[PARAM_APP_SEND_BUF_SIZE]; /* transmit buffer */
const nt_word_t app_send_buf_len = PARAM_APP_SEND_BUF_SIZE;
/* ------------------ End NT Layer parameter config --------------- */



/* Functions after this line, iNT un-necessary functions. Only an example for notice NT a frame was be received. */

#if 0 /* Examples for UDS support functions */
/*
 * Function:uds_read_request_msg
 * Param<uint8_t default_data>:
 * Return<uint8_t uds_read_request_msg>:
 * REQ IDs:
 * Discription:
 * Note: 
*/
nt_byte_t uds_read_request_msg (nt_byte_t default_data)
{
    return nt_req_pop_dt(default_data);
}

/*
 * Function:uds_transmit_response_msg
 * Param<nt_word_t uds_data_length>:
 * Param<const uint8_t * uds_data>:
 * Return<void uds_transmit_response_msg>:
 * REQ IDs:
 * Discription:
 * Note: 
*/
void uds_transmit_response_msg (nt_word_t uds_data_length, nt_byte_t *uds_data)
{
    (void)nt_rsp_set(uds_data, uds_data_length);
    nt_rsp_start();
}

/*
 * Function:uds_negative_response
 * Param<uint8_t service>:
 * Param<uint8_t response_code>:
 * Return<void uds_negative_response>:
 * REQ IDs:
 * Discription:
 * Note: 
*/
void uds_negative_response (uint8_t service, uint8_t response_code)
{
    uint8_t array_u8_response [3];

    array_u8_response[0] = NEGATIVE_RESPONSE;
    array_u8_response[1] = service;
    array_u8_response[2] = response_code;
    uds_transmit_response_msg (3u,array_u8_response);
}

void uds_response_pending(uint8_t service)
{
    UDS_RECEIVE_VALID_REQUEST();
    uds_rsp_pending((nt_byte_t)service);
}
/*
 * Function:uds_no_response
 * Param<void>:
 * Return<void uds_no_response>:
 * REQ IDs:
 * Discription:
 * Note: 
*/
void uds_no_response (void)
{
    UDS_RECEIVE_VALID_REQUEST();
    nt_unlock();
}
#endif


