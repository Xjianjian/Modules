
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
#include "pwm.h"
#include "pwm_Cfg.h"
/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/
static uint8_t pwm_subModule[m_pwm_outChannels] = {m_pwm_subModule2,m_pwm_subModule2,m_pwm_subModule3,m_pwm_subModule1};
static uint8_t pwm_subChannel[m_pwm_outChannels] = {m_pwm_subChannelA,m_pwm_subChannelB,m_pwm_subChannelX,m_pwm_subChannelX};
static uint8_t pwm_module[m_pwm_outChannels] = {m_pwm_module0,m_pwm_module0,m_pwm_module0,m_pwm_module0};

static pwmOutFreqDt_t pwmOutFreqDtGrp[m_pwm_outChannels] =
{
	{5000,-4999},
	{5000,-4999},
	{5000,-4999},
	{5000,-4999}
};

static bool pwmChannelUinit[m_pwm_outChannels] = {FALSE,FALSE,FALSE,FALSE};

/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/
static volatile struct FlexPWM_tag * const pFlexPwm[m_pwm_moduleCnt] = {&FlexPWM_0, &FlexPWM_1};


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
static void pwm_hwPinInit(uint8_t SIUL_Pin);
static void pwm_startRun(uint8_t pwm,bool start);
static void pwm_generateWave(uint8_t pwm,uint8_t duty);
/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
/*
 * Function: pwm_init
 * Param< void >:
 * Return< void>:
* Description:
* Note: 
* REQ IDs:
*/
void pwm_init(void)
{
	uint8_t channel_i = 0;
	uint8_t module = m_pwm_moduleNone;
	uint8_t subModule = m_pwm_subModuleNone;
	uint8_t channel = m_pwm_subChannelNone;
	uint8_t ModuleMask = 0;
	uint8_t subModuleMask[m_pwm_moduleCnt] = {0,0};
	uint16_t width = 0;
	int16_t initVal = 0;
	int16_t val1 = 0;
	for(;channel_i < m_pwm_outChannels; channel_i ++)
	{
		switch(PwmCfgGrp[channel_i].Mscr)
		{
			case SIUL_PA2: 
				module = m_pwm_module0; channel = m_pwm_subChannelA; subModule = m_pwm_subModule3;
				SIUL2.MSCR[SIUL_PA2].R = 0x02800002; 
				break;
			case SIUL_PA3: 
				module = m_pwm_module0; channel = m_pwm_subChannelB; subModule = m_pwm_subModule3;
				SIUL2.MSCR[SIUL_PA3].R = 0x02800003;
				break;
			case SIUL_PA4: 
				module = m_pwm_module1;channel = m_pwm_subChannelA; subModule = m_pwm_subModule2; 
				SIUL2.MSCR[SIUL_PA4].R = 0x02800004;
				break;
			case SIUL_PA9: 
				module = m_pwm_module0;channel = m_pwm_subChannelB; subModule = m_pwm_subModule3; 
				SIUL2.MSCR[SIUL_PA9].R = 0x02800003;
				break;
			case SIUL_PA10: 
				module = m_pwm_module0;channel = m_pwm_subChannelB; subModule = m_pwm_subModule0; 
				SIUL2.MSCR[SIUL_PA10].R = 0x02800002;
				break;
			case SIUL_PA11:
				module = m_pwm_module0;channel = m_pwm_subChannelA; subModule = m_pwm_subModule0; 
				SIUL2.MSCR[SIUL_PA11].R = 0x02800002;
				/*
				module = m_pwm_module0;channel = m_pwm_subChannelA; subModule = m_pwm_subModule2; 
				SIUL2.MSCR[SIUL_PA11].R = 0x02800003;
				*/
				break;
			case SIUL_PA12:
				module = m_pwm_module0;channel = m_pwm_subChannelA; subModule = m_pwm_subModule2; 
				SIUL2.MSCR[SIUL_PA12].R = 0x02800002;
				/*
				module = m_pwm_module0;channel = m_pwm_subChannelB; subModule = m_pwm_subModule2; 
				SIUL2.MSCR[SIUL_PA12].R = 0x02800003;
				*/
				break;
			case SIUL_PA13:
				module = m_pwm_module0;channel = m_pwm_subChannelB; subModule = m_pwm_subModule2; 
				SIUL2.MSCR[SIUL_PA13].R = 0x02800002;
				break;
			case SIUL_PC4:
				module = m_pwm_module0;channel = m_pwm_subChannelX; subModule = m_pwm_subModule1; 
				SIUL2.MSCR[SIUL_PC4].R = 0x02800002;
				break;
			case SIUL_PC6:
				module = m_pwm_module0;channel = m_pwm_subChannelB; subModule = m_pwm_subModule1; 
				SIUL2.MSCR[SIUL_PC6].R = 0x02800002;
				break;
			case SIUL_PC7:
				module = m_pwm_module0;channel = m_pwm_subChannelA; subModule = m_pwm_subModule1; 
				SIUL2.MSCR[SIUL_PC7].R = 0x02800002;
				break;
			case SIUL_PC10:
				module = m_pwm_module0;channel = m_pwm_subChannelA; subModule = m_pwm_subModule3; 
				SIUL2.MSCR[SIUL_PC10].R = 0x02800003;
				break;
			case SIUL_PC14:
				module = m_pwm_module1;channel = m_pwm_subChannelB; subModule = m_pwm_subModule0; 
				SIUL2.MSCR[SIUL_PC14].R = 0x02800004;
				break;
			case SIUL_PC15:
				module = m_pwm_module0;channel = m_pwm_subChannelA; subModule = m_pwm_subModule1; 
				SIUL2.MSCR[SIUL_PC15].R = 0x02800003;
				break;
			case SIUL_PD0:
				module = m_pwm_module0;channel = m_pwm_subChannelB; subModule = m_pwm_subModule1; 
				SIUL2.MSCR[SIUL_PD0].R = 0x02800003;
				break;
			case SIUL_PD2:
				module = m_pwm_module0;channel = m_pwm_subChannelX; subModule = m_pwm_subModule3; 
				SIUL2.MSCR[SIUL_PD2].R = 0x02800003;
				break;
			case SIUL_PD3:
				module = m_pwm_module0;channel = m_pwm_subChannelA; subModule = m_pwm_subModule3; 
				SIUL2.MSCR[SIUL_PD3].R = 0x02800003;
				break;
			case SIUL_PD4:
				module = m_pwm_module0;channel = m_pwm_subChannelB; subModule = m_pwm_subModule3; 
				SIUL2.MSCR[SIUL_PD4].R = 0x02800003;
				break;
			case SIUL_PD6:
				module = m_pwm_module0;channel = m_pwm_subChannelX; subModule = m_pwm_subModule3; 
				SIUL2.MSCR[SIUL_PD6].R = 0x02800003;
				break;
			case SIUL_PD9:
				module = m_pwm_module0;channel = m_pwm_subChannelX; subModule = m_pwm_subModule0; 
				SIUL2.MSCR[SIUL_PD9].R = 0x02800001;
				break;
			case SIUL_PD10:
				module = m_pwm_module0;channel = m_pwm_subChannelA; subModule = m_pwm_subModule0; 
				SIUL2.MSCR[SIUL_PD10].R = 0x02800001;
				break;
			case SIUL_PD11:
				module = m_pwm_module0;channel = m_pwm_subChannelB; subModule = m_pwm_subModule0; 
				SIUL2.MSCR[SIUL_PD11].R = 0x02800001;
				break;
			case SIUL_PD12:
				module = m_pwm_module0;channel = m_pwm_subChannelX; subModule = m_pwm_subModule1; 
				SIUL2.MSCR[SIUL_PD12].R = 0x02800001;
				break;
			case SIUL_PD14:
				//module = m_pwm_module0;channel = m_pwm_subChannelB; subModule = m_pwm_subModule1;
				//SIUL2.MSCR[SIUL_PD14].R = 0x02800001;
				break;
			case SIUL_PE14:
				module = m_pwm_module1;channel = m_pwm_subChannelB; subModule = m_pwm_subModule2; 
				SIUL2.MSCR[SIUL_PE14].R = 0x02800004;
				break;
			case SIUL_PF0:
				module = m_pwm_module0;channel = m_pwm_subChannelA; subModule = m_pwm_subModule1; 
				SIUL2.MSCR[SIUL_PF0].R = 0x02800001;
				break;
			case SIUL_PF13:
				module = m_pwm_module1;channel = m_pwm_subChannelB; subModule = m_pwm_subModule1; 
				SIUL2.MSCR[SIUL_PF13].R = 0x02800004;
				break;
			case SIUL_PG2:
				module = m_pwm_module0;channel = m_pwm_subChannelX; subModule = m_pwm_subModule2; 
				SIUL2.MSCR[SIUL_PG2].R = 0x02800001;
				break;
			case SIUL_PG3:
				module = m_pwm_module0;channel = m_pwm_subChannelA; subModule = m_pwm_subModule2; 
				SIUL2.MSCR[SIUL_PG3].R = 0x02800001;
				break;
			case SIUL_PG4:
				module = m_pwm_module0;channel = m_pwm_subChannelB; subModule = m_pwm_subModule2; 
				SIUL2.MSCR[SIUL_PG4].R = 0x02800001;
				break;
			case SIUL_PG5:
				module = m_pwm_module0;channel = m_pwm_subChannelX; subModule = m_pwm_subModule3; 
				SIUL2.MSCR[SIUL_PG5].R = 0x02800001;
				break;
			case SIUL_PG6:
				module = m_pwm_module0;channel = m_pwm_subChannelA; subModule = m_pwm_subModule3; 
				SIUL2.MSCR[SIUL_PG6].R = 0x02800001;
				break;
			case SIUL_PG7:
				module = m_pwm_module0;channel = m_pwm_subChannelB; subModule = m_pwm_subModule3; 
				SIUL2.MSCR[SIUL_PG7].R = 0x02800001;
				break;
			case SIUL_PH4:
				module = m_pwm_module1;channel = m_pwm_subChannelX; subModule = m_pwm_subModule0; 
				SIUL2.MSCR[SIUL_PH4].R = 0x02800001;
				break;
			case SIUL_PH5:
				module = m_pwm_module1;channel = m_pwm_subChannelA; subModule = m_pwm_subModule0; 
				SIUL2.MSCR[SIUL_PH5].R = 0x02800001;
				break;
			case SIUL_PH6:
				module = m_pwm_module1;channel = m_pwm_subChannelB; subModule = m_pwm_subModule0; 
				SIUL2.MSCR[SIUL_PH6].R = 0x02800001;
				break;
			case SIUL_PH7:
				module = m_pwm_module1;channel = m_pwm_subChannelX; subModule = m_pwm_subModule1; 
				SIUL2.MSCR[SIUL_PH7].R = 0x02800001;
				break;
			case SIUL_PH8:
				module = m_pwm_module1;channel = m_pwm_subChannelA; subModule = m_pwm_subModule1; 
				SIUL2.MSCR[SIUL_PH8].R = 0x02800001;
				break;
			case SIUL_PH9:
				module = m_pwm_module1;channel = m_pwm_subChannelB; subModule = m_pwm_subModule1; 
				SIUL2.MSCR[SIUL_PH9].R = 0x02800001;
				break;
			case SIUL_PH10:
				module = m_pwm_module1;channel = m_pwm_subChannelX; subModule = m_pwm_subModule2; 
				SIUL2.MSCR[SIUL_PH10].R = 0x02800001;
				break;
			case SIUL_PH11:
				module = m_pwm_module1;channel = m_pwm_subChannelA; subModule = m_pwm_subModule2; 
				SIUL2.MSCR[SIUL_PH11].R = 0x02800001;
				break;
			case SIUL_PH12:
				module = m_pwm_module1;channel = m_pwm_subChannelB; subModule = m_pwm_subModule2; 
				SIUL2.MSCR[SIUL_PH12].R = 0x02800001;
				break;
			case SIUL_PH13:
				module = m_pwm_module1;channel = m_pwm_subChannelX; subModule = m_pwm_subModule3; 
				SIUL2.MSCR[SIUL_PH13].R = 0x02800001;
				break;
			case SIUL_PH14:
				module = m_pwm_module1;channel = m_pwm_subChannelA; subModule = m_pwm_subModule3; 
				SIUL2.MSCR[SIUL_PH14].R = 0x02800001;
				break;
			case SIUL_PH15:
				module = m_pwm_module1;channel = m_pwm_subChannelB; subModule = m_pwm_subModule3; 
				SIUL2.MSCR[SIUL_PH15].R = 0x02800001;
				break;
			default:
				module = m_pwm_moduleNone;
				subModule = m_pwm_subModuleNone;
				channel = m_pwm_subChannelNone;
				break;
		}
		
		

		if((m_pwm_moduleNone != module) &&
   		   (m_pwm_subModuleNone != subModule) &&
   		   (channel != m_pwm_subChannelNone))
		{
			pwm_module[channel_i] = module;
			pwm_subModule[channel_i] = subModule;
			pwm_subChannel[channel_i] = channel;
			switch(channel)
			{
				case m_pwm_subChannelA:
					pFlexPwm[module]->OUTEN.B.PWMA_EN |= m_util_bitMask8(subModule);
					pFlexPwm[module]->SUB[subModule].OCTRL.B.POLA = (TRUE == PwmCfgGrp[channel_i].std)?(FALSE):(TRUE);
					subModuleMask[module] |= m_util_bitMask8(subModule);
					break;
				case m_pwm_subChannelB:
					pFlexPwm[module]->OUTEN.B.PWMB_EN |= m_util_bitMask8(subModule);
					pFlexPwm[module]->SUB[subModule].OCTRL.B.POLB = (TRUE == PwmCfgGrp[channel_i].std)?(FALSE):(TRUE);
					subModuleMask[module] |= m_util_bitMask8(subModule);
					break;
				case m_pwm_subChannelX:
					pFlexPwm[module]->OUTEN.B.PWMX_EN |= m_util_bitMask8(subModule);
					pFlexPwm[module]->SUB[subModule].OCTRL.B.POLX =(TRUE == PwmCfgGrp[channel_i].std)?(FALSE):(TRUE);
					subModuleMask[module] |= m_util_bitMask8(subModule);
					break;
				default:
					break;
			}
			pFlexPwm[module]->SUB[subModule].CTRL1.R = 0x0400|(m_pwm_PreDiv << 4);
			pFlexPwm[module]->SUB[subModule].CTRL2.R = 0x2000;	/*independent output*/
			pFlexPwm[module]->SUB[subModule].DTCNT0.R   = 0x0000;	/*deadtime values*/
			pFlexPwm[module]->SUB[subModule].DTCNT1.R   = 0x0000;

			width = (uint16_t)((m_pwm_cntFreq / pwmOutChannnelFreq[channel_i]) & 0xFFFF);
			val1 = (width  >> 1);
			initVal = -val1 + 1;
			pwmOutFreqDtGrp[channel_i].initVal = initVal;
			pwmOutFreqDtGrp[channel_i].val1 = val1;
			
			pFlexPwm[module]->SUB[subModule].INIT.R = initVal;
			pFlexPwm[module]->SUB[subModule].VAL0.R = PwmCfgGrp[channel_i].val0;
			pFlexPwm[module]->SUB[subModule].VAL1.R = val1;
			pFlexPwm[module]->SUB[subModule].DISMAP.R   = 0x0000;	/*disable fault pin condition*/
			ModuleMask |= m_util_bitMask8(module);
		}
	}
	if((ModuleMask & m_util_bitMask8(m_pwm_module0)) > 0)
	{
		/* Load the PRSC bits of CTRL1 and the INIT, and VALx registers  into a set of buffers */
		pFlexPwm[m_pwm_module0]->MCTRL.B.LDOK = subModuleMask[m_pwm_module0];
	}
	if((ModuleMask & m_util_bitMask8(m_pwm_module1)) > 0)
	{
		/* Load the PRSC bits of CTRL1 and the INIT, and VALx registers  into a set of buffers */
		pFlexPwm[m_pwm_module1]->MCTRL.B.LDOK = subModuleMask[m_pwm_module1];
	}
}


