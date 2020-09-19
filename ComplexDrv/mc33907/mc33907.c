

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "defines.h"
#include "typedefs.h"
#include "platformTypes.h"
#include "global_var.h"
#include "utils.h"
#include "PwSBC.h"
#include "mc33907_pri.h"
#include "spi.h"
#include "spiCfg.h"
#include "mc33907.h"
//#include "canCfg.h"
#include "diag.h"
#include "hdioCfg.h"
#include "hdio.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/
#define DebugOnly FALSE

#define m_33907_readDoubleCheck FALSE


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/
static spiStruct_t spiStruct;
static intStruct_t intStruct;
static intUpdtStru_t intUpdtStru =
{
	FALSE,FALSE,FALSE,FALSE,FALSE,
	FALSE,FALSE,FALSE,FALSE,FALSE,
	FALSE,FALSE,FALSE,FALSE,FALSE,
	FALSE,FALSE,FALSE,FALSE,FALSE,
	FALSE,FALSE,FALSE,FALSE,FALSE,
	FALSE,FALSE,FALSE,FALSE,FALSE,
	FALSE,FALSE,FALSE
};





/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/
uint32_t mc33907_errCode = m_33907_noErr;
uint32_t diagVreg1 = 0;
uint32_t diagVreg2 = 0;
uint32_t diagVreg3 = 0;

uint32_t diagCAN1 = 0;
uint32_t diagCANLin = 0;

uint32_t diagSPI = 0;

uint32_t StatusVreg1 = 0;
uint32_t StatusVreg2 = 0;
uint32_t diagFS1 = 0;
uint32_t diagFS2 = 0;

uint32_t wdAnswer = 0;

uint8_t hld_mc33907_di4 = 0;
uint8_t hld_mc33907_di5 = 0;

/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/
static void mc33907_sendSecureCmdRw(uint32_t cmd);
static void mc33907_computeParity(uint32_t * cmd);
static void mc33907_computeSecurityBits(uint32_t *cmd);
static uint8_t mc33907_ComputeLFSR(uint32_t actualLFSR);
static void mc33907_sendCmdR(uint32_t cmd);
static void mc33907_sendCmdRw(uint32_t cmd);
static void mc33907_processRecMsg(uint32_t resp);
static void mc33907_sendSecureCmdW(uint32_t cmd);
static void mc33907_sendCmdW(uint32_t cmd);
static void mc33907_spiFaultHandle(uint32_t eventMask);
static void mc33907_vreg1FaultHandle(uint32_t errMask);
static void mc33907_vreg2FaultHandle(uint32_t errMask);
static void mc33907_vreg3FaultHandle(uint32_t errMask);
static void mc33907_can1FaultHandle(uint32_t errMask);
static void mc33907_can2FaultHandle(uint32_t errMask);


/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/

void mc33907_init(void)
{

	uint8_t initRetryCnt = 100;

	intStruct.INIT_Vreg1.R = INIT_VREG1_DEF;
	intStruct.INIT_Vreg2.R = INIT_VREG2_DEF;
	intStruct.INIT_CAN_LIN.R = INIT_CAN_LIN_DEF;
	intStruct.INIT_IO_WU1.R = INIT_IO_WU1_DEF;
	intStruct.INIT_IO_WU2.R = INIT_IO_WU2_DEF;
	intStruct.INIT_INT.R = INIT_INT_DEF;
	intStruct.MODE.R = MODE_DEF;
	intStruct.Vreg_mode.R = VREG_MODE_DEF;
	intStruct.IO_OUT_AMUX.R = IO_OUT_AMUX_DEF;
	intStruct.CAN_LIN_mode.R = CAN_LIN_MODE_DEF;
	intStruct.Vreg_LPON.R = VREG_LPON_DEF;
	intStruct.INIT_Supervisor1.R = INIT_SUPERVISOR1_DEF;
	intStruct.INIT_Supervisor2.R = INIT_SUPERVISOR2_DEF;
	intStruct.INIT_Supervisor3.R = INIT_SUPERVISOR3_DEF;
	intStruct.INIT_FSSM1.R = INIT_FSSM1_DEF;
	intStruct.INIT_FSSM2.R = INIT_FSSM2_DEF;
	intStruct.WD_window.R = WD_WINDOW_DEF;
	intStruct.WD_LFSR.R = WD_LFSR_DEF;
	intStruct.WD_answer.R = WD_ANSWER_DEF;
	/*at first,verify the mode, init , or normal*/

	while(initRetryCnt > 0)
	{
		mc33907_getMode();
		if(TRUE == intUpdtStru.mode)
		{
			intUpdtStru.mode = FALSE;
			break;
		}
		initRetryCnt --;
	}
	if(initRetryCnt > 0)
	{
		if(intStruct.MODE.B.Init >0)
		{
			mc33907_setInitSupervisor1(0);	/*No effect of Vcore_OV and Vcore_uv on RSTb and FSxx*/

			mc33907_setInitSupervisor2(1);	/*No effect of Vaux_OV and Vaux_uv on RSTb and FSxx*/
											/*Disable the 8s timer used to enter Deep FAILSAFE mode*/
			mc33907_setInitSupervisor3(0);	/*Configure the Vcore,VCCA,VAUX undervoltage in degraded mode. Only valid for 5V*/

			mc33907_setInitFssm1(0);

			mc33907_setInitFssm2(0);

			mc33907_setWu1(0x53);		/*set IO0 IO1wake up at rising edge,set IO2 no wakeUp capability*/

			mc33907_setWu2(0x14);		/*set IO2 no wake up， IO4 IO5 wake-up at rising edge*/

			mc33907_enableVcan();

			/*then go to normal mode*/
			mc33907_setInitInt(0);
		}
	}
	else
	{
		sysPreSleep = TRUE;
		mc33907_setLpOff();
		return;

	}
	mc33907_setAmuxChannel(1);
	mc33907_setCanMode(CAN_NORMAL);
	mc33907_SetLINmode(LIN_NORMAL);
	mc33907_changeWdWindow(WD_WIN_256);
	mc33907_refreshWd();

}


/***************************************************************************!/
*	@brief The function mc33907_setLpOff switches the actual mode of the PwSBC
*			to the low power OFF mode.
*	@par Include
*
*	@par Description
*					This function sets bit LPOFF in the MODE register to switch PwSBC
*					into the Low power Vreg OFF mode and checks whether the mode
*					was entered or not.
*	@return 	Return value can be returned only in the case that the MCU is
*				supplied externally (not using Vcore of the PwSBC). In the
*				other case Vcore, PowerSBC goes into the LPOFF mode, Vcore
*				regulator is switched off and the MCU losts all data.
				- "TRUE" - Device entered the LPOFF mode successfuly. <br>
*				- "FALSE" - Operation failed.
*	@remarks	Normal and LPON modes have higher priority than LPOFF. Applying
*				this command will switch PwSBC into LPOFF mode, Vcore regulator
*				will be switched OFF and MCU will lost all the data.
*	@par Code sample
*			while(TRUE == mc33907_setLpOff())
*
*			- Program stays in an infinite loop till the until the LPOFF mode
*			is entered.
********************************************************************************/
bool mc33907_setLpOff(void)
{
	bool setOk = TRUE;
	MODE_Tx_32B_tag cmd;
	cmd.R = 0;

	cmd.B.RW = 1;									/*write command*/
	cmd.B.ADR = MODE_ADR;							/*set address*/
	cmd.B.LPOFF = 1;								/*LPOFF mode*/
	mc33907_sendSecureCmdW(cmd.R);
	return setOk;
}



/***************************************************************************!/
*   @brief The function mc33907_setIntReq requests an interrupt.
*	@par Include
*
* 	@par Description
*					This function sets bit INT_request in the MODE register, which
*					will generate an interrupt.
*	@remarks 	INT_request will be cleared automatically when the interrupt is
*				done.
*	@par Code sample
*			mc33907_setIntReq();
*			- Commad requests an interrupt.
********************************************************************************/
bool mc33907_setIntReq(void)
{
	bool setOk = TRUE;
	MODE_Tx_32B_tag cmd;
	cmd.R = 0;
	cmd.B.RW = 1;									/*write command*/
	cmd.B.ADR = MODE_ADR;							/*set address*/
	cmd.B.INT_request = 1;								/*LPOFF mode*/
	mc33907_sendSecureCmdRw(cmd.R);
	return setOk;
}


/***************************************************************************!/
*   @brief The function mc33907_enVcore enables the Vcore regulator during
*			the Normal mode.
*	@par Include
*
* 	@par Description
*					This function sets bit Vcore_EN in the Vreg Mode register, which
*					enables the Vcore power supply.
* 	@return
*				- "TRUE" - Vcore was successfuly enabled. <br>
*				- "FALSE" - Operation failed.
*	@remarks
*	@par Code sample
*			</para>
*			bool status; </para>
*			status = mc33907_enVcore(); </para>
*			- Command enables Vcore power supply and stores returned status.
********************************************************************************/

bool mc33907_enVcore(void)
{
	bool setOk = TRUE;
	Vreg_mode_Tx_32B_tag cmd;
	cmd.R = intStruct.Vreg_mode.R << 4;
	cmd.B.RW = 1;
	cmd.B.ADR = VREG_MODE_ADR;
	cmd.B.Vcore_EN = 1;
	mc33907_sendSecureCmdRw(cmd.R);
	if(TRUE == intUpdtStru.vregMode)
	{
		intUpdtStru.vregMode = FALSE;
		if(0 < intStruct.Vreg_mode.B.Vcore_EN)
		{
			setOk = TRUE;
		}
		else
		{
			setOk = FALSE;
		}
	}
	return setOk;
}



/****************************************************************************!
*   @brief The function mc33907_disVcore disables the Vcore regulator during the Normal mode.
*	@par Include
*					PwSBC.h
* 	@par Description
*					This function clears bit Vcore_EN in the Vreg Mode register,
*					which disables the Vcore power supply.
* 	@return
*				- "TRUE" - Vcore was successfuly disabled. <br>
*				- "FALSE" - Operation failed.
*	@remarks 	Do not disable Vcore if it is configured as a safety critical.
*	@par Code sample
*												</para>
*			bool status;					</para>
*			status = mc33907_disVcore();		</para>
*			- Command disables Vcore power supply and stores returned status.
********************************************************************************/
bool mc33907_disVcore(void)
{
	bool setOk = TRUE;
	Vreg_mode_Tx_32B_tag cmd;
	if(intStruct.INIT_Supervisor1.B.Vcore_FS >0)
	{
		/*safety critical, not allowed to disable it*/
		setOk = FALSE;
	}
	else
	{
		cmd.R = intStruct.Vreg_mode.R << 4;
		cmd.B.RW = 1;
		cmd.B.ADR = VREG_MODE_ADR;
		cmd.B.Vcore_EN = 0;
		mc33907_sendSecureCmdRw(cmd.R);
		if(TRUE == intUpdtStru.vregMode)
		{
			intUpdtStru.vregMode = FALSE;
			if(0 == intStruct.Vreg_mode.B.Vcore_EN)
			{
				setOk = TRUE;
			}
			else
			{
				setOk = FALSE;
			}
		}
	}
	return setOk;
}

