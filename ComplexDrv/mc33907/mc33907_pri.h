
#ifndef MC33907_PRI_H_
#define MC33907_PRI_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_33907_waitDtTick		100
/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef struct
{
	uint32_t 		writeCmd;
	uint32_t 		readCmd;
	uint32_t 		response;
	Status_32B_tag 	pwrStatus;
	uint32_t 	   	regAddr;
}spiStruct_t;

typedef struct
{
	bool 		Vreg1;
	bool 		Vreg2;
	bool		canLin;
	bool		ioWu1;
	bool 		ioWu2;
	bool		initInt;
	bool		hwCfg;
	bool		wuSoc;
	bool 		ioInputSt;
	bool 		vreg1St;
	bool		vreg2St;
	bool		diagVreg1;
	bool		diagVreg2;
	bool		diagVreg3;
	bool		diagCan;
	bool		diagCanLin;
	bool		diagSpi;
	bool		mode;
	bool		vregMode;
	bool		ioOutMux;
	bool		canLinMode;
	bool		vregLpon;
	bool		supervisor1;
	bool		supervisor2;
	bool		supervisor3;
	bool		fssm1;
	bool 		fssm2;
	bool		wdWindow;
	bool 		wdLfsr;
	bool		wdAnswer;
	bool		diagFs1;
	bool 		wdCnt;
	bool		diagFs2;
	bool		fsOut;
}intUpdtStru_t;

typedef struct{
	INIT_Vreg1_32B_tag								INIT_Vreg1;					/*last received state of the INIT_Vreg1 register*/
	INIT_Vreg2_32B_tag								INIT_Vreg2;					/*last received state of the INIT_Vreg2 register */
	INIT_CAN_LIN_32B_tag							INIT_CAN_LIN;				/*last received state of the INIT_CAN_LIN register */
	INIT_IO_WU1_32B_tag								INIT_IO_WU1;				/*last received state of the INIT_IO_WU1 register */
	INIT_IO_WU2_32B_tag								INIT_IO_WU2;				/*last received state of the INIT_IO_WU2 register */
	INIT_INT_32B_tag								INIT_INT;				/*last received state of the INIT_INT register */
	HW_Config_Rx_32B_tag							HW_Config;					/*last received state of the HW_Config register */
	WU_source_Rx_32B_tag							WU_source;					/*last received state of the WU_source register */
	IO_input_state_Rx_32B_tag						IO_input_state;			/*last received state of the IO_input_state register */
	Status_Vreg1_Rx_32B_tag							Status_Vreg1;				/*last received state of the Status_32B_tag_Vreg1 register */
	Status_Vreg2_Rx_32B_tag							Status_Vreg2;				/*last received state of the Status_32B_tag_Vreg2 register */
	Diag_Vreg1_Rx_32B_tag							Diag_Vreg1;					/*last received state of the Diag_Vreg1 register */
	Diag_Vreg2_Rx_32B_tag							Diag_Vreg2;					/*last received state of the Diag_Vreg2 register */
	Diag_Vreg3_Rx_32B_tag							Diag_Vreg3;					/*last received state of the Diag_Vreg3 register */
	Diag_CAN_Rx_32B_tag								Diag_CAN;						/*last received state of the Diag_CAN register */
	Diag_CAN_LIN_Rx_32B_tag							Diag_CAN_LIN;				/*last received state of the Diag_CAN_LIN register */
	Diag_SPI_Rx_32B_tag								Diag_SPI;						/*last received state of the Diag_SPI register */
	MODE_Rx_32B_tag									MODE;								/*last received state of the MODE register */
	Vreg_mode_Rx_32B_tag							Vreg_mode;					/*last received state of the Vreg_mode register */
	IO_OUT_AMUX_32B_tag								IO_OUT_AMUX;				/*last received state of the IO_OUT_AMUX register */
	CAN_LIN_mode_Rx_32B_tag							CAN_LIN_mode;				/*last received state of the CAN_LIN_mode register */
	Vreg_LPON_Rx_32B_tag							Vreg_LPON;					/*last received state of the Vreg_LPON register */
	INIT_Supervisor1_Rx_32B_tag						INIT_Supervisor1;		/*last received state of the INIT_Supervisor1 register */
	INIT_Supervisor2_Rx_32B_tag						INIT_Supervisor2;		/*last received state of the INIT_Supervisor2 register */
	INIT_Supervisor3_Rx_32B_tag						INIT_Supervisor3;		/*last received state of the INIT_Supervisor3 register */
	INIT_FSSM1_Rx_32B_tag							INIT_FSSM1;					/*last received state of the INIT_FSSM1 register */
	INIT_FSSM2_Rx_32B_tag							INIT_FSSM2;					/*last received state of the INIT_FSSM2 register */
	WD_window_Rx_32B_tag							WD_window;					/*last received state of the WD_window register */
	WD_LFSR_Rx_32B_tag								WD_LFSR;						/*last received state of the WD_LFSR register */
	WD_answer_Rx_32B_tag							WD_answer;					/*last received state of the WD_answer register */
	Diag_FS1_Rx_32B_tag								Diag_FS1;						/*last received state of the Diag_FS1 register */
	WD_counter_Rx_32B_tag							WD_counter;					/*last received state of the WD_counter register */
	Diag_FS2_Rx_32B_tag								Diag_FS2;						/*last received state of the Diag_FS2 register	*/
}intStruct_t;

struct {
	uint32_t	WD_answer;							/*actual content of the WD_answer register*/
	register32_struct currentLFSR;		/*last LFSR state*/
} PITstruct;



/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/


#endif


