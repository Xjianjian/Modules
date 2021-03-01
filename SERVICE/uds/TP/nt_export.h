/*
* Copyright (c) 2015,SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.:ACEC
* File:nt_export.h
* Description:
* REQ IDs:
* History:
* 2015-4-14, 1007040, original
*/

#ifndef __NT_EXPORT_H__
#define __NT_EXPORT_H__

#include "nt_types.h"

/* function export */
/* Get NT current state function */
extern nt_status_t nt_get_cur_state(void);
/* PDU (frame) receive call back function */
extern void nt_pdu_receive_cb(nt_bool_t func_addr, nt_byte_t nt_pdu_buf[], nt_byte_t buf_len);
/*
 * Send call back functions
 * You not need call thoese functions when As and Ar is zero
*/
extern void nt_pdu_send_ok_cb(void); /* for notice the send was success */
extern void nt_pdu_send_fail_cb(void); /* for notice the send was fail */
extern void nt_pdu_busErrStCb(nt_bool_t setOrClr);

/* export to upper layser */
/* export to init and task */
extern void nt_init(void);
extern void nt_task(void);
/* for request functions */
extern nt_word_t nt_req_len(void);
extern nt_byte_t nt_req_pop_dt(nt_byte_t def_val);
extern void nt_req_pos_reset(void);
extern void nt_req_pos_set(nt_word_t pos);
extern nt_word_t nt_get_req_pos(void);
extern nt_word_t nt_req_remain_len(void);
extern nt_bool_t nt_req_is_functional_check(void);

/* NT for upper layer response functions */
extern void nt_unlock(void);
extern nt_bool_t nt_is_locked(void);
extern nt_bool_t nt_rsp_push_dt(nt_byte_t val);
extern nt_bool_t nt_rsp_push_buf(nt_byte_t buf[], nt_word_t len);
extern nt_byte_t nt_rsp_get_data(nt_word_t rsp_pos);
extern nt_bool_t nt_rsp_set(nt_byte_t buf[], nt_word_t len);
extern nt_word_t nt_rsp_len(void);
extern nt_word_t nt_rsp_remain_len(void);
extern void nt_rsp_reset(void);
extern void nt_rsp_start(void);
/* NT for UDS special negtive response (response pending code 0x78) function */
extern void uds_rsp_pending(nt_byte_t serv_id);
extern nt_bool_t uds_rsp_is_pending(void);

#endif /*End of __NT_EXPORT_H__ */
/*--------EOF--------*/


