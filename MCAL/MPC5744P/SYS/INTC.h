/*******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2014 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
********************************************************************************
*
* $File Name:       INTC.h$
* @file             INTC.h
*
* $Date:            Jul-29-2014$
* @date             Jul-29-2014
*
* $Version:         0.9$
* @version          0.9
*
* Description:      INTC driver header file
* @brief            INTC driver header file
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

#ifndef _INTC_H
#define _INTC_H

/** All interrupt handlers should be of this type */
typedef void(*INTCInterruptFn)(void);

void INTC_InstallINTCInterruptHandler(INTCInterruptFn, unsigned short, unsigned char);
void INTC_Init(void);



#endif
