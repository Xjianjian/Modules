
#ifndef MCAL_LIN_LIN_CFG_H_
#define MCAL_LIN_LIN_CFG_H_
#include <math.h>
#include "MPC5744P.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "OS_Types.h"
#include "INTC.h"
#include "utils.h"
#include "SIUL.h"
#include "defines.h"
#include "sysClk.h"
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/

#define LIN_CFG_SYS_CLK	m_sysClk_cfg
/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/

typedef enum
{
	e_lin_0  = 0,
	e_lin_1,
	e_lin_max
}linCfg_lin_module_obj;

typedef struct
{
	uint8_t	module;
	uint8_t isen;
	volatile struct LINFlexD_tag * moduleTag;
	uint8_t	master;
	uint8_t c_checksum;
	uint32_t baud;
}linCfg_conf_t;


/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
extern const linCfg_conf_t	linCfg_conf[e_lin_max];
/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/
extern void linCfg_pinsInit(void);
extern void linCfg_INTCInterruptHandler(void);
extern uint8_t linCfg_HeaderIdMatch(uint8_t msgid);
#endif

