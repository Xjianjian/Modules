

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/

#include "MPC5744P.h"
#include "SIUL.h"
#include "defines.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "adcCfg.h"
#include "adc.h"


/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/
#define m_adc_module 	AdcCfgModule[module]


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/
static volatile struct ADC_tag * const AdcCfgModule[m_adc_cfgModuCnt] = 
{
	&ADC_0,&ADC_1
};



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
static bool adc_isPowDn(uint8_t module);
static bool adc_SetPowDn(uint8_t module, functionSt_t funSt,bool verify);
static void adc_clrWdFlag(uint8_t module, uint8_t wd,bool ClrHigh,	bool clrLow);
static void adc_clrAllWdFlag(uint8_t module);
/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void adc_init(uint8_t module,uint32_t jChanMask, uint32_t norchanMask)
{
	uint16_t initTimeout = 1000;
	uint32_t chanMask = jChanMask | norchanMask;
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId, m_adc_initFunID,	m_plat_adcModuErr);
	if(FALSE == adc_isPowDn(module))
	{
		while((FALSE == adc_SetPowDn(module,em_funSt_on,TRUE)) && 
		  (initTimeout > 0))
		{
			initTimeout -- ;
		}
	}
	if(initTimeout >0)
	{
		initTimeout = 1000;

	}
	m_adc_module->MCR.B.ADCLKSEL = ((m_adc_clockFull == adcModuleCfg[module].clockSel)?(1):(0));
	m_adc_module->MCR.B.OWREN = ((TRUE == adcModuleCfg[module].overWriteEn)?(1):(0));
	m_adc_module->MCR.B.WLSIDE = ((m_adc_dtAlignLeft == adcModuleCfg[module].convertAligned)?(1):(0));
	m_adc_module->MCR.B.CTUEN = ((TRUE == adcModuleCfg[module].ctuEn)?(1):(0));
	m_adc_module->MCR.B.JTRGEN = ((TRUE == adcModuleCfg[module].injectExtTriEn)?(1):(0));
	//m_adc_module->MCR.B.EDGE = ((m_adc_extTriFallEdge == adcModuleCfg[module].injectExtTriEdge)?(0):(1));
	m_adc_module->MCR.B.EDGE  = 0;
	m_adc_module->MCR.B.ACKO = ((TRUE == adcModuleCfg[module].autoClockOff)?(1):(0));
	m_adc_module->MCR.B.MODE = ((TRUE == adcModuleCfg[module].scanEn)?(1):(0));

	while((FALSE == adc_SetPowDn(module,em_funSt_off,TRUE)) && (initTimeout >0))
	{
		initTimeout --;	
	}
	m_adc_module->CTR0.R = 0x00008606;//?
	m_adc_module->CTR1.R = 0x00000016;
	m_adc_module->JCMR0.R = jChanMask;
	m_adc_module->NCMR0.R = norchanMask;
	m_adc_module->PSR0.R = jChanMask | norchanMask;
	m_adc_module->PSCR.B.PREVAL0 = 1;
	m_adc_module->PSCR.B.PRECONV = 0;
	switch(module)
	{
		case 0:
			if((m_util_bitMask32(0) & chanMask) >0)
			{
				SIUL_AnalogInput(SIUL_PB7, 0);		/*AN0*/
			}
			if((m_util_bitMask32(1) & chanMask) >0)	
			{
				SIUL_AnalogInput(SIUL_PB8, 0);		/*AN1*/
			}
			if((m_util_bitMask32(2) & chanMask) >0)	
			{
				SIUL_AnalogInput(SIUL_PC1, 0);		/*AN2*/
			}
			if((m_util_bitMask32(3) & chanMask) >0)	
			{
				SIUL_AnalogInput(SIUL_PC2, 0);		/*AN3*/
			}
			if((m_util_bitMask32(4) & chanMask) >0)	
			{
				SIUL_AnalogInput(SIUL_PE6, 0);		/*AN4*/
			}
			if((m_util_bitMask32(5) & chanMask) >0)	
			{
				SIUL_AnalogInput(SIUL_PE2, 0);		/*AN5*/
			}
			if((m_util_bitMask32(6) & chanMask) >0)	
			{
				SIUL_AnalogInput(SIUL_PE7, 0);		/*AN6*/
			}
			if((m_util_bitMask32(7) & chanMask) >0)	
			{
				SIUL_AnalogInput(SIUL_PE4, 0);		/*AN7*/
			}
			if((m_util_bitMask32(8) & chanMask) >0)	
			{
				SIUL_AnalogInput(SIUL_PE5, 0);		/*AN8*/
			}
			if((m_util_bitMask32(11) & chanMask) >0)	
			{
				SIUL_AnalogInput(SIUL_PB9, 0);		/*AN11	shared between ADC0 and ADC1*/
			}
			if((m_util_bitMask32(12) & chanMask) >0)	
			{
				SIUL_AnalogInput(SIUL_PB10, 0);		/*AN12	shared between ADC0 and ADC1*/
			}
			if((m_util_bitMask32(13) & chanMask) >0)	
			{
				SIUL_AnalogInput(SIUL_PB11, 0);		/*AN13	shared between ADC0 and ADC1*/
			}
			if((m_util_bitMask32(14) & chanMask) >0)	
			{
				SIUL_AnalogInput(SIUL_PB12, 0);		/*AN14	shared between ADC0 and ADC1*/
			}
			break;
		case 1:
			if((m_util_bitMask32(0) & chanMask) >0)		
			{
				SIUL_AnalogInput(SIUL_PB13, 0);		/*AN0*/
			}
			if((m_util_bitMask32(1) & chanMask) >0)		
			{
				SIUL_AnalogInput(SIUL_PB14, 0);		/*AN1*/
			}
			if((m_util_bitMask32(2) & chanMask) >0)		
			{
				SIUL_AnalogInput(SIUL_PB15, 0);		/*AN2*/
			}
			if((m_util_bitMask32(3) & chanMask) >0)		
			{
				SIUL_AnalogInput(SIUL_PC0, 0);		/*AN3*/
			}
			if((m_util_bitMask32(4) & chanMask) >0)		
			{
				SIUL_AnalogInput(SIUL_PE11, 0);		/*AN4*/
			}
			if((m_util_bitMask32(5) & chanMask) >0)		
			{
				SIUL_AnalogInput(SIUL_PE0, 0);		/*AN5*/
			}
			if((m_util_bitMask32(6) & chanMask) >0)		
			{
				SIUL_AnalogInput(SIUL_PE12, 0);		/*AN6*/
			}
			if((m_util_bitMask32(7) & chanMask) >0)		
			{
				SIUL_AnalogInput(SIUL_PE9, 0);		/*AN7*/
			}
			if((m_util_bitMask32(8) & chanMask) >0)		
			{
				SIUL_AnalogInput(SIUL_PE10, 0);		/*AN8*/
			}
			if((m_util_bitMask32(11) & chanMask) >0)		
			{
				SIUL_AnalogInput(SIUL_PB9, 0);		/*AN11	shared between ADC0 and ADC1*/
			}
			if((m_util_bitMask32(12) & chanMask) >0)		
			{
				SIUL_AnalogInput(SIUL_PB10, 0);		/*AN12	shared between ADC0 and ADC1*/
			}
			if((m_util_bitMask32(13) & chanMask) >0)		
			{
				SIUL_AnalogInput(SIUL_PB11, 0);		/*AN13	shared between ADC0 and ADC1*/
			}
			if((m_util_bitMask32(14) & chanMask) >0)		
			{
				SIUL_AnalogInput(SIUL_PB12, 0);		/*AN14	shared between ADC0 and ADC1*/
			}
			break;
		default:
			break;
	}
	adc_calibration(module);
	//adcSubModuleSel->DMAE.B.DMAEN = adcModuleConf[numOfAdc].dmaEn;
}


