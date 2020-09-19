

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "MPC5744P.h"
#include "SIUL.h"
#include "defines.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "global_var.h"
#include "eTimer.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/
#define mc_pi_InputAdjust			FALSE
/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/
static uint16_t PiOverTimeTick[e_pi_max] = {0,0,0,0,0};
/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/
static uint32_t pwmPeriod[e_pi_max] = {0,0,0,0,0};
static uint16_t pwmDuty = 0 ;
static uint16_t hld_pi_Duty[e_pi_max] = {0,0,0,0,0};

/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/
static void etim_hanldeCasCal(uint8_t indx);
static void etim_CascalDutyandPeriod(uint8_t PiIndx,uint8_t chanH, uint8_t chanL);
static void etim_inCaptCasStart(uint8_t casEtimIndx,bool start);
/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/

void etim_inCaptCasInit(void)
{
	uint8_t inCaptIndx;
	uint8_t secondSoc = 0;
	uint8_t module;
	uint8_t chanL;
	uint8_t chanH;
	uint8_t div;
	volatile struct ETIMER_tag *  pETim = NULL;
	
	for(inCaptIndx=0;inCaptIndx < e_pi_max; inCaptIndx ++)
	{
		module  = etimCasInCaptCfgGrp[inCaptIndx].module;
		chanL =   etimCasInCaptCfgGrp[inCaptIndx].chanL;
		chanH =   etimCasInCaptCfgGrp[inCaptIndx].chanH;
		div = 	  etimCasInCaptCfgGrp[inCaptIndx].div;	
		secondSoc = etimCasInCaptCfgGrp[inCaptIndx].secondSoc;	
		/*1st step, initialize the hardware pin*/
		switch(module)
		{
			case 0:
				pETim = &ETIMER_0;
				secondSoc = etimCasInCaptCfgGrp[inCaptIndx].secondSoc;
				switch(secondSoc)
				{
					case 0:SIUL2.MSCR[SIUL_PD10].B.IBE = 1;  SIUL2.IMCR[59].B.SSS = 1;  break;	/* PD10->eTimer0 ch0 */
					case 1:SIUL2.MSCR[SIUL_PA1].B.IBE = 1;  SIUL2.IMCR[60].B.SSS = 2;  break;	/* PA1->eTimer0 ch1 */
					case 2:SIUL2.MSCR[SIUL_PA2].B.IBE = 1;  SIUL2.IMCR[61].B.SSS = 2;  break;	/* PA2->eTimer0 ch2 */
					case 3:SIUL2.MSCR[SIUL_PD14].B.IBE = 1; SIUL2.IMCR[62].B.SSS = 1; break;	/* PD14->eTimer0 ch3 */
					case 4:SIUL2.MSCR[SIUL_PA4].B.IBE = 1;  SIUL2.IMCR[63].B.SSS = 3;  break;	/* PA4->eTimer0 ch4 */
					case 5:SIUL2.MSCR[SIUL_PC12].B.IBE = 1; SIUL2.IMCR[64].B.SSS = 3; break;	/* PC12->eTimer0 ch5 */


				}
				break;
			case 1:
				pETim = &ETIMER_1;
				secondSoc = etimCasInCaptCfgGrp[inCaptIndx].secondSoc;
				switch(secondSoc)
				{
					case 0:SIUL2.MSCR[SIUL_PA4].B.IBE = 1;   SIUL2.IMCR[65].B.SSS = 1;  break;	/* PA4->eTimer1 ch0 */
					case 1:SIUL2.MSCR[SIUL_PC13].B.IBE = 1;  SIUL2.IMCR[66].B.SSS = 1;  break;	/* PC13->eTimer1 ch1 */
					case 2:SIUL2.MSCR[SIUL_PC14].B.IBE = 1;  SIUL2.IMCR[67].B.SSS = 2; break;	/* PC14->eTimer1 ch2 */
					case 3:SIUL2.MSCR[SIUL_PF12].B.IBE = 1;  SIUL2.IMCR[68].B.SSS = 3; break;	/* PF12->eTimer1 ch3 */
					case 4:SIUL2.MSCR[SIUL_PD3].B.IBE = 1;  SIUL2.IMCR[69].B.SSS = 2;  break;	/* PD3->eTimer1 ch4 */
					case 5:SIUL2.MSCR[SIUL_PA5].B.IBE = 1;  SIUL2.IMCR[70].B.SSS = 1;  break;	/* PA5->eTimer1 ch5 */
				}
				break;
			case 2:
				pETim = &ETIMER_2;
				secondSoc = etimCasInCaptCfgGrp[inCaptIndx].secondSoc;
				switch(secondSoc)
				{
					case 0:SIUL2.MSCR[SIUL_PH4].B.IBE = 1;  SIUL2.IMCR[71].B.SSS = 1;  break;	/* PH4->eTimer2 ch0 */
					case 1:SIUL2.MSCR[SIUL_PH7].B.IBE = 1;  SIUL2.IMCR[72].B.SSS = 1;  break;	/* PH7->eTimer2 ch1 */
					case 2:SIUL2.MSCR[SIUL_PA6].B.IBE = 1;  SIUL2.IMCR[73].B.SSS = 1;  break;	/* PA6->eTimer2 ch2 */
					case 3:SIUL2.MSCR[SIUL_PA7].B.IBE = 1;  SIUL2.IMCR[74].B.SSS = 1;  break;	/* PA7->eTimer2 ch3 */
					case 4:SIUL2.MSCR[SIUL_PA8].B.IBE = 1;  SIUL2.IMCR[75].B.SSS = 1;  break;	/* PA8->eTimer2 ch4 */
					case 5:SIUL2.MSCR[SIUL_PA9].B.IBE = 1;  SIUL2.IMCR[76].B.SSS = 1;  break;	/* PA9->eTimer2 ch5 */
				}
				break;
			default:
				pETim = NULL;
				break;
		}	
		if((NULL != pETim) && mc_etim_isValidChan(chanL) && mc_etim_isValidChan(chanH))
		{
			/*2nd step: disable the corresponding channels*/
			pETim->ENBL.R &= ~(m_util_bitMask16(chanL) | m_util_bitMask16(chanH));

			/*3rd step, configure the registers*/
			pETim->CH[chanL].CTRL1.B.CNTMODE = 1; /*Count rising edges of primary source*/
			pETim->CH[chanL].CTRL1.B.PRISRC = 0x18 | (mc_etim_isValidDiv(div)?(div):(0));
			pETim->CH[chanL].CTRL1.B.ONCE = FALSE;	/*Count repeatedly.*/
			pETim->CH[chanL].CTRL1.B.LENGTH = FALSE;/*Roll over*/	
			#if mc_eTim_cntUp
			pETim->CH[chanL].CTRL1.B.DIR = 0;		/*count up*/
			#else
			pETim->CH[chanL].CTRL1.B.DIR = 1;		/*count down*/
			#endif
			pETim->CH[chanL].CTRL1.B.SECSRC = secondSoc;	/*select the input second source*/
			pETim->CH[chanL].COMP1.R = 0xFFFF;

			pETim->CH[chanL].CCCTRL.B.CLC2 = 0;		/*compare2 load control:never preload*/
			pETim->CH[chanL].CCCTRL.B.CLC1 = 0; 	/*compare1 load control:never preload*/
			pETim->CH[chanL].CCCTRL.B.CMPMODE = 2; 	/* compare on COMP1 when counting up, COMP2 when counting down*/
			pETim->CH[chanL].CCCTRL.B.CPT2MODE = 1; 	/* CAPT2 on falling edge*/
			pETim->CH[chanL].CCCTRL.B.CPT1MODE = 2; 	/*  CAPT1 on rising edge*/
			pETim->CH[chanL].CCCTRL.B.CFWM = 2;			/*Capture FIFO Water Mark : 1*/
			pETim->CH[chanL].CCCTRL.B.ONESHOT = 0;		/*free running mode is selected.*/								
			pETim->CH[chanL].CTRL3.R	= 1;		/* Halt channel counter during debug mode*/			

			pETim->CH[chanH].CTRL1.B.CNTMODE = 7;			/*Cascaded counter mode*/	
			pETim->CH[chanH].CTRL1.B.PRISRC = 0x10 | chanL;	/*primary source use the chanL output*/	
			pETim->CH[chanH].CTRL1.B.ONCE = FALSE;	/*Count repeatedly.*/
			pETim->CH[chanH].CTRL1.B.LENGTH = FALSE;/*Roll over*/	
			#if mc_eTim_cntUp
			pETim->CH[chanH].CTRL1.B.DIR = 0;		/*count up*/
			#else
			pETim->CH[chanH].CTRL1.B.DIR = 1;		/*count down*/
			#endif
			pETim->CH[chanH].CTRL1.B.SECSRC = secondSoc;	/*select the input second source*/
			pETim->CH[chanH].COMP1.R = 0xFFFF;
			pETim->CH[chanH].CCCTRL.B.CLC2 = 0;		/*compare2 load control:never preload*/
			pETim->CH[chanH].CCCTRL.B.CLC1 = 0; 	/*compare1 load control:never preload*/
			pETim->CH[chanH].CCCTRL.B.CMPMODE = 2; 	/* compare on COMP1 when counting up, COMP2 when counting down*/
			pETim->CH[chanH].CCCTRL.B.CPT2MODE = 1; 	/* CAPT2 on falling edge*/
			pETim->CH[chanH].CCCTRL.B.CPT1MODE = 2; 	/*  CAPT1 on rising edge*/
			pETim->CH[chanH].CCCTRL.B.CFWM = 2;			/*Capture FIFO Water Mark : 1*/
			pETim->CH[chanH].CCCTRL.B.ONESHOT = 0;		/*free running mode is selected.*/	
		  	pETim->CH[chanH].CTRL3.R	= 1;			/* free-running mode*/	
			if(TRUE == etimCasInCaptCfgGrp[inCaptIndx].isrEn)
			{
				pETim->CH[chanL].INTDMA.B.ICF1IE = 1;
				pETim->CH[chanH].INTDMA.B.ICF1IE = 1;
				pETim->CH[chanL].INTDMA.B.ICF2IE = 1;
				pETim->CH[chanH].INTDMA.B.ICF2IE = 1;	
			}
			else
			{
				pETim->CH[chanL].INTDMA.B.ICF1IE = 0;
				pETim->CH[chanH].INTDMA.B.ICF1IE = 0;
				pETim->CH[chanL].INTDMA.B.ICF2IE = 0;
				pETim->CH[chanH].INTDMA.B.ICF2IE = 0;
			}
			/*4th step*/
			/*re-enable the channels*/
			pETim->ENBL.R |= (m_util_bitMask16(chanL) | m_util_bitMask16(chanH));
		}

		etim_inCaptCasStart(inCaptIndx,TRUE);
	}
}

