/*******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2014 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
********************************************************************************
*
* $File Name:       SIUL.h$
* @file             SIUL.h
*
* $Date:            Jul-29-2014$
* @date             Jul-29-2014
*
* $Version:         0.9$
* @version          0.9
*
* Description:      SIUL driver source file
* @brief            SIUL driver source file
*
* --------------------------------------------------------------------
* $Name:  $
*******************************************************************************/
/****************************************************************************//*!
*
*  @mainpage SIUL driver for MPC5744P
*
*  @section Intro Introduction
*
*	This package contains SIUL driver for MPC5744P allowing to configure and 
*	control general Input/Output ports.

*  The key features of this package are the following:
*  - Configure pads as digital/analog
*  - Configure pads as input/outputs
*  - Set actual state of the output pads
*
*  For more information about the functions and configuration items see these documents: 
*
*******************************************************************************
*
* @attention 
*            
*******************************************************************************/
/*==================================================================================================
*   Project              : PowerSBC
*   Platform             : MPC5744P
*   Dependencies         : MPC5744P - Basic SW drivers.
*   All Rights Reserved.
==================================================================================================*/

/*==================================================================================================
Revision History:
                             Modification     Function
Author (core ID)              Date D/M/Y       Name		  Description of Changes
B35993		 				  29/07/2014 	   ALL		  Driver created

---------------------------   ----------    ------------  ------------------------------------------
==================================================================================================*/

#include "Defines.h"
#include "SIUL.h"