static void pwm_generateWave(uint8_t pwm,uint8_t duty)
{
	uint16_t pwm_highWith = 0;
	uint16_t pwm_lowWith = 0;
	uint16_t width = 0;
	int16_t initVal = 0;
	int16_t val1 = 0;
	if((pwm < m_pwm_outChannels) && (m_pwm_moduleNone != pwm_module[pwm]) && 
		(m_pwm_subModuleNone != pwm_subModule[pwm]) && (m_pwm_subChannelNone != pwm_subChannel[pwm]))
	{
		if(duty > 100)
		{
			duty = 100;
		}
		width = (uint16_t)((m_pwm_cntFreq / pwmOutChannnelFreq[pwm]) & 0xFFFF);
		val1 = (width  >> 1);
		initVal = -val1 + 1;
		pwmOutFreqDtGrp[pwm].initVal = initVal;
		pwmOutFreqDtGrp[pwm].val1 = val1;
		if(100 == duty)
		{
			pwmChannelUinit[pwm] = TRUE;
			SIUL2.MSCR[PwmCfgGrp[pwm].Mscr].R = 0x02800000;	/*set as a digital output*/
			if(TRUE == PwmCfgGrp[pwm].std)
			{
				dio_writeVal(PwmCfgGrp[pwm].Mscr,1,FALSE);
			}
			else
			{
				dio_writeVal(PwmCfgGrp[pwm].Mscr,0,FALSE);
			}

			//pwm_startRun(pwm,FALSE);
		}
		else if(0 == duty)
		{
			pwmChannelUinit[pwm] = TRUE;
			SIUL2.MSCR[PwmCfgGrp[pwm].Mscr].R = 0x02800000;	/*set as a digital output*/
			if(TRUE == PwmCfgGrp[pwm].std)
			{
				dio_writeVal(PwmCfgGrp[pwm].Mscr,0,FALSE);
			}
			else
			{
				dio_writeVal(PwmCfgGrp[pwm].Mscr,1,FALSE);
			}
			//pwm_startRun(pwm,FALSE);
		}
		else
		{
			if(TRUE == pwmChannelUinit[pwm])
			{
				pwm_hwPinInit(PwmCfgGrp[pwm].Mscr);
				 pwmChannelUinit[pwm] = FALSE;
			}
			pwm_highWith = width * duty / 100;
			pwm_lowWith = width - pwm_highWith;

			pFlexPwm[pwm_module[pwm]]->MCTRL.B.CLDOK  |= m_util_bitMask8(pwm_subModule[pwm]);
			switch(pwm_subChannel[pwm])
			{
				case m_pwm_subChannelA:
					pFlexPwm[pwm_module[pwm]]->SUB[pwm_subModule[pwm]].VAL2.R = pwmOutFreqDtGrp[pwm].initVal + (int16_t)(pwm_lowWith >> 1);
					pFlexPwm[pwm_module[pwm]]->SUB[pwm_subModule[pwm]].VAL3.R = pwmOutFreqDtGrp[pwm].val1 - (int16_t)(pwm_lowWith >> 1) ;
					break;
				case m_pwm_subChannelB:
					pFlexPwm[pwm_module[pwm]]->SUB[pwm_subModule[pwm]].VAL4.R = pwmOutFreqDtGrp[pwm].initVal + (int16_t)(pwm_lowWith >> 1);
					pFlexPwm[pwm_module[pwm]]->SUB[pwm_subModule[pwm]].VAL5.R = pwmOutFreqDtGrp[pwm].val1 - (int16_t)(pwm_lowWith >> 1) ;
					break;
				case m_pwm_subChannelX:
					pFlexPwm[pwm_module[pwm]]->SUB[pwm_subModule[pwm]].VAL0.R = pwm_lowWith + pwmOutFreqDtGrp[pwm].initVal;
					break;
			}
			pFlexPwm[pwm_module[pwm]]->MCTRL.B.LDOK = m_util_bitMask8(pwm_subModule[pwm]);
			pwm_startRun(pwm,TRUE);
		}
		
	}
}