static void etim_inCaptCasStart(uint8_t casEtimIndx,bool start)
{
	uint8_t chanL;
	uint8_t chanH;
	volatile struct ETIMER_tag *  pETim = NULL;
	if(casEtimIndx < e_pi_max)
	{
		switch(etimCasInCaptCfgGrp[casEtimIndx].module)
		{
			case 0:pETim = &ETIMER_0;break;
			case 1:pETim = &ETIMER_1;break;
			case 2:pETim = &ETIMER_2;break;
			default: pETim = NULL;break;
		}
		chanL =   etimCasInCaptCfgGrp[casEtimIndx].chanL;
		chanH =   etimCasInCaptCfgGrp[casEtimIndx].chanH;
		if((NULL != pETim) && mc_etim_isValidChan(chanL) && mc_etim_isValidChan(chanH))
		{
			if(TRUE == start)
			{
				pETim->CH[chanL].CCCTRL.B.ARM = 1;
				pETim->CH[chanH].CCCTRL.B.ARM = 1;
			}
			else
			{
				pETim->CH[chanL].CCCTRL.B.ARM = 0;
				pETim->CH[chanH].CCCTRL.B.ARM = 0;
			}
		}
	}
}

void etim_inputCaptHandle(void)
{
	uint8_t inCaptIndx = 0;
	uint8_t chanL;
	uint8_t chanH;
	volatile struct ETIMER_tag *  pETim = NULL;
	for(inCaptIndx=0;inCaptIndx < e_pi_max; inCaptIndx ++)
	{
		switch(etimCasInCaptCfgGrp[inCaptIndx].module)
		{
			case 0:pETim = &ETIMER_0;break;
			case 1:pETim = &ETIMER_1;break;
			case 2:pETim = &ETIMER_2;break;
			default: pETim = NULL;break;
		}
		chanL =   etimCasInCaptCfgGrp[inCaptIndx].chanL;
		chanH =   etimCasInCaptCfgGrp[inCaptIndx].chanH;
		if((NULL != pETim) && mc_etim_isValidChan(chanL) && mc_etim_isValidChan(chanH))
		{
			if((pETim->CH[chanL].STS.B.ICF2 >0) && (pETim->CH[chanH].STS.B.ICF2 >0))
			{
				PiOverTimeTick[inCaptIndx] = 20;

				etim_hanldeCasCal(inCaptIndx);
			}
			else
			{
				if(PiOverTimeTick[inCaptIndx] >0)
				{
					PiOverTimeTick[inCaptIndx] --;
				}
				else
				{
					hld_pi_Duty[inCaptIndx] = 0;
					pwmPeriod[inCaptIndx] = 0;
				}
			}
		}
		else
		{
			hld_pi_Duty[inCaptIndx] = 0;
			pwmPeriod[inCaptIndx] = 0;
		}
	}
}