/*
 * Function:adc_startNormalConversion
 * Param<void>:
 * Return<bool>:
 * REQ IDs:
 * Description:start normal conversion, the convert channels are init before this function
 * Note:
*/
void adc_startNormalConversion(uint8_t module)
{
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId, m_adc_initFunID,	m_plat_adcModuErr);
    m_adc_module->MCR.B.NSTART = 1;
}

/*
 * Function:adc_startInjectedConversion
 * Param<void>:
 * Return<bool>:
 * REQ IDs:
 * Description:start inject conversion, the convert channels are init before this function
 * Note:
*/
void adc_startInjectedConversion(uint8_t module)
{
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId, m_adc_initFunID,	m_plat_adcModuErr);
    m_adc_module->MCR.B.JSTART = 1;
}



/*
 * Function:adc_abortConversion
 * Param<void>:
 * Return<bool>:
 * REQ IDs:
 * Description:abort conversion, abort all coverting channel
 * Note:
*/
void adc_abortConversion(uint8_t module)
{
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId, m_adc_initFunID,	m_plat_adcModuErr);
    m_adc_module->MCR.B.ABORT = 1;
}

/*
 * Function:adc_StopConversion
 * Param<void>:
 * Return<bool>:
 * REQ IDs:
 * Description:stop the conversion
 * Note:
*/
void adc_StopConversion(uint8_t module)
{
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId, m_adc_initFunID,	m_plat_adcModuErr);
    m_adc_module->MCR.B.JSTART = 0;
	m_adc_module->MCR.B.NSTART = 0;	
}