/***************************************************************************//*!
*   @brief The function SIUL_DigitalInput configures a GPIO pin as a digital 
*			input.
*	@par Include 
*					SIUL.h
* 	@par Description 
*					This function configures specified IO pin as a digital input 
*					with different multiplexing options.
* 	@param[in] pad
*					Pad to be configured - use either PCR pad number or some 
*					predefined name.
*						Predefined values:		
*						- <tt>"SIUL_PA0" </tt><tt>"SIUL_PA1" </tt><tt>"SIUL_PA2" </tt>
*						- <tt>"SIUL_PA3"</tt><tt>"SIUL_PA4"</tt><tt>"SIUL_PA5"</tt>
*						- <tt>"SIUL_PA6"</tt><tt>"SIUL_PA7"</tt><tt>"SIUL_PA8"</tt>
*						- <tt>"SIUL_PA9"</tt><tt>"SIUL_PA10"</tt><tt>"SIUL_PA11"</tt>
*						- <tt>"SIUL_PA12"</tt><tt>"SIUL_PA13"</tt><tt>"SIUL_PA14"</tt>
*						- <tt> "SIUL_PA15" </tt> <tt>"SIUL_PB0"</tt><tt>"SIUL_PB1"</tt>
*						- <tt> "SIUL_PB2" </tt> <tt> "SIUL_PB3" </tt> <tt> "SIUL_PB4"</tt>
*						- <tt> "SIUL_PB5" </tt> <tt> "SIUL_PB6" </tt> <tt> "SIUL_PB7"</tt>
*						- <tt> "SIUL_PB8" </tt> <tt> "SIUL_PB9" </tt> <tt> "SIUL_PB10" </tt>
*						- <tt> "SIUL_PB11" </tt> <tt> "SIUL_PB12" </tt> <tt> "SIUL_PB13" </tt>
*						- <tt> "SIUL_PB14" </tt> <tt> "SIUL_PB15" </tt> <tt> "SIUL_PC0" </tt>
*						- <tt> "SIUL_PC1" </tt> <tt> "SIUL_PC2" </tt> <tt> "SIUL_PC3" </tt>
*						- <tt> "SIUL_PC4" </tt> <tt> "SIUL_PC5" </tt> <tt> "SIUL_PC6" </tt>
*						- <tt> "SIUL_PC7" </tt> <tt> "SIUL_PC8" </tt> <tt> "SIUL_PC9" </tt>
*						- <tt> "SIUL_PC10" </tt> <tt> "SIUL_PC11" </tt> <tt> "SIUL_PC12" </tt>
*						- <tt> "SIUL_PC13" </tt> <tt> "SIUL_PC14" </tt> <tt> "SIUL_PC15" </tt>
*						- <tt> "SIUL_PD0" </tt> <tt> "SIUL_PD1" </tt> <tt> "SIUL_PD2" </tt>
*						- <tt> "SIUL_PD3" </tt> <tt> "SIUL_PD4" </tt> <tt> "SIUL_PD5" </tt>
*						- <tt> "SIUL_PD6" </tt> <tt> "SIUL_PD7" </tt> <tt> "SIUL_PD8" </tt>
*						- <tt> "SIUL_PD9" </tt> <tt> "SIUL_PD10" </tt> <tt> "SIUL_PD11" </tt>
*						- <tt> "SIUL_PD12" </tt> <tt> "SIUL_PD13" </tt> <tt> "SIUL_PD14" </tt>
*						- <tt> "SIUL_PD15" </tt> <tt> "SIUL_PE0" </tt> <tt> "SIUL_PE1" </tt>
*						- <tt> "SIUL_PE2" </tt> <tt> "SIUL_PE3" </tt> <tt> "SIUL_PE4" </tt>
*						- <tt> "SIUL_PE5" </tt> <tt> "SIUL_PE6" </tt> <tt> "SIUL_PE7" </tt>
*						- <tt> "SIUL_PE8" </tt> <tt> "SIUL_PE9" </tt> <tt> "SIUL_PE10" </tt>
*						- <tt> "SIUL_PE11" </tt> <tt> "SIUL_PE12" </tt> <tt> "SIUL_PE13" </tt>
*						- <tt> "SIUL_PE14" </tt> <tt> "SIUL_PE15" </tt> <tt> "SIUL_PF0" </tt>
*						- <tt> "SIUL_PF1" </tt> <tt> "SIUL_PF2" </tt> <tt> "SIUL_PF3" </tt>
*						- <tt> "SIUL_PF4" </tt> <tt> "SIUL_PF5" </tt> <tt> "SIUL_PF6" </tt>
*						- <tt> "SIUL_PF7" </tt> <tt> "SIUL_PF8" </tt> <tt> "SIUL_PF9" </tt>
*						- <tt> "SIUL_PF10" </tt> <tt> "SIUL_PF11" </tt> <tt> "SIUL_PF12" </tt>
*						- <tt> "SIUL_PF13" </tt> <tt> "SIUL_PF14" </tt> <tt> "SIUL_PF15" </tt>
*						- <tt> "SIUL_PG0" </tt> <tt> "SIUL_PG1" </tt> <tt> "SIUL_PG2" </tt>
*						- <tt> "SIUL_PG3" </tt> <tt> "SIUL_PG4" </tt> <tt> "SIUL_PG5" </tt>
*						- <tt> "SIUL_PG6" </tt> <tt> "SIUL_PG7" </tt> <tt> "SIUL_PG8" </tt>
*						- <tt> "SIUL_PG9" </tt> <tt> "SIUL_PG10" </tt> <tt> "SIUL_PG11" </tt>
*						- <tt> "SIUL_PG12" </tt> <tt> "SIUL_PG13" </tt> <tt> "SIUL_PG14" </tt>
*						- <tt> "SIUL_PG15" </tt> <tt> "SIUL_PH0" </tt> <tt> "SIUL_PH1" </tt>
*						- <tt> "SIUL_PH2" </tt> <tt> "SIUL_PH3" </tt> <tt> "SIUL_PH4" </tt>
*						- <tt> "SIUL_PH5" </tt> <tt> "SIUL_PH6" </tt> <tt> "SIUL_PH7" </tt>
*						- <tt> "SIUL_PH8" </tt> <tt> "SIUL_PH9" </tt> <tt> "SIUL_PH10" </tt>
*						- <tt> "SIUL_PH11" </tt> <tt> "SIUL_PH12" </tt> <tt> "SIUL_PH13" </tt>
*						- <tt> "SIUL_PH14" </tt> <tt> "SIUL_PH15" </tt> <tt> "SIUL_PI0" </tt>
*						- <tt> "SIUL_PI1" </tt> <tt> "SIUL_PI2" </tt> <tt> "SIUL_PI3" </tt>
*						- <tt> "SIUL_PI4" </tt> <tt> "SIUL_PI5" </tt> <tt> "SIUL_PI6" </tt>
*						- <tt> "SIUL_PI7" </tt> <tt> "SIUL_PI8" </tt> <tt> "SIUL_PI9" </tt>
*						- <tt> "SIUL_PI10" </tt> <tt> "SIUL_PI11" </tt> <tt> "SIUL_PI12" </tt>
*						- <tt> "SIUL_PI13" </tt> <tt> "SIUL_PI14" </tt> <tt> "SIUL_PI15" </tt>
*						- <tt> "SIUL_PJ0" </tt> <tt> "SIUL_PJ1" </tt> <tt> "SIUL_PJ2" </tt>
*						- <tt> "SIUL_PJ3" </tt> <tt> "SIUL_PJ4" </tt>
*	@param[in] padIn
*					Defines number of the IMCR multiplexing register for the used pad.
*	@param[in] config
*					Additional configuration of the specified pad. Use number 
*						0 if no additional configuration is desired.
*						Predefined values:
*							</param>
*						Pad pull-up/Down resistors			</param> 
*						- "SIUL_PULL_UP" - Internal pull-up resistor will 
*						be used.	
*						- "SIUL_PULL_DOWN" - Internal pull-down resistor 
*						will be used.	
*						- "SIUL_PULL_DIS" - Internal pull-up/down resistors 
*						will be disabled.
*						Pad direction						</param>
*						- "SIUL_INPUT" - Input buffer will be activated.
*						- "SIUL_OUTPUT" - Output buffer will be activated.
*						- "SIUL_BIDIR" - Input and output buffers will be 
*						activated (0x0300).
*						Pad function						</param>	 
*						- "SIUL_GPIO" - Pad will be used as an General IO pin.
*						- "SIUL_ALT1" - Alternate function no.1 will be used on 
*						this pin.
*						- "SIUL_ALT2"	- Alternate function no.2 will be 
*						used on	this pin.
*						- "SIUL_ALT3" - Alternate function no.3 will be used 
*						on this pin.
*						Safe mode control					</param>
*						- "SIUL_SMC"	0x4000
*						Pad type	 
*						- "SIUL_DIGITAL" - Pin will be configured as a digital one.
*						- "SIUL_ANALOG" - Pin will be configured as an analog one 
*						(to be used as an input for ADC).
*						Pad slew rate						</param>	 
*						- "SIUL_SLOW" - Slow slew rate will be used. 	
*						- "SIUL_FAST" - Fast slew rate will be used.
*						Output type							</param> 
*						- "SIUL_NORMAL"	- Output will be in a Normal (push-pull) 
*						configuration.
*						- "SIUL_OPEND" - Output will be in an Open-Drain 
*						configuration.
*	@param[in] config_in
*					Defines configuration of the multiplexing IMCR register.
*	@remarks 	Predefined values can be combined using logical | operator.
*	@par Code sample
*			SIUL_DigitalInput(SIUL_PC7, 41, SIUL_PULL_UP | SIUL_SLEW_ENB, SIUL_ALT1);
*			- Command sets PC7 pad to be a digital input used as a serial input 
*			for the SPI module. Pad is using an internal pull-up resistor.
********************************************************************************/
void SIUL_DigitalInput(uint8_t pad, uint8_t pad_in, uint32_t config, uint32_t config_in)
{
    SIUL2.MSCR[pad].R = SIUL_INPUT | config;
    SIUL2.IMCR[pad_in].R = config_in;
}