/***************************************************************************!
*   @brief The function mc33907_enVcca enables the Vcca regulator during the Normal mode.
*	@par Include
*					PwSBC.h
* 	@par Description
*					This function sets bit Vcca_EN in the Vreg Mode register,
*					which enables the Vcca power supply.
* 	@return
*				- "TRUE" - Vcca was successfuly enabled. <br>
*				- "FALSE" - Operation failed.
*	@remarks
*	@par Code sample
*											</para>
*			bool status;				</para>
*			status = mc33907_enVcca();	</para>
*			- Command enables Vcca power supply and stores returned status.
********************************************************************************/
bool mc33907_enVcca(void)
{
	bool setOk = TRUE;
	Vreg_mode_Tx_32B_tag cmd;
	cmd.R = intStruct.Vreg_mode.R << 4;				/*restore old register setting*/
	cmd.B.RW = 1;									/*write command*/
	cmd.B.ADR = VREG_MODE_ADR;						/*set address*/
	cmd.B.Vcca_EN = 1;								/*enable Vcca*/
	mc33907_sendSecureCmdRw(cmd.R);
	if(TRUE == intUpdtStru.vregMode)
	{
		intUpdtStru.vregMode = FALSE;
		if(0 < intStruct.Vreg_mode.B.Vcca_EN)
		{
			setOk = TRUE;
		}
		else
		{
			setOk =  FALSE;
		}
											/*success*/
	}
	else
	{
		setOk =  FALSE;									/*error*/
	}
	return setOk;
}

void mc33907_enterSleep(void)
{
	Vreg_mode_Tx_32B_tag cmd;
	cmd.B.RW = 1;									/*write command*/
	cmd.B.ADR = VREG_MODE_ADR;						/*set address*/
	cmd.B.Vcca_EN = 0;								/*disable Vcca*/
	cmd.B.Vcore_EN = 0;								/*disable Vcore*/
	cmd.B.Vaux_EN = 0;								/*disable Vaux*/
	cmd.B.Vcan_EN = 0;								/*disable Vcan*/
	mc33907_sendSecureCmdRw(cmd.R);
}


void mc33907_exitSleep(void)
{
	Vreg_mode_Tx_32B_tag cmd;
	cmd.B.RW = 1;									/*write command*/
	cmd.B.ADR = VREG_MODE_ADR;						/*set address*/
	cmd.B.Vcca_EN = 1;								/*enable Vcca*/
	cmd.B.Vcore_EN = 1;								/*enable Vcore*/
	cmd.B.Vaux_EN = 1;								/*enable Vaux*/
	mc33907_sendSecureCmdRw(cmd.R);
}


/***************************************************************************!/
*   @brief The function mc33907_disVcca disables the Vcca regulator during the Normal mode.
*	@par Include
*
* 	@par Description
*					This function clears bit Vcca_EN in the Vreg Mode register,
*					which disables the Vcca power supply.
* 	@return
*				- "TRUE" - Vcca was successfuly disabled. <br>
*				- "FALSE" - Operation failed.
*	@remarks 	Do not disable Vcca if it is configured as a safety critical.
*	@par Code sample
*											</para>
*			bool status;				</para>
*			status = mc33907_disVcca();	</para>
*			- Command disables Vcca power supply and stores returned status.
********************************************************************************/

bool mc33907_disVcca(void)
{
	bool setOk = TRUE;
	Vreg_mode_Tx_32B_tag cmd;
	if(intStruct.INIT_Supervisor1.B.Vcca_FS >0)
	{
		/*safety critical, not allowed to disable it*/
		setOk = FALSE;
	}
	else
	{
		cmd.R = intStruct.Vreg_mode.R << 4;
		cmd.B.RW = 1;
		cmd.B.ADR = VREG_MODE_ADR;
		cmd.B.Vcca_EN = 0;
		mc33907_sendSecureCmdRw(cmd.R);
		if(TRUE == intUpdtStru.vregMode)
		{
			intUpdtStru.vregMode = FALSE;
			if(0 == intStruct.Vreg_mode.B.Vcca_EN)
			{
				setOk = TRUE;
			}
			else
			{
				setOk = FALSE;
			}
		}
	}
	return setOk;
}

/***************************************************************************!/
*   @brief The function mc33907_enVaux enables the Vaux regulator during the Normal mode.
*	@par Include
*					PwSBC.h
* 	@par Description
*					This function sets bit Vaux_EN in the Vreg Mode register, which
*					enables the Vaux power supply.
* 	@return
*				- "TRUE" - Vaux was successfuly enabled. <br>
*				- "FALSE" - Operation failed.
*	@remarks
*	@par Code sample
*											</para>
*			bool status;				</para>
*			status = mc33907_enVaux();	</para>
*			- Command enables Vaux power supply and stores returned status.
********************************************************************************/
bool mc33907_enVaux(void)
{
	bool setOk = TRUE;
	Vreg_mode_Tx_32B_tag cmd;
	cmd.R = intStruct.Vreg_mode.R << 4;				/*restore old register setting*/
	cmd.B.RW = 1;									/*write command*/
	cmd.B.ADR = VREG_MODE_ADR;						/*set address*/
	cmd.B.Vaux_EN = 1;								/*enable Vcca*/
	mc33907_sendSecureCmdRw(cmd.R);
	if(TRUE == intUpdtStru.vregMode)
	{
		intUpdtStru.vregMode = FALSE;
		if(0 < intStruct.Vreg_mode.B.Vaux_EN)
		{
			setOk = TRUE;
		}
		else
		{
			setOk =  FALSE;
		}

	}
	else
	{
		setOk =  FALSE;
	}
	return setOk;
}

/***************************************************************************
*   @brief The function mc33907_disVaux disables the Vaux regulator during the Normal mode.
*	@par Include
*
* 	@par Description
*					This function clears bit Vaux_EN in the Vreg Mode register,
*					which disables the Vaux power supply.
* 	@return
*				- "TRUE" - Vaux was successfuly disabled. <br>
*				- "FALSE" - Operation failed.
*	@remarks 	Do not disable Vaux if it is configured as a safety critical.
*	@par Code sample
*											</para>
*			bool status;				</para>
*			status = mc33907_disVaux();	</para>
*			- Command disables Vaux power supply and stores returned status.
********************************************************************************/
bool mc33907_disVaux(void)
{
	bool setOk = TRUE;
	Vreg_mode_Tx_32B_tag cmd;
	if(intStruct.INIT_Supervisor2.B.Vaux_FS >0)
	{
		/*safety critical, not allowed to disable it*/
		setOk = FALSE;
	}
	else
	{
		cmd.R = intStruct.Vreg_mode.R << 4;
		cmd.B.RW = 1;
		cmd.B.ADR = VREG_MODE_ADR;
		cmd.B.Vaux_EN = 0;
		mc33907_sendSecureCmdRw(cmd.R);
		if(TRUE == intUpdtStru.vregMode)
		{
			intUpdtStru.vregMode = FALSE;
			if(0 == intStruct.Vreg_mode.B.Vaux_EN)
			{
				setOk = TRUE;
			}
			else
			{
				setOk = FALSE;
			}
		}
	}
	return setOk;
}


/***************************************************************************
*   @brief The function mc33907_enableVcan enables the Vcan regulator during the Normal mode.
*	@par Include
*					PwSBC.h
* 	@par Description
*					This function sets bit Vcan_EN in the Vreg Mode register, which
*					enables the Vcan power supply.
* 	@return
*				- "TRUE" - Vcan was successfuly enabled. <br>
*				- "FALSE" - Operation failed.
*	@remarks
*	@par Code sample
*											</para>
*			bool status;				</para>
*			status = mc33907_enableVcan();	</para>
*			Command enables Vcan power supply and stores returned status.
********************************************************************************/
bool mc33907_enableVcan(void)
{
	bool setOk = TRUE;
	Vreg_mode_Tx_32B_tag cmd;
	cmd.R = intStruct.Vreg_mode.R << 4;				/*restore old register setting*/
	cmd.B.RW = 1;									/*write command*/
	cmd.B.ADR = VREG_MODE_ADR;						/*set address*/
	cmd.B.Vcan_EN = 1;
	cmd.B.Vcore_EN = 1;
	cmd.B.Vcca_EN = 1;
	cmd.B.Vaux_EN = 1;
	mc33907_sendSecureCmdRw(cmd.R);
	if(TRUE == intUpdtStru.vregMode)
	{
		intUpdtStru.vregMode = FALSE;
		if(0 < intStruct.Vreg_mode.B.Vcan_EN)
		{
			setOk = TRUE;
		}
		else
		{
			setOk =  FALSE;
		}

	}
	else
	{
		setOk =  FALSE;
	}
	return setOk;
}

/***************************************************************************
*   @brief The function mc33907_disVcan disables the Vcan regulator during the Normal mode.
*	@par Include
*
* 	@par Description
*					This function clears bit Vcan_EN in the Vreg Mode register,
*					which disables the Vcan power supply.
* 	@return
*				- "TRUE" - Vcan was successfuly disabled. <br>
*				- "FALSE" - Operation failed.
*	@remarks
*	@par Code sample
*											</para>
*			bool status;				</para>
*			status = mc33907_disVcan();	</para>
*			- Command disables Vcan power supply and stores returned status.
********************************************************************************/
bool mc33907_disVcan(void)
{
	bool setOk = TRUE;
	Vreg_mode_Tx_32B_tag cmd;

	cmd.R = intStruct.Vreg_mode.R << 4;
	cmd.B.RW = 1;
	cmd.B.ADR = VREG_MODE_ADR;
	cmd.B.Vcan_EN = 0;
	cmd.B.Vcore_EN = 1;
	cmd.B.Vcca_EN = 1;
	cmd.B.Vaux_EN = 1;
	mc33907_sendSecureCmdRw(cmd.R);
	if(TRUE == intUpdtStru.vregMode)
	{
		intUpdtStru.vregMode = FALSE;
		if(0 == intStruct.Vreg_mode.B.Vcan_EN)
		{
			setOk = TRUE;
		}
		else
		{
			setOk = FALSE;
		}
	}
	return setOk;
}

/***************************************************************************
*   @brief The function mc33907_enIo4OutGateDriver enables an output gate driver for the IO pins (IO4 and IO5).
*	@par Include
*
* 	@par Description
*					This function sets bit IO_out_4_EN (register IO_out-AMUX),
*					which enables output gate driver for IO_4.
* 	@return
*				- "TRUE" - Output gate driver was successfuly enabled. <br>
*				- "FALSE" - Operation failed.
*	@remarks
*	@par Code sample
*											</para>
*			bool status;				</para>
*			status = mc33907_enIo4OutGateDriver();	</para>
*			- Command enables output gate driver for IO_4 and stores returned status.
********************************************************************************/
bool mc33907_enIo4OutGateDriver(void)
{
	bool setOk = FALSE;
	IO_OUT_AMUX_32B_tag cmd;
	cmd.R = 0;
	cmd.R = intStruct.IO_OUT_AMUX.R;				/*restore old register setting*/
	cmd.B.RW = 1;									/*write command*/
	cmd.B.ADR = IO_OUT_AMUX_ADR;					/*set address*/
	cmd.B.IO_out_4_EN = 1;							/*enable IO4 OUT*/
	 mc33907_sendCmdRw(cmd.R);
	if(TRUE == intUpdtStru.ioOutMux)
	{
		intUpdtStru.ioOutMux = FALSE;
		if(0 == intStruct.IO_OUT_AMUX.B.IO_out_4_EN)
		{
			setOk = FALSE;
		}
		else
		{
			setOk = TRUE;
		}
	}
	return setOk;
}



/***************************************************************************
*   @brief The function mc33907_setIo4OutGateDriver sets the specified IO output (logic High).
*	@par Include
*
* 	@par Description
*					This function sets bit IO_out_4 (register IO_out-AMUX), which
*					sets IO_4 to logical high state.
* 	@return
*				- "TRUE" - Output was successfuly set to high state. <br>
*				- "FALSE" - Operation failed.
*	@remarks
*	@par Code sample
*											</para>
*			boll status;				</para>
*			status = mc33907_setIo4OutGateDriver();		</para>
*			- Command sets IO_4 to high state and stores returned status.
********************************************************************************/

