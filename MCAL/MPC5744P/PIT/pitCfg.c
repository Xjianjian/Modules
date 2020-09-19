
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "MPC5744P.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "INTC.h"
#include "pit.h"
#include "pitCfg.h"
#include "HardwareDogManager.h"

void pit_isr0(void);
void pit_isr1(void);
void pit_isr2(void);
void pit_isr3(void);

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/
#define m_pit_cfg		(pitCfgGrp[chanIndx])


const pitCfg_t pitCfgGrp[m_pit_chanNum] = 
{
	{
		TRUE,		/*start*/
		FALSE,		/*chain*/
		FALSE,		/*isrEn*/
		10,			/*isrPrio*/
		&pit_isr0,	/*isrFun*/
		140000,		/*period*/
	},
	{
		TRUE,		/*start*/
		FALSE,		/*chain*/
		TRUE,		/*isrEn*/
		11,			/*isrPrio*/
		&pit_isr1,	/*isrFun*/
		1000,		/*period*/
	},
	{
		TRUE,		/*start*/
		FALSE,		/*chain*/
		FALSE,		/*isrEn*/
		12,			/*isrPrio*/
		&pit_isr2,	/*isrFun*/
		10000000,	/*period*//*for test application time*/
	},
	{
		FALSE,		/*start*/
		FALSE,		/*chain*/
		FALSE,		/*isrEn*/
		13,			/*isrPrio*/
		&pit_isr3,	/*isrFun*/
		50000,		/*period*/
	}
};

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
void pit_init(void)
{
	bool timerEn = FALSE;
	uint8_t chanIndx = 0;
	for(;chanIndx < m_pit_chanNum; chanIndx ++)
	{
		timerEn |= m_pit_cfg.start;
		pit_setLoadVal(chanIndx,m_pit_cfg.period * m_pit_clkFreq);
		pit_setChanMode(chanIndx,m_pit_cfg.chain,m_pit_cfg.isrEn,m_pit_cfg.start);
		if((TRUE == m_pit_cfg.isrEn) && (NULL != m_pit_cfg.isrFun))
		{
			INTC_InstallINTCInterruptHandler(m_pit_cfg.isrFun,226 + chanIndx,m_pit_cfg.isrPrio);	
		}
	}
	if(TRUE == timerEn)
	{
		pit_start();
	}
	else
	{
		pit_stop();
	}
	pit_frz();
}

/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
void pit_isr0(void)
{
	pit_clrIsrFlag(0);
}

void pit_isr1(void)
{
	TmrHdl_IsrTask();
	pit_clrIsrFlag(1);
}

void pit_isr2(void)
{
	pit_clrIsrFlag(2);
}

void pit_isr3(void)
{
	pit_clrIsrFlag(3);
}


