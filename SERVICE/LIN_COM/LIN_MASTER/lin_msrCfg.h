
#ifndef LIN_MSTR__CFG_H_
#define LIN_MSTR__CFG_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"

/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/

#define	LIN_MODULE_MASTER_NUM	2
#define LIN_CFG_MSR_MSG_CNT	2

#define LIN0_TX_MSG_CNT	2
#define LIN0_RX_MSG_CNT	2
#define LIN0_SCHEDULE_CNT	(LIN0_TX_MSG_CNT + LIN0_RX_MSG_CNT)

#define LIN1_TX_MSG_CNT	1
#define LIN1_RX_MSG_CNT	1
#define LIN1_SCHEDULE_CNT	(LIN1_TX_MSG_CNT + LIN1_RX_MSG_CNT)

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef struct
{
	uint8_t txbuf[8];
	uint8_t txOk;
}lin_msr_txbuf_t;

typedef struct
{
	uint8_t rxbuf[8];
	uint8_t received;
	uint8_t timeout;
}lin_msr_rxbuf_t;

typedef struct
{
	uint8_t 			id;
	uint8_t 			tx;
	uint8_t 			dlc;
	lin_msr_txbuf_t 	*tbuf;
	lin_msr_rxbuf_t 	*rbuf;
	void				(*pack_unpack)(uint8_t,uint8_t);
	void				(*cb)(uint8_t,uint8_t);
}lin_msr_msg_cfg_t;


typedef struct
{
	uint8_t is_tx;
	uint8_t p_msg_indx;		/*the index not less than msg_cnt means invalid*/
	uint8_t	p_time_gap;
}lin_msr_schedule_t;

typedef struct
{
	uint8_t module;
	uint8_t isen;
	uint8_t	txnum;
	uint8_t	rxnum;
	const lin_msr_msg_cfg_t* txmsg;
	const lin_msr_msg_cfg_t* rxmsg;
	const lin_msr_schedule_t* scheduleList;
}lin_msr_t;

/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
extern const lin_msr_t lin_msr[LIN_MODULE_MASTER_NUM];

/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/

#endif