bool mc33907_setIo4OutGateDriver(void)
{
	bool setOk = FALSE;
	IO_OUT_AMUX_32B_tag cmd;
	cmd.R = 0;

	cmd.R = intStruct.IO_OUT_AMUX.R;				/*restore old register setting*/
	cmd.B.RW = 1;									/*write command*/
	cmd.B.ADR = IO_OUT_AMUX_ADR;					/*set address*/
	cmd.B.IO_out_4 = 1;							/*enable IO4 OUT*/
	 mc33907_sendCmdRw(cmd.R);
	if(TRUE == intUpdtStru.ioOutMux)
	{
		intUpdtStru.ioOutMux = FALSE;
		if(0 == intStruct.IO_OUT_AMUX.B.IO_out_4)
		{
			setOk = FALSE;
		}
		else
		{
			setOk = TRUE;
		}
	}
	return setOk;
}

/***************************************************************************
*   @brief The function mc33907_clrIo4OutGateDriver clears the specified IO output (logic Low).
*	@par Include
*
* 	@par Description
*					This function clears bit IO_out_4 (register IO_out-AMUX), which
*					sets IO_4 to logical low state.
* 	@return
*				- "TRUE" - Output was successfuly set to low state. <br>
*				- "FALSE" - Operation failed.
*	@remarks
*	@par Code sample
*											</para>
*			bool status;				</para>
*			status = mc33907_clrIo4OutGateDriver();		</para>
*			- Command sets IO_4 to low state and stores returned status.
********************************************************************************/
bool mc33907_clrIo4OutGateDriver(void)
{
	bool setOk = FALSE;
	IO_OUT_AMUX_32B_tag cmd;
	cmd.R = 0;

	cmd.R = intStruct.IO_OUT_AMUX.R;				/*restore old register setting*/
	cmd.B.RW = 1;									/*write command*/
	cmd.B.ADR = IO_OUT_AMUX_ADR;					/*set address*/
	cmd.B.IO_out_4 = 0;							/*enable IO4 OUT*/
	mc33907_sendCmdRw(cmd.R);
	if(TRUE == intUpdtStru.ioOutMux)
	{
		intUpdtStru.ioOutMux = FALSE;
		if(1 == intStruct.IO_OUT_AMUX.B.IO_out_4)
		{
			setOk = FALSE;
		}
		else
		{
			setOk = TRUE;
		}
	}
	return setOk;
}




/***************************************************************************
*   @brief The function mc33907_enIo5OutGateDriver enables an output gate driver for the IO pins (IO4 and IO5).
*	@par Include
*
* 	@par Description
*					This function sets bit IO_out_5_EN (register IO_out-AMUX),
*					which enables output gate driver for IO_5.
* 	@return
*				- "TRUE" - Output gate driver was successfuly enabled. <br>
*				- "FALSE" - Operation failed.
*	@remarks
*	@par Code sample
*											</para>
*			bool status;				</para>
*			status = mc33907_enIo5OutGateDriver();	</para>
*			- Command enables output gate driver for IO_5 and stores returned status.
********************************************************************************/
bool mc33907_enIo5OutGateDriver(void)
{
	bool setOk = FALSE;
	IO_OUT_AMUX_32B_tag cmd;
	cmd.R = 0;

	cmd.R = intStruct.IO_OUT_AMUX.R;				/*restore old register setting*/
	cmd.B.RW = 1;									/*write command*/
	cmd.B.ADR = IO_OUT_AMUX_ADR;					/*set address*/
	cmd.B.IO_out_5_EN = 1;							/*enable IO5 OUT*/
	 mc33907_sendCmdRw(cmd.R);
	if(TRUE == intUpdtStru.ioOutMux)
	{
		intUpdtStru.ioOutMux = FALSE;
		if(0 == intStruct.IO_OUT_AMUX.B.IO_out_5_EN)
		{
			setOk = FALSE;
		}
		else
		{
			setOk = TRUE;
		}
	}
	return setOk;
}



/***************************************************************************
*   @brief The function mc33907_setIo5OutGateDriver sets the specified IO output (logic High).
*	@par Include
*
* 	@par Description
*					This function sets bit IO_out_5 (register IO_out-AMUX), which
*					sets IO_5 to logical high state.
* 	@return
*				- "TRUE" - Output was successfuly set to high state. <br>
*				- "FALSE" - Operation failed.
*	@remarks
*	@par Code sample
*											</para>
*			boll status;				</para>
*			status = mc33907_setIo5OutGateDriver();		</para>
*			- Command sets IO_5 to high state and stores returned status.
********************************************************************************/

bool mc33907_setIo5OutGateDriver(void)
{
	bool setOk = FALSE;
	IO_OUT_AMUX_32B_tag cmd;
	cmd.R = 0;

	cmd.R = intStruct.IO_OUT_AMUX.R;				/*restore old register setting*/
	cmd.B.RW = 1;									/*write command*/
	cmd.B.ADR = IO_OUT_AMUX_ADR;					/*set address*/
	cmd.B.IO_out_5 = 1;							/*enable IO5 OUT*/
	 mc33907_sendCmdRw(cmd.R);
	if(TRUE == intUpdtStru.ioOutMux)
	{
		intUpdtStru.ioOutMux = FALSE;
		if(0 == intStruct.IO_OUT_AMUX.B.IO_out_5)
		{
			setOk = FALSE;
		}
		else
		{
			setOk = TRUE;
		}
	}
	return setOk;
}

/***************************************************************************
*   @brief The function mc33907_clrIo5OutGateDriver clears the specified IO output (logic Low).
*	@par Include
*
* 	@par Description
*					This function clears bit IO_out_5 (register IO_out-AMUX), which
*					sets IO_5 to logical low state.
* 	@return
*				- "TRUE" - Output was successfuly set to low state. <br>
*				- "FALSE" - Operation failed.
*	@remarks
*	@par Code sample
*											</para>
*			bool status;				</para>
*			status = mc33907_clrIo5OutGateDriver();		</para>
*			- Command sets IO_5 to low state and stores returned status.
********************************************************************************/
bool mc33907_clrIo5OutGateDriver(void)
{
	bool setOk = FALSE;
	IO_OUT_AMUX_32B_tag cmd;
	cmd.R = 0;

	cmd.R = intStruct.IO_OUT_AMUX.R;				/*restore old register setting*/
	cmd.B.RW = 1;									/*write command*/
	cmd.B.ADR = IO_OUT_AMUX_ADR;					/*set address*/
	cmd.B.IO_out_5 = 0;							/*enable IO5 OUT*/
	mc33907_sendCmdRw(cmd.R);
	if(TRUE == intUpdtStru.ioOutMux)
	{
		intUpdtStru.ioOutMux = FALSE;
		if(1 == intStruct.IO_OUT_AMUX.B.IO_out_5)
		{
			setOk = FALSE;
		}
		else
		{
			setOk = TRUE;
		}
	}
	return setOk;
}

/***************************************************************************
*   @brief The function mc33907_setAmuxChannel switches a desired channel to the output of AMUX.
*	@par Include
*
* 	@par Description
*					This function switches AMUX channel to the desired one
*					(bits Amux_2:0 in the IO_out-AMUX register.
* 	@param[in] nbChannel - Number of the desired AMUX channel (from 0 till 7).
* 	@return
*				- "TRUE" - AMUX channel successfuly switched. <br>
*				- "FALSE" - Operation failed.
*	@remarks
*	@par Code sample
*													</para>
*			bool status;						</para>
*			status = mc33907_setAmuxChannel(5);	</para>
*			- Command switches AMUX channel to the channel no. 5 and stores
*			returned status.
********************************************************************************/
bool mc33907_setAmuxChannel(uint8_t channel)
{
	uint8_t setChan;
	IO_OUT_AMUX_32B_tag cmd;
	bool setOk = FALSE;
	setChan = ((channel >7)?(0):(channel));/*paramter check*/
	cmd.R = intStruct.IO_OUT_AMUX.R;				/*restore old register setting*/
	cmd.B.RW = 1;									/*write command*/
	cmd.B.ADR = IO_OUT_AMUX_ADR;					/*set address*/
	cmd.B.Amux = setChan;							/*set AMUX channel*/
	mc33907_sendCmdRw(cmd.R);
	if(TRUE == intUpdtStru.ioOutMux)
	{
		intUpdtStru.ioOutMux = FALSE;
		if(setChan == intStruct.IO_OUT_AMUX.B.Amux)
		{
			setOk= TRUE;									/*success*/
		}
		else
		{
			setOk= FALSE;									/*error*/
		}
	}
	return setOk;

}

/***************************************************************************//*!
*   @brief The function mc33907_setCanMode sets the CAN mode.
*	@par Include
*
* 	@par Description
*					This function changes CAN mode in the CAN-LIN mode register.
* 	@param[in] mode - Desired CAN mode (from 0 till 3).
* 	@return
*				- "TRUE" - CAN mode was successfuly changed. <br>
*				- "FALSE" - Operation failed.
*	@remarks
*	@par Code sample
*														</para>
*			bool status;							</para>
*			status = mc33907_setCanMode(CAN_SLEEP);		</para>
*			- Command switches CAN transceiver into the Sleep mode without
*			a wake-up capability.
********************************************************************************/
bool mc33907_setCanMode(uint8_t mode)
{
	uint8_t setMode;
	bool setOk = FALSE;
	CAN_LIN_mode_Tx_32B_tag cmd;
	setMode = ((mode >3)?(0):(mode));
	cmd.R = intStruct.CAN_LIN_mode.R;				//restore old register setting
	cmd.B.RW = 1;									//write command
	cmd.B.ADR = CAN_LIN_MODE_ADR;					//set address
	cmd.B.CAN_mode = mode;							//set CAN mode
	mc33907_sendCmdRw(cmd.R);
	if(TRUE == intUpdtStru.canLinMode)
	{
		intUpdtStru.canLinMode = FALSE;
		if(setMode == intStruct.CAN_LIN_mode.B.CAN_mode)
		{
			setOk = TRUE;									/*success*/
		}
		else
		{
			setOk= FALSE;									/*error*/
		}
	}
	return setOk;
}

/***************************************************************************//*!
*   @brief The function PwSBC_SetLINmode sets the LIN mode.
*	@par Include
*					PwSBC.h
* 	@par Description
*					This function changes LIN mode in the CAN-LIN mode register.
* 	@param[in] mode - Desired LIN mode (from 0 till 3).
* 	@return
*				- "0" - LIN mode was successfuly changed. <br>
*				- "1" - Operation failed.
*	@remarks
*	@par Code sample
*													</para>
*			uint32_t status;						</para>
*			status = PwSBC_SetLINmode(LIN_RX);		</para>
*			- Command switches LIN transceiver into the Listen only mode.
********************************************************************************/
bool mc33907_SetLINmode(uint32_t mode)
{
	uint8_t setMode;
	bool setOk = FALSE;
	CAN_LIN_mode_Tx_32B_tag cmd;
	cmd.R = 0;

	setMode = ((mode >3)?(0):(mode));
	cmd.R = intStruct.CAN_LIN_mode.R;				//restore old register setting
	cmd.B.RW = 1;									//write command
	cmd.B.ADR = CAN_LIN_MODE_ADR;					//set address
	cmd.B.LIN_mode = mode;							//set LIN mode
	cmd.B.LIN_auto_dis = 0;

	mc33907_sendCmdRw(cmd.R);
	if(setMode == intStruct.CAN_LIN_mode.B.LIN_mode)
	{
		setOk = TRUE;									/*success*/
	}
	else
	{
		setOk= FALSE;									/*error*/
	}
	return setOk;
}


