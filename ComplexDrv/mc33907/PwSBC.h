/*******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2006-2012 Freescale Semiconductor, Inc.
* ALL RIGHTS RESERVED.
*
********************************************************************************
*
* $File Name:       PwSBC.h$
* @file             PwSBC.h
*
* $Date:            Jul-31-2014$
* @date             Jul-31-2014
*
* $Version:         1.4$
* @version          1.4
*
* Description:      PowerSBC driver header file
* @brief            PowerSBC driver header file
*
* --------------------------------------------------------------------
* $Name:  $
*******************************************************************************/
/****************************************************************************//*!
*
*  @mainpage PowerSBC driver for MC339XX
*
*  @section Intro Introduction
*
*	This package contains PowerSBC driver allowing to control MC33907 and MC33908 (PwSBC).
*	PwSBC driver was created as a demo code to enable our customers
*	to start their development quickly and to demonstrate functionality of the PowerSBC
*	chip. Presented driver is not AUTOSAR nor ISO26262 compliant. This driver package 
*	needs so-called MPC5744P Basic software drivers, which present the basic layer
*	for this application.
* 
*  The key features of this package are the following:
*  - Offer driver allowing to write and read into and from PwSBC registers
*  - Switch PwSBC modes (low power, normal etc.)
*  - Treat all interrupt sources (events)
*  - Supervise voltages presented in the PwSBC interface
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
Author (core ID)              Date D/M/Y       Name		  		Description of Changes
B35993		 				  01/03/2012 	   ALL		  		Driver created
B35993						  20/06/2013	 PwSBC_Init	  		Modified for Silicon Pass 3 (added 
														  		COMPILE_SILICON_PASS3 pre-compiler option)
B35993						  24/07/2013	  Mode_Rx/Tx  		Tags upgraded for Pass 3/4	
B35993						  24/07/2013  PwSBC_SetLPONmode   	Function deleted
B35993						  24/07/2013	  PwSBC_Init 		Upgraded for the Init bit		
B35993						  05/12/2013		All				Structures updated		
B35993						  31/07/2014		All				Format updated										  
---------------------------   ----------    ------------  ------------------------------------------
==================================================================================================*/

#ifndef _PW_SBC_H_
#define _PW_SBC_H_

#include "DEFINES.h"

/*==================================================================================================
*   Configurable parameters
*	User shall change configuration in this section regarding needs of the application.
==================================================================================================*/
#define MCU_SYS_CLK		200000000		///defines system clock frequency in Hz 
#define MCU_PLL0_CLK	160000000		///defines frequency of the PLL0 clock	
#define DSPI_CLK		MCU_SYS_CLK/4	///defined by the m_CGM.SC_DC0.R setting
#define LINFLEX_CLK		MCU_SYS_CLK/2	///given by the HALFSYS_CLK distribution
#define ADC_CLK			MCU_PLL0_CLK/2	///defined by the m_CGM.AC0_DC2.R register setting
#define DSPI_CLK		MCU_SYS_CLK/4	///defined by the m_CGM.SC_DC0.R setting
#define PIT_CLK			MCU_SYS_CLK/4	///defined by the m_CGM.SC_DC0.R setting

#define	DSPI_NB	0					///defines number of DSPI module
#define	DSPI_CS	1			///defines Chip Select 
#define	DSPI_BR	5000000				///defines DSPI baud rate in b/s

#define	ADC_NB	0	///defines number of ADC module to be used 
#define	ADC_CH	0	///defines number of used ADC channel 
#define	ADC_MASK	3	///Channel mask created from the channel number ADC_MASK = 2^ADC_CH 
#define	ADC_SOURCE_VCCA				///defines that Vcca is used as a reference voltage for ADC 
//#define	ADC_SOURCE_CALIB	4.9		///defines voltage used as a reference for ADC if the default Vcca is not used 

#define	PIT_WD_CH	0									///defines PIT channel number used for the Watchdog refresh 
#define	PIT_FS_DELAY_CH	1							///defines PIT channel number used for delay between two fail safe commands 
#define	PIT_UART_CH	2									///defines PIT channel number used for sending data via UART 
#define	PIT_WD_PERIOD     	0.00225		///defines WD refresh period in seconds 

