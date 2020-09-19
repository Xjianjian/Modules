

/* ---- Source switch on ------------------------------------------------------*/
#ifndef OS_MAIN_H_
#define OS_MAIN_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "OS_Types.h"

/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define OS_SYS_MODE_PRESAMPLE				0
#define OS_SYS_MODE_NORMAL					1
#define OS_SYS_MODE_PRESLEEP				2
#define OS_SYS_MODE_SLEEP					3
#define OS_SYS_MODE_STANDBY					4
//#define OS_SYS_MODE_LOWVOLTAGE				5

#define OS_SYS_TICK_BASE					(1)		/*unit:ms*/





#define NM_NONE_CAN_SLEEP					0x00
#define NM_MASTER_CAN_SLEEP					0x01
#define NM_SLAVE_CAN_SLEEP					0x02
#define NM_BOTH_CAN_SLEEP					0x03

#define VB_TO_ADC(x)	 ((x*1023)/5)

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/









/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/
extern os_byte_t sysPreSleep;

/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/
extern void OS_ModeManager(void);


#endif


