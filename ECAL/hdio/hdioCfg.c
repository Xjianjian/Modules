
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "dio.h"
#include "global_var.h"
#include "hdioCfg.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/
/**********************************************/
//					 input signals
/**********************************************/
const hdioCfg_di_t  diCfgGrp[e_di_max] =
{   /* chanId*/	/* dir*/	/* pullType*/		/* safeMode*/	/* ioStd*/	/* slewFast*/ /* openDrain*/  /* readDeb*/
	{SIUL_PA11, m_dio_input,m_dio_pullDisable,	FALSE,			 TRUE,		 TRUE,			FALSE,		  	FALSE},
	{SIUL_PA12, m_dio_input,m_dio_pullDisable,	FALSE,			 TRUE,		 TRUE,			FALSE,			FALSE},
	{SIUL_PD6,  m_dio_input,m_dio_pullDisable,	FALSE,			 TRUE,		 TRUE,			FALSE,			FALSE},
	{SIUL_PF4,  m_dio_input,m_dio_pullDisable,	FALSE,			 TRUE,		 TRUE,			FALSE,		  	FALSE},
	{SIUL_PE13, m_dio_input,m_dio_pullDisable,	FALSE,			 TRUE,		 TRUE,			FALSE,		  	FALSE},
	{SIUL_PA9,  m_dio_input,m_dio_pullDisable,	FALSE,			 TRUE,		 TRUE,			FALSE,		  	FALSE},
	{SIUL_PD3,  m_dio_input,m_dio_pullDisable,	FALSE,			 TRUE,		 TRUE,			FALSE,			FALSE},
	{SIUL_PD8,  m_dio_input,m_dio_pullDisable,	FALSE,			 TRUE,		 TRUE,			FALSE,			FALSE},
	{SIUL_PE15, m_dio_input,m_dio_pullDisable,	FALSE,			 TRUE,		 TRUE,			FALSE,		 	FALSE},
	{SIUL_PA10, m_dio_input,m_dio_pullDisable,	FALSE,			 TRUE,		 TRUE,			FALSE,			FALSE},
	{SIUL_PD5,  m_dio_input,m_dio_pullDisable,	FALSE,			 TRUE,		 TRUE,			FALSE,			FALSE},
	{SIUL_PD4,  m_dio_input,m_dio_pullDisable,	FALSE,			 TRUE,		 TRUE,			FALSE,			FALSE},
	{SIUL_PE14, m_dio_input,m_dio_pullDisable,	FALSE,			 TRUE,		 TRUE,			FALSE,			FALSE},
	{SIUL_PB6,  m_dio_input,m_dio_pullDisable,	FALSE,			 TRUE,		 TRUE,			FALSE,			FALSE},
	{SIUL_PD2,  m_dio_input,m_dio_pullDisable,	FALSE,			 TRUE,		 TRUE,			FALSE,			FALSE}
};


/**********************************************/
//					 output signals
/**********************************************/
const hdioCfg_do_t  doCfgGrp[e_do_max] =
{	/* chanId*/ 	/* dir*/	/* pullType*/	/* safeMode*/	/* ioStd*/	/* slewFast*/	/* openDrain*/ /* writeCheck*/	/* initVal*/
	{SIUL_PG6,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PF6,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PJ8,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PF0,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PD0,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PC15,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PF13,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PA0,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PA1,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PG11,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PG7,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PG9,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PD11,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PG10,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PC4,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PD7,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PG8,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PF7,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PC11,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PC10,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PA3,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PF10,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
	{SIUL_PF9,	m_dio_output,	m_dio_pullUp,	FALSE,			TRUE,		TRUE,			FALSE,			FALSE,			FALSE},
};



/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/



