/*
* Copyright (c) 2015,SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.:ACEC
* File:nt_import.h
* Description:
* REQ IDs:
* History:
* 2015-4-14, 1007040, original
*/

#ifndef __NT_IMPORT_H__
#define __NT_IMPORT_H__

#include "nt_types.h"




/* function import */
/* for under layer (CAN driver) */
extern nt_bool_t nt_pdu_transmit(nt_byte_t nt_pdu_buf[], nt_byte_t buf_len);
extern void nt_busErrInit(void);
/* for uper layer (Application service) */
extern void nt_request_cb(void);

/* variables import */
extern const nt_byte_t nt_npci_start_pos;
extern const nt_byte_t nt_ext_rsp_addr; /* externd address for response */

/* Parameters import */
/* Config paramters */
/* network layer timing parameters
 * Note: This const array can used volatile array to allowed up layer modify.
*/
extern const nt_word_t nt_busErrRepMaxTime;
extern const nt_word_t      nt_tm_params[NT_TM_TYPES_COUNT];
extern const nt_byte_t      pdu_fill_byte;
extern const nt_byte_t      pdu_fc_bs; /* FC Blocksize */
extern const nt_byte_t      pdu_fc_stmin; /* FC STmin */
extern const nt_word_t      nt_task_cycle;

/* UDS application layer special parameters */
extern const nt_word_t       uds_rsp_pd_cycle;

/* NT Rx, Tx Buffer define (for application used data buffer) */
extern nt_byte_t app_receive_buf[];
extern const nt_word_t app_receive_buf_len;
extern nt_byte_t app_send_buf[];
extern const nt_word_t app_send_buf_len;
#define APP_RECEIVE_BUF_SIZE app_receive_buf_len
#define APP_SEND_BUF_SIZE    app_send_buf_len

#endif /*End of __NT_IMPORT_H__ */
/*--------EOF--------*/