#define PIT_WD_PERIOD1		0.00075
#define PIT_WD_PERIOD2		0.0015
#define PIT_WD_PERIOD3		0.00225
#define PIT_WD_PERIOD4		0.003
#define PIT_WD_PERIOD6		0.0045
#define PIT_WD_PERIOD8		0.006
#define PIT_WD_PERIOD12		0.009
#define PIT_WD_PERIOD16		0.012
#define PIT_WD_PERIOD24		0.018
#define PIT_WD_PERIOD32		0.024	
#define PIT_WD_PERIOD64		0.048
#define PIT_WD_PERIOD128	0.096
#define PIT_WD_PERIOD256	0.192
#define PIT_WD_PERIOD512	0.384
#define PIT_WD_PERIOD1024	0.768

/*==================================================================================================
*   NON - configurable parameters
*	User should not modify configuration in this section.
==================================================================================================*/
/****************************************************************************\
* IO_OUT / AMUX
\****************************************************************************/
#define	AMUX_VREF				0
#define	AMUX_VSNS_WIDE			1
#define	AMUX_IO0_WIDE			2
#define	AMUX_IO1_WIDE			3
#define	AMUX_VSNS_TIGHT			4
#define	AMUX_IO0_TIGHT			5
#define	AMUX_IO1_TIGHT			6
#define	AMUX_TEMP				7
		
/****************************************************************************\
* CAN/LIN mode
\****************************************************************************/
#define	CAN_SLEEP				0
#define	CAN_RX					1
#define	CAN_SLEEP_WU			2
#define	CAN_NORMAL				3
#define	LIN_SLEEP				0
#define	LIN_RX					1
#define	LIN_SLEEP_WU			2
#define	LIN_NORMAL				3
		
/****************************************************************************\
* WD_window
\****************************************************************************/
#define	WD_DISABLE			0
#define	WD_WIN_1			1
#define	WD_WIN_2			2
#define	WD_WIN_3			3
#define	WD_WIN_4			4
#define	WD_WIN_6			5
#define	WD_WIN_8			6
#define	WD_WIN_12			7
#define	WD_WIN_16			8
#define	WD_WIN_24			9
#define	WD_WIN_32			10
#define	WD_WIN_64			11
#define	WD_WIN_128			12
#define	WD_WIN_256			13
#define	WD_WIN_512			14
#define	WD_WIN_1024			15

/****************************************************************************\
* Addresses of the PwSBC registers
\****************************************************************************/
#define	INIT_VREG1_ADR				1
#define	INIT_VREG2_ADR				2
#define	INIT_CAN_LIN_ADR			3
#define	INIT_IO_WU1_ADR				4
#define	INIT_IO_WU2_ADR				5
#define	INIT_INT_ADR				6
#define	HW_CONFIG_ADR				8
#define	WU_SOURCE_ADR				9
#define	IO_INPUT_STATE_ADR			11
#define	STATUS_VREG1_ADR			12
#define	STATUS_VREG2_ADR			13
#define	DIAG_VREG1_ADR				14
#define	DIAG_VREG2_ADR				15
#define	DIAG_VREG3_ADR				16
#define	DIAG_CAN_ADR				17
#define	DIAG_CAN_LIN_ADR			18
#define	DIAG_SPI_ADR				19
#define	MODE_ADR					21
#define	VREG_MODE_ADR				22
#define	IO_OUT_AMUX_ADR				23
#define	CAN_LIN_MODE_ADR			24
#define	VREG_LPON_ADR				25

#define	INIT_SUPERVISOR1_ADR	0x21
#define	INIT_SUPERVISOR2_ADR	0x22
#define	INIT_SUPERVISOR3_ADR	0x23
#define	INIT_FSSM1_ADR			0x24
#define	INIT_FSSM2_ADR			0x25	
#define	WD_WINDOW_ADR			0x26
#define	WD_LFSR_ADR				0x27
#define	WD_ANSWER_ADR			0x28
#define	DIAG_FS1_ADR			0x2C
#define	WD_COUNTER_ADR			0x2D
#define	DIAG_FS2_ADR			0x2E
#define	FS_OUT_ADR				0x29
#define	RSTB_REQUEST_ADR		0x2A