/***************************************************************************//*!
*   @brief The function SIUL_DigitalInputSimple configures GPIO pin as a digital input.
*	@par Include: 
*					SIUL.h
* 	@par Description: 
*					This function configures specified IO pin as a digital input.
* 	@param[in] pad - 	Pad to be configured - use either MSCR pad number or some 
*						predefined name. For predefined values, see the 
*						SIUL_DigitalInput function.
*	@param[in] config - Additional configuration of the specified pad. Use number 
*						0 if no additional configuration is desired. For predefined 
*						values, see the SIUL_DigitalInput function.
*	@remarks 	Predefined values can be combined using logical | operator.
********************************************************************************/
void SIUL_DigitalInputSimple(uint8_t pad, uint32_t config)
{
    SIUL2.MSCR[pad].R = SIUL_INPUT | config;
}

/***************************************************************************//*!
*   @brief The function SIUL_AnalogInput configures a GPIO pin as an analog input.
*	@par Include 
*					SIUL.h
* 	@par Description 
*					This function configures specified IO pin as an analog input.
* 	@param[in] pad
*					Pad to be configured - use either PCR pad number or some 
*						predefined name.
*	@param[in] config
*					Additional configuration of the specified pad. Use number 
*						0 if no additional configuration is desired.
*	@remarks 	Predefined values can be combined using logical | operator.
*	@par Code sample
*			SIUL_AnalogInput(SIUL_PB7, 0);
*			- Command sets PB7 pin as an analog input with no additional 
*			configuration.
********************************************************************************/
void SIUL_AnalogInput(uint8_t pad, uint32_t config)
{
    SIUL2.MSCR[pad].R = SIUL_ANALOG | 0x10000000;
}

