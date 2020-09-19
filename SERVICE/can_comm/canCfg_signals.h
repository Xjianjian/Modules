
#ifndef CAN_CFG_SIGNALS_H_
#define CAN_CFG_SIGNALS_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "hs_types.h"


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/



               
/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/
/*Message Name:do_ctrl_msg	 start bit:7	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_do_THREE_WAY_VALVE_POWER_HS;
/*Message Name:do_ctrl_msg	 start bit:6	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_do_FOUR_WAY_VALVE_POWER_HS;
/*Message Name:do_ctrl_msg	 start bit:5	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_do_ARI_COOLING_POWER_HS;
/*Message Name:do_ctrl_msg	 start bit:4	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_do_COMPRESSOR_POWER_HS;
/*Message Name:do_ctrl_msg	 start bit:3	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_do_DCDC_ENABLE_HS;
/*Message Name:do_ctrl_msg	 start bit:2	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_do_BAT_COOLING_POWER_HS;
/*Message Name:do_ctrl_msg	 start bit:1	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_do_MAIN_INTAKE_GRID_POWER_HS;
/*Message Name:do_ctrl_msg	 start bit:0	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_do_BAT_PTC_POWER_HS;
/*Message Name:do_ctrl_msg	 start bit:23	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_bd8la7002_flg_ch1;
/*Message Name:do_ctrl_msg	 start bit:22	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_bd8la7002_flg_ch2;
/*Message Name:do_ctrl_msg	 start bit:21	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_bd8la7002_flg_ch3;
/*Message Name:do_ctrl_msg	 start bit:20	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_bd8la7002_flg_ch4;
/*Message Name:do_ctrl_msg	 start bit:19	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_bd8la7001_flg_ch1;
/*Message Name:do_ctrl_msg	 start bit:18	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_bd8la7001_flg_ch2;
/*Message Name:do_ctrl_msg	 start bit:17	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_bd8la7001_flg_ch3;
/*Message Name:do_ctrl_msg	 start bit:16	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_do_LSD_SPARE9_EN_HS;
/*Message Name:do_ctrl_msg	 start bit:31	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_do_LSD_SPARE10_EN_HS;
/*Message Name:do_ctrl_msg	 start bit:24	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_bd8la7001_flg_ch4;
/*Message Name:do_ctrl_msg	 start bit:39	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_bd8la7001_flg_ch5;
/*Message Name:do_ctrl_msg	 start bit:38	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_bd8la7001_flg_ch6;
/*Message Name:do_ctrl_msg	 start bit:37	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_bd8la7001_flg_ch7;
/*Message Name:do_ctrl_msg	 start bit:36	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_bd8la7001_flg_ch8;
/*Message Name:di_display_msg1	 start bit:7	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_IGN_SW_HS;
/*Message Name:di_display_msg1	 start bit:6	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_BRAKE1_SW_HS;
/*Message Name:di_display_msg1	 start bit:5	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_BRAKE2_SW_HS;
/*Message Name:di_display_msg1	 start bit:4	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_04_HS;
/*Message Name:di_display_msg1	 start bit:3	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_05_HS;
/*Message Name:di_display_msg1	 start bit:2	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_06_HS;
/*Message Name:di_display_msg1	 start bit:1	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_07_HS;
/*Message Name:di_display_msg1	 start bit:0	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_08_HS;
/*Message Name:di_display_msg1	 start bit:28	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_09_LS;
/*Message Name:di_display_msg1	 start bit:27	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_10_LS;
/*Message Name:di_display_msg1	 start bit:26	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_11_LS;
/*Message Name:di_display_msg1	 start bit:25	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_12_LS;
/*Message Name:di_display_msg2	 start bit:6	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_LSD_SPARE9_FB_LS;
/*Message Name:di_display_msg2	 start bit:5	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_LSD_SPARE10_FB_LS;
/*Message Name:ai_displayFb_msg1	 start bit:8	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_HIGHSIGNAL1_OUTPUT_FB;
/*Message Name:ai_displayFb_msg1	 start bit:56	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_HIGHSIGNAL2_OUTPUT_FB;
/*Message Name:ai_display_msg1	 start bit:24	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_APS1_ADC;
/*Message Name:ai_display_msg1	 start bit:40	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_APS2_ADC;
/*Message Name:ai_display_msg1	 start bit:56	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_LIQUID_LEVEL_SENSOR_ADC;
/*Message Name:ai_display_msg2	 start bit:8	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_04_ADC;
/*Message Name:ai_display_msg2	 start bit:24	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_05_ADC;
/*Message Name:ai_display_msg3	 start bit:8	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_08_ADC;
/*Message Name:ai_display_msg3	 start bit:24	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_09_ADC;
/*Message Name:ai_display_msg3	 start bit:40	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_10_ADC;
/*Message Name:ai_display_msg3	 start bit:56	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_11_ADC;
/*Message Name:ai_display_msg4	 start bit:8	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_12_ADC;
/*Message Name:ai_display_msg4	 start bit:24	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_13_ADC;
/*Message Name:ai_display_msg4	 start bit:40	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_14_ADC;
/*Message Name:ai_display_msg6	 start bit:8	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_NCHG_CC_ADC;
/*Message Name:ai_display_msg6	 start bit:24	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t hal_ai_QCHG_CC2_ADC;
/*Message Name:di_display_msg3	 start bit:7	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_APS1_PG_LS;
/*Message Name:di_display_msg3	 start bit:6	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_APS2_PG_LS;
/*Message Name:di_display_msg3	 start bit:5	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_LIQUID_LEVEL_SENSOR_PG_LS;
/*Message Name:di_display_msg3	 start bit:4	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hal_di_PS_PG_LS;
/*Message Name:po_ctrl_msg1	 start bit:8	 bit len:8	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_po_fan_CONTROL;
/*Message Name:po_ctrl_msg1	 start bit:16	 bit len:8	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_po_pump1_CONTROL;
/*Message Name:po_ctrl_msg1	 start bit:24	 bit len:8	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_po_pump2_CONTROL;
/*Message Name:po_ctrl_msg1	 start bit:32	 bit len:8	 format:Motorola_Forward_Lsb*/
extern hs_u1_t hld_po_pump3_CONTROL;
/*Message Name:pi_display_msg1	 start bit:40	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t pi_1;
/*Message Name:pi_display_msg1	 start bit:56	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t pi_2;
/*Message Name:pi_display_msg2	 start bit:8	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t pi_3;
/*Message Name:pi_display_msg2	 start bit:24	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t pi_4;
/*Message Name:pi_display_msg2	 start bit:40	 bit len:16	 format:Motorola_Forward_Lsb*/
extern hs_u2_t pi_5;
/*Message Name:lin_testResult	 start bit:7	 bit len:1	 format:Motorola_Forward_Lsb*/
extern hs_u1_t lin_selfTestResult;


