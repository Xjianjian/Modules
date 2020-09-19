/*******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
********************************************************************************
*
* $File Name:       DEFINES.h$
* @file             DEFINES.h
*
* $Date:            Jul-01-2014$
* @date             Jul-01-2014
*
* $Version:         0.9$
* @version          0.9
*
* Description:      DEFINES header file
* @brief            DEFINES header file
*
* --------------------------------------------------------------------
* $Name:  $
*******************************************************************************/
/****************************************************************************//*!
*
*  @mainpage DEFINES header file for MC339XX-MPC5744P demo package
*
*  @section Intro Introduction
*
*	This file contains global defines necessary for the MC339XX-MPC5744P demo 
*	package. This file shall be included in each of the package files.
*  The key features of this package are the following:
*	- provide defined constants for the whole package
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
B35993		 				  01/07/2014 	   ALL		  Driver created

---------------------------   ----------    ------------  ------------------------------------------
==================================================================================================*/

#ifndef _DEFINES_H_
#define _DEFINES_H_

/************************************************************************/
/*					Processor type				*/
/************************************************************************/
#define MPC5744P		
#include "MPC5744P.h"

/************************************************************************/
/*	  		  Software Option				*/
/************************************************************************/
#define StartupCalibDuration    0.01 	// in sec 
#define APIPeriod               0.1	// in sec 
//#define CalibDuration	   	0.060	// in sec => 65ms max at 16MHz (20 bits)
//#define CalibPeriod	   	5	// in sec => 32bits timer based on 16MIRC sysclk
//#define CalibStartDelay		5	// nb of Wakeup before starting qualibration
#define MeasureDeepness         2	// use for stat & calib
//#define EnableDisplay
//#define PITPeriodicalUpdateEnable
#define _Debug

//#define MPC5604B
#define CUT2
/************************************************************************/
// 	Software defines DO NOT MODIFY Following section 
/************************************************************************/
//Initialize clocks value - DO NOT MODIFY IT - Set it up with Hardware theoritical value
#define	fFXOSC      16000000
#define	fSXOSC      32000
#define	fFIRC       16000000
#define	fSIRC       128000

//Related to EVB
#define LED1        68 //PCR[68]
#define LED2        69 //PCR[69]
#define LED3        70 //PCR[70]
#define LED4        71 //PCR[71]

//Declaration for SPI state Machine
#define STOPPED     0
#define RUNNING     1

//use for CMU
#define FIRC_clk    0
#define SIRC_clk    1
#define SXOSC_clk   2 

//use for Mode Entry
#define	ME_RESET	0
#define	ME_TEST		1
#define	ME_SAFE		2
#define	ME_DRUN		3
#define	ME_RUN0		4
#define	ME_RUN1		5
#define	ME_RUN2		6
#define	ME_RUN3		7
#define	ME_HALT0	8
#define	ME_STOP0	10
#define	ME_STANDBY0	13



//Pad Pull 		
#define 	SIUL_PULL_UP	0x00030000
#define 	SIUL_PULL_DOWN	0x00010000
#define 	SIUL_PULL_DIS	0x00000000

//Pad dir		
#define  	SIUL_INPUT	0x00080000
#define 	SIUL_OUTPUT	0x02000000
#define     SIUL_BIDIR  0x02080000

//Pad function		
#define 	SIUL_GPIO	0x0
#define 	SIUL_ALT1	0x1
#define 	SIUL_ALT2	0x2
#define 	SIUL_ALT3	0x3
#define 	SIUL_ALT4	0x4
		
//Safe mode control		
#define 	SIUL_SMC	0x00800000
		
//Pad type		
#define 	SIUL_DIGITAL	0x00000000
#define 	SIUL_ANALOG 	0x00400000
		
//Pad slew rate		
#define 	SIUL_SLEW_ENB 	0x00000000
#define 	SIUL_SLEW_DIS	0x20000000
#define 	SIUL_DRIVE_HALF 0x00000000
#define 	SIUL_DRIVE_FULL 0x1000000
		
//Output type		
#define 	SIUL_NORMAL	0x00000000
#define 	SIUL_OPEND	0x01000000
		
// external interruption masks EIRQx		
	
