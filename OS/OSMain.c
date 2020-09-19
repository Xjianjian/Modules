

/* ---- Source switch on ------------------------------------------------------*/

#define OS_MAIN_C_
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "OS_Types.h"
#include "global_var.h"
#include "OSMain.h"
#include "EvtHdl.h"
#include "TmrHdl.h"
#include "HSAEPlatformTask.h"

#include "OpeHdl_Level1.h"
#include "OpeHdl_Level2.h"
#include "OpeHdl_Level3.h"
#include "OpeHdl_Level4.h"

#include "fee.h"

#include "ccpCanInterface.h"
#include "obd.h"

#include "global_var.h"
#include "pitCfg.h"
#include "bd8la700driver.h"
#include "MemIf.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/
static os_byte_t OsSysMode = OS_SYS_MODE_PRESAMPLE;
static os_word_t OSSysTick = 0;

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
static void OSInitSw(void);
static void OSDeinitHw(void);
static void OSLeaveMode(os_byte_t mode);
static void OSEnterMode(os_byte_t mode);
static void OSPowerOnInit(void);


/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void OS_Main(void)
{
	os_byte_t Prio;

	OSPowerOnInit();
	OSEnterMode(OS_SYS_MODE_PRESAMPLE);
	for(;;)
	{
		Prio = 	EvtHdl_CalStdTaskPrio();
		if(OS_EVT_PRIO_LEVEL1 <= Prio)
		{
			if(OS_RET_OK != OpeHdl_level1())
			{
				break;
			}
		}
		else if(OS_EVT_PRIO_LEVEL2 <= Prio)
		{
			if(OS_RET_OK != OpeHdl_level2())
			{
				break;
			}
		}
		else if(OS_EVT_PRIO_LEVEL3 <= Prio)
		{
			if(OS_RET_OK != OpeHdl_level3())
			{
				break;
			}
		}
		else
		{
			/*No Normal Task*/
		}

	}
	OS_DISABLE_INTERRUPT;
	OSDeinitHw();

}

void OS_ModeManager(void)
{
	switch(OsSysMode)
	{
		case OS_SYS_MODE_PRESAMPLE:
			if(OSSysTick >0)
			{
				OSSysTick --;
				if(0 == OSSysTick)
				{
					if(TRUE == hdio_readSig(e_di_ign_sw))
					{
						hdio_writeSig(e_do_12v_digital_pwr,1);
						hdio_writeSig(e_do_ps_en,1);
						hdio_writeSig(e_do_lin_mode,1);
						hdio_writeSig(e_do_lin_wake,1);
					}
					OSLeaveMode(OsSysMode);
					OSEnterMode(OS_SYS_MODE_NORMAL);
				}
			}
			else
			{}
			break;
		case OS_SYS_MODE_NORMAL:
			if(0x03 == sysPreSleep && FALSE == hdio_readSig(e_di_ign_sw))
			{
				OSSysTick --;
				if(0 == OSSysTick)
				{
					sysPreSleep = 0x00;
					OSLeaveMode(OsSysMode);
					OSEnterMode(OS_SYS_MODE_PRESLEEP);
				}
				else
				{}
			}
			else
			{
				OSSysTick = 1000/OS_SYS_TICK_BASE;/*1000ms*/
			}
			break;
		case OS_SYS_MODE_PRESLEEP:

			if(OSSysTick > 0)
			{
				OSSysTick --;
			}
			if(0 == OSSysTick)
			{
				hdio_writeSig(e_do_lin_mode,0);
				hdio_writeSig(e_do_lin_wake,0);
				hdio_writeSig(e_do_ps_en,0);
				hdio_writeSig(e_do_12v_digital_pwr,0);
				OSLeaveMode(OsSysMode);
				OSEnterMode(OS_SYS_MODE_SLEEP);
			}
			break;
		case OS_SYS_MODE_SLEEP:
			if(TRUE == hdio_readSig(e_di_ign_sw))
			{
				OSPowerOnInit();
				OSEnterMode(OS_SYS_MODE_PRESAMPLE);
			}
			else if(OSSysTick > 0)
			{
				OSSysTick --;
				if(0 == OSSysTick)
				{
					OSLeaveMode(OsSysMode);
					OSEnterMode(OS_SYS_MODE_STANDBY);
				}
			}
			break;
		case OS_SYS_MODE_STANDBY:
			if(TRUE == hdio_readSig(e_di_ign_sw))
			{
				OSPowerOnInit();
				OSEnterMode(OS_SYS_MODE_PRESAMPLE);
			}
			break;
#if 0
		case OS_SYS_MODE_LOWVOLTAGE:
			if(0x03 == sysPreSleep)
			{
				sysPreSleep = 0x00;
				OSLeaveMode(OsSysMode);
				OSEnterMode(OS_SYS_MODE_PRESLEEP);
			}
			/*VCU send message when vVB_bios equal or bigger than 7V,,or do not send message when vVB_bios equal or less than 6.5V*/
			if(vVB_bios >= VB_TO_ADC(7))/*7V*/
			{
				if(0 < OSSysTick)
				{
					OSSysTick--;
					if(0 == OSSysTick)
					{
						OSPowerOnInit();
						OSEnterMode(OS_SYS_MODE_PRESAMPLE);
					}
				}
				else
				{
					OSSysTick = 500/OS_SYS_TICK_BASE;
				}
			}
			break;
#endif
	}
}



