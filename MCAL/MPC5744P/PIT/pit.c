
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "MPC5744P.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "pit.h"
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

/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void pit_start(void)
{
	PIT_0.MCR.B.MDIS = FALSE;
}


void pit_stop(void)
{
	PIT_0.MCR.B.MDIS = TRUE;
}


void pit_frz(void)
{
	PIT_0.MCR.B.FRZ = TRUE;
}

void pit_free(void)
{
	PIT_0.MCR.B.FRZ = FALSE;
}

void pit_enChan(uint8_t chan)
{
	if(chan < m_pit_chanNum)
	{
		PIT_0.TIMER[chan].TCTRL.B.TEN = TRUE;
	}
}

void pit_disChan(uint8_t chan)
{
	if(chan < m_pit_chanNum)
	{
		PIT_0.TIMER[chan].TCTRL.B.TIE = FALSE;
		PIT_0.TIMER[chan].TCTRL.B.TEN = FALSE;
		PIT_0.TIMER[chan].TFLG.R = 1;
	}
}


void pit_setLoadVal(uint8_t chan,uint32_t periodVal)
{
	if(chan < m_pit_chanNum)
	{
		PIT_0.TIMER[chan].LDVAL.R = periodVal;
	}
}

uint32_t pit_readCnt(uint8_t chan)
{
	uint32_t cnt = 0;
	if(chan < m_pit_chanNum)
	{
		cnt = PIT_0.TIMER[chan].CVAL.R;
	}
	return cnt;
}

void pit_setChanMode(uint8_t chan,bool chain,bool isrEn,bool start)
{
	if(chan < m_pit_chanNum)
	{
		if(chan >0)
		{
			PIT_0.TIMER[chan].TCTRL.B.CHN = ((TRUE == chain)?(1):(0));
		}
		else
		{
			PIT_0.TIMER[chan].TCTRL.B.CHN = 0;
		}
		PIT_0.TIMER[chan].TCTRL.B.TIE = ((TRUE == isrEn)?(1):(0));
		PIT_0.TIMER[chan].TCTRL.B.TEN = ((TRUE == start)?(1):(0));
		PIT_0.TIMER[chan].TFLG.B.TIF = 1;
	}
}



void pit_clrIsrFlag(uint8_t chan)
{
	if(chan < m_pit_chanNum)
	{
		 PIT_0.TIMER[chan].TFLG.B.TIF = 1;
	}
}

bool pit_getIsrFlag(uint8_t chan)
{
	bool isrFlag = FALSE;
	if(chan < m_pit_chanNum)
	{
		isrFlag = PIT_0.TIMER[chan].TFLG.B.TIF;
	}
	return isrFlag;
}

/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/


