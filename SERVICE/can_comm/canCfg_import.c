
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "globalVarExport.h"
#include "canCfg_signals.h"
#include "canCfg_export.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void PT_CAN_0x3B5_can1Can_ComTestRec_msg_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/
	hs_u1_t byte_i = 0;
	//PT_CAN_0x3C5_can1Can_ComTest_msg_txReady = TRUE;


}


void RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/
	hs_u1_t byte_i = 0;
	RESERVED_CAN_0x2C5_can2Can_ComTest_msg_txReady = TRUE;
	for(byte_i = 0; byte_i < 8; byte_i ++)
	{
		RESERVED_CAN_0x2C5_can2Can_ComTest_msg_txBuf[byte_i] = RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rxBuf[byte_i];
	}

}


void PT_CAN_0x1B5_can3Can_ComTestRec_msg_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/
	hs_u1_t byte_i = 0;
	PT_CAN_0x1C5_can3Can_ComTest_msg_txReady = TRUE;
	for(byte_i = 0; byte_i < 8; byte_i ++)
	{
		PT_CAN_0x1C5_can3Can_ComTest_msg_txBuf[byte_i] = PT_CAN_0x1B5_can3Can_ComTestRec_msg_rxBuf[byte_i];
	}

}

void PT_CAN_0x3A6_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x3F3_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x492_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x394_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x407_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x487_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x075_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x07E_po_ctrl_msg1_rx_cb(void)
{


}

void C_CAN_0x4FE_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x514_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x574_po_ctrl_msg1_rx_cb(void)
{


}
void PT_CAN_0x102_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x103_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x104_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x112_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x113_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x114_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x126_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x134_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x2C1_po_ctrl_msg1_rx_cb(void)
{


}

void PT_CAN_0x309_po_ctrl_msg1_rx_cb(void)
{


}
void PT_CAN_0x312_po_ctrl_msg1_rx_cb(void)
{


}
void PT_CAN_0x313_po_ctrl_msg1_rx_cb(void)
{


}
void PT_CAN_0x354_po_ctrl_msg1_rx_cb(void)
{


}
void PT_CAN_0x387_po_ctrl_msg1_rx_cb(void)
{


}
void PT_CAN_0x38D_po_ctrl_msg1_rx_cb(void)
{


}
void PT_CAN_0x38E_po_ctrl_msg1_rx_cb(void)
{


}
void C_CAN_0x39E_po_ctrl_msg1_rx_cb(void)
{

}

void C_CAN_0x222_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x232_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x058_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x3FE_po_ctrl_msg1_rx_cb(void)
{

}

void C_CAN_0x0A5_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x122_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x124_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x142_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x144_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x1BE_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x1C5_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x1D5_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x212_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x219_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x21A_po_ctrl_msg1_rx_cb(void)
{

}

void C_CAN_0x242_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x252_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x272_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x282_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x2A1_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x2A2_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x2A5_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x2C2_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x2CE_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x2FE_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x319_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x341_po_ctrl_msg1_rx_cb(void)
{

}
void C_CAN_0x380_po_ctrl_msg1_rx_cb(void)
{

}

void PT_CAN_0x300_do_ctrl_msg_rx_cb(void)
{
    /*add your code here to realize the call-back hook function*/
	//PT_CAN_0x400_test_txReady = 1;
}

void PT_CAN_0x302_do_ctrl_msg_rx_cb(void)
{

}

void PT_CAN_0x303_do_ctrl_msg_rx_cb(void)
{

}

void PT_CAN_0x304_do_ctrl_msg_rx_cb(void)
{

}

void PT_CAN_0x310_pi_display_msg1_tx_cb(void)
{
    /*add your code here to realize the call-back hook function*/
	//pi_1 = (hs_u2_t)(hld_pi_chargerCP[0] * 2.5f);
	//pi_2 = (hs_u2_t)(hld_pi_chargerCP[1] * 2.5f);

}


void PT_CAN_0x311_pi_display_msg2_tx_cb(void)
{
    /*add your code here to realize the call-back hook function*/
	//pi_3 = (hs_u2_t)(hld_pi_chargerCP[2] * 2.5f);
	//pi_4 = (hs_u2_t)(hld_pi_chargerCP[3] * 2.5f);
	//pi_5 = (hs_u2_t)(hld_pi_chargerCP[4] * 2.5f);

}


void PT_CAN_0x312_pi_display_msg3_tx_cb(void)
{
    /*add your code here to realize the call-back hook function*/


}

void PT_CAN_0x731_msg_rx_cb(void)
{


}

void PT_CAN_0x7E7_msg_rx_cb(void)
{


}

void PT_CAN_0x7DF_msg_rx_cb(void)
{


}

void PT_CAN_0x632_msg_rx_cb(void)
{


}

void PT_CAN_0x634_msg_rx_cb(void)
{


}

void PT_CAN_0x637_msg_rx_cb(void)
{


}

void PT_CAN_0x63E_msg_rx_cb(void)
{


}


/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/


/* ---- Source switch off ---------------------------------------------------- */

/*--- End of file ------------------------------------------------------------*/