/****************************************************************************\
* DEFAULT setting of the Writable registers
\****************************************************************************/
#define	INIT_VREG1_DEF				0x00008342	//8201		//8342 Vcore second bridge disabled 
#define	INIT_VREG2_DEF				0x00008544
#define	INIT_CAN_LIN_DEF			0x00008600 
#define	INIT_IO_WU1_DEF				0x0000890C	//8900
#define	INIT_IO_WU2_DEF				0x00008BFE  // default 8B02
#define	INIT_INT_DEF					0x00008C00
#define	MODE_DEF							0x0000AB8E
#define	VREG_MODE_DEF					0x0000000F//0x0000ADF3
#define	IO_OUT_AMUX_DEF				0x0000AF07 //AE00
#define	CAN_LIN_MODE_DEF			0x0000B0FC //B0D8 //B0B4
#define	VREG_LPON_DEF					0x0000B3E7
#define	INIT_SUPERVISOR1_DEF	0x0000C20C	//C2F3
#define	INIT_SUPERVISOR2_DEF	0x0000C40C	//C4CF
#define	INIT_SUPERVISOR3_DEF	0x0000C70C
#define	INIT_FSSM1_DEF				0x0000C80C //C80C //C84D SC 
#define	INIT_FSSM2_DEF				0x0000CB0C		//CB4D	//CB0C Io_23 not safety critical //CB4D	 //IO2_3 safety_crit	//	//CB0C	//C80C//D60C  //D64D	
#define	WD_WINDOW_DEF					0x0000CD30  //CD0C
#define	WD_LFSR_DEF						0x0000CF10
#define	WD_ANSWER_DEF					0x00000000
#define	FS_OUT_DEF						0x00000000
#define	RSTB_REQUEST_DEF			0x0000D50C

/****************************************************************************\
* SIUL parameters
\****************************************************************************/
#define	SIUL_INT_EIRQ	SIUL_EIRQ0	///defines a pad used for external interruption by its mask (EIRQ0 - EIRQ31)

/****************************************************************************\
* UART parameters
\****************************************************************************/
#define	LINFLEX_UART_NB	1			///defines number of the LINFlex module used for the UART communication
#define	LINFLEX_UART_BR	19200 ///defines Baud rate in Bd used for the UART <=> PC communication
#define	MSG_FRAME_SIZE	6	///defines number of bytes for each frame of the UART communication (1 - 4)
#define LINFLEX_UART_RX_BUFFER_SIZE 1	///defines size of the receive UART buffer

/****************************************************************************\
* PIT parameters
\****************************************************************************/
#define	PIT_UART_PERIOD	0.0043		///defines period for sending data via UART to PC in seconds
#define PIT_FS_DELAY	0.000004	///delay between two fail safe commands in seconds 

/****************************************************************************\
* ADC parameters
\****************************************************************************/  
#define	ADC_RESOLUTION	4095	///ADC resolution 2^12 
#define	ADC_RATIO	(ADC_SOURCE_CALIB / ADC_RESOLUTION)	///Ratio necessary for ADC computations 
#define	VAMUX_WD_5 	5	///Divider Ratio wide input voltage range with 5V reference 
#define	VAMUX_WD_33 	7	///Divider Ratio wide input voltage range with 3.3V reference 
#define	VAMUX_TG_5	2	///Divider Ratio tight input voltage range with 5V reference 
#define	VAMUX_TG_33 	3	///Divider Ratio tight input voltage range with 3.3V reference 
#define	VAMUX_TP_CO	10	///Internal temp sensor coefficient in mV/癈
#define	VAMUX_TP_0	500	///Temp sensor  mux output voltage for 0 癈 in mV  ->   Vamux_tp - Tj*V_amux_tp_co = (2150mV) - (165癈)*(10mV/癈) = 500 mV

/****************************************************************************\
* INTC parameters
\****************************************************************************/
#define	INT_CEIL_PRIORITY	12	///ceil priority has to be equal to the highest priority of interrupts sharing DSPI to communicate with PwSBC
#define	INT_WD_PRIORITY	12	///priority for WD refresh interrupt caused by PIT
#define	INT_SIUL_PRIORITY	10	///priority for interrupt caused by INT pin
#define	INT_UART_RX_PRIORITY	8	///priority for commands receiving from PC
#define	INT_ADC_PRIORITY	6	///priority for end of conversion of ADC

