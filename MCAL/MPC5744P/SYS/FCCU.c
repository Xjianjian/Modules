/*******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2014 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
********************************************************************************
*
* $File Name:       FCCU.c$
* @file             FCCU.c
*
* $Date:            Jul-25-2014$
* @date             Jul-25-2014
*
* $Version:         0.9$
* @version          0.9
*
* Description:      FCCU driver source file
* @brief            FCCU driver source file
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

#include "Defines.h"
#include "ME.h" 
#include "FCCU.h"

/***************************************************************************//*!
*   @brief The function FCCU_SetProtocol sets protocol for the FCCU's safety output pins.
*	@par Include 
*					FCCU.h
* 	@par Description 
*					This function sets protocol that will be used to signal actual 
*					state of the FCCU via FCCU_F0 and FCCU_F1 pins.
*	@param[in] protocol
*					Protocol that will be used by the FCCU_F pins.
*					Predefined values:
*					- FCCU_RAIL	0b000
*					- FCCU_TIME_SWITCHING 0b001
*					- FCCU_BI_STABLE	0b010
*					- FCCU_TEST0	0b101
*					- FCCU_TEST1	0b110
*					- FCCU_TEST2	0b111
*	@remarks 	Execution of this function should not be interrupt by any event 
*				(e.g. break point). Function must be executed in the real time 
*				in one sequence, otherwise time delays for using access keys will 
*				be applied and protocol will not be changed.
*	@par Code sample
*			FCCU_SetProtocol(FCCU_BI_STABLE);
*			- Command sets bistable protocol for the FCCU_Fx pins.
********************************************************************************/
void FCCU_Init(void)
{
	FCCU_SetProtocol(FCCU_BI_STABLE);

	/* Unlock configuration */
	FCCU.TRANS_LOCK.B.TRANSKEY = 0xBC;
	/* provide Config state key */
	FCCU.CTRLK.R = 0x913756AF;
	/* enter config state - OP1 */
	FCCU.CTRL.R = 0x1;
	/* Verify if state change was sucessful */
	while (FCCU.CTRL.B.OPS != 0x3);	//operation status successful

	FCCU.NCFS_CFG[0].B.NCFSC14 = 0x1;         //NCF[14]---SWT_0

	//set up the NOMAL mode of FCCU
	FCCU.CTRLK.R = 0x825A132B;		//key for OP2
	FCCU.CTRL.R = 0x2;			//set the OP2 - set up FCCU into the NORMAL mode
	while (FCCU.CTRL.B.OPS != 0x3);	//operational status succesfull

}//FCCU_Init

void FCCU_SetProtocol(uint32_t protocol)
{
	/* Clear all status bits so configuration does
	 * not cause an interrupt to instantly fire.
	 */
    FCCU.NCFK.R =  0xAB3498FE;
    FCCU.NCF_S[0].R = 0xFFFFFFFF;
    while (FCCU.CTRL.B.OPS != 0x3);

    FCCU.NCFK.R =  0xAB3498FE;
    FCCU.NCF_S[1].R = 0xFFFFFFFF;
    while (FCCU.CTRL.B.OPS != 0x3);

    FCCU.NCFK.R =  0xAB3498FE;
    FCCU.NCF_S[2].R = 0xFFFFFFFF;
    while (FCCU.CTRL.B.OPS != 0x3);

	// Enter FCCU Configuration Mode
    FCCU.TRANS_LOCK.R = 0xBC;    
    FCCU.CTRLK.R = FCCU_CTRLK_OP1;
    FCCU.CTRL.R = FCCU_OPR_SET_CONFIG;
    while(FCCU.CTRL.B.OPS != FCCU_OPS_SUCCESS);
    
    // Disable all faults
    FCCU.NCF_E[0].R = 0x00004000;
    FCCU.NCF_E[1].R = 0x00000000;
    FCCU.NCF_E[2].R = 0x00000000;
	//SW reaction
    FCCU.NCF_CFG[0].R = 0xFFFFFFFF; 	
    FCCU.NCF_CFG[1].R = 0xFFFFFFFF;
    FCCU.NCF_CFG[2].R = 0xFFFFFFFF;

    FCCU.CFG.R = 0x01000000;
    FCCU.CFG.B.FOM = protocol;
    
    // Exit FCCU Configuration Mode
    FCCU.CTRLK.R = FCCU_KEY_NORMAL ;
    FCCU.CTRL.R = FCCU_OPR_SET_NORMAL;
    while(FCCU.CTRL.B.OPS != FCCU_OPS_SUCCESS);
}

void FCCU_read_faults(uint32_t * fbSt) 
{ 
  /* To read faults OP10 must be set in FCCU_CFG[OPR] field */ 
  FCCU.CTRL.R = 0xA;  //Read the NCF status register [OP10] 
 
  /* wait for the  operation run (OPR) mode change */ 
  while (FCCU.CTRL.B.OPS != 0x3);  //operation status  
 
  /* Read all NCFS registers to variables */ 
  fbSt[0] = FCCU.NCF_S[0].R;  //read FCCU.NCF_S0 register 
  fbSt[1] = FCCU.NCF_S[1].R;  //read FCCU.NCF_S1 register 
  fbSt[2] = FCCU.NCF_S[2].R;  //read FCCU.NCF_S2 register 
}//FCCU_read_faults 




void FCCU_clear_faults(void) 
{ 
  /* 1. Write a proper key into the FCCU_NCFK register */ 
  //Non-critical fault key = AB34_98FEh 
  FCCU.NCFK.R = 0xAB3498FE; 
 
  /* 2. Clear the status (flag) bit NCFSx => the opcode OP12 is automatically */
  /* Read all NCFS registers to clear all faults.*/ 
  /* For details which faults can be cleared see Table 7-36. FCCU Non-Critical Faults Mapping in RM */ 
  FCCU.NCF_S[0].R = 0xFFFFFFFF;    // read FCCU.NCF_S0 register   
     
  /* Verify if state change was successful */ 
  while (FCCU.CTRL.B.OPS != 0x3);  //Operation status successful 
 
  /* NCFS_1 register clear */ 
  FCCU.NCFK.R = 0xAB3498FE;    //Non-critical fault key = AB34_98FEh 
  FCCU.NCF_S[1].R = 0xFFFFFFFF;    // clear FCCU.NCF_S1 register 
  /* Verify if state change was successful */ 
  while (FCCU.CTRL.B.OPS != 0x3);  //Operation status successful 
 
  /* NCFS_2 register clear */ 
  FCCU.NCFK.R = 0xAB3498FE;    //Non-critical fault key = AB34_98FEh 
  FCCU.NCF_S[2].R = 0xFFFFFFFF;    // clear FCCU.NCF_S2 register 
  /* Verify if state change was successful */ 
  while (FCCU.CTRL.B.OPS != 0x3);  //Operation status successful 
}//FCCU_clear_faults 