static void pwm_startRun(uint8_t pwm,bool start)
{
	if((pwm < m_pwm_outChannels) && (m_pwm_moduleNone != pwm_module[pwm]))
	{
		if(TRUE == start)
		{
			pFlexPwm[pwm_module[pwm]]->MCTRL.B.RUN |= m_util_bitMask8(pwm_subModule[pwm]);
		}
		else
		{
			pFlexPwm[pwm_module[pwm]]->MCTRL.B.RUN &= ~ m_util_bitMask8(pwm_subModule[pwm]);
		}
	}
}


void pwm_update(void)
{
	uint8_t pwm_i = 0;
	for(;pwm_i < m_pwm_outChannels; pwm_i ++)
	{
		if(NULL != pwm_duty_grp[pwm_i])
		{
			pwm_generateWave(pwm_i, *pwm_duty_grp[pwm_i]);
		}

	}

}

/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static void pwm_hwPinInit(uint8_t SIUL_Pin)
{
	switch(SIUL_Pin)
	{
		case SIUL_PA2: 
			SIUL2.MSCR[SIUL_PA2].R = 0x02800002; 
			break;
		case SIUL_PA3: 
			SIUL2.MSCR[SIUL_PA3].R = 0x02800003;
			break;
		case SIUL_PA4: 
			SIUL2.MSCR[SIUL_PA4].R = 0x02800004;
			break;
		case SIUL_PA9: 
			SIUL2.MSCR[SIUL_PA9].R = 0x02800003;
			break;
		case SIUL_PA10: 
			SIUL2.MSCR[SIUL_PA10].R = 0x02800002;
			break;
		case SIUL_PA11:
			SIUL2.MSCR[SIUL_PA11].R = 0x02800002;
			/*
			SIUL2.MSCR[SIUL_PA11].R = 0x02800003;
			*/
			break;
		case SIUL_PA12:
			SIUL2.MSCR[SIUL_PA12].R = 0x02800002;
			/*
			SIUL2.MSCR[SIUL_PA12].R = 0x02800003;
			*/
			break;
		case SIUL_PA13:
			SIUL2.MSCR[SIUL_PA13].R = 0x02800002;
			break;
		case SIUL_PC4:
			SIUL2.MSCR[SIUL_PC4].R = 0x02800002;
			break;
		case SIUL_PC6:
			SIUL2.MSCR[SIUL_PC6].R = 0x02800002;
			break;
		case SIUL_PC7:
			SIUL2.MSCR[SIUL_PC7].R = 0x02800002;
			break;
		case SIUL_PC10:
			SIUL2.MSCR[SIUL_PC10].R = 0x02800003;
			break;
		case SIUL_PC14:
			SIUL2.MSCR[SIUL_PC14].R = 0x02800004;
			break;
		case SIUL_PC15:
			SIUL2.MSCR[SIUL_PC15].R = 0x02800003;
			break;
		case SIUL_PD0:
			SIUL2.MSCR[SIUL_PD0].R = 0x02800003;
			break;
		case SIUL_PD2:
			SIUL2.MSCR[SIUL_PD2].R = 0x02800003;
			break;
		case SIUL_PD3:
			SIUL2.MSCR[SIUL_PD3].R = 0x02800003;
			break;
		case SIUL_PD4:
			SIUL2.MSCR[SIUL_PD4].R = 0x02800003;
			break;
		case SIUL_PD6:
			SIUL2.MSCR[SIUL_PD6].R = 0x02800003;
			break;
		case SIUL_PD9:
			SIUL2.MSCR[SIUL_PD9].R = 0x02800001;
			break;
		case SIUL_PD10:
			SIUL2.MSCR[SIUL_PD10].R = 0x02800001;
			break;
		case SIUL_PD11:
			SIUL2.MSCR[SIUL_PD11].R = 0x02800001;
			break;
		case SIUL_PD12:
			SIUL2.MSCR[SIUL_PD12].R = 0x02800001;
			break;
		case SIUL_PD14:
			//SIUL2.MSCR[SIUL_PD14].R = 0x02800001;
			break;
		case SIUL_PE14:
			SIUL2.MSCR[SIUL_PE14].R = 0x02800004;
			break;
		case SIUL_PF0:
			SIUL2.MSCR[SIUL_PF0].R = 0x02800001;
			break;
		case SIUL_PF13:
			SIUL2.MSCR[SIUL_PF13].R = 0x02800004;
			break;
		case SIUL_PG2:
			SIUL2.MSCR[SIUL_PG2].R = 0x02800001;
			break;
		case SIUL_PG3:
			SIUL2.MSCR[SIUL_PG3].R = 0x02800001;
			break;
		case SIUL_PG4:
			SIUL2.MSCR[SIUL_PG4].R = 0x02800001;
			break;
		case SIUL_PG5:
			SIUL2.MSCR[SIUL_PG5].R = 0x02800001;
			break;
		case SIUL_PG6:
			SIUL2.MSCR[SIUL_PG6].R = 0x02800001;
			break;
		case SIUL_PG7:
			SIUL2.MSCR[SIUL_PG7].R = 0x02800001;
			break;
		case SIUL_PH4:
			SIUL2.MSCR[SIUL_PH4].R = 0x02800001;
			break;
		case SIUL_PH5:
			SIUL2.MSCR[SIUL_PH5].R = 0x02800001;
			break;
		case SIUL_PH6:
			SIUL2.MSCR[SIUL_PH6].R = 0x02800001;
			break;
		case SIUL_PH7:
			SIUL2.MSCR[SIUL_PH7].R = 0x02800001;
			break;
		case SIUL_PH8:
			SIUL2.MSCR[SIUL_PH8].R = 0x02800001;
			break;
		case SIUL_PH9:
			SIUL2.MSCR[SIUL_PH9].R = 0x02800001;
			break;
		case SIUL_PH10:
			SIUL2.MSCR[SIUL_PH10].R = 0x02800001;
			break;
		case SIUL_PH11:
			SIUL2.MSCR[SIUL_PH11].R = 0x02800001;
			break;
		case SIUL_PH12:
			SIUL2.MSCR[SIUL_PH12].R = 0x02800001;
			break;
		case SIUL_PH13:
			SIUL2.MSCR[SIUL_PH13].R = 0x02800001;
			break;
		case SIUL_PH14:
			SIUL2.MSCR[SIUL_PH14].R = 0x02800001;
			break;
		case SIUL_PH15:
			SIUL2.MSCR[SIUL_PH15].R = 0x02800001;
			break;
		default:
			break;
	}
}




