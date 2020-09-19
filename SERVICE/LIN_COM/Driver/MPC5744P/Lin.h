
#ifndef MCAL_LIN_LIN_H_
#define MCAL_LIN_LIN_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "LinCfg.h"
/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/

#define LIN_MSG_MODE_PERIOD		0x00
#define LIN_MSG_MODE_EVENT		0x01
#define LIN_MSG_MODE_PE  		0x02

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/


typedef void (*p_lin_rx_msg_isr)(uint8_t, uint8_t, uint8_t *, uint8_t);
typedef void (*p_lin_rx_head_isr)(uint8_t, uint8_t);

/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/

typedef struct
{
	 p_lin_rx_msg_isr  rx_msg_cb;
	 p_lin_rx_head_isr rx_head_cb;
}lin_rx_isr_cb_t;


/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/
extern void lin_init(void);
extern void lin_master_sendHeader(uint8_t module,uint8_t id,uint8_t dlc);
extern bool lin_master_sendMsg(uint8_t module,uint8_t id, uint8_t * buf, uint8_t dlc,bool wait_over);
extern bool lin_slave_sendMsg(uint8_t module,uint8_t id, uint8_t * buf, uint8_t dlc,bool wait_over);
extern void lin_0_rxMsg_isr(void);
extern void lin_1_rxMsg_isr(void);
extern void lin_register_rx_msg_cb(uint8_t module,p_lin_rx_msg_isr pFun);
extern void lin_register_rx_head_cb(uint8_t module,p_lin_rx_head_isr pFun);



#endif

