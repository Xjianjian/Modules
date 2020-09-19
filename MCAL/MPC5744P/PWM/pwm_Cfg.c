

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "MPC5744P.h"
#include "SIUL.h"
#include "defines.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "pwm_Cfg.h"
/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/


const PwmCfg_t PwmCfgGrp[m_pwm_outChannels] =
{
	{0,	FALSE,	SIUL_PG3},
	{0,	FALSE,	SIUL_PG5},
	{0,	FALSE,	SIUL_PG4},
	{0,	FALSE,	SIUL_PD12}
};

uint8_t *  pwm_duty_grp[m_pwm_outChannels] =
{
	&hld_po_fan_CONTROL,&hld_po_pump1_CONTROL,&hld_po_pump2_CONTROL,&hld_po_pump3_CONTROL
};
/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/