uint8_t adc_getStatus(uint8_t module)
{
	uint8_t status = 0;
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId, m_adc_initFunID,	m_plat_adcModuErr);
    status = m_adc_module->MSR.B.ADCSTATUS;
	return status;
}
	

bool adc_getDt(uint8_t module,uint16_t adcChan,uint16_t * dt,bool moveCut)
{
	uint16_t covDt = 0;
	bool GetDtSucces = FALSE;
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId, m_adc_initFunID,	m_plat_adcModuErr);	
	m_InParaCheck(m_adc_isValidChan(adcChan), m_plat_adcModuleId, m_adc_initFunID,	m_plat_adcChanErr);	
	/* pointer settings */
   	if(m_adc_module->CDR[adcChan].B.VALID > 0)
   	{
   		if(TRUE ==moveCut)
   		{
			if(m_adc_dtAlignLeft == adcModuleCfg[module].convertAligned)
			{
				covDt = (m_adc_module->CDR[adcChan].B.CDATA >> 4) & 0x0FFF;
			}
			else
			{
				covDt = m_adc_module->CDR[adcChan].B.CDATA  & 0x0FFF;
			}
   		}
   		else
   		{
   			covDt = m_adc_module->CDR[adcChan].B.CDATA;
   		}
		if(NULL !=  dt)
		{
			*dt = covDt;
			GetDtSucces = TRUE;
		}
	}
	return GetDtSucces;
}

void adc_setInt(uint8_t module,uint8_t intMask,uint16_t chanMask,functionSt_t funSt)
{
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId, m_adc_initFunID,	m_plat_adcModuErr);	
	m_InParaCheck(m_plat_isFunctionState(funSt),	m_plat_adcModuleId,	m_adc_setPowerDownFunId,	m_plat_adcfunStERR);
	m_adc_module->IMR.R =0;
	m_adc_module->CIMR0.R =0;
	if(em_funSt_on == funSt)
	{
		m_adc_module->IMR.R = intMask; 
		m_adc_module->CIMR0.R = chanMask;
	}
}

void adc_clrIsrFlag(uint8_t module,uint8_t intMask)
{
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId, m_adc_initFunID,	m_plat_adcModuErr);	
	m_adc_module->ISR.R = intMask;
}