/***************************************************************************//*!
*   @brief The function mc33907_setCanAutoTxDis sets the Automatic CAN Tx disable.
*	@par Include
*
* 	@par Description
*					This function sets  or clears bit CAN_auto_dis in the CAN-LIN mode register.
*					This will enable or disable  CAN transceiver to switch its mode from
*					"Normal" to "Listen only" automatically when an event (CAN
*					overtemperature/TXD dominant/RXD recessive) occurs.
* 	@return
*				- "TRUE" - CAN_auto_dis bit was successfuly set. <br>
*				- "FALSE" - Operation failed.
*	@remarks
*	@par Code sample
*													</para>
*			bool status;						</para>
*			status = mc33907_setCanAutoTxDis(TRUE);	</para>
*			- Command enables CAN transceiver to change its mode automatically.
********************************************************************************/
bool mc33907_setCanAutoTxDis(bool set)
{
	bool setOk = FALSE;
	CAN_LIN_mode_Tx_32B_tag cmd;
	uint8_t autoDis = ((TRUE == set)?(1):(0));
	cmd.R = intStruct.CAN_LIN_mode.R;				//restore old register setting
	cmd.B.RW = 1;									//write command
	cmd.B.ADR = CAN_LIN_MODE_ADR;					//set address
	cmd.B.CAN_auto_dis = autoDis;							//set CAN mode
	mc33907_sendCmdRw(cmd.R);
	if(TRUE == intUpdtStru.canLinMode)
	{
		intUpdtStru.canLinMode = FALSE;
		if(autoDis == intStruct.CAN_LIN_mode.B.CAN_auto_dis)
		{
			setOk = TRUE;									/*success*/
		}
		else
		{
			setOk= FALSE;									/*error*/
		}
	}
	return setOk;
}





/***************************************************************************//*!
*   @brief The function mc33907_changeWdWindow changes the WD window duration.
*	@par Include
*
* 	@par Description
*					This function changes WD window duration.
* 	@param[in] duration - WD window duration coded in binary (from 0 till 15).
*				Predefined values:
*				- WD_DISABLE - WD window disabled
*				- WD_WIN_1 - 1 ms
*				- WD_WIN_2 - 2 ms
*				- WD_WIN_3 - 3 ms
*				- WD_WIN_4 - 4 ms
*				- WD_WIN_6 - 6 ms
*				- WD_WIN_8 - 8 ms
*				- WD_WIN_12	- 12 ms
*				- WD_WIN_16	- 16 ms
*				- WD_WIN_24	- 24 ms
*				- WD_WIN_32	- 32 ms
*				- WD_WIN_64	- 64 ms
*				- WD_WIN_128 - 128 ms
*				- WD_WIN_256 - 256 ms
*				- WD_WIN_512 - 512 ms
*				- WD_WIN_1024 - 1024 ms
* 	@return
*				- "TRUE" - WD window duration was successfuly set. <br>
*				- "FALSE" - Operation failed.
*	@remarks
*	@par Code sample
*														</para>
*			bool status;							</para>
*			status = mc33907_changeWdWindow(WD_WIN_32);	</para>
*			- Command changes WD window duration to 32 ms.
********************************************************************************/

bool mc33907_changeWdWindow(uint8_t duration)
{
	WD_window_Tx_32B_tag cmd;
	bool setOk = FALSE;
	uint8_t setDur;
	if(duration > WD_WIN_1024)
	{
		setDur = WD_DISABLE;
	}
	else
	{
		setDur = duration;
	}
	cmd.R = 0;
	cmd.B.RW = 1;
	cmd.B.ADR = WD_WINDOW_ADR;
	cmd.B.WD_window = setDur;
	mc33907_sendSecureCmdRw(cmd.R);
	if(TRUE == intUpdtStru.wdWindow)
	{
		intUpdtStru.wdWindow = FALSE;
		if(setDur == intStruct.WD_window.B.WD_window)
		{
			setOk = TRUE;									/*success*/
		}
		else
		{
			setOk = FALSE;									/*error*/
		}
	}
	return setOk;
}

/***************************************************************************//*!
*   @brief The function mc33907_sendWdSeed sends an LFSR seed from the MCU to the PwSBC.
*	@par Include
*
* 	@par Description
*					This function sends new seed for the implemented LFSR.
* 	@param[in] seed - 	New seed for the LFSR. This can be any 8-bit value
*						excluding 0.
* 	@return
*				- "TRUE" - Seed was successfuly changed. <br>
*				- "FALSE" - Operation failed. <br>
*	@remarks 	Zero state is not allowed for this type of LFSR, because it
*				should never evolve from this state.
*	@par Code sample
*											</para>
*			bool status;				</para>
*			status = mc33907_sendWdSeed(20);	</para>
*			- Command changes LFSR value to 20 (decimal).
********************************************************************************/
bool mc33907_sendWdSeed(uint8_t seed)
{
	WD_LFSR_Tx_32B_tag cmd;
	bool setOk = FALSE;
	if(0 != seed)
	{
		cmd.R = 0;
		cmd.B.RW = 1;
		cmd.B.ADR = WD_LFSR_ADR;
		cmd.B.WD_LFSR = seed;
		mc33907_sendCmdRw(cmd.R);
		if(TRUE == intUpdtStru.wdLfsr)
		{
			intUpdtStru.wdLfsr = FALSE;
			if(seed == intStruct.WD_LFSR.B.WD_LFSR)
			{
				setOk = TRUE;									/*success*/
			}
			else
			{
				setOk = FALSE;									/*error*/
			}
		}
	}
	return setOk;
}

/***************************************************************************//*!
*   @brief The function mc33907_refreshWd refreshes the WD.
*	@par Include
*
* 	@par Description
*					This function refreshes WD using the WD answer calculated by LFSR. This is
*					done by writing into the WD answer register.
* 	@param[in] answer - 8-bit WD answer to be sent.
* 	@return
*				- "TRUE" - refresh successfully
*				- "FALSE" - refresh failed <br>
*	@remarks 	8-bit answer must be constisted of the control computations
*				that have been done on the actual LFSR content.
*	@par Code sample
*												</para>
*			uint32_t status;					</para>
*			status = mc33907_refreshWd();		</para>
*			- command to refresh the WD
********************************************************************************/
bool mc33907_refreshWd(void)
{
	bool refreshOk = FALSE;
	#if 1
	WD_answer_Tx_32B_tag cmd;
	uint8_t lfsr = 0;
	cmd.R = 0;
	cmd.B.RW = 1;										/*write command*/
	cmd.B.ADR = WD_ANSWER_ADR;							/*set address*/
	mc33907_getLfsr();
	if(TRUE == intUpdtStru.wdLfsr)
	{
		intUpdtStru.wdLfsr = FALSE;
		lfsr = intStruct.WD_LFSR.R;
		cmd.R = 0;
		cmd.B.RW = 1;										//write command
		cmd.B.ADR = WD_ANSWER_ADR;							//set address
		cmd.B.WD_answer = mc33907_ComputeLFSR(lfsr);/*set answer*/
		mc33907_sendCmdRw(cmd.R);
		wdAnswer = 0xFFFFFFFF;
		if(TRUE == intUpdtStru.wdAnswer)
		{
			intUpdtStru.wdAnswer = FALSE;
			wdAnswer = intStruct.WD_answer.R;
			if(0 == intStruct.WD_answer.B.WD)
			{
				refreshOk = TRUE;
			}
			else
			{
				refreshOk = FALSE;
			}
		}
	}
	#endif
	return refreshOk;
}



bool mc33907_setInitSupervisor1(uint8_t cmd)
{
	bool setOk = FALSE;
	INIT_Supervisor1_Tx_32B_tag cmdTx;
	cmdTx.B.RW = 1;
	cmdTx.B.ADR = INIT_SUPERVISOR1_ADR;
	cmdTx.B.Vcore_FS = ((cmd  & 0X0C) >> 2) & 0x03;
	cmdTx.B.Vcca_FS = cmd   & 0x03;
	mc33907_sendSecureCmdRw(cmdTx.R);
	if(TRUE == intUpdtStru.supervisor1)
	{
		intUpdtStru.supervisor1 = FALSE;
		if((intStruct.INIT_Supervisor1.R & 0X0F) == (cmd & 0X0F))
		{
			setOk = TRUE;
		}
		else
		{
			setOk = FALSE;
		}
	}
	return setOk;
}

bool mc33907_setInitSupervisor2(uint8_t cmd)
{
	bool setOk = FALSE;
	INIT_Supervisor2_Tx_32B_tag cmdTx;
	cmdTx.B.RW = 1;
	cmdTx.B.ADR = INIT_SUPERVISOR2_ADR;
	cmdTx.B.Vaux_FS = ((cmd  & 0X06) >> 1) & 0x03;
	cmdTx.B.DIS_8s = cmd   & 0x01;
	mc33907_sendSecureCmdRw(cmdTx.R);
	if(TRUE == intUpdtStru.supervisor2)
	{
		intUpdtStru.supervisor2 = FALSE;
		if((intStruct.INIT_Supervisor2.R & 0X07) == (cmd & 0X07))
		{
			setOk = TRUE;
		}
		else
		{
			setOk = FALSE;
		}
	}
	return setOk;
}

bool mc33907_setInitSupervisor3(uint8_t cmd)
{
	bool setOk = FALSE;
	INIT_Supervisor3_Tx_32B_tag cmdTx;
	cmdTx.B.RW = 1;
	cmdTx.B.ADR = INIT_SUPERVISOR3_ADR;
	cmdTx.B.Vcore_5D = ((cmd  & 0X04) >> 2) & 0x01;
	cmdTx.B.Vcca_5D = ((cmd  & 0X02) >> 2) & 0x01;
	cmdTx.B.Vaux_5D = cmd  & 0x01;

	mc33907_sendSecureCmdRw(cmdTx.R);
	if(TRUE == intUpdtStru.supervisor3)
	{
		intUpdtStru.supervisor3 = FALSE;
		if((intStruct.INIT_Supervisor3.R & 0X07) == (cmd & 0X07))
		{
			setOk = TRUE;
		}
		else
		{
			setOk = FALSE;
		}
	}
	return setOk;
}

bool mc33907_setInitFssm1(uint8_t cmd)
{
	bool setOk = FALSE;
	INIT_FSSM1_Tx_32B_tag cmdTx;
	cmdTx.R = cmd;
	cmdTx.B.RW = 1;
	cmdTx.B.ADR = INIT_FSSM1_ADR;
	/*cmdTx.B.IO_01_FS = bit7 of CMD; 1 safety critical,0 not safety*/
	/*cmdTx.B.IO_45_FS = bit6 of CMD; 1 safety critical,0 not safety*/
	/*cmdTx.B.RSTb_low = bit4 of CMD; Configure the Rstb LOW duration time 1:1ms, 0:10ms */
	mc33907_sendSecureCmdRw(cmdTx.R);
	if(TRUE == intUpdtStru.fssm1)
	{
		intUpdtStru.fssm1 = FALSE;
		if((intStruct.INIT_FSSM1.R & 0X0B) == ((cmd  >> 4) &0X0B))
		{
			setOk = TRUE;
		}
		else
		{
			setOk = FALSE;
		}
	}
	return setOk;
}

bool mc33907_setInitFssm2(uint8_t cmd)
{
	bool setOk = FALSE;
	INIT_FSSM2_Tx_32B_tag cmdTx;
	cmdTx.R = cmd;
	cmdTx.B.RW = 1;
	cmdTx.B.ADR = INIT_FSSM2_ADR;
	/*cmdTx.B.RSTb_err_FS = bit7 of CMD; Configure the values of the RSTb error counter,0:intermediate=3; final=6,1:intermediate=1; final=2*/
	/*cmdTx.B.IO_23_FS = bit6 of CMD; 1 safety critical,0 not safety*/
	/*cmdTx.B.PS = bit5 of CMD;Configure the Fccu polarity 1:Fccu_eaout_1:0 active LOW, 0:Fccu_eaout_1:0 active HIGH */
	mc33907_sendSecureCmdRw(cmdTx.R);
	if(TRUE == intUpdtStru.fssm2)
	{
		intUpdtStru.fssm2 = FALSE;
		if((intStruct.INIT_FSSM2.R & 0X0E) == ((cmd  >> 4) &0X0E))
		{
			setOk = TRUE;
		}
		else
		{
			setOk = FALSE;
		}
	}
	return setOk;
}