/***************************************************************************//*!
*   @brief The function SIUL_DigitalOutput configures a GPIO pin as a digital output.
*	@par Include 
*					SIUL.h
* 	@par Description 
*					This function configures specified IO pin as a digital output.
* 	@param[in] pad
*					Pad to be configured - use either PCR pad number or some 
*					predefined name.
*	@param[in] config
*					Additional configuration of the specified pad. Use number 
*					0 if no additional configuration is desired.
*	@remarks 	Predefined values can be combined using logical | operator.
*	@par Code sample
*			SIUL_DigitalOutput(SIUL_PD8, SIUL_ALT1);
*			- Command sets pad PD8 as a digital output with its first alternate 
*			function.
********************************************************************************/
void SIUL_DigitalOutput(uint8_t pad, uint32_t config)
{
    SIUL2.MSCR[pad].R = SIUL_OUTPUT | config;
}

/***************************************************************************//*!
*   @brief The function SIUL_DigitalIO configures a GPIO pin as an Input/Output.
*	@par Include 
*					SIUL.h
* 	@par Description 
*					This function configures specified IO pin as a digital Input/Output.
* 	@param[in] pad
*					Pad to be configured - use either PCR pad number or some 
*					predefined name.
*	@param[in] config
*					Additional configuration of the specified pad. Use number 
*					0 if no additional configuration is desired.
*	@remarks 	Predefined values can be combined using logical | operator.
*	@par Code sample
*			SIUL_DigitalIO(SIUL_PD2,0);
*			- Command sets pad PD2 as a digital Input/Output (both input and 
*			output buffers are enabled).
********************************************************************************/
void SIUL_DigitalIO(uint8_t pad, uint32_t config)
{
    SIUL2.MSCR[pad].R = SIUL_BIDIR | config;
}

/***************************************************************************//*!
*   @brief The function SIUL_ToggleIO toggles a GPIO pin.
*	@par Include 
*					SIUL.h
* 	@par Description 
*					This function toggles actual state  of the specified pad.
* 	@param[in] pad
*					Pad to be toggled - use either PCR pad number or some 
*					predefined name.
*	@remarks 	Predefined values can be combined using logical | operator.
*	@par Code sample
*															</param>
*			SIUL_DigitalIO(SIUL_PD2,0);						</param>
*			SIUL_ToggleIO(SIUL_PD2);						</param>
*			- First command sets pad PD2 as a digital Input/Output. Second 
*			command inverts actual state of the PD2 pad.
********************************************************************************/
void SIUL_ToggleIO(uint8_t pad)
{
    SIUL2.GPDO[pad].R = ~SIUL2.GPDI[pad].R;
}