/*==================================================================================================
*   Structures/Type defines
==================================================================================================*/
/****************************************************************************\
* Write registers
\****************************************************************************/

	typedef union { ///INIT_Vreg1 
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t     							: 1;
			vuint32_t Reserved			: 1;
			vuint32_t IpFF_DIS					: 1;
			vuint32_t Vpre_DIS					: 1;
			vuint32_t     							: 2;
			vuint32_t Reserved2 							: 1;
			vuint32_t 					: 1;
		} B;
	} INIT_Vreg1_32B_tag;

	typedef union { ///INIT_Vreg2
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t     							: 1;
			vuint32_t Tcca_lim_off			: 1;
			vuint32_t Icca_lim					: 1;
			vuint32_t     							: 1;
			vuint32_t Reserved					: 1;
			vuint32_t Taux_lim_off			: 1;
			vuint32_t Vaux_trk_EN 			: 1;
			vuint32_t Reserved2					: 1;
		} B;
	} INIT_Vreg2_32B_tag;

	typedef union { ///INIT_CAN_LIN
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t    							: 1;
			vuint32_t CAN_WU_conf			  : 1;
			vuint32_t Reserved					: 1;
			vuint32_t CAN_TOY						: 1;
			vuint32_t CAN_WU_TO				  : 1;
			vuint32_t Reserved2			: 1;
			vuint32_t Reserved3       			: 2;
		} B;
	} INIT_CAN_LIN_32B_tag;

	typedef union { ///INIT_IO_WU1
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t WU_0							: 2;
			vuint32_t WU_1							: 2;
			vuint32_t WU_2							: 2;
			vuint32_t INT_inh_IO				: 2;
		} B;
	} INIT_IO_WU1_32B_tag;

	typedef union { ///INIT_IO_WU2
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t WU_3							: 2;
			vuint32_t WU_4							: 2;
			vuint32_t WU_5							: 2;
			vuint32_t INT_inh_IO_23	  	: 1;
			vuint32_t INT_inh_IO_45		: 1;
		} B;
	} INIT_IO_WU2_32B_tag;

	typedef union { ///INIT_INT
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t INT_duration			: 1;
			vuint32_t Reserved    							: 1;
			vuint32_t INT_inh_all		  	: 1;
			vuint32_t INT_inh_Vsns			: 1;
			vuint32_t INT_inh_Vpre			: 1;
			vuint32_t INT_inh_Vcore	  	: 1;
			vuint32_t INT_inh_Voth			: 1;
			vuint32_t Reserved2    							: 1;
		} B;
	} INIT_INT_32B_tag;

	typedef union { ///MODE_Tx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t Reserved						: 1;
			vuint32_t Reserved2							: 1;
			vuint32_t LPOFF							: 1;
			vuint32_t INT_request		  	: 1;
			vuint32_t Secure_3					: 1;
			vuint32_t Secure_2					: 1;
			vuint32_t Secure_1					: 1;
			vuint32_t Secure_0					: 1;
		} B;
	} MODE_Tx_32B_tag;

	typedef union { ///Vreg_mode_Tx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			uint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t Vcore_EN					: 1;
			vuint32_t Vcca_EN						: 1;
			vuint32_t Vaux_EN						: 1;
			vuint32_t Vcan_EN						: 1;
			vuint32_t Secure_3					: 1;
			vuint32_t Secure_2					: 1;
			vuint32_t Secure_1					: 1;
			vuint32_t Secure_0					: 1;
		} B;
	} Vreg_mode_Tx_32B_tag;

	typedef union { ///IO_OUT_AMUX 
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t IO_out_4_EN				: 1;
			vuint32_t IO_out_4					: 1;
			vuint32_t IO_out_5_EN				: 1;
			vuint32_t IO_out_5					: 1;
			vuint32_t Reserved					: 1;
			vuint32_t Amux    					: 3;
		} B;
	} IO_OUT_AMUX_32B_tag;

	typedef union { ///CAN_LIN_mode_Tx
		vuint32_t R;
		struct {
			vuint32_t			:16;
			vuint32_t RW			: 1;
			vuint32_t ADR			: 6;
			vuint32_t P			: 1;
			vuint32_t CAN_mode		: 2;
			vuint32_t CAN_auto_dis		: 1;
			vuint32_t LIN_mode		: 2;	
			vuint32_t LIN_auto_dis		: 1;
			vuint32_t             		: 2;
		} B;
	} CAN_LIN_mode_Tx_32B_tag;

	typedef union { ///CAN_MODE_2_Tx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t Reserved					: 1;
			vuint32_t Reserved2						: 1;
			vuint32_t Reserved3						: 1;
			vuint32_t Vcan_OV_Mon					: 1;
			vuint32_t Secure_3					: 1;
			vuint32_t Secure_2					: 1;
			vuint32_t Secure_1					: 1;
			vuint32_t Secure_0					: 1;
			
		} B;
	} Vreg_LPON_Tx_32B_tag;

	typedef union { ///INIT_Supervisor1_Tx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t Vcore_FS					: 2;
			vuint32_t Vcca_FS				  	: 2;
			vuint32_t Secure_3					: 1;
			vuint32_t Secure_2					: 1;
			vuint32_t Secure_1					: 1;
			vuint32_t Secure_0					: 1;
		} B;
	} INIT_Supervisor1_Tx_32B_tag;

	typedef union { ///INIT_Supervisor2_Tx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t Vaux_FS				  	: 2;
			vuint32_t     							: 1;
			vuint32_t DIS_8s						: 1;
			vuint32_t Secure_3					: 1;
			vuint32_t Secure_2					: 1;
			vuint32_t Secure_1					: 1;
			vuint32_t Secure_0					: 1;
		} B;
	} INIT_Supervisor2_Tx_32B_tag;

	typedef union { ///INIT_Supervisor3_Tx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t Vcore_5D					: 1;
			vuint32_t Vcca_5D						: 1;
			vuint32_t Vaux_5D						: 1;
			vuint32_t     							: 1;
			vuint32_t Secure_3					: 1;
			vuint32_t Secure_2					: 1;
			vuint32_t Secure_1					: 1;
			vuint32_t Secure_0					: 1;
		} B;
	} INIT_Supervisor3_Tx_32B_tag;

	typedef union { ///INIT_FSSM1_Tx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t IO_01_FS					: 1;
			vuint32_t 						: 1;
			vuint32_t IO_45_FS					: 1;
			vuint32_t RSTb_low					: 1;
			vuint32_t Secure_3					: 1;
			vuint32_t Secure_2					: 1;
			vuint32_t Secure_1					: 1;
			vuint32_t Secure_0					: 1;
		} B;
	} INIT_FSSM1_Tx_32B_tag;

	typedef union { ///INIT_FSSM2_Tx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t RSTb_err_FS	  		: 1;
			vuint32_t IO_23_FS					: 1;
			vuint32_t PS								: 1;
			vuint32_t 							: 1;
			vuint32_t Secure_3					: 1;
			vuint32_t Secure_2					: 1;
			vuint32_t Secure_1					: 1;
			vuint32_t Secure_0					: 1;
		} B;
	} INIT_FSSM2_Tx_32B_tag;

	typedef union { ///WD_window_Tx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t WD_window		  		: 4;
			vuint32_t Secure_3					: 1;
			vuint32_t Secure_2					: 1;
			vuint32_t Secure_1					: 1;
			vuint32_t Secure_0					: 1;
		} B;
	} WD_window_Tx_32B_tag;

	typedef union { ///WD_LFSR_Tx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t WD_LFSR	 					: 8;
		} B;
	} WD_LFSR_Tx_32B_tag;

	typedef union { ///WD_answer_Tx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t WD_answer					: 8;
		} B;
	} WD_answer_Tx_32B_tag;

	typedef union { ///FS_OUT_Tx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t FS_out_7					: 1;
			vuint32_t FS_out_6					: 1;
			vuint32_t FS_out_5					: 1;
			vuint32_t FS_out_4					: 1;
			vuint32_t FS_out_3					: 1;
			vuint32_t FS_out_2					: 1;
			vuint32_t FS_out_1					: 1;
			vuint32_t FS_out_0					: 1;
		} B;
	} FS_OUT_Tx_32B_tag;

	typedef union { ///RSTb_request_Tx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t RW								: 1;
			vuint32_t ADR								: 6;
			vuint32_t P									: 1;
			vuint32_t 									: 2;
			vuint32_t RSTb_request			: 1;
			vuint32_t 									: 1;
			vuint32_t Secure_3					: 1;
			vuint32_t Secure_2					: 1;
			vuint32_t Secure_1					: 1;
			vuint32_t Secure_0					: 1;
		} B;
	} RSTb_request_Tx_32B_tag;