uint8_t adc_getIsrFlag(uint8_t module)
{
	uint8_t isrStatus = 0;
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId, m_adc_initFunID,	m_plat_adcModuErr);	
	isrStatus = (m_adc_module->ISR.R & m_adc_validIsrMask);
	return isrStatus;
}


uint16_t adc_getChanEoc(uint8_t module)
{
	uint16_t chanEoc = 0;
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId, m_adc_initFunID,	m_plat_adcModuErr);
	chanEoc = m_adc_module->CEOCFR0.R & 0xFFFF;
	return chanEoc;
}

void adc_ClrChanEoc(uint8_t module,uint8_t chan)
{
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId, m_adc_clearChanEocFunId,	m_plat_adcModuErr);
	m_InParaCheck(m_adc_isValidChan(chan), m_plat_adcModuleId, m_adc_clearChanEocFunId,	m_plat_adcChanErr);
	m_adc_module->CEOCFR0.R |=  m_util_bitMask32(chan);
}

void adc_clrEoc(uint8_t module,uint32_t mask)
{
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId, m_adc_clearEocFunId,	m_plat_adcModuErr);
	m_adc_module->CEOCFR0.R |=  mask;
}


void adc_wdSet(uint8_t module,uint8_t chanIndx,uint16_t threL,uint16_t threH, uint8_t wdIndx,bool isrHSideEn,bool isrLSideEn)
{
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId,   m_adc_initFunID,		m_plat_adcModuErr);
	m_InParaCheck(m_adc_isValidChan(chanIndx), m_plat_adcModuleId,   m_adc_initFunID,		m_plat_adcChanErr);
	m_InParaCheck(m_adc_isValidCovDt(threL),   m_plat_adcModuleId,   m_adc_initFunID,		m_plat_adcChanErr);
	m_InParaCheck(m_adc_isValidCovDt(threH),   m_plat_adcModuleId,   m_adc_initFunID,		m_plat_adcChanErr);
	m_InParaCheck(m_adc_isValidWd(wdIndx), 	 m_plat_adcModuleId, 	m_adc_initFunID,		m_plat_adcWdErr);
	switch(chanIndx)
	{
		case  0:	 m_adc_module->THRHLR0.B.THRH = threH;	m_adc_module->THRHLR0.B.THRL = threL;	break;
		case  1:	 m_adc_module->THRHLR1.B.THRH = threH;	m_adc_module->THRHLR1.B.THRL = threL;	break;
		case  2:	 m_adc_module->THRHLR2.B.THRH = threH;	m_adc_module->THRHLR2.B.THRL = threL;	break;
		case  3:	 m_adc_module->THRHLR3.B.THRH = threH;	m_adc_module->THRHLR3.B.THRL = threL;	break;
		case  4:	 m_adc_module->THRHLR4.B.THRH = threH;	m_adc_module->THRHLR4.B.THRL = threL;	break;
		case  5:	 m_adc_module->THRHLR5.B.THRH = threH;	m_adc_module->THRHLR5.B.THRL = threL;	break;
		case  6:	 m_adc_module->THRHLR6.B.THRH = threH;	m_adc_module->THRHLR6.B.THRL = threL;	break;
		case  7:	 m_adc_module->THRHLR7.B.THRH = threH;	m_adc_module->THRHLR7.B.THRL = threL;	break;
		case  8:	 m_adc_module->THRHLR8.B.THRH = threH;	m_adc_module->THRHLR8.B.THRL = threL;	break;
		case  9:	 m_adc_module->THRHLR9.B.THRH = threH;	m_adc_module->THRHLR9.B.THRL = threL;	break;
		case 10:	 m_adc_module->THRHLR10.B.THRH = threH;m_adc_module->THRHLR10.B.THRL = threL;	break;
		case 11:	 m_adc_module->THRHLR11.B.THRH = threH;m_adc_module->THRHLR11.B.THRL = threL;	break;
		case 12:	 m_adc_module->THRHLR12.B.THRH = threH;m_adc_module->THRHLR12.B.THRL = threL;	break;
		case 13:	 m_adc_module->THRHLR13.B.THRH = threH;m_adc_module->THRHLR13.B.THRL = threL;	break;
		case 14:	 m_adc_module->THRHLR14.B.THRH = threH;m_adc_module->THRHLR14.B.THRL = threL;	break;
		case 15:	 m_adc_module->THRHLR15.B.THRH = threH;m_adc_module->THRHLR15.B.THRL = threL;	break;  
		default:																					break;
	}
	switch (wdIndx)
	{
		case  0:	 
			m_adc_module->CWSELR0.B.WSEL_CH0 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG0L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG0H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		case  1:	 
			m_adc_module->CWSELR0.B.WSEL_CH1 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG1L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG1H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		case  2:	 
			m_adc_module->CWSELR0.B.WSEL_CH2 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG2L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG2H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		case  3:	 
			m_adc_module->CWSELR0.B.WSEL_CH3 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG3L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG3H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		case  4:	 
			m_adc_module->CWSELR0.B.WSEL_CH4 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG4L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG4H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		case  5:	 
			m_adc_module->CWSELR0.B.WSEL_CH5 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG5L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG5H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		case  6:	 
			m_adc_module->CWSELR0.B.WSEL_CH6 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG6L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG6H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		case  7:	 
			m_adc_module->CWSELR0.B.WSEL_CH7 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG7L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG7H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		case  8:	 
			m_adc_module->CWSELR1.B.WSEL_CH8 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG8L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG8H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		case  9:	 
			m_adc_module->CWSELR1.B.WSEL_CH9 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG9L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG9H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		case  10:	 
			m_adc_module->CWSELR1.B.WSEL_CH10 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG10L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG10H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		case  11:	 
			m_adc_module->CWSELR1.B.WSEL_CH11 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG11L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG11H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		case  12:	 
			m_adc_module->CWSELR1.B.WSEL_CH12 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG12L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG12H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		case  13:	 
			m_adc_module->CWSELR1.B.WSEL_CH13 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG13L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG13H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		case  14:	 
			m_adc_module->CWSELR1.B.WSEL_CH14 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG14L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG14H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		case  15:	 
			m_adc_module->CWSELR1.B.WSEL_CH15 = chanIndx;
			m_adc_module->WTIMR.B.MSKWDG15L = ((TRUE == isrLSideEn)?(0):(1));
			m_adc_module->WTIMR.B.MSKWDG15H = ((TRUE == isrHSideEn)?(0):(1));
			break;
		default:
			break;
	}
}


