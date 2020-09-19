
/* ---- Source switch on ------------------------------------------------------*/

#define OPE_HDL_LEVEL3_C_
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "OS_Types.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "EvtHdl.h"
#include "TmrHdl.h"
#include "OpeHdl_Level3.h"
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
unsigned char test_bccCnt = 0;





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
static os_byte_t opeEvtLevel3_task1(void);
static os_byte_t opeEvtLevel3_task2(void);
static os_byte_t opeEvtLevel3_task3(void);
static os_byte_t opeEvtLevel3_task4(void);

/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
os_byte_t OpeHdl_level3(void)
{
	os_byte_t Ret = OS_RET_OK;
	os_byte_t EvtNo;
	OS_DISABLE_INTERRUPT;
	EvtNo = EvtHdl_GetEvt(OS_EVT_TYPE_LEVEL3);
	switch(EvtNo)
	{
		case OS_TMR_LV3_TASK1:
			Ret = opeEvtLevel3_task1();
			break;
		case OS_TMR_LV3_TASK2:
			Ret = opeEvtLevel3_task2();
			break;
		case OS_TMR_LV3_TASK3:
			Ret = opeEvtLevel3_task3();
			break;
		case OS_TMR_LV3_TASK4:
			Ret = opeEvtLevel3_task4();
			break;
	}
	OS_ENABLE_INTERRUPT;
	return Ret;
}


/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static os_byte_t opeEvtLevel3_task1(void)
{
	os_byte_t Ret = OS_RET_OK;
#if m_rtc_wkup
	rv3149c3_cleanINT();
#endif
	return Ret;
}







static os_byte_t opeEvtLevel3_task2(void)
{
	os_byte_t Ret = OS_RET_OK;

	return Ret;
}

static os_byte_t opeEvtLevel3_task3(void)
{
	os_byte_t Ret = OS_RET_OK;
	return Ret;
}

static os_byte_t opeEvtLevel3_task4(void)
{
	os_byte_t Ret = OS_RET_OK;
	//bus_detect_dispose();
	return Ret;
}





/* ---- Source switch off ---------------------------------------------------- */
#undef OPE_HDL_LEVEL3_C_

/*--- End of file ------------------------------------------------------------*/
