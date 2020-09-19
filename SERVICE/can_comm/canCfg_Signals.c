

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "canCfg_export.h"
#include "canCfg_signals.h"


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
/*Message Name:pi_display_msg1	 start bit:40	 bit len:16	 format:Motorola_Forward_Lsb*/
hs_u2_t pi_1 = 0;
/*Message Name:pi_display_msg1	 start bit:56	 bit len:16	 format:Motorola_Forward_Lsb*/
hs_u2_t pi_2 = 0;
/*Message Name:pi_display_msg2	 start bit:8	 bit len:16	 format:Motorola_Forward_Lsb*/
hs_u2_t pi_3 = 0;
/*Message Name:pi_display_msg2	 start bit:24	 bit len:16	 format:Motorola_Forward_Lsb*/
hs_u2_t pi_4 = 0;
/*Message Name:pi_display_msg2	 start bit:40	 bit len:16	 format:Motorola_Forward_Lsb*/
hs_u2_t pi_5 = 0;
/*Message Name:lin_testResult	 start bit:7	 bit len:1	 format:Motorola_Forward_Lsb*/
hs_u1_t lin_selfTestResult = 0;


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

void PT_CAN_0x301_di_display_msg1_txPack(void)
{
    PT_CAN_0x301_di_display_msg1_txBuf[0] = 0;
    PT_CAN_0x301_di_display_msg1_txBuf[1] = 0;
    PT_CAN_0x301_di_display_msg1_txBuf[2] = 0;
    PT_CAN_0x301_di_display_msg1_txBuf[3] = 0;
    PT_CAN_0x301_di_display_msg1_txBuf[4] = 0;
    PT_CAN_0x301_di_display_msg1_txBuf[5] = 0;
    PT_CAN_0x301_di_display_msg1_txBuf[6] = 0;
    PT_CAN_0x301_di_display_msg1_txBuf[7] = 0;
    /*start bit:7	 bit len:1	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x301_di_display_msg1_txBuf[0] |= (hs_u1_t)((hal_di_IGN_SW_HS & 0x00000001ul) << 7);
    /*start bit:6	 bit len:1	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x301_di_display_msg1_txBuf[0] |= (hs_u1_t)((hal_di_BRAKE1_SW_HS & 0x00000001ul) << 6);
    /*start bit:5	 bit len:1	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x301_di_display_msg1_txBuf[0] |= (hs_u1_t)((hal_di_BRAKE2_SW_HS & 0x00000001ul) << 5);
    /*start bit:4	 bit len:1	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x301_di_display_msg1_txBuf[0] |= (hs_u1_t)((hal_di_04_HS & 0x00000001ul) << 4);
    /*start bit:3	 bit len:1	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x301_di_display_msg1_txBuf[0] |= (hs_u1_t)((hal_di_05_HS & 0x00000001ul) << 3);
    /*start bit:2	 bit len:1	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x301_di_display_msg1_txBuf[0] |= (hs_u1_t)((hal_di_06_HS & 0x00000001ul) << 2);
    /*start bit:1	 bit len:1	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x301_di_display_msg1_txBuf[0] |= (hs_u1_t)((hal_di_07_HS & 0x00000001ul) << 1);
    /*start bit:0	 bit len:1	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x301_di_display_msg1_txBuf[0] |= (hs_u1_t)(hal_di_08_HS & 0x00000001ul);
    /*start bit:28	 bit len:1	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x301_di_display_msg1_txBuf[3] |= (hs_u1_t)((hal_di_09_LS & 0x00000001ul) << 4);
    /*start bit:27	 bit len:1	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x301_di_display_msg1_txBuf[3] |= (hs_u1_t)((hal_di_10_LS & 0x00000001ul) << 3);
    /*start bit:26	 bit len:1	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x301_di_display_msg1_txBuf[3] |= (hs_u1_t)((hal_di_11_LS & 0x00000001ul) << 2);
    /*start bit:25	 bit len:1	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x301_di_display_msg1_txBuf[3] |= (hs_u1_t)((hal_di_12_LS & 0x00000001ul) << 1);
}

void C_CAN_0x201_di_display_msg1_txPack(void)
{

}
void C_CAN_0x211_di_display_msg1_txPack(void)
{

}
void C_CAN_0x3A1_di_display_msg1_txPack(void)
{

}
void C_CAN_0x241_di_display_msg1_txPack(void)
{

}
void C_CAN_0x3B1_di_display_msg1_txPack(void)
{

}
void C_CAN_0x101_di_display_msg1_txPack(void)
{

}

void PT_CAN_0x302_do_ctrl_msg_rxUnpack(void)
{

}

void PT_CAN_0x303_do_ctrl_msg_rxUnpack(void)
{
#if 0
    PT_CAN_0x303_ai_displayFb_msg1_txBuf[0] = 0;
    PT_CAN_0x303_ai_displayFb_msg1_txBuf[1] = 0;
    PT_CAN_0x303_ai_displayFb_msg1_txBuf[2] = 0;
    PT_CAN_0x303_ai_displayFb_msg1_txBuf[3] = 0;
    PT_CAN_0x303_ai_displayFb_msg1_txBuf[4] = 0;
    PT_CAN_0x303_ai_displayFb_msg1_txBuf[5] = 0;
    PT_CAN_0x303_ai_displayFb_msg1_txBuf[6] = 0;
    PT_CAN_0x303_ai_displayFb_msg1_txBuf[7] = 0;
    /*start bit:8	 bit len:16	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x303_ai_displayFb_msg1_txBuf[1] = (hs_u1_t)(hal_ai_HIGHSIGNAL1_OUTPUT_FB & 0x000000FFul);
    PT_CAN_0x303_ai_displayFb_msg1_txBuf[0] = (hs_u1_t)((hal_ai_HIGHSIGNAL1_OUTPUT_FB & 0x0000FF00ul) >> 8);
    /*start bit:56	 bit len:16	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x303_ai_displayFb_msg1_txBuf[7] = (hs_u1_t)(hal_ai_HIGHSIGNAL2_OUTPUT_FB & 0x000000FFul);
    PT_CAN_0x303_ai_displayFb_msg1_txBuf[6] = (hs_u1_t)((hal_ai_HIGHSIGNAL2_OUTPUT_FB & 0x0000FF00ul) >> 8);
#endif
}

void PT_CAN_0x304_do_ctrl_msg_rxUnpack(void)
{
#if 0
    PT_CAN_0x304_ai_display_msg1_txBuf[0] = 0;
    PT_CAN_0x304_ai_display_msg1_txBuf[1] = 0;
    PT_CAN_0x304_ai_display_msg1_txBuf[2] = 0;
    PT_CAN_0x304_ai_display_msg1_txBuf[3] = 0;
    PT_CAN_0x304_ai_display_msg1_txBuf[4] = 0;
    PT_CAN_0x304_ai_display_msg1_txBuf[5] = 0;
    PT_CAN_0x304_ai_display_msg1_txBuf[6] = 0;
    PT_CAN_0x304_ai_display_msg1_txBuf[7] = 0;
    /*start bit:24	 bit len:16	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x304_ai_display_msg1_txBuf[3] = (hs_u1_t)(hal_ai_APS1_ADC & 0x000000FFul);
    PT_CAN_0x304_ai_display_msg1_txBuf[2] = (hs_u1_t)((hal_ai_APS1_ADC & 0x0000FF00ul) >> 8);
    /*start bit:40	 bit len:16	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x304_ai_display_msg1_txBuf[5] = (hs_u1_t)(hal_ai_APS2_ADC & 0x000000FFul);
    PT_CAN_0x304_ai_display_msg1_txBuf[4] = (hs_u1_t)((hal_ai_APS2_ADC & 0x0000FF00ul) >> 8);
    /*start bit:56	 bit len:16	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x304_ai_display_msg1_txBuf[7] = (hs_u1_t)(hal_ai_LIQUID_LEVEL_SENSOR_ADC & 0x000000FFul);
    PT_CAN_0x304_ai_display_msg1_txBuf[6] = (hs_u1_t)((hal_ai_LIQUID_LEVEL_SENSOR_ADC & 0x0000FF00ul) >> 8);
#endif
}

void PT_CAN_0x5A4_ai_display_msg2_txPack(void)
{

}

void PT_CAN_0x1B1_ai_display_msg2_txPack(void)
{

}

void PT_CAN_0x2FA_ai_display_msg4_txPack(void)
{

}

void PT_CAN_0x5A5_ai_display_msg2_txPack(void)
{

}

void PT_CAN_0x331_ai_display_msg4_txPack(void)
{

}

void PT_CAN_0x330_ai_display_msg4_txPack(void)
{

}

void PT_CAN_0x32E_ai_display_msg4_txPack(void)
{

}

void PT_CAN_0x321_ai_display_msg4_txPack(void)
{

}

void PT_CAN_0x561_ai_display_msg4_txPack(void)
{

}

void PT_CAN_0x551_ai_display_msg4_txPack(void)
{

}

void PT_CAN_0x111_ai_display_msg4_txPack(void)
{

}

void PT_CAN_0x401_ai_display_msg4_txPack(void)
{

}

void PT_CAN_0x511_ai_display_msg4_txPack(void)
{

}

void PT_CAN_0x521_ai_display_msg4_txPack(void)
{

}

void PT_CAN_0x531_ai_display_msg4_txPack(void)
{

}

void PT_CAN_0x5F1_ai_display_msg4_txPack(void)
{

}

void PT_CAN_0x101_ai_display_msg4_txPack(void)
{

}

void PT_CAN_0x441_ai_display_msg4_txPack(void)
{

}

void PT_CAN_0x309_ai_display_msg6_txPack(void)
{
    PT_CAN_0x309_ai_display_msg6_txBuf[0] = 0;
    PT_CAN_0x309_ai_display_msg6_txBuf[1] = 0;
    PT_CAN_0x309_ai_display_msg6_txBuf[2] = 0;
    PT_CAN_0x309_ai_display_msg6_txBuf[3] = 0;
    PT_CAN_0x309_ai_display_msg6_txBuf[4] = 0;
    PT_CAN_0x309_ai_display_msg6_txBuf[5] = 0;
    PT_CAN_0x309_ai_display_msg6_txBuf[6] = 0;
    PT_CAN_0x309_ai_display_msg6_txBuf[7] = 0;
    /*start bit:8	 bit len:16	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x309_ai_display_msg6_txBuf[1] = (hs_u1_t)(hal_ai_NCHG_CC_ADC & 0x000000FFul);
    PT_CAN_0x309_ai_display_msg6_txBuf[0] = (hs_u1_t)((hal_ai_NCHG_CC_ADC & 0x0000FF00ul) >> 8);
    /*start bit:24	 bit len:16	 format:Motorola_Forward_Lsb*/
    PT_CAN_0x309_ai_display_msg6_txBuf[3] = (hs_u1_t)(hal_ai_QCHG_CC2_ADC & 0x000000FFul);
    PT_CAN_0x309_ai_display_msg6_txBuf[2] = (hs_u1_t)((hal_ai_QCHG_CC2_ADC & 0x0000FF00ul) >> 8);
}