#define	SIUL_EIRQ0	0x1
#define	SIUL_EIRQ1	0x2
#define	SIUL_EIRQ2	0x4
#define	SIUL_EIRQ3	0x8
#define	SIUL_EIRQ4	0x10
#define	SIUL_EIRQ5	0x20
#define	SIUL_EIRQ6	0x40
#define	SIUL_EIRQ7	0x80
#define	SIUL_EIRQ8	0x100
#define	SIUL_EIRQ9	0x200
#define	SIUL_EIRQ10	0x400
#define	SIUL_EIRQ11	0x800
#define	SIUL_EIRQ12	0x1000
#define	SIUL_EIRQ13	0x2000
#define	SIUL_EIRQ14	0x4000
#define	SIUL_EIRQ15	0x8000
#define	SIUL_EIRQ16	0x10000
#define	SIUL_EIRQ17	0x20000
#define	SIUL_EIRQ18	0x40000
#define	SIUL_EIRQ19	0x80000
#define	SIUL_EIRQ20	0x100000
#define	SIUL_EIRQ21	0x200000
#define	SIUL_EIRQ22	0x400000
#define	SIUL_EIRQ23	0x800000
#define	SIUL_EIRQ24	0x1000000
#define	SIUL_EIRQ25	0x2000000
#define	SIUL_EIRQ26	0x4000000
#define	SIUL_EIRQ27	0x8000000
#define	SIUL_EIRQ28	0x10000000
#define	SIUL_EIRQ29	0x20000000
#define	SIUL_EIRQ30	0x40000000
#define	SIUL_EIRQ31	0x80000000
		
// pad list Name versus MSCR[SSS]		
	
#define	SIUL_PA0	0
#define	SIUL_PA1	1
#define	SIUL_PA2	2
#define	SIUL_PA3	3
#define	SIUL_PA4	4
#define	SIUL_PA5	5
#define	SIUL_PA6	6
#define	SIUL_PA7	7
#define	SIUL_PA8	8
#define	SIUL_PA9	9
#define	SIUL_PA10	10
#define	SIUL_PA11	11
#define	SIUL_PA12	12
#define	SIUL_PA13	13
#define	SIUL_PA14	14
#define	SIUL_PA15	15
#define	SIUL_PB0	16
#define	SIUL_PB1	17
#define	SIUL_PB2	18
#define	SIUL_PB3	19
#define	SIUL_PB4	20
#define	SIUL_PB5	21
#define	SIUL_PB6	22
#define	SIUL_PB7	23
#define	SIUL_PB8	24
#define	SIUL_PB9	25
#define	SIUL_PB10	26
#define	SIUL_PB11	27
#define	SIUL_PB12	28
#define	SIUL_PB13	29
#define	SIUL_PB14	30
#define	SIUL_PB15	31
#define	SIUL_PC0	32
#define	SIUL_PC1	33
#define	SIUL_PC2	34
#define	SIUL_PC3	35
#define	SIUL_PC4	36
#define	SIUL_PC5	37
#define	SIUL_PC6	38
#define	SIUL_PC7	39
#define	SIUL_PC8	40
#define	SIUL_PC9	41
#define	SIUL_PC10	42
#define	SIUL_PC11	43
#define	SIUL_PC12	44
#define	SIUL_PC13	45
#define	SIUL_PC14	46
#define	SIUL_PC15	47
#define	SIUL_PD0	48
#define	SIUL_PD1	49
#define	SIUL_PD2	50
#define	SIUL_PD3	51
#define	SIUL_PD4	52
#define	SIUL_PD5	53
#define	SIUL_PD6	54
#define	SIUL_PD7	55
#define	SIUL_PD8	56
#define	SIUL_PD9	57
#define	SIUL_PD10	58
#define	SIUL_PD11	59
#define	SIUL_PD12	60
#define	SIUL_PD13	61
#define	SIUL_PD14	62
#define	SIUL_PD15	63
#define	SIUL_PE0	64
#define	SIUL_PE1	65
#define	SIUL_PE2	66
#define	SIUL_PE3	67
#define	SIUL_PE4	68
#define	SIUL_PE5	69
#define	SIUL_PE6	70
#define	SIUL_PE7	71
#define	SIUL_PE8	72
#define	SIUL_PE9	73
#define	SIUL_PE10	74
#define	SIUL_PE11	75
#define	SIUL_PE12	76
#define	SIUL_PE13	77
#define	SIUL_PE14	78
#define	SIUL_PE15	79
#define	SIUL_PF0	80
#define	SIUL_PF1	81
#define	SIUL_PF2	82
#define	SIUL_PF3	83
#define	SIUL_PF4	84
#define	SIUL_PF5	85
#define	SIUL_PF6	86
#define	SIUL_PF7	87
#define	SIUL_PF8	88
#define	SIUL_PF9	89
#define	SIUL_PF10	90
#define	SIUL_PF11	91
#define	SIUL_PF12	92
#define	SIUL_PF13	93
#define	SIUL_PF14	94
#define	SIUL_PF15	95
#define	SIUL_PG0	96
#define	SIUL_PG1	97
#define	SIUL_PG2	98
#define	SIUL_PG3	99
#define	SIUL_PG4	100
#define	SIUL_PG5	101
#define	SIUL_PG6	102
#define	SIUL_PG7	103
#define	SIUL_PG8	104
#define	SIUL_PG9	105
#define	SIUL_PG10	106
#define	SIUL_PG11	107
#define	SIUL_PG12	108
#define	SIUL_PG13	109
#define	SIUL_PG14	110
#define	SIUL_PG15	111
#define	SIUL_PH0	112
#define	SIUL_PH1	113
#define	SIUL_PH2	114
#define	SIUL_PH3	115
#define	SIUL_PH4	116
#define	SIUL_PH5	117
#define	SIUL_PH6	118
#define	SIUL_PH7	119
#define	SIUL_PH8	120
#define	SIUL_PH9	121
#define	SIUL_PH10	122
#define	SIUL_PH11	123
#define	SIUL_PH12	124
#define	SIUL_PH13	125
#define	SIUL_PH14	126
#define	SIUL_PH15	127
#define	SIUL_PI0	128
#define	SIUL_PI1	129
#define	SIUL_PI2	130
#define	SIUL_PI3	131
#define	SIUL_PI4	132
#define	SIUL_PI5	133
#define	SIUL_PI6	134
#define	SIUL_PI7	135
#define	SIUL_PI8	136
#define	SIUL_PI9	137
#define	SIUL_PI10	138
#define	SIUL_PI11	139
#define	SIUL_PI12	140
#define	SIUL_PI13	141
#define	SIUL_PI14	142
#define	SIUL_PI15	143
#define	SIUL_PJ0	144
#define	SIUL_PJ1	145
#define	SIUL_PJ2	146
#define	SIUL_PJ3	147
#define	SIUL_PJ4	148
#define	SIUL_PJ5	149
#define	SIUL_PJ6	150
#define	SIUL_PJ7	151
#define	SIUL_PJ8	152
#define	SIUL_PJ9	153