/***************************************************************************//*!
*   @brief The function SIUL_EnableExtInt enables an interrupt for a GPIO pin.
*	@par Include 
*					SIUL.h
* 	@par Description 
*					This function enables external interrupt for the specified 
*					EIRQ - External Interrupt Request Input specified by its 
*					mask. This is done in the IRER - Interrupt Request Enable 
*					Register.
* 	@param[in] maskEIRQ
*					Mask to specify which EIRQs will be activated.
*					Predefined values:
*					- "SIUL_EIRQ0"
*					- "SIUL_EIRQ1"
*					- "SIUL_EIRQ2"
*					- "SIUL_EIRQ3"
*					- "SIUL_EIRQ4"
*					- "SIUL_EIRQ5"
*					- "SIUL_EIRQ6"
*					- "SIUL_EIRQ7"
*					- "SIUL_EIRQ8"
*					- "SIUL_EIRQ9"
*					- "SIUL_EIRQ10"
*					- "SIUL_EIRQ11"
*					- "SIUL_EIRQ12"
*					- "SIUL_EIRQ13"
*					- "SIUL_EIRQ14"
*					- "SIUL_EIRQ15"
*					- "SIUL_EIRQ16"
*					- "SIUL_EIRQ17"
*					- "SIUL_EIRQ18"
*					- "SIUL_EIRQ19"
*					- "SIUL_EIRQ20"
*					- "SIUL_EIRQ21"
*					- "SIUL_EIRQ22"
*					- "SIUL_EIRQ23"
*					- "SIUL_EIRQ24"
*					- "SIUL_EIRQ25"
*					- "SIUL_EIRQ26"
*					- "SIUL_EIRQ27"
*					- "SIUL_EIRQ28"
*					- "SIUL_EIRQ29"
*					- "SIUL_EIRQ30"
*					- "SIUL_EIRQ31"
*	@remarks 	Predefined values can be combined using logical | operator.
*	@par Code sample
*			SIUL_EnableExtInt(SIUL_EIRQ31);
*			- Command enables external interrupt for the PF13 pad.
********************************************************************************/
void SIUL_EnableExtInt(uint32_t maskEIRQ){
		
	SIUL2.IREER0.R = SIUL2.IREER0.R | maskEIRQ;				//ext INT enable	
}

/***************************************************************************//*!
*   @brief The function SIUL_EnableExtIntRisingEdge enables an interrupt 
*			on a rising edge.
*	@par Include 
*					SIUL.h
* 	@par Description 
*					This function enables an input filter and interrupt launched on 
*					the rising edge. Filter enables e.g. to filter out switching 
*					noise, when a button is connected directly to the input 
*					without using any special external circuitry.
* 	@param[in] maskEIRQ
*					Mask to specify for which EIRQs the filter and 
*					interrupt will be enabled.
*	@remarks 	Predefined values can be combined using logical | operator.
*	@par Code sample
*			SIUL_EnableExtIntRisingEdge(64);
*			- Command enables filter and an interrupt on the rising edge for 
*			the EIRQ6 - pad PA6.
********************************************************************************/
void SIUL_EnableExtIntRisingEdge(uint32_t maskEIRQ){

	SIUL2.IFER0.R = SIUL2.IFER0.R | maskEIRQ;				//filter enable
	SIUL2.IREER0.R = SIUL2.IREER0.R | maskEIRQ;			//rising edge INT enabled
	
}

