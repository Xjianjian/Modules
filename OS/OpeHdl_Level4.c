

/* ---- Source switch on ------------------------------------------------------*/

#define OPE_HDL_LEVEL4_C_
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "OS_Types.h"
#include "OSMain.h"
#include "EvtHdl.h"
#include "TmrHdl.h"

#include "dataLink.h"
#include "hdio.h"
#include "HardwareDogManager.h"

#include "global_var.h"
#include "bd8la700driver.h"

#include "bd8la700driver.h"
#include "bd8la700driver_private.h"
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
                                Global data at RAM
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/
static os_byte_t opeEvtLevel4_task1(void);
static os_byte_t opeEvtLevel4_task2(void);
static os_byte_t opeEvtLevel4_task3(void);
static os_byte_t opeEvtLevel4_task4(void);


/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
os_byte_t OpeHdl_level4(void)
{
	os_byte_t Ret = OS_RET_OK;
	os_byte_t EvtNo;
	OS_DISABLE_INTERRUPT;
	EvtNo = EvtHdl_GetEvt(OS_EVT_TYPE_LEVEL4);
	OS_ENABLE_INTERRUPT;
	switch(EvtNo)
	{
		case OS_TMR_LV4_TASK1:
			Ret = opeEvtLevel4_task1();
			break;
		case OS_TMR_LV4_TASK2:
			Ret = opeEvtLevel4_task2();
			break;
		case OS_TMR_LV4_TASK3:
			Ret = opeEvtLevel4_task3();
			break;
		case OS_TMR_LV4_TASK4:
			Ret = opeEvtLevel4_task4();
			break;
	}
	OS_ENABLE_INTERRUPT;
	return Ret;
}



/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static os_byte_t opeEvtLevel4_task1(void)
{
	os_byte_t Ret = OS_RET_OK;
	if(TRUE == hdio_readSig(e_di_ign_sw))
	{
		hcan_classCan_txPackMain();
	}

	return Ret;
}

static os_byte_t opeEvtLevel4_task2(void)
{
	os_byte_t Ret = OS_RET_OK;
	if(TRUE == hdio_readSig(e_di_ign_sw))
	{
		hcan_ptCan_txPackMain();
		hcan_reservedCan_txPackMain();
	}

	return Ret;
}

static os_byte_t opeEvtLevel4_task3(void)
{
	os_byte_t Ret = OS_RET_OK;

	bd8la700_update();
	return Ret;
}

static os_byte_t opeEvtLevel4_task4(void)
{
	os_byte_t Ret = OS_RET_OK;
	hardWareDogManager();
	return Ret;
}




/* ---- Source switch off ---------------------------------------------------- */
#undef OPE_HDL_LEVEL4_C_

/*--- End of file ------------------------------------------------------------*/