/****************************************************************************\
* Read registers
\****************************************************************************/

	typedef union { ///HW_Config_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t LS_detect					: 1;
			vuint32_t Vaux_not_used				: 1;
			vuint32_t Vcca_PNP_detect			: 1;
			vuint32_t Vcca_HW						: 1;
			vuint32_t 								: 2;
			vuint32_t     							: 1;
			vuint32_t DBG								: 1;
		} B;
	} HW_Config_Rx_32B_tag;

	typedef union { ///WU_source_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t IO_5_WU						: 1;
			vuint32_t IO_4_WU						: 1;
			vuint32_t IO_3_WU						: 1;
			vuint32_t IO_2_WU						: 1;
			vuint32_t IO_1_WU						: 1;
			vuint32_t IO_0_WU						: 1;
			vuint32_t Vreg_WU						: 1;
			vuint32_t Phy_WU						: 1;
		} B;
	} WU_source_Rx_32B_tag;


	typedef union { ///IO_input state_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t IO_5							: 1;
			vuint32_t IO_4							: 1;
			vuint32_t     							: 1;
			vuint32_t IO_3							: 1;
			vuint32_t IO_2							: 1;
			vuint32_t     							: 1;
			vuint32_t IO_1							: 1;
			vuint32_t IO_0							: 1;
		} B;
	} IO_input_state_Rx_32B_tag;

	typedef union { ///Status_Vreg1_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t IpFF							: 1;
			vuint32_t Ilim_pre					: 1;
			vuint32_t Twarn_pre				  : 1;
			vuint32_t BoB								: 1;
			vuint32_t Vpre_state				: 1;
			vuint32_t     							: 1;
			vuint32_t 						: 1;
			vuint32_t 					: 1;
		} B;
	} Status_Vreg1_Rx_32B_tag;

	typedef union { ///Status_Vreg2_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t Ilim_core					: 1;
			vuint32_t Twarn_core				: 1;
			vuint32_t Vcore_state		  	: 1;
			vuint32_t Twarn_cca			  	: 1;
			vuint32_t Ilim_cca					: 1;
			vuint32_t Ilim_aux					: 1;
			vuint32_t Ilim_can					: 1;
			vuint32_t     							: 1;
		} B;
	} Status_Vreg2_Rx_32B_tag;

	typedef union { ///Diag_Vreg1_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t Vsns_uv						: 1;
			vuint32_t Vsup_uv_7			  	: 1;
			vuint32_t Tsd_pre						: 1;
			vuint32_t Vpre_OV						: 1;
			vuint32_t Vpre_UV						: 1;
			vuint32_t Tsd_core					: 1;
			vuint32_t Vcore_OV					: 1;
			vuint32_t Vcore_UV					: 1;
		} B;
	} Diag_Vreg1_Rx_32B_tag;

	typedef union { ///Diag_Vreg2_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t Tsd_can						: 1;
			vuint32_t Vcan_ov						: 1;
			vuint32_t Vcan_uv						: 1;
			vuint32_t     							: 1;
			vuint32_t Tsd_aux						: 1;
			vuint32_t Ilim_aux_off			: 1;
			vuint32_t Vaux_OV						: 1;
			vuint32_t Vaux_UV						: 1;
		} B;
	} Diag_Vreg2_Rx_32B_tag;

	typedef union { ///Diag_Vreg3_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t Tsd_cca						: 1;
			vuint32_t     							: 1;
			vuint32_t Ilim_cca_off			: 1;
			vuint32_t     							: 1;
			vuint32_t Vcca_OV						: 1;
			vuint32_t     							: 1;
			vuint32_t Vcca_UV						: 1;
			vuint32_t     							: 1;
		} B;
	} Diag_Vreg3_Rx_32B_tag;

	typedef union { ///Diag_CAN_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t CANH_batt					: 1;
			vuint32_t CANH_gnd					: 1;
			vuint32_t CANL_batt					: 1;
			vuint32_t CANL_gnd					: 1;
			vuint32_t CAN_dominant			: 1;
			vuint32_t     							: 1;
			vuint32_t RXD_recessive		  : 1;
			vuint32_t TXD_dominant			: 1;
		} B;
	} Diag_CAN_Rx_32B_tag;

	typedef union { ///Diag_CAN_LIN_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t Reserved			: 1;
			vuint32_t Reserved2 	  	: 1;
			vuint32_t     							: 1;
			vuint32_t Reserved3		: 1;
			vuint32_t Reserved4						: 1;
			vuint32_t     							: 1;
			vuint32_t CAN_OT						: 1;
			vuint32_t CAN_OC						: 1;
		} B;
	} Diag_CAN_LIN_Rx_32B_tag;

	typedef union { ///Diag_SPI_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag		: 8;
			vuint32_t SPI_err						: 1;
			vuint32_t bit6    							: 1;
			vuint32_t SPI_CLK						: 1;
			vuint32_t bit4    							: 1;
			vuint32_t SPI_Req						: 1;
			vuint32_t bit2    							: 1;
			vuint32_t SPI_Parity				: 1;
			vuint32_t bit0    							: 1;
		} B;
	} Diag_SPI_Rx_32B_tag;


	typedef union { ///MODE_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag		: 8;
			vuint32_t Init					: 5;
			vuint32_t Normal						: 1;
			vuint32_t Reserved							: 1;
			vuint32_t Reserved2							: 1;
		} B;
	} MODE_Rx_32B_tag;

	typedef union { ///Vreg_mode_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t Reserved					: 4;
			vuint32_t Vcore_EN					: 1;
			vuint32_t Vcca_EN						: 1;
			vuint32_t Vaux_EN						: 1;
			vuint32_t Vcan_EN						: 1;
		} B;
	} Vreg_mode_Rx_32B_tag;

	typedef union { ///CAN_LIN mode_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t CAN_mode					: 2;
			vuint32_t CAN_auto_dis			: 1;
			vuint32_t LIN_mode					: 2;
			vuint32_t LIN_auto_dis			: 1;
			vuint32_t CAN_wu						: 1;
			vuint32_t LIN_wu						: 1;
		} B;
	} CAN_LIN_mode_Rx_32B_tag;

	typedef union { ///Vreg_LPON_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t Reserved					: 4;
			vuint32_t Vcan_OV_Mon					: 1;
			vuint32_t Reserved2					: 1;
			vuint32_t Reserved3						: 1;
			vuint32_t Reserved4						: 1;
		} B;
	} Vreg_LPON_Rx_32B_tag;


	typedef union { ///INIT_Supervisor1_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t SPI_FS_err				: 1;
			vuint32_t SPI_FS_CLK				: 1;
			vuint32_t SPI_FS_Req				: 1;
			vuint32_t SPI_FS_Parity		  : 1;
			vuint32_t Vcore_FS					: 2;
			vuint32_t Vcca_FS 					: 2;
		} B;
	} INIT_Supervisor1_Rx_32B_tag;

	typedef union { ///INIT_Supervisor2_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t SPI_FS_err				: 1;
			vuint32_t SPI_FS_CLK				: 1;
			vuint32_t SPI_FS_Req				: 1;
			vuint32_t SPI_FS_Parity	    : 1;
			vuint32_t     							: 1;
			vuint32_t DIS_8s						: 1;
			vuint32_t Vaux_FS			  		: 2;
		} B;
	} INIT_Supervisor2_Rx_32B_tag;

	typedef union { ///INIT_Supervisor3_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t SPI_FS_err				: 1;
			vuint32_t SPI_FS_CLK				: 1;
			vuint32_t SPI_FS_Req				: 1;
			vuint32_t SPI_FS_Parity	  	: 1;
			vuint32_t     							: 1;
			vuint32_t Vcore_5D					: 1;
			vuint32_t Vcca_5D						: 1;
			vuint32_t Vaux_5D						: 1;
		} B;
	} INIT_Supervisor3_Rx_32B_tag;

	typedef union { ///INIT_FSSM1_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t SPI_FS_err				: 1;
			vuint32_t SPI_FS_CLK				: 1;
			vuint32_t SPI_FS_Req				: 1;
			vuint32_t SPI_FS_Parity		  : 1;
			vuint32_t IO_01_FS					: 1;
			vuint32_t 							: 1;
			vuint32_t IO_45_FS					: 1;
			vuint32_t RSTb_low					: 1;
		} B;
	} INIT_FSSM1_Rx_32B_tag;

	typedef union { ///INIT_FSSM2_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t SPI_FS_err				: 1;
			vuint32_t SPI_FS_CLK				: 1;
			vuint32_t SPI_FS_Req				: 1;
			vuint32_t SPI_FS_Parity	  	: 1;
			vuint32_t RSTb_err_FS		  	: 1;
			vuint32_t IO_23_FS			 		: 1;
			vuint32_t PS								: 1;
			vuint32_t 							: 1;
		} B;
	} INIT_FSSM2_Rx_32B_tag;

	typedef union { ///WD_window_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t SPI_FS_err				: 1;
			vuint32_t SPI_FS_CLK				: 1;
			vuint32_t SPI_FS_Req				: 1;
			vuint32_t SPI_FS_Parity		  : 1;
			vuint32_t WD_window   		  : 4;
		} B;
	} WD_window_Rx_32B_tag;

	typedef union { ///WD_LFSR_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag		: 8;
			vuint32_t WD_LFSR			  		: 8;
		} B;
	} WD_LFSR_Rx_32B_tag;

	typedef union { ///WD_answer_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t RSTb							: 1;
			vuint32_t FSx								: 1;
			vuint32_t WD								: 1;
			vuint32_t FSO_G							: 1;
			vuint32_t IO_FS_G						: 1;
			vuint32_t 								: 1;
			vuint32_t FS_ECC						: 1;
			vuint32_t FS_reg_ECC				: 1;
		} B;
	} WD_answer_Rx_32B_tag;

	typedef union { ///Diag_FS1_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t RSTb_ext					: 1;
			vuint32_t RSTb_diag					: 1;
			vuint32_t     							: 1;
			vuint32_t FS0b_diag					: 2;
			vuint32_t Reserved    							: 1;
			vuint32_t Reserved2    		  : 2;
		} B;
	} Diag_FS1_Rx_32B_tag;

	typedef union { ///WD_counter_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t WD_err						: 3;
			vuint32_t     							: 1;
			vuint32_t WD_refresh				: 3;
			vuint32_t     							: 1;
		} B;
	} WD_counter_Rx_32B_tag;

	typedef union { ///Diag_FS2_Rx
		vuint32_t R;
		struct {
			vuint32_t										:16;
			vuint32_t Status_32B_tag			: 8;
			vuint32_t RSTb_err				  : 3;
			vuint32_t     							: 1;
			vuint32_t IO_45_fail				: 1;
			vuint32_t IO_23_fail				: 1;
			vuint32_t 							: 1;
			vuint32_t IO_01_fail				: 1;
		} B;
	} Diag_FS2_Rx_32B_tag;