bool adc_calibration(uint8_t module)
{
	bool caliOk;
	uint16_t caliTimeLeft = 10000;
    m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId,   m_adc_initFunID,		m_plat_adcModuErr);
    /*Configure the ADC (reset MCR[PWDN] and MCR[ADCLKSEL]).*/
	if(TRUE == adc_isPowDn(module))
	{
		(void)adc_SetPowDn(module, em_funSt_off,FALSE);
	}
    //m_adc_module->MCR.B.ADCLKSEL = 0;
    /*Configure the Calibration, BIST Control, and Status Register (CALBISTREG) for
    test conditions. The default values are set for maximum accuracy (recommended).*/

    /*Set CALBISTREG[TEST_EN]. The routine will start immediately.*/
	AdcCfgModule[module]->CALBISTREG.B.NR_SMPL = 1;
	AdcCfgModule[module]->CALBISTREG.B.AVG_EN = 1;
    m_adc_module->CALBISTREG.R |= 1;

    /*Wait until CALBISTREG[C_T_BUSY] becomes 0.*/
    while (0 != m_adc_module->CALBISTREG.B.C_T_BUSY)
    {
    	caliTimeLeft--;
    	if (0 == caliTimeLeft)
    	{
    		caliOk = 0;
    		break;
    	}
    }
    if((1 == m_adc_module->MSR.B.CALIBRTD) && \
		(0 == m_adc_module->CALBISTREG.B.TEST_FAIL))
    {
    	caliOk = 1;
    }
    else
    {
    	caliOk = 0;
    }	
    return caliOk;
}

