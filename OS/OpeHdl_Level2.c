

/* ---- Source switch on ------------------------------------------------------*/

#define OPE_HDL_LEVEL2_C_
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "OS_Types.h"
#include "OpeHdl_Level2.h"
#include "OSMain.h"
#include "EvtHdl.h"
#include "TmrHdl.h"
#include "mc33907.h"

#include "dataLink.h"
#include "ccp.h"
#include "uds.h"

#include "obd.h"

#include "pit.h"
#include "global_var.h"
#include "PWM.h"
#include "lin_msr.h"
#include "lin_slv.h"
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
uint32_t diff1_value = 0;

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
static os_byte_t opeEvtLevel2_task1(void);
static os_byte_t opeEvtLevel2_task2(void);
static os_byte_t opeEvtLevel2_task3(void);
static os_byte_t opeEvtLevel2_task4(void);
static os_byte_t opeEvtLevel2_task5(void);
static os_byte_t opeEvtLevel2_task6(void);
static os_byte_t opeEvtLevel2_task7(void);
static os_byte_t opeEvtLevel2_task8(void);
static os_byte_t opeEvtLevel2_task9(void);
static os_byte_t opeEvtLevel2_task10(void);
static os_byte_t opeEvtLevel2_task11(void);
static os_byte_t opeEvtLevel2_task12(void);
static os_byte_t opeEvtLevel2_task13(void);
static os_byte_t opeEvtLevel2_task14(void);
static os_byte_t opeEvtLevel2_task15(void);
static os_byte_t opeEvtLevel2_task16(void);

/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
os_byte_t OpeHdl_level2(void)
{
	os_byte_t Ret = OS_RET_OK;
	os_byte_t EvtNo;
	OS_DISABLE_INTERRUPT;
	EvtNo = EvtHdl_GetEvt(OS_EVT_TYPE_LEVEL2);
	switch(EvtNo)
	{
		case OS_TMR_LV2_TASK1:
			Ret = opeEvtLevel2_task1();
			break;
		case OS_TMR_LV2_TASK2:
			Ret = opeEvtLevel2_task2();
			break;
		case OS_TMR_LV2_TASK3:
			Ret = opeEvtLevel2_task3();
			break;
		case OS_TMR_LV2_TASK4:
			Ret = opeEvtLevel2_task4();
			break;
		case OS_TMR_LV2_TASK5:
			Ret = opeEvtLevel2_task5();
			break;
		case OS_TMR_LV2_TASK6:
			Ret = opeEvtLevel2_task6();
			break;
		case OS_TMR_LV2_TASK7:
			Ret = opeEvtLevel2_task7();
			break;
		case OS_TMR_LV2_TASK8:
			Ret = opeEvtLevel2_task8();
			break;
		case OS_TMR_LV2_TASK9:
			Ret = opeEvtLevel2_task9();
			break;
		case OS_TMR_LV2_TASK10:
			Ret = opeEvtLevel2_task10();
			break;
		case OS_TMR_LV2_TASK11:
			Ret = opeEvtLevel2_task11();
			break;
		case OS_TMR_LV2_TASK12:
			Ret = opeEvtLevel2_task12();
			break;
		case OS_TMR_LV2_TASK13:
			Ret = opeEvtLevel2_task13();
			break;
		case OS_TMR_LV2_TASK14:
			Ret = opeEvtLevel2_task14();
			break;
		case OS_TMR_LV2_TASK15:
			Ret = opeEvtLevel2_task15();
			break;
		case OS_TMR_LV2_TASK16:
			Ret = opeEvtLevel2_task16();
			break;
		default:
			break;
	}
	OS_ENABLE_INTERRUPT;
	return Ret;
}



/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/

/*10ms */
static os_byte_t opeEvtLevel2_task1(void)
{
	os_byte_t Ret = OS_RET_OK;

	hcan_recUnpackMain();
	return Ret;
}

/*10ms */
static os_byte_t opeEvtLevel2_task2(void)
{
	os_byte_t Ret = OS_RET_OK;
	return Ret;
}

/*10ms */
static os_byte_t opeEvtLevel2_task3(void)
{
	os_byte_t Ret = OS_RET_OK;
	return Ret;
}

/*10ms */
static os_byte_t opeEvtLevel2_task4(void)
{
	os_byte_t Ret = OS_RET_OK;
	hcan_txFinishMain();
	return Ret;
}

static os_byte_t opeEvtLevel2_task5(void)
{
	os_byte_t Ret = OS_RET_OK;
	ccpDaq(1);
	ccpBackground();
	return Ret;
}


/*10ms*/
static os_byte_t opeEvtLevel2_task6(void)
{
	os_byte_t Ret = OS_RET_OK;
	uds_task();
	return Ret;
}

/*10ms*/
static os_byte_t opeEvtLevel2_task7(void)
{
	os_byte_t Ret = OS_RET_OK;
	return Ret;
}

/*33907 sleep*/
static os_byte_t opeEvtLevel2_task8(void)
{
	os_byte_t Ret = OS_RET_OK;
	if(FALSE == hdio_readSig(e_di_ign_sw))
	{
		mc33907_setLpOff();
	}
	return Ret;
}

/*10ms*/
static os_byte_t opeEvtLevel2_task9(void)
{
	os_byte_t Ret = OS_RET_OK;
#if 0
	nm_s_task();
#endif
	return Ret;
}

static os_byte_t opeEvtLevel2_task10(void)
{
	os_byte_t Ret = OS_RET_OK;
	REM_DES = m_RGM.DES.R;
	REM_FES = m_RGM.FES.R;
	return Ret;
}

static os_byte_t opeEvtLevel2_task11(void)
{
	os_byte_t Ret = OS_RET_OK;
	return Ret;
}

/*2ms*/
static os_byte_t opeEvtLevel2_task12(void)
{
	os_byte_t Ret = OS_RET_OK;
	return Ret;
}

static os_byte_t opeEvtLevel2_task13(void)
{
	os_byte_t Ret = OS_RET_OK;
	//gateway_dispose();
	return Ret;
}

static os_byte_t opeEvtLevel2_task14(void)
{
	os_byte_t Ret = OS_RET_OK;
	pwm_update();
	return Ret;
}

static os_byte_t opeEvtLevel2_task15(void)
{
	os_byte_t Ret = OS_RET_OK;

	lin_msr_task();
	return Ret;
}




static os_byte_t opeEvtLevel2_task16(void)
{
	static uint32_t old_pit0_value = 0;
	static uint32_t now_pit0_value = 0;
	os_byte_t Ret = OS_RET_OK;
	old_pit0_value = pit_readCnt(2);
	AppTaskStep();
	now_pit0_value = pit_readCnt(2);
	if(old_pit0_value > now_pit0_value)
	{
		diff1_value = old_pit0_value - now_pit0_value;
	}
	else
	{
		diff1_value = old_pit0_value + 500000000 -  now_pit0_value;
	}
	diff1_value /= 50;

	return Ret;
}


/* ---- Source switch off ---------------------------------------------------- */
#undef OPE_HDL_LEVEL2_C_

/*--- End of file ------------------------------------------------------------*/