/***************************************************************************//*!
*   @brief The function SIUL_EnableExtIntFallingEdge enables an interrupt on a falling edge.
*	@par Include 
*					SIUL.h
* 	@par Description 
*					This function enables an input filter and interrupt launched on 
*					the falling edge. Filter enables e.g. to filter out switching 
*					noise, when a button is connected directly to the input 
*					without using any special external circuitry.
* 	@param[in] maskEIRQ
*					Mask to specify for which EIRQs the filter and 
*					interrupt will be enabled.
*	@remarks 	Predefined values can be combined using logical | operator.
*	@par Code sample
*			SIUL_EnableExtIntFallingEdge(64);
*			- Command enables filter and an interrupt on the falling edge for 
*			the EIRQ6 - pad PA6.
********************************************************************************/
void SIUL_EnableExtIntFallingEdge(uint32_t maskEIRQ){

	SIUL2.IFER0.R = SIUL2.IFER0.R | maskEIRQ;				//filter enable
	SIUL2.IFEER0.R = SIUL2.IFEER0.R | maskEIRQ;			//falling edge INT enabled	
}

/***************************************************************************//*!
*   @brief The function SIUL_GetExtIntFlag returns the interrupt flag.
*	@par Include 
*					SIUL.h
* 	@par Description 
*					This function returns interrupt flag for the specified 
*					EIRQ
*						External Interrupt Request Input.
* 	@param[in] maskEIRQ
*						Mask to specify for which EIRQs the filter and 
*						interrupt will be enabled.
*	@return	Returns interrupt flag from the ISR - Interrupt Status Flag Register.
*	@remarks 	Predefined values can be combined using logical | operator.
*	@par Code sample
*			int_flag = SIUL_GetExtIntFlag(SIUL_EIRQ31);
*			- Command returns and stores interrupt flag for EIRQ31 - pad PF13.
********************************************************************************/
uint32_t SIUL_GetExtIntFlag(uint32_t maskEIRQ){
	if((SIUL2.DISR0.R & maskEIRQ) > 0){		//test EIF flag
		return 1;
	}
	else{
		return 0;
	}
}

/***************************************************************************//*!
*   @brief The function SIUL_ClearExtIntFlag clears the interrupt flag.
*	@par Include 
*					SIUL.h
* 	@par Description 
*					This function clears interrupt flag in the Interrupt Status 
*					Register (ISR) for the specified External Interrupt 
*					Request Input (EIRQ).
* 	@param[in] maskEIRQ
*					Mask to specify which EIRQ's flag will be cleared.
*	@remarks 	Predefined values can be combined using logical | operator.
*	@par Code sample
*			SIUL_ClearExtIntFlag(SIUL_EIRQ31);
*			- Command clears interrupt flag for EIRQ31 - pad PF13.
********************************************************************************/
void SIUL_ClearExtIntFlag(uint32_t maskEIRQ){
	SIUL2.DISR0.R = maskEIRQ;											//clear EIF flag
}

/***************************************************************************//*!
*   @brief The function SIUL_SetPad sets the GPIO pin state (logical HIGH).
*	@par Include 
*					SIUL.h
* 	@par Description 
*					This function sets state of the specified pad to the logical HIGH 
*					state.
* 	@param[in] pad
*					Pad to be set - use either PCR pad number or some 
*					predefined name.
*	@remarks 	Pad should be previously configured as a digital output 
*				(see SIUL_DigitalOutput function).
*	@par Code sample
*																	</param>
*			SIUL_DigitalOutput(SIUL_PD8);							</param>
*			SIUL_SetPad(SIUL_PD8);									</param>
*			- First command configures pad PD8 as a digital output and 
*			the second command sets its state to logical HIGH.
********************************************************************************/
void SIUL_SetPad(uint8_t pad){				
	SIUL2.GPDO[pad].R = 1;
}

/***************************************************************************//*!
*   @brief The function SIUL_ClearPad clears the GPIO pin state (logical LOW).
*	@par Include 
*					SIUL.h
* 	@par Description 
*					This function clears state of the specified pad (logical LOW state).
* 	@param[in] pad
*					Pad to be cleared - use either PCR pad number or some 
*					predefined name.
*	@remarks 	Predefined values can be combined using logical | operator.
*	@par Code sample
*																	</param>
*			SIUL_DigitalOutput(SIUL_PD8);							</param>
*			SIUL_ClearPad(SIUL_PD8);								</param>
*			- First command configures pad PD8 as a digital output and the second 
*			command clears its state (logical LOW).
********************************************************************************/
void SIUL_ClearPad(uint8_t pad){						
	SIUL2.GPDO[pad].R = 0;
}
 