bool mc33907_setInitInt(uint32_t cmd)
{
	bool setOk = FALSE;
	INIT_INT_32B_tag cmdTx;
	cmdTx.R = cmd;
	cmdTx.B.RW = 1;
	cmdTx.B.ADR = INIT_INT_ADR;
	mc33907_sendCmdRw(cmdTx.R);
	if(TRUE == intUpdtStru.initInt)
	{
		intUpdtStru.initInt = FALSE;
		setOk = TRUE;
	}
	return setOk;
}



bool mc33907_releaseFSout(void)
{
	bool releaseOk = TRUE;
	FS_OUT_Tx_32B_tag cmd;
	cmd.R = 0;
	mc33907_getLfsr();
	if(TRUE == intUpdtStru.wdLfsr)
	{
		intUpdtStru.wdLfsr = FALSE;
		PITstruct.currentLFSR.R = intStruct.WD_LFSR.R;		/*store current LFSR in PIT structure*/
		cmd.B.FS_out_0 = PITstruct.currentLFSR.B.bit7;		/*swap current LFSR register content*/
		cmd.B.FS_out_1 = PITstruct.currentLFSR.B.bit6;
		cmd.B.FS_out_2 = PITstruct.currentLFSR.B.bit5;
		cmd.B.FS_out_3 = PITstruct.currentLFSR.B.bit4;
		cmd.B.FS_out_4 = PITstruct.currentLFSR.B.bit3;
		cmd.B.FS_out_5 = PITstruct.currentLFSR.B.bit2;
		cmd.B.FS_out_6 = PITstruct.currentLFSR.B.bit1;
		cmd.B.FS_out_7 = PITstruct.currentLFSR.B.bit0;
		cmd.R = ~ cmd.R;									/*negate already swapped bits*/
		cmd.B.RW = 1;										/*write command*/
		cmd.B.ADR = FS_OUT_ADR;								/*set address*/
		mc33907_sendCmdW(cmd.R);
		#if 0
		if(TRUE == intUpdtStru.fsOut)
		{
			intUpdtStru.fsOut = FALSE;
			if(0 == spiStruct.pwrStatus.B.SPI_G)
			{
				releaseOk = TRUE;
			}
		}
		#endif
	}
	return releaseOk;
}

bool mc33907_setWu1(uint32_t cmd)
{
	bool setOk = FALSE;
	INIT_IO_WU1_32B_tag cmdTx;
	cmdTx.R = cmd;
	cmdTx.B.RW = 1;
	cmdTx.B.ADR = INIT_IO_WU1_ADR;
	mc33907_sendCmdRw(cmdTx.R);
	if(TRUE == intUpdtStru.ioWu1)
	{
		intUpdtStru.ioWu1 = FALSE;
		if((intStruct.INIT_IO_WU1.R & 0xFF) == (cmd & 0xFF))
		{
			setOk = TRUE;
		}
		else
		{
			setOk = FALSE;
		}
	}
	return setOk;
}


bool mc33907_setWu2(uint32_t cmd)
{
	bool setOk = FALSE;
	INIT_IO_WU2_32B_tag cmdTx;
	cmdTx.R = cmd;
	cmdTx.B.RW = 1;
	cmdTx.B.ADR = INIT_IO_WU2_ADR;
	mc33907_sendCmdRw(cmdTx.R);
	if(TRUE == intUpdtStru.ioWu2)
	{
		intUpdtStru.ioWu2 = FALSE;
		if((intStruct.INIT_IO_WU2.R & 0xFF) == (cmd & 0xFF))
		{
			setOk = TRUE;
		}
		else
		{
			setOk = FALSE;
		}
	}
	return setOk;
}



void mc33907_reqInt(void)
{
	MODE_Tx_32B_tag cmd;
	cmd.R = 0;

	cmd.B.RW = 1;									//write command
	cmd.B.ADR = MODE_ADR;							//set address
	cmd.B.INT_request = 1;							//INTerruption request
	mc33907_sendSecureCmdRw(cmd.R);
}



/***************************************************************************//*!
*   @brief The function PwSBC_RequestReset requests a low pulse on the RSTb - MCU reset.
*	@par Include
*					PwSBC.h
* 	@par Description
*					This function requests one Reset pulse.
*	@remarks 	Reset can cause a data loss thanks to the restart of the MCU.
*	@par Code sample
*			PwSBC_RequestReset();
*			- Command requests one Reset pulse.
********************************************************************************/
void mc33907_reqReset(void){
	RSTb_request_Tx_32B_tag cmd;
	cmd.R = 0;

	cmd.B.RW = 1;										//write command
	cmd.B.ADR = RSTB_REQUEST_ADR;						//set address
	cmd.B.RSTb_request = 1;

	mc33907_sendSecureCmdW(cmd.R);
}

void mc33907_getcanlinmode(void)
{
	mc33907_sendCmdR(CAN_LIN_MODE_ADR << 9);
}

void mc33907_getWUSoc(void)
{
	mc33907_sendCmdR(WU_SOURCE_ADR << 9);
}

void mc33907_getDiagSpi(void)
{
	mc33907_sendCmdR(DIAG_SPI_ADR << 9);
}

void mc33907_getDiagVreg1(void)
{
	mc33907_sendCmdR(DIAG_VREG1_ADR << 9);
}

void mc33907_getDiagVreg2(void)
{
	mc33907_sendCmdR(DIAG_VREG2_ADR << 9);
}

void mc33907_getDiagVreg3(void)
{
	mc33907_sendCmdR(DIAG_VREG3_ADR << 9);
}

void mc33907_getDiagCan(void)
{
	mc33907_sendCmdR(DIAG_CAN_ADR << 9);
}

void mc33907_getDiagCanLin(void)
{
	mc33907_sendCmdR(DIAG_CAN_LIN_ADR << 9);
}


void mc33907_getDiagFs1(void)
{
	mc33907_sendCmdR(DIAG_FS1_ADR << 9);
}

void mc33907_getwdCnt(void)
{
	mc33907_sendCmdR(WD_COUNTER_ADR << 9);
}

void mc33907_getDiagFs2(void)
{
	mc33907_sendCmdR(DIAG_FS2_ADR << 9);
}

void mc33907_getIoInput(void)
{
	mc33907_sendCmdR(IO_INPUT_STATE_ADR << 9);
}
void mc33907_getInitSupervisor1(void)
{
	mc33907_sendCmdR(INIT_SUPERVISOR1_ADR << 9);
}

void mc33907_getInitSupervisor2(void)
{
	mc33907_sendCmdR(INIT_SUPERVISOR2_ADR << 9);
}

void mc33907_getInitSupervisor3(void)
{
	mc33907_sendCmdR(INIT_SUPERVISOR3_ADR << 9);
}

void mc33907_getLfsr(void)
{
	mc33907_sendCmdR(WD_LFSR_ADR << 9);
}

void mc33907_getMode(void)
{
	mc33907_sendCmdR(MODE_ADR << 9);
}

void mc33907_getHwCfg(void)
{
	mc33907_sendCmdR(HW_CONFIG_ADR << 9);
}

/*normally this fault will be detected in Isr function*/
void mc33907_spiFaultDiag(void)
{
	uint32_t eventMask = 0;
	mc33907_getDiagSpi();
	diagSPI = 0xFFFFFFFF;
	if(TRUE == intUpdtStru.diagSpi)
	{
		intUpdtStru.diagSpi = FALSE;
		diagSPI = intStruct.Diag_SPI.R;
		if(0 < intStruct.Diag_SPI.B.SPI_err)
		{
			eventMask |= m_33907_spiSecErrMask;
		}
		if(0 < intStruct.Diag_SPI.B.SPI_CLK)
		{
			eventMask |= m_33907_spiClkErrMask;
		}
		if(0 < intStruct.Diag_SPI.B.SPI_Req)
		{
			eventMask |= m_33907_spiReqErr;
		}
		if(0 < intStruct.Diag_SPI.B.SPI_Parity)
		{
			eventMask |= m_33907_spiPErr;
		}
		if(0 != eventMask)
		{
			mc33907_spiFaultHandle(eventMask);
		}
	}
}

void mc33907_diagVreg1(void)
{
	uint32_t errMask = 0;
	mc33907_getDiagVreg1();
	diagVreg1 = 0xFFFFFFFF;
	if(TRUE == intUpdtStru.diagVreg1)
	{
		intUpdtStru.diagVreg1 = FALSE;
		diagVreg1 = intStruct.Diag_Vreg1.R;
		if(0 < intStruct.Diag_Vreg1.B.Vsns_uv)
		{
			/*detection of Vbattery < Vsns_uv, Vsns_uv is 8.5V*/
			errMask |= m_33907_vbatUVErrMask;
		}
		if(0 < intStruct.Diag_Vreg1.B.Vsup_uv_7)
		{
			/*detection of Vsup < Vsup_uv_7*/
			errMask |= m_33907_vsupUVErrMask;
		}
		if(0 < intStruct.Diag_Vreg1.B.Tsd_pre)
		{
			/*detection of Vpre shutdown due to thermal*/
			errMask |= m_33907_vpreTherShutDownErrMask;
		}
		if(0 < intStruct.Diag_Vreg1.B.Vpre_OV)
		{
			/*detection of Vpre over voltage, VPRE_ov varies from 7.2v to 8v*/
			errMask |= m_33907_vpreOVErrMask;
		}
		if(0 < intStruct.Diag_Vreg1.B.Vpre_UV)
		{
			/*detection of Vpre under voltage, VPRE_uv varies from 5.5v to 6v*/
			errMask |= m_33907_vpreUVErrMask;
		}

		if(0 < intStruct.Diag_Vreg1.B.Tsd_core)
		{
			/*detection of Vcore shutdown due to thermal*/
			errMask |= m_33907_vcoreTherShutDownErrMask;
		}
		if(0 < intStruct.Diag_Vreg1.B.Vcore_OV)
		{
			/*detection of Vcore over vaoltage*/
			errMask |= m_33907_vcoreOVErrMask;
		}
		if(0 < intStruct.Diag_Vreg1.B.Vcore_UV)
		{
			/*detection of Vcore under vaoltage*/
			errMask |= m_33907_vcoreUVErrMask;
		}
		if(0 !=  errMask)
		{
			mc33907_vreg1FaultHandle(errMask);
		}


	}
}

void mc33907_diagVreg2(void)
{
	uint32_t errMask = 0;
	mc33907_getDiagVreg2();
	diagVreg2 = 0xFFFFFFFF;
	if(TRUE == intUpdtStru.diagVreg2)
	{
		intUpdtStru.diagVreg2 = FALSE;
		diagVreg2 = intStruct.Diag_Vreg2.R;
		if(0 < intStruct.Diag_Vreg2.B.Tsd_can)
		{
			/*detection of Vcan shutdown due to thermal*/
			errMask |= m_33907_vcanTherSdErrMask;
		}
		if(0 < intStruct.Diag_Vreg2.B.Vcan_ov)
		{
			/*detection of Vsup < Vsup_uv_7*/
			errMask |= m_33907_vcanOVErrMask;   
		}
		if(0 < intStruct.Diag_Vreg2.B.Vcan_uv)
		{
			/*detection of Vpre shutdown due to thermal*/
			errMask |= m_33907_vcanUVErrMask;
		}
		if(0 < intStruct.Diag_Vreg2.B.Tsd_aux)
		{
			/*detection of Vpre over voltage, VPRE_ov varies from 7.2v to 8v*/
			errMask |= m_33907_vauxTherSdErrMask;
		}
		if(0 < intStruct.Diag_Vreg2.B.Ilim_aux_off)
		{
			/*detection of Vpre under voltage, VPRE_uv varies from 5.5v to 6v*/
			errMask |= m_33907_vauxCurrLimitErrMask;
		}

		if(0 < intStruct.Diag_Vreg2.B.Vaux_OV)
		{
			/*detection of Vcore shutdown due to thermal*/
			errMask |= m_33907_vauxOVErrMask;
		}
		if(0 < intStruct.Diag_Vreg2.B.Vaux_UV)
		{
			/*detection of Vcore over vaoltage*/
			errMask |= m_33907_vauxUVErrMask;
		}
		if(0 !=  errMask)
		{
			mc33907_vreg2FaultHandle(errMask);
		}
	}

}