void PT_CAN_0x3A6_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x3F3_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x492_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x394_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x407_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x487_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x075_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x07E_po_ctrl_msg1_rxUnpack(void)
{

}

void C_CAN_0x4FE_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x514_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x574_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x102_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x103_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x104_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x112_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x113_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x114_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x126_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x134_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x2C1_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x309_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x312_po_ctrl_msg1_rxUnpack(void)
{

}

void PT_CAN_0x313_po_ctrl_msg1_rxUnpack(void)
{

}
void PT_CAN_0x354_po_ctrl_msg1_rxUnpack(void)
{

}
void PT_CAN_0x387_po_ctrl_msg1_rxUnpack(void)
{

}
void PT_CAN_0x38D_po_ctrl_msg1_rxUnpack(void)
{

}
void PT_CAN_0x38E_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x39E_po_ctrl_msg1_rxUnpack(void)
{

}

void C_CAN_0x232_po_ctrl_msg1_rxUnpack(void)
{

}

void C_CAN_0x222_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x058_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x3FE_po_ctrl_msg1_rxUnpack(void)
{

}

void C_CAN_0x0A5_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x122_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x124_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x142_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x144_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x1BE_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x1C5_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x1D5_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x212_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x219_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x21A_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x242_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x252_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x272_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x282_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x2A1_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x2A2_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x2A5_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x2C2_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x2CE_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x2FE_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x319_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x341_po_ctrl_msg1_rxUnpack(void)
{

}
void C_CAN_0x380_po_ctrl_msg1_rxUnpack(void)
{

}




/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/


/* ---- Source switch off ---------------------------------------------------- */

/*--- End of file ------------------------------------------------------------*/