/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/
extern void C_CAN_0x201_di_display_msg1_txPack(void);
extern void C_CAN_0x211_di_display_msg1_txPack(void);
extern void C_CAN_0x3A1_di_display_msg1_txPack(void);
extern void C_CAN_0x241_di_display_msg1_txPack(void);
extern void C_CAN_0x3B1_di_display_msg1_txPack(void);
extern void C_CAN_0x101_di_display_msg1_txPack(void);

extern void C_CAN_0x4FE_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x232_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x222_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x058_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x3FE_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x0A5_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x122_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x124_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x142_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x144_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x1BE_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x1C5_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x1D5_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x212_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x219_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x21A_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x242_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x252_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x272_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x282_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x2A1_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x2A2_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x2A5_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x2C2_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x2CE_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x2FE_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x319_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x341_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x380_po_ctrl_msg1_rxUnpack(void);
extern void C_CAN_0x39E_po_ctrl_msg1_rxUnpack(void);

extern void PT_CAN_0x3F3_po_ctrl_msg1_rxUnpack(void);
extern void PT_CAN_0x492_po_ctrl_msg1_rxUnpack(void);
extern void PT_CAN_0x394_po_ctrl_msg1_rxUnpack(void);
extern void PT_CAN_0x407_po_ctrl_msg1_rxUnpack(void);