void adc_dmaCmd(uint8_t module,functionSt_t funSt)
{
	m_InParaCheck(m_adc_isValidModule(module),	m_plat_adcModuleId,	m_adc_isPowerDownFunId,	m_plat_adcModuErr);
	m_InParaCheck(m_plat_isFunctionState(funSt),	m_plat_adcModuleId,	m_adc_setPowerDownFunId,	m_plat_adcfunStERR);
	m_adc_module->DMAE.B.DMAEN = ((em_funSt_on == funSt)?(1):(0));
}

void adc_dmaChanCmd(uint8_t module,uint8_t chan,functionSt_t funSt)
{
	m_InParaCheck(m_adc_isValidModule(module),	m_plat_adcModuleId,	m_adc_isPowerDownFunId,	m_plat_adcModuErr);
	m_InParaCheck(m_adc_isValidChan(chan), m_plat_adcModuleId,   m_adc_initFunID,		m_plat_adcChanErr);
	m_InParaCheck(m_plat_isFunctionState(funSt),	m_plat_adcModuleId,	m_adc_setPowerDownFunId,	m_plat_adcfunStERR);
	(em_funSt_on == funSt)?(m_adc_module->DMAR0.R |= m_util_bitMask32(chan)):(m_adc_module->DMAR0.R &= (~ m_util_bitMask32(chan)));
}



/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static bool adc_isPowDn(uint8_t module)
{
	bool isPowDn = TRUE;
	m_InParaCheck(m_adc_isValidModule(module),	m_plat_adcModuleId,	m_adc_isPowerDownFunId,	m_plat_adcModuErr);	
	isPowDn = ((m_adc_module->MCR.B.PWDN >0)?(TRUE):(FALSE));
	return isPowDn;
}

static bool adc_SetPowDn(uint8_t module, functionSt_t funSt,bool verify)
{
	bool setOK = TRUE;
	bool powDn = 0;
	m_InParaCheck(m_adc_isValidModule(module),	m_plat_adcModuleId,	m_adc_setPowerDownFunId,	m_plat_adcModuErr);
	m_InParaCheck(m_plat_isFunctionState(funSt),	m_plat_adcModuleId,	m_adc_setPowerDownFunId,	m_plat_adcfunStERR);
	powDn = ((em_funSt_on == funSt)?(TRUE):(FALSE));
	m_adc_module->MCR.B.PWDN = ((TRUE == powDn)?(1):(0));
	if(TRUE == verify)
	{
		if((TRUE == powDn) && (1 == m_adc_module->MCR.B.PWDN))
		{
			setOK = TRUE;
		}
		else if((FALSE == powDn) && (0 == m_adc_module->MCR.B.PWDN))
		{
			setOK = TRUE;
		}
		else
		{
			setOK = FALSE;
		}
	}
	return setOK;
}

static void adc_clrWdFlag(uint8_t module, uint8_t wd,bool ClrHigh,	bool clrLow)
{
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId,   m_adc_initFunID,		m_plat_adcModuErr);
	m_InParaCheck(m_adc_isValidWd(wd), 	 	m_plat_adcModuleId, 	m_adc_initFunID,		m_plat_adcWdErr);	
	m_adc_module->WTISR.R |= ((TRUE == clrLow)?(m_util_bitMask32(wd << 1)):(0));
	m_adc_module->WTISR.R |= ((TRUE == ClrHigh)?(m_util_bitMask32((wd << 1) + 1)):(0));
}

static void adc_clrAllWdFlag(uint8_t module)
{
	m_InParaCheck(m_adc_isValidModule(module), m_plat_adcModuleId,   m_adc_initFunID,		m_plat_adcModuErr);
	m_adc_module->WTISR.R |= 0xFFFFFFFF;
}