/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static void OSInitSw(void)
{

	os_appVarInit();

	OS_DISABLE_INTERRUPT;
	EvtHdl_Init();
	TmrHdl_init();
	OS_ENABLE_INTERRUPT;
}

static void OSPowerOnInit(void)
{
	os_hwInit();
	OSInitSw();

	pit_init();

}

static void OSDeinitHw(void)
{


}


static void OSLeaveMode(os_byte_t mode)
{

}


static void OSEnterMode(os_byte_t mode)
{
	switch (mode)
	{
		case OS_SYS_MODE_PRESAMPLE:
			OsSysMode = OS_SYS_MODE_PRESAMPLE;
			OSSysTick = 50/OS_SYS_TICK_BASE;
			TmrHdl_switchSession(STA_TYPE_STATE_PRESAMPLE);
			break;
		case OS_SYS_MODE_NORMAL:
			OsSysMode = OS_SYS_MODE_NORMAL;
			OSSysTick = 1000/OS_SYS_TICK_BASE;/*10000ms*/
			TmrHdl_switchSession(STA_TYPE_STATE_NORMAL);
			break;
		case OS_SYS_MODE_PRESLEEP:
			OsSysMode = OS_SYS_MODE_PRESLEEP;
			OSSysTick = 100/OS_SYS_TICK_BASE;
			TmrHdl_switchSession(STA_TYPE_STATE_PRESLEEP);
			break;
		case OS_SYS_MODE_SLEEP:
			OsSysMode = OS_SYS_MODE_SLEEP;
			OSSysTick = 5000/OS_SYS_TICK_BASE;
			//obd_shutdown();
			MemIf_mainFunction();
			TmrHdl_switchSession(STA_TYPE_STATE_SLEEP);
			break;
		case OS_SYS_MODE_STANDBY:
			OsSysMode = OS_SYS_MODE_STANDBY;
			TmrHdl_switchSession(STA_TYPE_STATE_FAIL);
			break;
#if 0
		case OS_SYS_MODE_LOWVOLTAGE:
			OsSysMode = OS_SYS_MODE_LOWVOLTAGE;
			OSSysTick = 500/OS_SYS_TICK_BASE;
			TmrHdl_switchSession(STA_TYPE_STATE_LOWVOLTAGE);
			break;
#endif
	}
}

/* ---- Source switch off ---------------------------------------------------- */
#undef OS_MAIN_C_

/*--- End of file ------------------------------------------------------------*/