void mc33907_diagVreg3(void)
{
	uint32_t errMask = 0;
	mc33907_getDiagVreg3();
	diagVreg3 = 0xFFFFFFFF;
	if(TRUE == intUpdtStru.diagVreg3)
	{
		intUpdtStru.diagVreg3 = FALSE;
		diagVreg3 = intStruct.Diag_Vreg3.R;
		if(0 < intStruct.Diag_Vreg3.B.Tsd_cca)
		{
			/*detection of Vcan shutdown due to thermal*/
			errMask |= m_33907_vccaTherSdErrMask;
		}
		if(0 < intStruct.Diag_Vreg3.B.Ilim_cca_off)
		{
			/*detection of Vsup < Vsup_uv_7*/
			errMask |= m_33907_vccaCurrLimitErrMask;
		}
		if(0 < intStruct.Diag_Vreg3.B.Vcca_OV)
		{
			/*detection of Vpre shutdown due to thermal*/
			errMask |= m_33907_vccaOVErrMask;
		}
		if(0 < intStruct.Diag_Vreg3.B.Vcca_UV)
		{
			/*detection of Vpre over voltage, VPRE_ov varies from 7.2v to 8v*/
			errMask |= m_33907_vccaUVErrMask;
		}
		if(0 !=  errMask)
		{
			mc33907_vreg3FaultHandle(errMask);
		}
	}

}

void mc33907_diagCan1(void)
{
	uint32_t errMask = 0;
	mc33907_getDiagCan();
	diagCAN1 = 0xFFFFFFFF;
	if(TRUE == intUpdtStru.diagCan)
	{
		intUpdtStru.diagCan = FALSE;
		diagCAN1 = intStruct.Diag_CAN.R;
		if(0 < intStruct.Diag_CAN.B.CANH_batt)
		{
			/*detection of Vcan shutdown due to thermal*/
			errMask |= m_33907_canH2Bat;
		}
		if(0 < intStruct.Diag_CAN.B.CANH_gnd)
		{
			/*detection of Vsup < Vsup_uv_7*/
			errMask |= m_33907_canH2Gnd;
		}
		if(0 < intStruct.Diag_CAN.B.CANL_batt)
		{
			/*detection of Vpre shutdown due to thermal*/
			errMask |= m_33907_canL2Bat;
		}
		if(0 < intStruct.Diag_CAN.B.CANL_gnd)
		{
			/*detection of Vpre shutdown due to thermal*/
			errMask |= m_33907_canL2Gnd;
		}
		if(0 < intStruct.Diag_CAN.B.CAN_dominant)
		{
			/*detection of Vpre shutdown due to thermal*/
			errMask |= m_33907_canDominant;
		}
		if(0 < intStruct.Diag_CAN.B.RXD_recessive)
		{
			/*detection of Vpre shutdown due to thermal*/
			errMask |= m_33907_canRxRecessive;
		}
		if(0 < intStruct.Diag_CAN.B.TXD_dominant)
		{
			/*detection of Vpre shutdown due to thermal*/
			errMask |= m_33907_canTxDominant;
		}
		if(0 != errMask)
		{
			mc33907_can1FaultHandle(errMask);
		}
	}
}


void mc33907_diagCan2(void)
{
	uint32_t errMask = 0;
	mc33907_releaseFSout();
	mc33907_getDiagCanLin();
	diagCANLin = 0xFFFFFFFF;
	if(TRUE == intUpdtStru.diagCanLin)
	{
		intUpdtStru.diagCanLin = FALSE;
		diagCANLin = intStruct.Diag_CAN_LIN.R;
		if(0 < intStruct.Diag_CAN_LIN.B.CAN_OT)
		{
			/*detection of Vcan shutdown due to thermal*/
			errMask |= m_33907_canOV;
		}
		if(0 < intStruct.Diag_CAN_LIN.B.CAN_OC)
		{
			/*detection of Vsup < Vsup_uv_7*/
			errMask |= m_33907_canOC;
		}
		if(0 != errMask)
		{
			mc33907_can2FaultHandle(errMask);
		}
	}
}

void mc33907_dispose(void)
{
#if 0
	mc33907_sendCmdR(STATUS_VREG1_ADR << 9);
	StatusVreg1 = 0xFFFFFFFF;
	if(TRUE == intUpdtStru.vreg1St)
	{
		intUpdtStru.vreg1St = FALSE;
		StatusVreg1 = intStruct.Status_Vreg1.R;

	}

	mc33907_sendCmdR(STATUS_VREG2_ADR << 9);
	StatusVreg2 = 0xFFFFFFFF;
	if(TRUE == intUpdtStru.vreg2St)
	{
		intUpdtStru.vreg2St = FALSE;
		StatusVreg2 = intStruct.Status_Vreg2.R;

	}



	mc33907_getDiagFs1();
	diagFS1 = 0xFFFFFFFF;
	if(TRUE == intUpdtStru.diagFs1)
	{
		intUpdtStru.diagFs1 = FALSE;
		diagFS1 = intStruct.Diag_FS1.R;

	}

	mc33907_getDiagFs2();
	diagFS2 = 0xFFFFFFFF;
	if(TRUE == intUpdtStru.diagFs2)
	{
		intUpdtStru.diagFs2 = FALSE;
		diagFS2 = intStruct.Diag_FS2.R;
	}
#endif



}

void mc33907_get_io_state(void)
{
	mc33907_getIoInput();
	if(TRUE == intUpdtStru.ioInputSt)
	{
		intUpdtStru.ioInputSt = FALSE;
		hld_mc33907_di4 = intStruct.IO_input_state.B.IO_4;
		hld_mc33907_di5 = intStruct.IO_input_state.B.IO_5;
	}
}
/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static void mc33907_spiFaultHandle(uint32_t eventMask)
{
	if(0 != (eventMask & m_33907_spiSecErrMask))
	{
		diag_logSf(m_diagSf_33907SpiSecErr);
	}
	if(0 != (eventMask & m_33907_spiClkErrMask))
	{
		diag_logRf(m_diagRf_33907SpiClkErr);
	}
	if(0 != (eventMask & m_33907_spiReqErr))
	{
		diag_logSf(m_diagSf_33907SpiReqErr);
	}
	if(0 != (eventMask & m_33907_spiPErr))
	{
		diag_logSf(m_33907_spiPErr);
	}
}

static void mc33907_vreg1FaultHandle(uint32_t errMask)
{
	if(0 != (errMask & m_33907_vbatUVErrMask))
	{
		diag_logRf(m_diagRf_33907VbatUV);
	}
	if(0 != (errMask & m_33907_vsupUVErrMask))
	{
		diag_logRf(m_diagRf_33907VsupUv7);
	}
	if(0 != (errMask & m_33907_vpreTherShutDownErrMask))
	{
		diag_logRf(m_diagRf_33907VpreSdTher);
	}
	if(0 != (errMask & m_33907_vpreOVErrMask))
	{
		diag_logNrf(m_diagNrf_33907VpreOV);
	}
	if(0 != (errMask & m_33907_vpreUVErrMask))
	{
		diag_logRf(m_diagRf_33907VpreUV);
	}
	if(0 != (errMask & m_33907_vcoreTherShutDownErrMask))
	{
		diag_logNrf(m_diagNrf_33907VcoreSdTher);
	}
	if(0 != (errMask & m_33907_vcoreOVErrMask))
	{
		diag_logNrf(m_diagNrf_33907VcoreOv);
	}
	if(0 != (errMask & m_33907_vcoreUVErrMask))
	{
		diag_logRf(m_diagRf_33907VcoreUv);
	}
}


static void mc33907_vreg2FaultHandle(uint32_t errMask)
{
	if(0 != (errMask & m_33907_vcanTherSdErrMask))
	{
		diag_logNrf(m_diagNrf_33907VcanSdTher);
	}
	if(0 != (errMask & m_33907_vcanOVErrMask))
	{
		diag_logNrf(m_diagNrf_33907VcanOV);
	}
	if(0 != (errMask & m_33907_vcanUVErrMask))
	{
		diag_logRf(m_diagRf_33907VcanUV);
	}
	if(0 != (errMask & m_33907_vauxTherSdErrMask))
	{
		diag_logNrf(m_diagNrf_33907VauxSdTher);
	}
	if(0 != (errMask & m_33907_vauxCurrLimitErrMask))
	{
		diag_logRf(m_diagRf_33907VauxCurrDuration);
	}
	if(0 != (errMask & m_33907_vauxOVErrMask))
	{
		diag_logNrf(m_diagNrf_33907VauxOV);
	}
	if(0 != (errMask & m_33907_vauxUVErrMask))
	{
		diag_logRf(m_diagRf_33907VauxUV);
	}
}

static void mc33907_vreg3FaultHandle(uint32_t errMask)
{
	if(0 != (errMask & m_33907_vccaTherSdErrMask))
	{
		diag_logNrf(m_diagNrf_33907VccaSdTher);
	}
	if(0 != (errMask & m_33907_vccaCurrLimitErrMask))
	{
		diag_logRf(m_diagRf_33907VccaCurrDuration);
	}
	if(0 != (errMask & m_33907_vccaOVErrMask))
	{
		diag_logNrf(m_diagNrf_33907VccaOV);
	}
	if(0 != (errMask & m_33907_vccaUVErrMask))
	{
		diag_logRf(m_diagRf_33907VccaUV);
	}
}


static void mc33907_can1FaultHandle(uint32_t errMask)
{
	if(0 != (errMask & m_33907_canH2Bat))
	{
		diag_logRf(m_diagRf_33907CanH2Bat);
	}
	if(0 != (errMask & m_33907_canH2Gnd))
	{
		diag_logRf(m_diagRf_33907CanH2Gnd);
	}
	if(0 != (errMask & m_33907_canL2Bat))
	{
		diag_logRf(m_diagRf_33907CanL2Bat);
	}
	if(0 != (errMask & m_33907_canL2Gnd))
	{
		diag_logRf(m_diagRf_33907CanL2Gnd);
	}
	if(0 != (errMask & m_33907_canDominant))
	{
		diag_logRf(m_diagRf_33907CanDominant);
	}
	if(0 != (errMask & m_33907_canRxRecessive))
	{
		diag_logRf(m_diagRf_33907CanRxRecessive);
	}
	if(0 != (errMask & m_33907_canTxDominant))
	{
		diag_logRf(m_diagRf_33907CanTxDominant);
	}
}



static void mc33907_can2FaultHandle(uint32_t errMask)
{
	if(0 != (errMask & m_33907_canOC))
	{
		diag_logNrf(m_diagNrf_33907CanOC);
	}
}



