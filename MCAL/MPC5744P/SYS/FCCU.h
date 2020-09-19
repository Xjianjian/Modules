/*******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2014 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
********************************************************************************
*
* $File Name:       FCCU.h$
* @file             FCCU.h
*
* $Date:            Jul-25-2014$
* @date             Jul-25-2014
*
* $Version:         0.9$
* @version          0.9
*
* Description:      FCCU driver header file
* @brief            FCCU driver header file
*
* --------------------------------------------------------------------
* $Name:  $
*******************************************************************************/
/****************************************************************************//*!
*
*  @mainpage FCCU driver for MPC5744P
*
*  @section Intro Introduction
*
*	This package contains FCCU driver for MPC5744P allowing to clear fault 
*	flags and to generate fake faults for test purposes.
*  The key features of this package are the following:
*  - Clear critical and non-critical error flags
*  - Generate critical and non-critical fake faults

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
B35993		 				  25/07/2014 	   ALL		  Driver created

---------------------------   ----------    ------------  ------------------------------------------
==================================================================================================*/

#ifndef _FCCU_H_
#define _FCCU_H_

#define	FCCU_CFK           	0x618B7A50
#define	FCCU_NCF_KEY         	0xAB3498FE

#define FCCU_OP1  0x01
#define FCCU_OP3  0x03
#define FCCU_OP10 0x0A
#define FCCU_OP15 0x0F


#define FCCU_NCF_KEY  	0xAB3498FE
#define FCCU_CTRLK_OP1	0x913756AF
#define FCCU_CTRLK_OP2	0x825A132B
		
//different FCCU protocols		
#define	FCCU_DUAL_RAIL          0
#define	FCCU_TIME_SWITCHING     1	
#define	FCCU_BI_STABLE          2
#define	FCCU_TEST0              5
#define	FCCU_TEST1              6 
#define	FCCU_TEST2              7
		
//keys to enable secured operations in the FCCU_CTRL register		
#define	FCCU_KEY_CONFIG         0x913756AF
#define	FCCU_KEY_NORMAL         0x825A132B
#define	FCCU_KEY_LOCK           0x7ACB32F0
#define	FCCU_KEY_NVM_LOAD       0x29AF8752
		
//different FCCU_CTRL operations		
#define	FCCU_OPR_NO                 0
#define	FCCU_OPR_SET_CONFIG         1
#define	FCCU_OPR_SET_NORMAL         2
#define	FCCU_OPR_READ_STAT          3
#define	FCCU_OPR4                   4
#define	FCCU_OPR5                   5
#define	FCCU_OPR6                   6
#define	FCCU_OPR7                   7
#define	FCCU_OPR8                   8
#define	FCCU_OPR_READ_CF	        9
#define	FCCU_OPR_READ_NCF	        10
#define	FCCU_OPR_CLEAR_CF           11
#define	FCCU_OPR_CLEAR_NCF          12
#define	FCCU_OPR_CLEAR_FRZ_STAT     13
#define	FCCU_OPR_CONF_TIMEOUT	    14
#define	FCCU_OPR_CLEAR_OP_STAT	    15
#define	FCCU_OPR_LOCK_FCCU	        16
#define	FCCU_OPR_READ_ALARM	        17
#define	FCCU_OPR_READ_SMRT	        18
#define	FCCU_OPR_READ_CFG	        19
#define	FCCU_OPR_NVM_LOAD	        31
		
//different FCCU_CTRL status		
#define	FCCU_OPS_IDLE           0
#define	FCCU_OPS_IN_PROGR       1
#define	FCCU_OPS_ABORTED        2
#define	FCCU_OPS_SUCCESS        3

extern void FCCU_Init(void);
extern void FCCU_SetProtocol(uint32_t);

#endif
