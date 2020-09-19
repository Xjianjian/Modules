

/* ---- Source switch on ------------------------------------------------------*/

#define OPE_HDL_LEVEL1_C_
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "OS_Types.h"
#include "OSMain.h"
#include "EvtHdl.h"
#include "TmrHdl.h"

#include "swt.h"
#include "hai.h"
#include "hdio.h"
#include "eTimer.h"


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
static os_byte_t opeEvtLevel1_task1(void);
static os_byte_t opeEvtLevel1_task2(void);
static os_byte_t opeEvtLevel1_task3(void);
static os_byte_t opeEvtLevel1_task4(void);




/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
os_byte_t OpeHdl_level1(void)
{
	os_byte_t Ret = OS_RET_OK;
	os_byte_t EvtNo;
	OS_DISABLE_INTERRUPT;
	EvtNo = EvtHdl_GetEvt(OS_EVT_TYPE_LEVEL1);
	switch(EvtNo)
	{
		case OS_TMR_LV1_TASK1:
			Ret = opeEvtLevel1_task1();
			break;
		case OS_TMR_LV1_TASK2:
			Ret = opeEvtLevel1_task2();
			break;
		case OS_TMR_LV1_TASK3:
			Ret = opeEvtLevel1_task3();
			break;
		case OS_TMR_LV1_TASK4:
			Ret = opeEvtLevel1_task4();
			break;
	}
	OS_ENABLE_INTERRUPT;
	return Ret;
}



/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static os_byte_t opeEvtLevel1_task1(void)
{
	os_byte_t Ret = OS_RET_OK;
	swt_feedDog();
	return Ret;
}

static os_byte_t opeEvtLevel1_task2(void)
{
	os_byte_t Ret = OS_RET_OK;
	hai_adc_sampleTask();
	etim_inputCaptHandle();
	return Ret;
}

static os_byte_t opeEvtLevel1_task3(void)
{
	static os_byte_t crc8Checksum;
	static os_byte_t crc8Data[7] = {0x90,0x91,0x92,0x93,0x94,0x95,0x96};
	os_byte_t Ret = OS_RET_OK;
	hdio_diSampleCyclic();
	//mc33907_diagCan2();
	//crc8_creat_table();
	//crc8Checksum = crc8_calBytes_checksum(&crc8Data[6],7);
	return Ret;
}

static os_byte_t opeEvtLevel1_task4(void)
{
	os_byte_t Ret = OS_RET_OK;
	OS_ModeManager();
	return Ret;
}





/* ---- Source switch off ---------------------------------------------------- */
#undef OPE_HDL_LEVEL1_C_

/*--- End of file ------------------------------------------------------------*/
