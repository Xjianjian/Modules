

/* ---- Source switch on ------------------------------------------------------*/
#ifndef ETIMER_H_
#define ETIMER_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "eTimerCfg.h"

/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define mc_etim_singInCaptCnt			0

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/
extern uint16_t pi_1;
/*Message Name:pi_display_msg1	 start bit:56	 bit len:16	 format:Motorola_Forward_Lsb*/
extern uint16_t pi_2;
/*Message Name:pi_display_msg2	 start bit:8	 bit len:16	 format:Motorola_Forward_Lsb*/
extern uint16_t pi_3;
/*Message Name:pi_display_msg2	 start bit:24	 bit len:16	 format:Motorola_Forward_Lsb*/
extern uint16_t pi_4;
/*Message Name:pi_display_msg2	 start bit:40	 bit len:16	 format:Motorola_Forward_Lsb*/
extern uint16_t pi_5;



/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/
extern void etim_inCaptCasInit(void);
extern void etim_inputCaptHandle(void);
extern uint16_t etim_pi_getDuty(uint8_t indx);

#endif