// pad list Name versus IMCR[SSS]
#define SIUL_IN_PA0_DSPI2_SCK			48
#define SIUL_IN_PA0_ETIMER0_ETC0     	59
#define SIUL_IN_PA0_SIUL2_REQ0     		173
#define SIUL_IN_PA1_ETIMER0_ETC1		60
#define SIUL_IN_PA1_SIUL2_REQ1     		174
#define SIUL_IN_PA2_m_RGM_ABS0			169
#define SIUL_IN_PA2_DSPI2_SIN     		47
#define SIUL_IN_PA2_ETIMER0_ETC2    	61
#define SIUL_IN_PA2_FLEXPWM0_A3			97
#define SIUL_IN_PA2_SIUL2_REQ2     		175
#define SIUL_IN_PA3_m_RGM_ABS2			171
#define SIUL_IN_PA3_ETIMER0_ETC3     	62
#define SIUL_IN_PA3_DSPI2_CS0    		49
#define SIUL_IN_PA3_FLEXPWM0_B3			98
#define SIUL_IN_PA3_SIUL2_REQ3     		176

#define SIUL_IN_PA3		49	
#define SIUL_IN_PB3     165
#define SIUL_IN_PB7     165
#define SIUL_IN_PC7     41
#define SIUL_IN_PD12    166
#define SIUL_IN_PB1		32
#define SIUL_IN_PA15	33
#define SIUL_IN_PF15	34
#define SIUL_IN_PA8		44
#define SIUL_IN_PA2		47
#define SIUL_IN_PD7		50
#define SIUL_IN_PD6		51
#define	SIUL_IN_PC11	52
#define SIUL_IN_PB13    166

#endif
