
#ifndef LIN_SLV_CFG_H_
#define LIN_SLV_CFG_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"

/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/

#define LIN_SLAVE_MODULE_NUM	2


#define LIN0_SLV_TX_MSG_CNT	1
#define LIN0_SLV_RX_MSG_CNT	1

#define LIN1_SLV_TX_MSG_CNT	1
#define LIN1_SLV_RX_MSG_CNT	1
/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef struct
{
	uint8_t txbuf[8];
	uint8_t txOk;
}lin_slv_txbuf_t;

typedef struct
{
	uint8_t rxbuf[8];
	uint8_t received;
	uint8_t timeout;
}lin_slv_rxbuf_t;

typedef struct
{
	uint8_t 			id;
	uint8_t 			tx;
	uint8_t 			dlc;
	lin_slv_txbuf_t 	*tbuf;
	lin_slv_rxbuf_t		*rbuf;
	void				(*pack_unpack)(uint8_t,uint8_t);
	void				(*cb)(uint8_t,uint8_t);
}lin_slv_msg_t;

typedef struct
{
	uint8_t module;
	uint8_t isen;
	uint8_t txnum;
	uint8_t rxnum;
	const lin_slv_msg_t*	txmsg;
	const lin_slv_msg_t*	rxmsg;
}lin_slv_t;

/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
extern const lin_slv_t lin_slv[LIN_SLAVE_MODULE_NUM];


/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/


#endif

