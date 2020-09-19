/*******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
********************************************************************************
*
* $File Name:       ME.h$
* @file             ME.h
*
* $Date:            Jul-31-2014$
* @date             Jul-31-2014
*
* $Version:         0.9$
* @version          0.9
*
* Description:      ME driver header file
* @brief            ME driver header file
*
* --------------------------------------------------------------------
* $Name:  $
*******************************************************************************/
/****************************************************************************//*!
*
*  @mainpage ME driver for MPC5744P
*
*  @section Intro Introduction
*
*	This package contains ME driver for MPC5744P allowing to 
*	initialize and configure ME module.
*
*  The key features of this package are the following:
*  - Initialize ME module
*  - Change actual mode
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
B35993		 				  31/07/2014 	   ALL		  Driver created

---------------------------   ----------    ------------  ------------------------------------------
==================================================================================================*/
#ifndef _ME_H_
#define _ME_H_

#define DBGm 	0 //to active it => 0x40
		
#define	ME_LPPC0	0
#define	ME_LPPC1 	8
#define	ME_LPPC2 	16
#define	ME_LPPC3 	24
#define	ME_LPPC4 	32
#define	ME_LPPC5 	40
#define	ME_LPPC6 	48
#define	ME_LPPC7 	56


#define	ME_RUNPC0 	0
#define	ME_RUNPC1 	1
#define	ME_RUNPC2 	2
#define	ME_RUNPC3 	3
#define	ME_RUNPC4 	4
#define	ME_RUNPC5 	5
#define	ME_RUNPC6 	6
#define	ME_RUNPC7 	7

#define ME_MODE_KEY 0x5AF0


#define ME_PAD_ON 	0x00000000
#define ME_PAD_OFF	0x00800000

#define ME_VREG_ON 	0x00100000
#define ME_VREG_OFF 	0x00000000

#define ME_DFLASH_ON 	0x000C0000
#define ME_DFLASH_LPM 	0x00080000
#define ME_DFLASH_OFF 	0x00040000

#define ME_CFLASH_ON 	0x00030000
#define ME_CFLASH_LPM 	0x00020000
#define ME_CFLASH_OFF 	0x00010000

#define ME_FMPLL_ON 	0x00000040
#define ME_FMPLL_OFF 	0x00000000

#define ME_XOSC_ON 	0x00000020
#define ME_XOSC_OFF 	0x00000000

#define ME_FIRC_ON 	0x00000010
#define ME_FIRC_OFF 	0x00000000

#define ME_SYS_CLK_ON_FIRC	0x00000000
#define ME_SYS_CLK_ON_DIV_FIRC	0x00000001
#define ME_SYS_CLK_ON_XOSC 	0x00000002
#define ME_SYS_CLK_ON_DIV_XOSC	0x00000003
#define ME_SYS_CLK_ON_FMPLL 	0x00000004
#define ME_SYS_CLK_DISABLED 	0x0000000F

#define ME_STANDBY0_EN 	0x00002000
#define ME_STOP0_EN	   	0x00000400
#define ME_HALT0_EN	   	0x00000100
#define ME_RUN3_EN	   	0x00000080
#define ME_RUN2_EN     	0x00000040
#define ME_RUN1_EN     	0x00000020
#define ME_TEST_EN     	0x00000002

void m_MODE_INIT(void);


#endif
