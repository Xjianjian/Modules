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
* Description:      SIUL driver header file
* @brief            SIUL driver header file
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

#ifndef _GPIO_H_
#define _GPIO_H_

void SIUL_DigitalInput(uint8_t, uint8_t, uint32_t, uint32_t);
void SIUL_AnalogInput(uint8_t, uint32_t);
void SIUL_DigitalInputSimple(uint8_t, uint32_t);
void SIUL_DigitalOutput(uint8_t, uint32_t);
void SIUL_DigitalIO(uint8_t, uint32_t);
void SIUL_ToggleIO(uint8_t pad);
void SIUL_EnableExtInt(uint32_t);
void SIUL_EnableExtIntRisingEdge(uint32_t);
void SIUL_EnableExtIntFallingEdge(uint32_t);
uint32_t SIUL_GetExtIntFlag(uint32_t);
void SIUL_ClearExtIntFlag(uint32_t);


void SIUL_SetPad(uint8_t);
void SIUL_ClearPad(uint8_t);
uint32_t SIUL_GetPadState(uint32_t);


#if 1
void SIUL_SetIRQPin(void);
void SIUL_extIRQ5(void);
#endif

#endif
