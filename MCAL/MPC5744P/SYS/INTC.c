/*******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2014 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
********************************************************************************
*
* $File Name:       INTC.c$
* @file             INTC.c
*
* $Date:            Jul-29-2014$
* @date             Jul-29-2014
*
* $Version:         0.9$
* @version          0.9
*
* Description:      INTC driver source file
* @brief            INTC driver source file
*
* --------------------------------------------------------------------
* $Name:  $
*******************************************************************************/
/****************************************************************************//*!
*
*  @mainpage INTC driver for MPC5744P
*
*  @section Intro Introduction
*
*	This package contains INTC driver for MPC5744P allowing initialize 
*	interrupt controller and to register interrupt service routines.

*  The key features of this package are the following:
*  - Initialize interrupt controller
*  - Register interrupt service routines
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
#include "PwSBC.h"
//#include "PIT.h"
#include "INTC.h"

extern uint32_t IntcIsrVectorTable[];


 /***************************************************************************//*!
*   @brief The function INTC_InstallINTCInterruptHandler installs an 
*			interrupt handler into the vector table.
*	@par Include: 
*					INTC.h
* 	@par Description: 
*					This function installs interrupt handler into the vector table 
*					and sets its priority.
*	@remarks 
********************************************************************************/
void INTC_InstallINTCInterruptHandler(INTCInterruptFn handlerFn, unsigned short vectorNum, unsigned char psrPriority)
{
    /* Set the function pointer in the ISR Handler table */
	//if(NULL_PTR != handlerFn)
	//{
		//IntcIsrVectorTable[vectorNum] = (uint32_t)&handlerFn;
	//}
    /* Set the PSR Priority */
    INTC_0.PSR[vectorNum].B.PRIN = psrPriority; 
}

 /***************************************************************************//*!
*   @brief The function INTC_Init initializes Interrupt Controller (INTC).
*	@par Include 
*					INTC.h
* 	@par Description 
*					This function initializes interrupts in the core, in  the 
*					Interrupt Controller (INTC) and registers interrupt 
*					service routines for PIT, SIUL, ADC and LINFLEX_UART modules.
*	@remarks 	Interrupt vector for each module is chosen depending on the 
*				actual setting that is made in external files.
*	@par Code sample
*			INTC_Init();
*			- Command initializes interrupt controller and registers interrupt 
*			service routines.
********************************************************************************/
void INTC_Init(void)
{
    // Initialise Core IVPR and IVOR Registers
    //e200zX_Interrupt_Setup();

	//Choose software (HVEN=0) or hardware (HVEN=1) vector mode
    INTC_0.BCR.R = 0x00000000;

    //Set INTC ISR vector table base addr.
    INTC_0.IACKR0.R = (uint32_t) &IntcIsrVectorTable[0];
	//Set priorities - PIT
	switch(PIT_WD_CH){
		case 0 : INTC_0.PSR[226].B.PRIN = INT_WD_PRIORITY; break;
		case 1 : INTC_0.PSR[227].B.PRIN = INT_WD_PRIORITY; break;
		case 2 : INTC_0.PSR[228].B.PRIN = INT_WD_PRIORITY; break;
		case 3 : INTC_0.PSR[229].B.PRIN = INT_WD_PRIORITY; break;
	}
	//Set priorities - ADC
	switch(ADC_NB){
		case 0 : INTC_0.PSR[496].B.PRIN = INT_ADC_PRIORITY; break;
		case 1 : INTC_0.PSR[500].B.PRIN = INT_ADC_PRIORITY; break;
		case 2 : INTC_0.PSR[504].B.PRIN = INT_ADC_PRIORITY; break;
		case 3 : INTC_0.PSR[508].B.PRIN = INT_ADC_PRIORITY; break;
	}
	//Set priorities - SIUL
	if(SIUL_INT_EIRQ <= SIUL_EIRQ7)
    {
		INTC_0.PSR[243].B.PRIN = INT_SIUL_PRIORITY;		//SIUL ISR IRQ 243
	}
	else
    {
		if(SIUL_INT_EIRQ <= SIUL_EIRQ15)
        {
			INTC_0.PSR[244].B.PRIN = INT_SIUL_PRIORITY;		//SIUL ISR IRQ 244
		}
		else
        {
			if(SIUL_INT_EIRQ <= SIUL_EIRQ23)
            {
				INTC_0.PSR[245].B.PRIN = INT_SIUL_PRIORITY;		//SIUL ISR IRQ 245
			}
			else
            {
				INTC_0.PSR[246].B.PRIN = INT_SIUL_PRIORITY;		//SIUL ISR IRQ 246
			}
		}
	}

	INTC_0.PSR[380].B.PRIN = INT_UART_RX_PRIORITY;		//UART ISR IRQ 380

	//The INTC's current priority must be lower than peripheral priority to activate interrupt
	INTC_0.CPR0.B.PRI = 0;
	// Enable external interrupts
	asm(" wrteei 1");	
}