/****************************************************************************\
* Additional typedefs
\****************************************************************************/
	typedef union { /* register32_struct */
	uint32_t R;
	struct {
		uint32_t				:16;
		uint32_t	bit15	:	1;
		uint32_t	bit14	:	1;
		uint32_t	bit13	:	1;
		uint32_t	bit12	:	1;
		uint32_t	bit11	:	1;
		uint32_t	bit10	:	1;
		uint32_t	bit9	:	1;
		uint32_t	bit8	:	1;
		uint32_t	bit7	:	1;
		uint32_t	bit6	:	1;
		uint32_t	bit5	:	1;
		uint32_t	bit4	:	1;
		uint32_t	bit3	:	1;
		uint32_t	bit2	:	1;
		uint32_t	bit1	:	1;
		uint32_t	bit0	:	1;
	} B;
} register32_struct;

	typedef union { ///PowerSBC general status register
	uint32_t R;
	struct {
		uint32_t						:24;
		uint32_t	SPI_G			:	1;		//SPI error
		uint32_t	WU				:	1;		//Wake-up event
		uint32_t	CAN_G			:	1;		//CAN event
		uint32_t	LIN_G			:	1;		//LIN event
		uint32_t	IO_G			:	1;		//event on I/Os
		uint32_t	Vpre_G		:	1;		//Vpre event
		uint32_t	Vcore_G		:	1;		//Vcore event
		uint32_t	Vothers_G	:	1;		//Vcca, Vaux or Vcan event
	} B;
} Status_32B_tag;

#endif