uint16_t etim_pi_getDuty(uint8_t indx)
{
	if(indx >= e_pi_max)
	{
		return 0;
	}

	return hld_pi_Duty[indx];
}

static void etim_hanldeCasCal(uint8_t indx)
{
	uint8_t chanL = etimCasInCaptCfgGrp[indx].chanL;
	uint8_t chanH = etimCasInCaptCfgGrp[indx].chanH;
	etim_CascalDutyandPeriod(indx,chanH,chanL);
}

static void etim_CascalDutyandPeriod(uint8_t PiIndx,uint8_t chanH, uint8_t chanL)
{
	volatile struct ETIMER_tag *  pETim = NULL;	
	uint32_t firstRiseEdge;
	uint32_t secondRiseEdge;
	uint32_t firstFallEdge;
	uint32_t secondFallEdge;
	uint32_t period;
	uint32_t period1;
	uint32_t HighPeriod;
	uint32_t LowPeriod = 0;
	bool riseEdgeCaptureFirst = TRUE;
	bool pwmOk = TRUE;
	uint8_t module = etimCasInCaptCfgGrp[PiIndx].module;
	switch(module)
	{
		case 0:pETim = &ETIMER_0;break;
		case 1:pETim = &ETIMER_1;break;
		case 2:pETim = &ETIMER_2;break;
		default: pETim = NULL;break;
	}
	if((NULL != pETim) && mc_etim_isValidChan(chanL) && mc_etim_isValidChan(chanH))
	{
		pETim->CH[chanL].STS.R |= 0xC0;
		pETim->CH[chanH].STS.R |= 0xC0;
	}

	firstRiseEdge = (pETim->CH[chanH].CAPT1.R << 16) | pETim->CH[chanL].CAPT1.R;
	secondRiseEdge = (pETim->CH[chanH].CAPT1.R << 16) | pETim->CH[chanL].CAPT1.R;
	firstFallEdge =(pETim->CH[chanH].CAPT2.R << 16) | pETim->CH[chanL].CAPT2.R;
	secondFallEdge = (pETim->CH[chanH].CAPT2.R << 16) | pETim->CH[chanL].CAPT2.R;

	/*here need to do lots of calculations*/
	
	/*----> Region 1: at first judge any two of all the edges time do not equal*/
	if((firstRiseEdge ==  secondRiseEdge) || (firstRiseEdge == firstFallEdge) || 
	  (firstRiseEdge == secondFallEdge) || 	(secondRiseEdge == firstFallEdge) ||
	  (secondRiseEdge == secondFallEdge) || (firstFallEdge == secondFallEdge))
	{
		pwmOk = FALSE;
	}
	
	if(TRUE == pwmOk)
	{
		/*----> Region 2: to judge the period is correct or not*/
		period =(uint32_t)(secondRiseEdge - firstRiseEdge);
		period1=(uint32_t)(secondFallEdge - firstFallEdge);
		#if 0
		if(FALSE == (((1.1 * period) > period1) && ((1.1 * period1) > period) && 
			((1.1 * period)  > mc_eTim_captPeriodCnt) && ((1.1 * mc_eTim_captPeriodCnt)  > period)))
		#else
		if(FALSE == (((1.1 * period) > period1) && ((1.1 * period1) > period)))
		#endif
		{
			/*because the time-space between two rising edges  with the time-space between two fall edges are very different*/
			/*or because the period calculated is very different with the expected input pwm*/
			pwmOk =  FALSE;	
		}
		else
		{
			period = ((period + period1) >> 1); /*average them to minium the error*/
		}
	}
	if(TRUE == pwmOk)
	{
		/*----> Region 3: to decide which edge captured first,rise or fall? */	
		if(firstRiseEdge < firstFallEdge) 
		{
			if((firstFallEdge - firstRiseEdge) <  period)
			{
				/*the rise edge  capture first*/
				riseEdgeCaptureFirst = TRUE;
			}
			else if((uint32_t)(firstRiseEdge - firstFallEdge) <  period)
			{
				/*roll over happened, actually fall edge captured first*/
				riseEdgeCaptureFirst = FALSE;
			}
			else
			{
				/*bad pwm, due to the wrong period*/
				pwmOk = FALSE;
			}
		}
		else
		{
			if((firstRiseEdge - firstFallEdge) <  period)
			{
				/*the fall edge  capture first*/
				riseEdgeCaptureFirst = FALSE;
			}
			else if((uint32_t)(firstFallEdge - firstRiseEdge) <  period)
			{
				/*roll over happened, actually rise edge captured first*/
				riseEdgeCaptureFirst = TRUE;
			}
			else
			{
				/*bad pwm, due to the wrong period*/
				pwmOk = FALSE;
			}
		}
	}
	if(TRUE == pwmOk)
	{
		/*----> Region 4: to decide whether the capture edges sequence is correct or not*/
		if(TRUE == riseEdgeCaptureFirst)
		{
			/*to find if roll over happened*/
			if(FALSE == ((firstRiseEdge < firstFallEdge) && (firstFallEdge < secondRiseEdge) 
						 && (secondRiseEdge < secondFallEdge)))
			{
				/*2 possibilities: counter roll over or bad sequence*/
				if(firstRiseEdge > firstFallEdge)
				{
					/*roll over happened between first rise edge and first fall edge*/	
					if(FALSE == (firstFallEdge < secondRiseEdge && secondRiseEdge < secondFallEdge))
					{
						pwmOk = FALSE;	/*bad sequence order*/
					}
				}
				else
				{
					if(firstFallEdge > secondRiseEdge)
					{
						/*roll over happened between firstFallEdge and secondRiseEdge*/
						if(secondRiseEdge > secondFallEdge)
						{
							pwmOk = FALSE;	/*bad sequence order*/
						}
					}
					else
					{
						if(secondRiseEdge < secondFallEdge)
						{
							/*surely one and only one roll over happen*/
							/*and it must happen between secondFallEdge and secondRiseEdge
							* so secondFallEdge shold be less than secondRiseEdge
							*/
							pwmOk = FALSE;	
						}
					}
				}
				
			}
			else
			{
				/*no over roll and their sequence order is correct*/
			}	
		}
		else
		{
			/*fall edge captured first*/
			if(FALSE == ((firstFallEdge < firstRiseEdge) && (firstRiseEdge < secondFallEdge) 
						 && (secondFallEdge < secondRiseEdge)))
			{
				/*2 possibilities: counter roll over or bad sequence*/
				if(firstFallEdge > firstRiseEdge)
				{
					/*roll over happened between firstFallEdge and firstRiseEdge*/	
					if(FALSE == (firstRiseEdge < secondFallEdge && secondFallEdge < secondRiseEdge))
					{
						pwmOk = FALSE;	/*bad sequence order*/
					}
				}
				else
				{
					if(firstRiseEdge > secondFallEdge)
					{
						/*roll over happened between firstRiseEdge and secondFallEdge*/
						if(secondFallEdge > secondRiseEdge)
						{
							pwmOk = FALSE;	/*bad sequence order*/
						}
					}
					else
					{
						if(secondFallEdge < secondRiseEdge)
						{
							/*surely one and only one roll over happen*/
							/*and it must happen between secondFallEdge and secondRiseEdge
							* so secondFallEdge shold be less than secondRiseEdge
							*/
							pwmOk = FALSE;	
						}
					}
				}
				
			}
			else
			{
				/*no over roll and their sequence order is correct*/
			}
		}
	}

	
	if(TRUE == pwmOk)
	{
		/* the pwm judge is correct, and calculate the period and duty*/
		if(TRUE == riseEdgeCaptureFirst)
		{
			if(firstFallEdge >= firstRiseEdge)
			{
				HighPeriod = firstFallEdge - firstRiseEdge;
			}
			else
			{
				HighPeriod = (uint32_t)(0xFFFFFFFFul - firstRiseEdge + firstFallEdge  + 1);
			}
			if(secondFallEdge >= secondRiseEdge)
			{
				HighPeriod += (secondFallEdge - secondRiseEdge);
				HighPeriod >>= 1;
			}
			else
			{
				HighPeriod += (uint32_t)(0xFFFFFFFFul - secondRiseEdge + secondFallEdge + 1); 
				HighPeriod >>= 1;
			}
			LowPeriod = period - HighPeriod;
		}
		else
		{
			if(firstRiseEdge >= firstFallEdge)
			{
				LowPeriod = firstRiseEdge - firstFallEdge;
			}
			else
			{
				LowPeriod = (uint32_t)(0xFFFFFFFFul - firstFallEdge + firstRiseEdge  + 1);
			}
			
			if(secondRiseEdge >= secondFallEdge)
			{
				LowPeriod += (secondRiseEdge - secondFallEdge);
				LowPeriod >>= 1;
			}
			else
			{
				LowPeriod += (uint32_t)(0xFFFFFFFFul - secondFallEdge + secondRiseEdge  + 1);
				LowPeriod >>= 1;
			}
			HighPeriod = period - LowPeriod;
		}
		
		if((0 == period) || (HighPeriod > period))
		{
			/*bad period or high period*/
			pwmOk = FALSE;
		}
		if(TRUE == pwmOk)
		{
			#if mc_pi_InputAdjust
			if(LowPeriod > 28)
			{
				LowPeriod -= 28;
			}

			HighPeriod = period - LowPeriod;

			HighPeriod+= 28;
			if(HighPeriod > period)
			{

				HighPeriod = period;
			}

			#endif
			pwmDuty = (HighPeriod * 10000)/period;
			pwmPeriod[PiIndx] = period;
			hld_pi_Duty[PiIndx] = pwmDuty;
		}
		else
		{
			/*wrong PWM*/
			
		}
	}
}


/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/




