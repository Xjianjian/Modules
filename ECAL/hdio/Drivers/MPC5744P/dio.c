

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "MPC5744P.h"
#include "SIUL.h"
#include "defines.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "dio.h"
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
void dio_setDir(dioChannel_t chanId, dioDir_t dir)
{
	m_InParaCheck(m_dio_isValidChanId(chanId),	m_plat_dioModuleId,	m_dio_setDirFunId,	m_plat_dioChanIdErr);
	m_InParaCheck(m_dio_isValidDir(dir),			m_plat_dioModuleId,	m_dio_setDirFunId,	m_plat_dioDirErr);
	SIUL2.MSCR[chanId].R &= (~m_dio_mscrSSSMask);/*set as the general-purpose Pin*/
	SIUL2.MSCR[chanId].R &= (~(SIUL_INPUT | SIUL_OUTPUT));
	if(m_dio_input == dir)
	{
		SIUL2.MSCR[chanId].R |= SIUL_INPUT;		
	}
	else if(m_dio_output == dir)
	{
		SIUL2.MSCR[chanId].R |= SIUL_OUTPUT;		
	}
	else
	{
		SIUL2.MSCR[chanId].R |= (SIUL_INPUT | SIUL_OUTPUT);
	}	
}


void dio_setOutAbility(dioChannel_t chanId, dioOutType_t outType)
{
	m_InParaCheck(m_dio_isValidChanId(chanId),	m_plat_dioModuleId,	m_dio_setOutAbilityFunId,	m_plat_dioChanIdErr);
	m_InParaCheck(m_dio_isValidOutType(outType),	m_plat_dioModuleId,	m_dio_setOutAbilityFunId,	m_plat_dioOutTypeErr);
	SIUL2.MSCR[chanId].R &= (~SIUL_OPEND);/*set as a noraml output pin*/
	if(m_dio_outOpenDrain == outType)
	{
		SIUL2.MSCR[chanId].R |= SIUL_OPEND;/*set as a open-drain output pin*/
	}
}


void dio_safeModeCmd(dioChannel_t chanId,functionSt_t funSt)
{
	m_InParaCheck(m_dio_isValidChanId(chanId),	m_plat_dioModuleId,	m_dio_safeModeCmdFunId,	m_plat_dioChanIdErr);
	m_InParaCheck(m_plat_isFunctionState(funSt),m_plat_dioModuleId,	m_dio_safeModeCmdFunId,	m_plat_funStErr);
	SIUL2.MSCR[chanId].R &= (~SIUL_SMC);/*disable the saft mode temporily*/
	if(em_funSt_on == funSt)
	{
		SIUL2.MSCR[chanId].R |= SIUL_SMC;
	}
}


void dio_setPullType(dioChannel_t chanId,dioPullType_t pullType)
{
	m_InParaCheck(m_dio_isValidChanId(chanId),	 m_plat_dioModuleId,	m_dio_setPullTypeFunId,	m_plat_dioChanIdErr);
	m_InParaCheck(m_dio_isPullType(pullType),		m_plat_dioModuleId,	m_dio_setPullTypeFunId,	m_plat_dioPullTypeErr);
	SIUL2.MSCR[chanId].R &= (~m_dio_mscrPUSMask);/*disable the pull temporily*/
	if(m_dio_pullUp == pullType)
	{
		SIUL2.MSCR[chanId].R |= SIUL_PULL_UP;
	}
	else if(m_dio_pullDown== pullType)
	{
		SIUL2.MSCR[chanId].R |= SIUL_PULL_DOWN;
	}
	else
	{
		/*nothing to do*/
	}
}

void dio_setHysteresis(dioChannel_t chanId,functionSt_t funSt)
{
	m_InParaCheck(m_dio_isValidChanId(chanId),	 m_plat_dioModuleId,	m_dio_setHysteresisFunId,	m_plat_dioChanIdErr);
	m_InParaCheck(m_plat_isFunctionState(funSt),	m_plat_dioModuleId,	m_dio_setHysteresisFunId,	m_plat_funStErr);
	SIUL2.MSCR[chanId].R &= (~m_dio_mscrHYSMask);
	if(em_funSt_off == funSt)
	{
		SIUL2.MSCR[chanId].R &= (~m_dio_mscrHYSMask);
	}
	else
	{
		SIUL2.MSCR[chanId].R |= (m_dio_mscrHYSMask);
	}

}
void dio_setSlewCtrl(dioChannel_t chanId,functionSt_t funSt)
{
	m_InParaCheck(m_dio_isValidChanId(chanId),	 m_plat_dioModuleId,	m_dio_setSlewCtrlFunId,	m_plat_dioChanIdErr);
	m_InParaCheck(m_plat_isFunctionState(funSt),	m_plat_dioModuleId,	m_dio_setSlewCtrlFunId,	m_plat_funStErr);
	SIUL2.MSCR[chanId].R &= (~m_dio_mscrSRSMask);
	if(em_funSt_on == funSt)
	{
		SIUL2.MSCR[chanId].R |= 0x100000000;
	}
	else
	{
		SIUL2.MSCR[chanId].R |= 0x300000000;
	}
	
}

uint8_t dio_readVal(dioChannel_t chanId)
{
	uint8_t pinSt = 0;
	//uint8_t chan = 0;
	//uint8_t pin = 0;
	m_InParaCheck(m_dio_isValidChanId(chanId),	 m_plat_dioModuleId,	m_dio_readValFunId,	m_plat_dioChanIdErr);	
	//chan = m_dio_ChanPort(chanId);
	//pin =  m_dio_ChanPin(chanId);
	//pinSt = ((SIUL2.PGPDI[chan].R & ((0x0001) << pin)) >0)?(1):(0);
	pinSt = ((SIUL2.GPDI[chanId].R  >0)?(1):(0));
	return pinSt;
}

bool dio_writeVal(dioChannel_t chanId,uint8_t val, bool readBackVerify)
{
	bool verifySuccess = TRUE; 
	//uint8_t chan = 0;
	//uint8_t pin = 0;
	uint8_t readBack =0;
	uint16_t cnt = 200;
	m_InParaCheck(m_dio_isValidChanId(chanId),	 m_plat_dioModuleId,	m_dio_readValFunId,	m_plat_dioChanIdErr);
	//chan = m_dio_ChanPort(chanId);
	//pin =  m_dio_ChanPin(chanId);
	SIUL2.GPDO[chanId].R = ((val >0)?(1):(0));
	//(val >0) ? (SIUL2.PGPDO[chan].R |= (0x0001 << pin)):(SIUL2.PGPDO[chan].R &= (~(0x0001 << pin)));//why can not
	if(TRUE == readBackVerify)
	{
		while(cnt>0)
		{
			cnt--;
		}
		readBack = dio_readVal(chanId);
		if(readBack != ((val>0)?(1):(0)))
		{
			verifySuccess = FALSE;	
		}
	}
	return verifySuccess;
}

bool dio_toggleChan(dioChannel_t chanId,bool readBackVerify)
{
	bool verifySuccess = TRUE; 
	uint8_t readSignal = 0;
	m_InParaCheck(m_dio_isValidChanId(chanId),	 m_plat_dioModuleId,	m_plat_toggleChanFunId,	m_plat_dioChanIdErr);
	readSignal = dio_readVal(chanId);
	verifySuccess = dio_writeVal(chanId,!readSignal,readBackVerify);
	return verifySuccess;
}

/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/