static void mc33907_sendSecureCmdRw(uint32_t cmd)
{
	uint8_t comRslt = m_spi_sendNoErr;
	uint16_t waitRspCnt = 0;
	bool faultExit = FALSE;
	spiStruct.writeCmd = cmd | 0x8000;
	spiStruct.readCmd = cmd & 0x7E00;
	spiStruct.regAddr = (cmd >> 9) & 0x3F;
	mc33907_computeSecurityBits(&spiStruct.writeCmd);
	mc33907_computeParity(&spiStruct.writeCmd);

	comRslt = spi_sendDt(m_33907_spiModule,m_33907_spiCtas,m_33907_spiCsMask,(uint16_t)(spiStruct.writeCmd & 0xFFFF),FALSE,TRUE);

	if(m_spi_sendNoErr == comRslt)
	{
		while(waitRspCnt < m_33907_waitDtTick)
		{
			comRslt = spi_readDt(m_33907_spiModule,&spiStruct.response);
			if(m_spi_readNoErr == comRslt)
			{
				break;
			}
			waitRspCnt ++;
		}
		if(waitRspCnt < m_33907_waitDtTick)
		{
			waitRspCnt = 0;

			comRslt = spi_sendDt(m_33907_spiModule,m_33907_spiCtas,m_33907_spiCsMask,(uint16_t)(spiStruct.readCmd & 0xFFFF),FALSE,TRUE);

			if(m_spi_sendNoErr == comRslt)
			{
				while(waitRspCnt < m_33907_waitDtTick)
				{
					comRslt = spi_readDt(m_33907_spiModule,&spiStruct.response);
					if(m_spi_readNoErr == comRslt)
					{
						break;
					}
					waitRspCnt ++;
				}
				if(waitRspCnt < m_33907_waitDtTick)
				{
					waitRspCnt = 0;
				}
				else
				{
					faultExit = TRUE;
				}
			}
			else
			{
				faultExit = TRUE;
			}

			if(FALSE == faultExit)
			{
#if TRUE == m_33907_readDoubleCheck

				comRslt = spi_sendDt(m_33907_spiModule,m_33907_spiCtas,m_33907_spiCsMask,(uint16_t)(spiStruct.readCmd & 0xFFFF),FALSE,TRUE);

				if(m_spi_sendNoErr == comRslt)
				{
					while(waitRspCnt < m_33907_waitDtTick)
					{
						comRslt = spi_readDt(m_33907_spiModule,&spiStruct.response);
						if(m_spi_readNoErr == comRslt)
						{
							break;
						}
						waitRspCnt ++;
					}
					if(waitRspCnt < m_33907_waitDtTick)
					{
						waitRspCnt = 0;
						spiStruct.pwrStatus.R = (spiStruct.response >> 8);
						if(0 == spiStruct.pwrStatus.B.SPI_G)
						{
							/*no error*/
							mc33907_processRecMsg(spiStruct.response);
						}
						else if(0xFFFF == spiStruct.response)
						{
							/*no data receive at all*/
						}
						else
						{
							/*spi error occured, the handle funtion will be done by mc33907_spiFaultHandle*/
						}
					}
				}
#else
				spiStruct.pwrStatus.R = (spiStruct.response >> 8);
				if(0 == spiStruct.pwrStatus.B.SPI_G)
				{
					/*no error*/
					mc33907_processRecMsg(spiStruct.response);
				}
				else if(0xFFFF == spiStruct.response)
				{
					/*no data receive at all*/
				}
				else
				{
					/*spi error occured, the handle funtion will be done by mc33907_spiFaultHandle*/
				}
#endif
			}
		}
		else
		{
			mc33907_errCode = m_33907_waitRspTo;
		}
	}

}

static void mc33907_sendSecureCmdW(uint32_t cmd)
{
	uint8_t comRslt = m_spi_sendNoErr;
	uint16_t waitRspCnt = 0;
	bool faultExit = FALSE;
	spiStruct.readCmd = 0;
	spiStruct.writeCmd = cmd;
	spiStruct.regAddr = (cmd >> 9) & 0x3F;
	mc33907_computeSecurityBits(&spiStruct.writeCmd);
	mc33907_computeParity(&spiStruct.writeCmd);

	comRslt = spi_sendDt(m_33907_spiModule,m_33907_spiCtas,m_33907_spiCsMask,(uint16_t)(spiStruct.writeCmd & 0xFFFF),FALSE,TRUE);

	if(m_spi_sendNoErr == comRslt)
	{
		while(waitRspCnt < m_33907_waitDtTick)
		{
			comRslt = spi_readDt(m_33907_spiModule,&spiStruct.response);
			if(m_spi_readNoErr == comRslt)
			{
				break;
			}
			waitRspCnt ++;
		}
		if(waitRspCnt < m_33907_waitDtTick)
		{
			waitRspCnt = 0;
		}
		else
		{
			faultExit = TRUE;
		}
	}
	else
	{
		faultExit = TRUE;
	}
	if(FALSE == faultExit)
	{
#if TRUE == m_33907_readDoubleCheck

		comRslt = spi_sendDt(m_33907_spiModule,m_33907_spiCtas,m_33907_spiCsMask,(uint16_t)(spiStruct.writeCmd & 0xFFFF),FALSE,TRUE);

		if(m_spi_sendNoErr == comRslt)
		{
			while(waitRspCnt < m_33907_waitDtTick)
			{
				comRslt = spi_readDt(m_33907_spiModule,&spiStruct.response);
				if(m_spi_readNoErr == comRslt)
				{
					break;
				}
				waitRspCnt ++;
			}
			if(waitRspCnt < m_33907_waitDtTick)
			{
				waitRspCnt = 0;
				spiStruct.pwrStatus.R = (spiStruct.response >> 8);
				if(0 == spiStruct.pwrStatus.B.SPI_G)
				{
					/*no error*/
					mc33907_processRecMsg(spiStruct.response);
				}
				else if(0xFFFF == spiStruct.response)
				{
					/*no data receive at all*/
				}
				else
				{
					/*spi error occured, the handle funtion will be done by mc33907_spiFaultHandle*/
				}
			}
			else
			{
				mc33907_errCode = m_33907_waitRspTo;
			}
		}
#else
		spiStruct.pwrStatus.R = (spiStruct.response >> 8);
		if(0 == spiStruct.pwrStatus.B.SPI_G)
		{
			/*no error*/
			mc33907_processRecMsg(spiStruct.response);
		}
		else if(0xFFFF == spiStruct.response)
		{
			/*no data receive at all*/
		}
		else
		{
			/*spi error occured, the handle funtion will be done by mc33907_spiFaultHandle*/
		}
#endif
	}

}

static void mc33907_sendCmdR(uint32_t cmd)
{
	uint8_t comRslt = m_spi_sendNoErr;
	uint16_t waitRspCnt = 0;
	bool faultExit = FALSE;
	spiStruct.writeCmd = 0;
	spiStruct.readCmd = cmd;
	spiStruct.regAddr = (cmd >> 9) & 0x3F;

	comRslt = spi_sendDt(m_33907_spiModule,m_33907_spiCtas,m_33907_spiCsMask,(uint16_t)(spiStruct.readCmd & 0xFFFF),FALSE,TRUE);

	if(m_spi_sendNoErr == comRslt)
	{
		while(waitRspCnt < m_33907_waitDtTick)
		{
			comRslt = spi_readDt(m_33907_spiModule,&spiStruct.response);
			if(m_spi_readNoErr == comRslt)
			{
				break;
			}
			waitRspCnt ++;
		}
		if(waitRspCnt < m_33907_waitDtTick)
		{
			waitRspCnt = 0;

		}
		else
		{
			faultExit = TRUE;
		}
	}
	else
	{
		faultExit = TRUE;
	}
	if(FALSE == faultExit)
	{
#if TRUE == m_33907_readDoubleCheck

		comRslt = spi_sendDt(m_33907_spiModule,m_33907_spiCtas,m_33907_spiCsMask,(uint16_t)(spiStruct.readCmd & 0xFFFF),FALSE,TRUE);

		if(m_spi_sendNoErr == comRslt)
		{
			while(waitRspCnt < m_33907_waitDtTick)
			{
				comRslt = spi_readDt(m_33907_spiModule,&spiStruct.response);
				if(m_spi_readNoErr == comRslt)
				{
					break;
				}
				waitRspCnt ++;
			}
			if(waitRspCnt < m_33907_waitDtTick)
			{
				waitRspCnt = 0;
				spiStruct.pwrStatus.R = (spiStruct.response >> 8);
				if(0 == spiStruct.pwrStatus.B.SPI_G)
				{
					/*no error*/
					mc33907_processRecMsg(spiStruct.response);
				}
				else if(0xFFFF == spiStruct.response)
				{
					/*no data receive at all*/
				}
				else
				{
					/*spi error occured, the handle funtion will be done by mc33907_spiFaultHandle*/
				}
			}
			else
			{
				mc33907_errCode = m_33907_spiErr;
			}
		}
#else
		spiStruct.pwrStatus.R = (spiStruct.response >> 8);
		if(0 == spiStruct.pwrStatus.B.SPI_G)
		{
			/*no error*/
			mc33907_processRecMsg(spiStruct.response);
		}
		else if(0xFFFF == spiStruct.response)
		{
			/*no data receive at all*/
		}
		else
		{
			/*spi error occured, the handle funtion will be done by mc33907_spiFaultHandle*/
		}
#endif
	}

}

static void mc33907_sendCmdW(uint32_t cmd)
{
	uint8_t comRslt = m_spi_sendNoErr;
	uint16_t waitRspCnt = 0;
	bool faultExit = FALSE;
	spiStruct.readCmd = 0;
	spiStruct.writeCmd = cmd;
	spiStruct.regAddr = (cmd >> 9) & 0x3F;
	mc33907_computeParity(&spiStruct.writeCmd);

	comRslt = spi_sendDt(m_33907_spiModule,m_33907_spiCtas,m_33907_spiCsMask,(uint16_t)(spiStruct.writeCmd & 0xFFFF),FALSE,TRUE);

	if(m_spi_sendNoErr == comRslt)
	{
		while(waitRspCnt < m_33907_waitDtTick)
		{
			comRslt = spi_readDt(m_33907_spiModule,&spiStruct.response);
			if(m_spi_readNoErr == comRslt)
			{
				break;
			}
			waitRspCnt ++;
		}
		if(waitRspCnt < m_33907_waitDtTick)
		{
			waitRspCnt = 0;
		}
		else
		{
			faultExit = TRUE;
		}

	}
	else
	{
		faultExit = TRUE;
	}
	if(FALSE ==  faultExit)
	{
#if TRUE == m_33907_readDoubleCheck

		comRslt = spi_sendDt(m_33907_spiModule,m_33907_spiCtas,m_33907_spiCsMask,(uint16_t)(spiStruct.writeCmd & 0xFFFF),FALSE,TRUE);

		if(m_spi_sendNoErr == comRslt)
		{
			while(waitRspCnt < m_33907_waitDtTick)
			{
				comRslt = spi_readDt(m_33907_spiModule,&spiStruct.response);
				if(m_spi_readNoErr == comRslt)
				{
					break;
				}
				waitRspCnt ++;
			}
			if(waitRspCnt < m_33907_waitDtTick)
			{
				waitRspCnt = 0;
				spiStruct.pwrStatus.R = (spiStruct.response >> 8);
				if(0 == spiStruct.pwrStatus.B.SPI_G)
				{
					/*no error*/
					mc33907_processRecMsg(spiStruct.response);
				}
				else if(0xFFFF == spiStruct.response)
				{
					/*no data receive at all*/
				}
				else
				{
					/*spi error occured, the handle funtion will be done by mc33907_spiFaultHandle*/
				}
			}
			else
			{
				mc33907_errCode = m_33907_waitRspTo;
			}
		}
#else
		spiStruct.pwrStatus.R = (spiStruct.response >> 8);
		if(0 == spiStruct.pwrStatus.B.SPI_G)
		{
			/*no error*/
			mc33907_processRecMsg(spiStruct.response);
		}
		else if(0xFFFF == spiStruct.response)
		{
			/*no data receive at all*/
		}
		else
		{
			/*spi error occured, the handle funtion will be done by mc33907_spiFaultHandle*/
		}
#endif

	}
}

