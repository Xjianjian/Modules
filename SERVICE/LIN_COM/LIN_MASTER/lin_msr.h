
/* ---- Source switch on ------------------------------------------------------*/
#ifndef LIN_MSTR_H_
#define LIN_MSTR_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "Lin.h"
#include "lin_msrCfg.h"

/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/






/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
#if LIN_0_MODULE_ENABLE
extern const lin_msg_cfg_t lin0_msg_cfg_grp[LIN_COMMCFG_MSG_CNT];
extern const uint16_t lin0_msg_cnt;
extern const lin_schedule_t lin0_schedule;
extern const uint16_t lin0_schedule_item_cnt;
#endif


#if LIN_1_MODULE_ENABLE
extern const lin_msg_cfg_t lin1_msg_cfg_grp[];
extern const uint16_t lin1_msg_cnt;
extern const lin_schedule_t lin1_schedule;
extern const uint16_t lin1_schedule_item_cnt;

#endif
/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/
extern void lin_msr_init(void);
extern void lin_msr_task(void);

#endif