extern void PT_CAN_0x487_po_ctrl_msg1_rxUnpack(void);
extern void PT_CAN_0x075_po_ctrl_msg1_rxUnpack(void);
extern void PT_CAN_0x07E_po_ctrl_msg1_rxUnpack(void);
extern void PT_CAN_0x514_po_ctrl_msg1_rxUnpack(void);
extern void PT_CAN_0x574_po_ctrl_msg1_rxUnpack(void);

extern void PT_CAN_0x102_po_ctrl_msg1_rxUnpack(void);
extern void PT_CAN_0x103_po_ctrl_msg1_rxUnpack(void);

extern void PT_CAN_0x104_po_ctrl_msg1_rxUnpack(void);

extern void PT_CAN_0x112_po_ctrl_msg1_rxUnpack(void);

extern void PT_CAN_0x113_po_ctrl_msg1_rxUnpack(void);

extern void PT_CAN_0x114_po_ctrl_msg1_rxUnpack(void);

extern void PT_CAN_0x126_po_ctrl_msg1_rxUnpack(void);

extern void PT_CAN_0x134_po_ctrl_msg1_rxUnpack(void);
extern void PT_CAN_0x2C1_po_ctrl_msg1_rxUnpack(void);

extern void PT_CAN_0x309_po_ctrl_msg1_rxUnpack(void);

extern void PT_CAN_0x312_po_ctrl_msg1_rxUnpack(void);
extern void PT_CAN_0x313_po_ctrl_msg1_rxUnpack(void);
extern void PT_CAN_0x354_po_ctrl_msg1_rxUnpack(void);
extern void PT_CAN_0x387_po_ctrl_msg1_rxUnpack(void);
extern void PT_CAN_0x38D_po_ctrl_msg1_rxUnpack(void);
extern void PT_CAN_0x38E_po_ctrl_msg1_rxUnpack(void);

extern void PT_CAN_0x302_do_ctrl_msg_rxUnpack(void);

extern void PT_CAN_0x303_do_ctrl_msg_rxUnpack(void);

extern void PT_CAN_0x304_do_ctrl_msg_rxUnpack(void);

extern void PT_CAN_0x5A4_ai_display_msg2_txPack(void);

extern void PT_CAN_0x1B1_ai_display_msg2_txPack(void);

extern void PT_CAN_0x2FA_ai_display_msg4_txPack(void);

extern void PT_CAN_0x5A5_ai_display_msg2_txPack(void);

extern void PT_CAN_0x331_ai_display_msg4_txPack(void);

extern void PT_CAN_0x330_ai_display_msg4_txPack(void);
extern void PT_CAN_0x32E_ai_display_msg4_txPack(void);

extern void PT_CAN_0x321_ai_display_msg4_txPack(void);

extern void PT_CAN_0x561_ai_display_msg4_txPack(void);

extern void PT_CAN_0x551_ai_display_msg4_txPack(void);

extern void PT_CAN_0x111_ai_display_msg4_txPack(void);
extern void PT_CAN_0x401_ai_display_msg4_txPack(void);

extern void PT_CAN_0x511_ai_display_msg4_txPack(void);
extern void PT_CAN_0x521_ai_display_msg4_txPack(void);

extern void PT_CAN_0x531_ai_display_msg4_txPack(void);

extern void PT_CAN_0x5F1_ai_display_msg4_txPack(void);

extern void PT_CAN_0x101_ai_display_msg4_txPack(void);

extern void PT_CAN_0x441_ai_display_msg4_txPack(void);



extern void PT_CAN_0x301_di_display_msg1_txPack(void);
extern void PT_CAN_0x302_di_display_msg2_txPack(void);
extern void PT_CAN_0x303_ai_displayFb_msg1_txPack(void);
extern void PT_CAN_0x304_ai_display_msg1_txPack(void);
extern void PT_CAN_0x305_ai_display_msg2_txPack(void);
extern void PT_CAN_0x306_ai_display_msg3_txPack(void);
extern void PT_CAN_0x307_ai_display_msg4_txPack(void);
extern void PT_CAN_0x309_ai_display_msg6_txPack(void);

extern void PT_CAN_0x3A6_po_ctrl_msg1_rxUnpack(void);
extern void PT_CAN_0x310_pi_display_msg1_txPack(void);
extern void PT_CAN_0x311_pi_display_msg2_txPack(void);
extern void PT_CAN_0x33F_lin_testResult_txPack(void);


#endif