static void mc33907_sendCmdRw(uint32_t cmd)
{
	uint8_t comRslt = m_spi_sendNoErr;
	uint16_t waitRspCnt = 0;
	bool faultExit = FALSE;
	spiStruct.writeCmd = cmd | 0x8000;
	spiStruct.regAddr = (cmd >> 9) & 0x3F;
	spiStruct.readCmd = cmd & 0x7E00;
	mc33907_computeParity(&spiStruct.writeCmd);

	comRslt = spi_sendDt(m_33907_spiModule,m_33907_spiCtas,m_33907_spiCsMask,(uint16_t)(spiStruct.writeCmd & 0xFFFF),FALSE,TRUE);

	if(m_spi_sendNoErr == comRslt)
	{
		while(waitRspCnt < m_33907_waitDtTick)
		{
			comRslt = spi_readDt(m_33907_spiModule,&spiStruct.response);
			if(m_spi_readNoErr == comRslt)
			{
				break;
			}
			waitRspCnt ++;
		}
		if(waitRspCnt < m_33907_waitDtTick)
		{
			waitRspCnt = 0;

			comRslt = spi_sendDt(m_33907_spiModule,m_33907_spiCtas,m_33907_spiCsMask,(uint16_t)(spiStruct.readCmd & 0xFFFF),FALSE,TRUE);

			if(m_spi_sendNoErr == comRslt)
			{
				while(waitRspCnt < m_33907_waitDtTick)
				{
					comRslt = spi_readDt(m_33907_spiModule,&spiStruct.response);
					if(m_spi_readNoErr == comRslt)
					{
						break;
					}
					waitRspCnt ++;
				}
				if(waitRspCnt < m_33907_waitDtTick)
				{
					waitRspCnt = 0;
				}
				else
				{
					faultExit = TRUE;
				}
			}
			else
			{
				faultExit = TRUE;
			}
			if(FALSE == faultExit)
			{
#if TRUE == m_33907_readDoubleCheck

				comRslt = spi_sendDt(m_33907_spiModule,m_33907_spiCtas,m_33907_spiCsMask,(uint16_t)(spiStruct.readCmd & 0xFFFF),FALSE,TRUE);

				if(m_spi_sendNoErr == comRslt)
				{
					while(waitRspCnt < m_33907_waitDtTick)
					{
						comRslt = spi_readDt(m_33907_spiModule,&spiStruct.response);
						if(m_spi_readNoErr == comRslt)
						{
							break;
						}
						waitRspCnt ++;
					}
					if(waitRspCnt < m_33907_waitDtTick)
					{
						waitRspCnt = 0;
						spiStruct.pwrStatus.R = (spiStruct.response >> 8);
						if(0 == spiStruct.pwrStatus.B.SPI_G)
						{
							/*no error*/
							mc33907_processRecMsg(spiStruct.response);
						}
						else if(0xFFFF == spiStruct.response)
						{
							/*no data receive at all*/
						}
						else
						{
							/*spi error occured, the handle funtion will be done by mc33907_spiFaultHandle*/
						}
					}
					else
					{
						mc33907_errCode = m_33907_waitRspTo;
					}
				}
#else
				spiStruct.pwrStatus.R = (spiStruct.response >> 8);
				if(0 == spiStruct.pwrStatus.B.SPI_G)
				{
					/*no error*/
					mc33907_processRecMsg(spiStruct.response);
				}
				else if(0xFFFF == spiStruct.response)
				{
					/*no data receive at all*/
				}
				else
				{
					/*spi error occured, the handle funtion will be done by mc33907_spiFaultHandle*/
				}
#endif
			}
		}
		else
		{
			mc33907_errCode = m_33907_waitRspTo;
		}
	}
}





static void mc33907_computeParity(uint32_t * cmd)
{
	uint8_t bitIndx = 0;
	uint8_t sum = 0;
	for(bitIndx = 0; bitIndx < 16; bitIndx ++)
	{
		if(8 != bitIndx)
		{
			sum += ((*cmd >> bitIndx) & 0x0001);
		}
	}
	if(0 == (sum % 2))
	{
		*cmd |= 0x00000100;
	}
	else
	{
		*cmd &= 0xFFFFFEFF;
	}
}


static void mc33907_computeSecurityBits(uint32_t *cmd)
{
	register32_struct command;
	command.R = *cmd;
	command.B.bit0 = command.B.bit6;
	command.B.bit1 = command.B.bit7;
	command.B.bit2 = ~command.B.bit4;
	command.B.bit3 = ~command.B.bit5;
	*cmd = command.R;
}

static void mc33907_processRecMsg(uint32_t resp)
{
	uint32_t regAddr = 0;
	spiStruct.response = resp;
	spiStruct.pwrStatus.R = (spiStruct.response >> 8);
	regAddr = spiStruct.regAddr;
	switch(regAddr)
	{
		case INIT_VREG1_ADR: 		intStruct.INIT_Vreg1.R = spiStruct.response;		intUpdtStru.Vreg1 = TRUE;		break;
		case INIT_VREG2_ADR: 		intStruct.INIT_Vreg2.R = spiStruct.response;		intUpdtStru.Vreg2 = TRUE; 		break;
		case INIT_CAN_LIN_ADR:		intStruct.INIT_CAN_LIN.R = spiStruct.response;		intUpdtStru.canLin = TRUE; 		break;
		case INIT_IO_WU1_ADR:		intStruct.INIT_IO_WU1.R = spiStruct.response;		intUpdtStru.ioWu1 = TRUE; 		break;
		case INIT_IO_WU2_ADR:		intStruct.INIT_IO_WU2.R = spiStruct.response;		intUpdtStru.ioWu2 = TRUE; 		break;
		case INIT_INT_ADR:			intStruct.INIT_INT.R = spiStruct.response;			intUpdtStru.initInt = TRUE;		break;
		case HW_CONFIG_ADR:			intStruct.HW_Config.R = spiStruct.response;			intUpdtStru.hwCfg = TRUE;		break;
		case WU_SOURCE_ADR:			intStruct.WU_source.R = spiStruct.response;			intUpdtStru.wuSoc = TRUE;		break;
		case IO_INPUT_STATE_ADR:	intStruct.IO_input_state.R = spiStruct.response;	intUpdtStru.ioInputSt = TRUE;	break;
		case STATUS_VREG1_ADR:		intStruct.Status_Vreg1.R = spiStruct.response;		intUpdtStru.vreg1St = TRUE;		break;
		case STATUS_VREG2_ADR:		intStruct.Status_Vreg2.R = spiStruct.response;		intUpdtStru.vreg2St = TRUE;		break;
		case DIAG_VREG1_ADR:		intStruct.Diag_Vreg1.R = spiStruct.response;		intUpdtStru.diagVreg1 = TRUE;	break;
		case DIAG_VREG2_ADR:		intStruct.Diag_Vreg2.R = spiStruct.response;		intUpdtStru.diagVreg2 = TRUE;	break;
		case DIAG_VREG3_ADR:		intStruct.Diag_Vreg3.R = spiStruct.response;		intUpdtStru.diagVreg3 = TRUE;	break;
		case DIAG_CAN_ADR:			intStruct.Diag_CAN.R = spiStruct.response;			intUpdtStru.diagCan = TRUE;		break;
		case DIAG_CAN_LIN_ADR:		intStruct.Diag_CAN_LIN.R = spiStruct.response;		intUpdtStru.diagCanLin = TRUE;	break;
		case DIAG_SPI_ADR:			intStruct.Diag_SPI.R = spiStruct.response;			intUpdtStru.diagSpi = TRUE;		break;
		case MODE_ADR:				intStruct.MODE.R = spiStruct.response;				intUpdtStru.mode = TRUE;		break;
		case VREG_MODE_ADR:			intStruct.Vreg_mode.R = spiStruct.response;			intUpdtStru.vregMode = TRUE;	break;
		case IO_OUT_AMUX_ADR:		intStruct.IO_OUT_AMUX.R = spiStruct.response;		intUpdtStru.ioOutMux = TRUE;	break;
		case CAN_LIN_MODE_ADR:		intStruct.CAN_LIN_mode.R = spiStruct.response;		intUpdtStru.canLinMode = TRUE;	break;
		case VREG_LPON_ADR:			intStruct.Vreg_LPON.R = spiStruct.response;			intUpdtStru.vregLpon = TRUE;	break;
		case INIT_SUPERVISOR1_ADR:	intStruct.INIT_Supervisor1.R = spiStruct.response;	intUpdtStru.supervisor1 = TRUE; break;
		case INIT_SUPERVISOR2_ADR:	intStruct.INIT_Supervisor2.R = spiStruct.response;	intUpdtStru.supervisor2 = TRUE;	break;
		case INIT_SUPERVISOR3_ADR:	intStruct.INIT_Supervisor3.R = spiStruct.response;	intUpdtStru.supervisor3 = TRUE;	break;
		case INIT_FSSM1_ADR:		intStruct.INIT_FSSM1.R = spiStruct.response;		intUpdtStru.fssm1 = TRUE;		break;
		case INIT_FSSM2_ADR:		intStruct.INIT_FSSM2.R = spiStruct.response;		intUpdtStru.fssm2 = TRUE;		break;
		case WD_WINDOW_ADR:			intStruct.WD_window.R = spiStruct.response;			intUpdtStru.wdWindow = TRUE;	break;
		case WD_LFSR_ADR:			intStruct.WD_LFSR.R = spiStruct.response;			intUpdtStru.wdLfsr = TRUE;		break;
		case WD_ANSWER_ADR:			intStruct.WD_answer.R = spiStruct.response;			intUpdtStru.wdAnswer = TRUE;	break;
		case DIAG_FS1_ADR:			intStruct.Diag_FS1.R = spiStruct.response;			intUpdtStru.diagFs1 = TRUE;		break;
		case WD_COUNTER_ADR:		intStruct.WD_counter.R = spiStruct.response;		intUpdtStru.wdCnt = TRUE;		break;
		case DIAG_FS2_ADR:			intStruct.Diag_FS2.R = spiStruct.response;			intUpdtStru.diagFs2 = TRUE;		break;
		case FS_OUT_ADR:																intUpdtStru.fsOut = TRUE;		break;
		default:																										break;
	}
}


static uint8_t mc33907_ComputeLFSR(uint32_t actualLFSR)
{
	#if 0
	uint32_t lfsrMove2 = ((actualLFSR) <<2);
	uint32_t lfsrMove2Reverse = 0;
	lfsrMove2 += 2;
	lfsrMove2Reverse =(uint32_t)( 0xFFFFFFFF - lfsrMove2);
	return (uint8_t)((lfsrMove2Reverse >> 2) & 0xFF);
	#else
	
	volatile uint32_t newLFSR;
	newLFSR  = actualLFSR<<2;
	newLFSR += 6;
	newLFSR -= 4;
	newLFSR  = ~newLFSR;
	newLFSR  = newLFSR>>2;

	return (newLFSR);
	#endif
#if 0
		__asm("se_li 		r25,0x04	");	/*load nb.4 -> r25*/
		__asm("	mullw 		r3,r25,r3	");	/*lfsr * 4 	-> r3*/
		__asm("	e_add16i	r3,r3,6		");	/*r3 + 6 		-> r3*/
		__asm("	se_subi 	r3,4		");	/*r3 - 4 		-> r3*/
		__asm("	se_not 	r3			");		/*NOT r3 		-> r3*/
		__asm("	e_li 		r24,0xFFFF	");	/*mask 			-> r24*/
		__asm("	se_and 	r3,r24			");	/*r24 & r3  -> r3*/
		__asm("	divwu		r3,r3,r25	");	/*r3 / 4		-> r3	-> as a return value*/
		__asm("	e_li 		r24,0xFF	");	/*mask 			-> r24*/
		__asm("	se_and 	r3,r24			");	/*store only lower 8 bits -> r3*/
#endif
}



