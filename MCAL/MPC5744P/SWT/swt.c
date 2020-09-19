

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "MPC5744P.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "swt.h"

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

/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/
static void swt_swUnlock(void);
static void swt_writeServCode(uint16_t code);


/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/

void swt_uninit(void)
{
	SWT_0.CR.R &= 0xFFFFFFFEul;
}

void swt_init(void)
{
	SWT_CR_tag regCrShadow = {0};
	uint8_t waitTick = 100;

	if(1 != SWT_0.CR.B.HLK)
	{
		if(1 == SWT_0.CR.B.SLK)
		{
			swt_swUnlock();
		}
		while((1 == SWT_0.CR.B.SLK) && (waitTick > 0))
		{
			waitTick --;
			if(50 == waitTick)
			{
				swt_swUnlock();
			}
		}
		if(waitTick >0)
		{
			regCrShadow.R = SWT_0.CR.R;
			regCrShadow.B.SMD = m_swt_serviceWay;
			regCrShadow.B.WND =	m_swt_mode;
			regCrShadow.B.RIA =	m_swt_IAR;
			regCrShadow.B.ITR =	m_swt_ITR;
			regCrShadow.B.STP =	m_swt_stopInstopMode;
			regCrShadow.B.FRZ =	m_swt_stopInDebugMode;
			regCrShadow.B.WEN =	m_swt_en;

			SWT_0.TO.R = m_swt_timeoutCnt;
			
#if m_swt_mode == m_swt_winMode
			SWT_0.WN.R = m_swt_windowStartVal;
#endif
			SWT_0.CR.R = regCrShadow.R;
			
		}
		else
		{
			/*software unlock fail*/
		}

	}
	else
	{
		/*the software can do nothing this drive cycle, need to do power-off then power-on again*/
	}
}

void swt_feedDog(void)
{
	#if m_swt_serviceWay == m_swt_fixedService
	swt_writeServCode(0xA602);
	swt_writeServCode(0xB480);
	#else
	uint16_t servKey = 0;
	servKey = SWT_0.SK.B.SK;
	servKey = (uint16_t)((servKey * 17 + 3) & 0xFFFF);
	swt_writeServCode(servKey);
	servKey = (uint16_t)((servKey * 17 + 3) & 0xFFFF);
	swt_writeServCode(servKey);
	#endif
}




/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static void swt_swUnlock(void)
{
	swt_writeServCode(0xC520);
	swt_writeServCode(0xD928);
}


static void swt_writeServCode(uint16_t code)
{
	SWT_0.SR.R = code;
}