/***************************************************************************//*!
*   @brief The function SIUL_GetPadState gets states of the GPIO pin.
*	@par Include 
*					SIUL.h
* 	@par Description 
*					This function gets state of the specified pad.
* 	@param[in] pad
*					Pad that will be read.
*	@remarks 	The specified pad has to be configured as input (its input buffer 
*				has to be previously enabled).
*	@par Code sample
*			SIUL_GetPadState(SIUL_PD8);							</param>
*			- Command gets and returns actual state of the pin PD8.
********************************************************************************/
uint32_t SIUL_GetPadState(uint32_t pad){
	return SIUL2.GPDI[pad].R;
}


#if 1

void SIUL_SetIRQPin(void)
{
	SIUL2.IMCR[178].B.SSS = 1; /*SIUL2 External Interrupt Source 5*/

	/*Mask interrupts by clearing the EIREn bits in DIRER0*/
	SIUL2.DIRER0.B.EIRE5 = 0;

	/*Select the pin polarity by setting the appropriate IREEn bits in IREER0 and the
      appropriate IFEEn bits in IFEER0 as desired.*/
	SIUL_EnableExtIntRisingEdge(SIUL_EIRQ5);

	/*Configure the appropriate bits in the MSCR[0:511] register for the external interrupt
      pin(s) desired as follows:*/
	/*Clear the OBE and ODE bits to disable output*/
	SIUL2.MSCR[5].B.OBE = 0;
	SIUL2.MSCR[5].B.ODE = 0;

	/*Set the IBE bit to enable the pin's input buffer.*/
	SIUL2.MSCR[5].B.IBE = 1;
	/*If using the internal pullup/pulldown, configure the appropriate PUE and PUS fields.*/

	/*Select the request desired between DMA or Interrupt by writing the appropriate DIRSn bits in DIRSR0.*/
	SIUL2.DIRSR0.B.DIRSR5 = 0;

	/*Select the desired glitch filter setup for the pins by writing the following:*/
	/*Write the Filter Counter setting to the desired value by writing the MAXCNT[3:0] bits in
	 the IFMCn register for the respective external interrupt that is being used.*/
	SIUL2.IFMCR[5].B.MAXCNT = 0;
	/*Set the Filter Clock Prescaler setting from 0 to 15 to the desired value by writing
      the IFCP[3:0] bits in the IFCPR register.*/
	SIUL2.IFCPR.B.IFCP = 0;
	/*Enable the glitch filter for the desired external interrupt pins by setting the
      appropriate IFEn bits in IFER0.*/
	SIUL2.IFER0.B.IFE5 = 1;

	/*Write to EIFn bits in DISR0 to as desired to clear any flags.*/
	SIUL2.DISR0.B.EIF5 = 0;

	/*Enable the interrupt pins by setting the appropriate EIREn bits in DIRER0.*/
	SIUL2.DIRER0.B.EIRE5 = 1;

#if 0
	SIUL2.IMCR[178].B.SSS = 1; /*SIUL2 External Interrupt Source 5*/
	SIUL2.DIRSR0.B.DIRSR5 = 0;
	SIUL2.IFER0.B.IFE5 = 1;
	SIUL2.DISR0.B.EIF5 = 1;
	SIUL2.DIRSR0.B.DIRSR5 = 1;
#endif
}

void SIUL_extIRQ5(void)
{
	SIUL_ClearPad(SIUL_PD4);
	SIUL_ClearPad(SIUL_PD5);
	SIUL_ClearPad(SIUL_PD6);
	SIUL_ClearPad(SIUL_PD7);
}
#endif
