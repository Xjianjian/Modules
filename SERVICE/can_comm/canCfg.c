/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "canCfg.h"
#include "hs_types.h"
#include "canCfg_signals.h"
#include "canCfg_import.h"


/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/
#define m_C_CAN_MsgCnt 36
#define m_PT_CAN_MsgCnt 59
#define m_RESERVED_CAN_MsgCnt 3
#define m_canMod_cnt 3


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/
static canMsgScheData_t CLASS_CANMsgScheData[m_C_CAN_MsgCnt] =
{
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},

	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},

	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},

	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0},
	{FALSE,0,0}
};


static canMsgSortInfo_t CLASS_CANMsgSortOrder[m_C_CAN_MsgCnt] =
{
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},

    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},

    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},

    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0}
};

static canMsgScheData_t RESERVED_CANMsgScheData[m_RESERVED_CAN_MsgCnt] =
{
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0}
};


static canMsgSortInfo_t RESERVED_CANMsgSortOrder[m_RESERVED_CAN_MsgCnt] = 
{
    {0,0,0},
    {0,0,0},
    {0,0,0}
};

static canMsgScheData_t PT_CANMsgScheData[m_PT_CAN_MsgCnt] =
{
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},

    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},

    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},

    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},

    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},

    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0},
    {FALSE,0,0}
};


static canMsgSortInfo_t PT_CANMsgSortOrder[m_PT_CAN_MsgCnt] = 
{
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},


    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},

    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},

    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},

    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},

    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0},
    {0,0,0}
};



/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/
hs_u1_t RESERVED_CAN_0x2A0_can2WakeUp_display_msg_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL RESERVED_CAN_0x2A0_can2WakeUp_display_msg_txReady = FALSE;
hs_BOOL RESERVED_CAN_0x2A0_can2WakeUp_display_msg_txFail = FALSE;
hs_BOOL RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rxOverLap = TRUE;
hs_u1_t RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rxTimeout = FALSE;
hs_BOOL RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_received = FALSE;
hs_u1_t RESERVED_CAN_0x2C5_can2Can_ComTest_msg_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL RESERVED_CAN_0x2C5_can2Can_ComTest_msg_txReady = FALSE;
hs_BOOL RESERVED_CAN_0x2C5_can2Can_ComTest_msg_txFail = FALSE;


hs_u1_t C_CAN_0x201_di_display_msg1_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x201_di_display_msg1_txReady = FALSE;
hs_BOOL C_CAN_0x201_di_display_msg1_txFail = FALSE;

hs_u1_t C_CAN_0x211_di_display_msg1_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x211_di_display_msg1_txReady = FALSE;
hs_BOOL C_CAN_0x211_di_display_msg1_txFail = FALSE;

hs_u1_t C_CAN_0x3A1_di_display_msg1_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x3A1_di_display_msg1_txReady = FALSE;
hs_BOOL C_CAN_0x3A1_di_display_msg1_txFail = FALSE;

hs_u1_t C_CAN_0x241_di_display_msg1_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x241_di_display_msg1_txReady = FALSE;
hs_BOOL C_CAN_0x241_di_display_msg1_txFail = FALSE;

hs_u1_t C_CAN_0x3B1_di_display_msg1_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x3B1_di_display_msg1_txReady = FALSE;
hs_BOOL C_CAN_0x3B1_di_display_msg1_txFail = FALSE;

hs_u1_t C_CAN_0x101_di_display_msg1_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x101_di_display_msg1_txReady = FALSE;
hs_BOOL C_CAN_0x101_di_display_msg1_txFail = FALSE;

hs_u1_t C_CAN_0x1C5_can3Can_ComTest_msg_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x1C5_can3Can_ComTest_msg_txReady = FALSE;
hs_BOOL C_CAN_0x1C5_can3Can_ComTest_msg_txFail = FALSE;

hs_BOOL C_CAN_0x39E_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x39E_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x39E_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x39E_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x232_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x232_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x232_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x232_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x222_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x222_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x222_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x222_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x058_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x058_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x058_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x058_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x3FE_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x3FE_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x3FE_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x3FE_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x0A5_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x0A5_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x0A5_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x0A5_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x122_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x122_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x122_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x122_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x124_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x124_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x124_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x124_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x142_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x142_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x142_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x142_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x144_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x144_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x144_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x144_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x1BE_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x1BE_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x1BE_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x1BE_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x1C5_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x1C5_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x1C5_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x1C5_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x1D5_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x1D5_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x1D5_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x1D5_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x212_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x212_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x212_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x212_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x219_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x219_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x219_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x219_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x21A_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x21A_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x21A_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x21A_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x242_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x242_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x242_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x242_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x252_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x252_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x252_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x252_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x272_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x272_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x272_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x272_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x282_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x282_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x282_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x282_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x2A1_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x2A1_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x2A1_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x2A1_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x2A2_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x2A2_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x2A2_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x2A2_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x2A5_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x2A5_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x2A5_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x2A5_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x2C2_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x2C2_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x2C2_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x2C2_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x2CE_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x2CE_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x2CE_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x2CE_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x2FE_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x2FE_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x2FE_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x2FE_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x319_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x319_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x319_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x319_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x341_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x341_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x341_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x341_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x380_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x380_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x380_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x380_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x731_msg_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x731_msg_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x731_msg_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x731_msg_received = FALSE;

hs_BOOL PT_CAN_0x7E7_msg_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x7E7_msg_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x7E7_msg_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x7E7_msg_received = FALSE;

hs_BOOL PT_CAN_0x7DF_msg_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x7DF_msg_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x7DF_msg_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x7DF_msg_received = FALSE;

hs_BOOL PT_CAN_0x797_msg_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x797_msg_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x797_msg_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x797_msg_received = FALSE;

hs_u1_t PT_CAN_0x79A_msg_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x79A_msg_txReady = FALSE;
hs_BOOL PT_CAN_0x79A_msg_txFail = FALSE;

hs_BOOL PT_CAN_0x632_msg_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x632_msg_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x632_msg_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x632_msg_received = FALSE;

hs_BOOL PT_CAN_0x634_msg_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x634_msg_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x634_msg_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x634_msg_received = FALSE;

hs_BOOL PT_CAN_0x637_msg_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x637_msg_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x637_msg_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x637_msg_received = FALSE;

hs_BOOL PT_CAN_0x63E_msg_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x63E_msg_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x63E_msg_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x63E_msg_received = FALSE;

hs_u1_t PT_CAN_0x301_di_display_msg1_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x301_di_display_msg1_txReady = FALSE;
hs_BOOL PT_CAN_0x301_di_display_msg1_txFail = FALSE;

hs_BOOL PT_CAN_0x302_do_ctrl_msg_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x302_do_ctrl_msg_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x302_do_ctrl_msg_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x302_do_ctrl_msg_received = FALSE;

hs_BOOL PT_CAN_0x303_do_ctrl_msg_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x303_do_ctrl_msg_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x303_do_ctrl_msg_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x303_do_ctrl_msg_received = FALSE;

hs_BOOL PT_CAN_0x304_do_ctrl_msg_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x304_do_ctrl_msg_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x304_do_ctrl_msg_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x304_do_ctrl_msg_received = FALSE;

hs_u1_t PT_CAN_0x5A4_ai_display_msg2_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x5A4_ai_display_msg2_txReady = FALSE;
hs_BOOL PT_CAN_0x5A4_ai_display_msg2_txFail = FALSE;

hs_u1_t PT_CAN_0x1B1_ai_display_msg3_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x1B1_ai_display_msg3_txReady = FALSE;
hs_BOOL PT_CAN_0x1B1_ai_display_msg3_txFail = FALSE;

hs_u1_t PT_CAN_0x2FA_ai_display_msg4_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x2FA_ai_display_msg4_txReady = FALSE;
hs_BOOL PT_CAN_0x2FA_ai_display_msg4_txFail = FALSE;

hs_u1_t PT_CAN_0x5A5_ai_display_msg2_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x5A5_ai_display_msg2_txReady = FALSE;
hs_BOOL PT_CAN_0x5A5_ai_display_msg2_txFail = FALSE;

hs_u1_t PT_CAN_0x331_ai_display_msg4_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x331_ai_display_msg4_txReady = FALSE;
hs_BOOL PT_CAN_0x331_ai_display_msg4_txFail = FALSE;

hs_u1_t PT_CAN_0x330_ai_display_msg4_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x330_ai_display_msg4_txReady = FALSE;
hs_BOOL PT_CAN_0x330_ai_display_msg4_txFail = FALSE;

hs_u1_t PT_CAN_0x32E_ai_display_msg4_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x32E_ai_display_msg4_txReady = FALSE;
hs_BOOL PT_CAN_0x32E_ai_display_msg4_txFail = FALSE;

hs_u1_t PT_CAN_0x321_ai_display_msg4_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x321_ai_display_msg4_txReady = FALSE;
hs_BOOL PT_CAN_0x321_ai_display_msg4_txFail = FALSE;

hs_u1_t PT_CAN_0x561_ai_display_msg4_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x561_ai_display_msg4_txReady = FALSE;
hs_BOOL PT_CAN_0x561_ai_display_msg4_txFail = FALSE;

hs_u1_t PT_CAN_0x551_ai_display_msg4_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x551_ai_display_msg4_txReady = FALSE;
hs_BOOL PT_CAN_0x551_ai_display_msg4_txFail = FALSE;

hs_u1_t PT_CAN_0x111_ai_display_msg4_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x111_ai_display_msg4_txReady = FALSE;
hs_BOOL PT_CAN_0x111_ai_display_msg4_txFail = FALSE;

hs_u1_t PT_CAN_0x401_ai_display_msg4_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x401_ai_display_msg4_txReady = FALSE;
hs_BOOL PT_CAN_0x401_ai_display_msg4_txFail = FALSE;

hs_u1_t PT_CAN_0x511_ai_display_msg4_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x511_ai_display_msg4_txReady = FALSE;
hs_BOOL PT_CAN_0x511_ai_display_msg4_txFail = FALSE;

hs_u1_t PT_CAN_0x521_ai_display_msg4_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x521_ai_display_msg4_txReady = FALSE;
hs_BOOL PT_CAN_0x521_ai_display_msg4_txFail = FALSE;

hs_u1_t PT_CAN_0x531_ai_display_msg4_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x531_ai_display_msg4_txReady = FALSE;
hs_BOOL PT_CAN_0x531_ai_display_msg4_txFail = FALSE;

hs_u1_t PT_CAN_0x5F1_ai_display_msg4_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x5F1_ai_display_msg4_txReady = FALSE;
hs_BOOL PT_CAN_0x5F1_ai_display_msg4_txFail = FALSE;

hs_u1_t PT_CAN_0x101_ai_display_msg4_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x101_ai_display_msg4_txReady = FALSE;
hs_BOOL PT_CAN_0x101_ai_display_msg4_txFail = FALSE;

hs_u1_t PT_CAN_0x441_ai_display_msg4_txBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x441_ai_display_msg4_txReady = FALSE;
hs_BOOL PT_CAN_0x441_ai_display_msg4_txFail = FALSE;


hs_BOOL PT_CAN_0x3A6_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x3A6_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x3A6_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x3A6_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x3F3_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x3F3_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x3F3_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x3F3_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x492_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x492_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x492_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x492_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x394_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x394_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x394_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x394_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x407_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x407_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x407_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x407_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x487_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x487_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x487_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x487_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x075_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x075_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x075_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x075_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x07E_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x07E_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x07E_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x07E_po_ctrl_msg1_received = FALSE;

hs_BOOL C_CAN_0x4FE_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t C_CAN_0x4FE_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL C_CAN_0x4FE_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL C_CAN_0x4FE_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x514_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x514_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x514_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x514_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x574_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x574_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x574_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x574_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x102_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x102_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x102_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x102_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x103_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x103_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x103_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x103_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x104_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x104_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x104_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x104_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x112_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x112_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x112_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x112_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x113_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x113_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x113_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x113_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x114_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x114_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x114_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x114_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x126_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x126_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x126_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x126_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x134_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x134_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x134_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x134_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x2C1_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x2C1_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x2C1_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x2C1_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x309_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x309_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x309_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x309_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x312_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x312_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x312_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x312_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x313_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x313_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x313_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x313_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x354_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x354_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x354_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x354_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x387_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x387_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x387_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x387_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x38D_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x38D_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x38D_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x38D_po_ctrl_msg1_received = FALSE;

hs_BOOL PT_CAN_0x38E_po_ctrl_msg1_rxOverLap = TRUE;
hs_u1_t PT_CAN_0x38E_po_ctrl_msg1_rxBuf[8] = {0,0,0,0,0,0,0,0};
hs_BOOL PT_CAN_0x38E_po_ctrl_msg1_rxTimeout = FALSE;
hs_BOOL PT_CAN_0x38E_po_ctrl_msg1_received = FALSE;


/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/
static const canMboxCfg_t RESERVED_CANMsgCfgGrp[m_RESERVED_CAN_MsgCnt] = 
{
   /*id         msgType           msgMode              intIsr         extFrame         dir              dlc         period         txFirstDelay         rxOverlap                                                 buf                                                  rxTimeout                                                received                                                txRdy                                                  txFailed                                              pack_unpcak                                                cbHook                                               txSuccessCb                                          */
    {0x2A0,     m_can_norMsg,     m_can_eventMsg,      FALSE,         FALSE,           m_can_dirTx,     8,          0,             0,                   NULL,                                                     RESERVED_CAN_0x2A0_can2WakeUp_display_msg_txBuf,     NULL,                                                    NULL,                                                   &RESERVED_CAN_0x2A0_can2WakeUp_display_msg_txReady,    &RESERVED_CAN_0x2A0_can2WakeUp_display_msg_txFail,    NULL,                                                      NULL,                                                NULL                                                      },/*0[can2WakeUp_display_msg]*/
    {0x2B5,     m_can_norMsg,     m_can_eventMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          0,             0,                   &RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rxOverLap,     RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rxBuf,     &RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rxTimeout,    &RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_received,    NULL,                                                  NULL,                                                 NULL,                                                      &RESERVED_CAN_0x2B5_can2Can_ComTestRec_msg_rx_cb,    NULL                                                      },/*1[can2Can_ComTestRec_msg]*/
    {0x2C5,     m_can_norMsg,     m_can_eventMsg,      FALSE,         FALSE,           m_can_dirTx,     8,          0,             0,                   NULL,                                                     RESERVED_CAN_0x2C5_can2Can_ComTest_msg_txBuf,        NULL,                                                    NULL,                                                   &RESERVED_CAN_0x2C5_can2Can_ComTest_msg_txReady,       &RESERVED_CAN_0x2C5_can2Can_ComTest_msg_txFail,       NULL,                                                      NULL,                                                NULL                                                      }/*2[can2Can_ComTest_msg]*/
};
static const canMboxCfg_t CLASS_CANMsgCfgGrp[m_C_CAN_MsgCnt] =
{
   /*id         msgType           msgMode              intIsr         extFrame         dir              dlc         period         txFirstDelay         rxOverlap                                                 buf                                                  rxTimeout                                                received                                                txRdy                                                  txFailed                                              pack_unpcak                                                cbHook                                               txSuccessCb                                          */
    {0x201,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          2,             0,                   NULL,                                                     C_CAN_0x201_di_display_msg1_txBuf,                  NULL,                                                    NULL,                                                   &C_CAN_0x201_di_display_msg1_txReady,                 &C_CAN_0x201_di_display_msg1_txFail,                 &C_CAN_0x201_di_display_msg1_txPack,                      NULL,                                                NULL                                                      },/*1[di_display_msg1]*/
    {0x211,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          2,             0,                   NULL,                                                     C_CAN_0x211_di_display_msg1_txBuf,                  NULL,                                                    NULL,                                                   &C_CAN_0x211_di_display_msg1_txReady,                 &C_CAN_0x211_di_display_msg1_txFail,                 &C_CAN_0x211_di_display_msg1_txPack,                      NULL,                                                NULL                                                      },/*1[di_display_msg1]*/
    {0x3A1,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          10,             0,                   NULL,                                                     C_CAN_0x3A1_di_display_msg1_txBuf,                  NULL,                                                   NULL,                                                   &C_CAN_0x3A1_di_display_msg1_txReady,                 &C_CAN_0x3A1_di_display_msg1_txFail,                 &C_CAN_0x3A1_di_display_msg1_txPack,                      NULL,                                                NULL                                                      },/*1[di_display_msg1]*/
    {0x241,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          2,             0,                   NULL,                                                     C_CAN_0x241_di_display_msg1_txBuf,                  NULL,                                                    NULL,                                                   &C_CAN_0x241_di_display_msg1_txReady,                 &C_CAN_0x241_di_display_msg1_txFail,                 &C_CAN_0x241_di_display_msg1_txPack,                      NULL,                                                NULL                                                      },/*1[di_display_msg1]*/
    {0x3B1,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          10,             0,                   NULL,                                                     C_CAN_0x3B1_di_display_msg1_txBuf,                  NULL,                                                   NULL,                                                   &C_CAN_0x3B1_di_display_msg1_txReady,                 &C_CAN_0x3B1_di_display_msg1_txFail,                 &C_CAN_0x3B1_di_display_msg1_txPack,                      NULL,                                                NULL                                                      },/*1[di_display_msg1]*/
    {0x101,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          1,             0,                   NULL,                                                     C_CAN_0x101_di_display_msg1_txBuf,                  NULL,                                                    NULL,                                                   &C_CAN_0x101_di_display_msg1_txReady,                 &C_CAN_0x101_di_display_msg1_txFail,                 &C_CAN_0x101_di_display_msg1_txPack,                      NULL,                                                NULL                                                      },/*1[di_display_msg1]*/

	{0x232,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x232_po_ctrl_msg1_rxOverLap,                     C_CAN_0x232_po_ctrl_msg1_rxBuf,                     &C_CAN_0x232_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x232_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x232_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x232_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x222,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x222_po_ctrl_msg1_rxOverLap,                     C_CAN_0x222_po_ctrl_msg1_rxBuf,                     &C_CAN_0x222_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x222_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x222_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x222_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
    {0x058,     m_can_norMsg,     m_can_eventMsg,       FALSE,         FALSE,           m_can_dirRx,     8,          0,             0,                  &C_CAN_0x058_po_ctrl_msg1_rxOverLap,     				  C_CAN_0x058_po_ctrl_msg1_rxBuf,     				 &C_CAN_0x058_po_ctrl_msg1_rxTimeout,    				 &C_CAN_0x058_po_ctrl_msg1_received,    				NULL,                                                  NULL,                                                 &C_CAN_0x058_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x058_po_ctrl_msg1_rx_cb,   				   NULL                                                      },/*1[can2Can_ComTestRec_msg]*/
    {0x3FE,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          10,            0,                  &C_CAN_0x3FE_po_ctrl_msg1_rxOverLap,                     C_CAN_0x3FE_po_ctrl_msg1_rxBuf,                     &C_CAN_0x3FE_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x3FE_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x3FE_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x3FE_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x4FE,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          100,           0,                  &C_CAN_0x4FE_po_ctrl_msg1_rxOverLap,                     C_CAN_0x4FE_po_ctrl_msg1_rxBuf,                     &C_CAN_0x4FE_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x4FE_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x4FE_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x4FE_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x0A5,     m_can_norMsg,     m_can_peMsg,      	FALSE,         FALSE,           m_can_dirRx,     8,          10,            0,                  &C_CAN_0x0A5_po_ctrl_msg1_rxOverLap,                     C_CAN_0x0A5_po_ctrl_msg1_rxBuf,                     &C_CAN_0x0A5_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x0A5_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x0A5_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x0A5_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x122,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x122_po_ctrl_msg1_rxOverLap,                     C_CAN_0x122_po_ctrl_msg1_rxBuf,                     &C_CAN_0x122_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x122_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x122_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x122_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x124,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          1,             0,                  &C_CAN_0x124_po_ctrl_msg1_rxOverLap,                     C_CAN_0x124_po_ctrl_msg1_rxBuf,                     &C_CAN_0x124_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x124_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x124_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x124_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x142,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x142_po_ctrl_msg1_rxOverLap,                     C_CAN_0x142_po_ctrl_msg1_rxBuf,                     &C_CAN_0x142_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x142_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x142_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x142_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x144,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          1,             0,                  &C_CAN_0x144_po_ctrl_msg1_rxOverLap,                     C_CAN_0x144_po_ctrl_msg1_rxBuf,                     &C_CAN_0x144_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x144_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x144_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x144_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/

	{0x1BE,     m_can_norMsg,     m_can_peMsg,      	FALSE,         FALSE,           m_can_dirRx,     8,          10,            0,                  &C_CAN_0x1BE_po_ctrl_msg1_rxOverLap,                     C_CAN_0x1BE_po_ctrl_msg1_rxBuf,                     &C_CAN_0x1BE_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x1BE_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x1BE_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x1BE_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x1C5,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          1,             0,                  &C_CAN_0x1C5_po_ctrl_msg1_rxOverLap,                     C_CAN_0x1C5_po_ctrl_msg1_rxBuf,                     &C_CAN_0x1C5_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x1C5_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x1C5_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x1C5_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x1D5,     m_can_norMsg,     m_can_peMsg,      	FALSE,         FALSE,           m_can_dirRx,     8,          10,            0,                  &C_CAN_0x1D5_po_ctrl_msg1_rxOverLap,                     C_CAN_0x1D5_po_ctrl_msg1_rxBuf,                     &C_CAN_0x1D5_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x1D5_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x1D5_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x1D5_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x212,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x212_po_ctrl_msg1_rxOverLap,                     C_CAN_0x212_po_ctrl_msg1_rxBuf,                     &C_CAN_0x212_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x212_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x212_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x212_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x219,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x219_po_ctrl_msg1_rxOverLap,                     C_CAN_0x219_po_ctrl_msg1_rxBuf,                     &C_CAN_0x219_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x219_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x219_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x219_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x21A,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x21A_po_ctrl_msg1_rxOverLap,                     C_CAN_0x21A_po_ctrl_msg1_rxBuf,                     &C_CAN_0x21A_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x21A_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x21A_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x21A_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x242,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x242_po_ctrl_msg1_rxOverLap,                     C_CAN_0x242_po_ctrl_msg1_rxBuf,                     &C_CAN_0x242_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x242_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x242_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x242_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x252,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x252_po_ctrl_msg1_rxOverLap,                     C_CAN_0x252_po_ctrl_msg1_rxBuf,                     &C_CAN_0x252_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x252_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x252_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x252_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x272,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x272_po_ctrl_msg1_rxOverLap,                     C_CAN_0x272_po_ctrl_msg1_rxBuf,                     &C_CAN_0x272_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x272_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x272_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x272_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x282,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x282_po_ctrl_msg1_rxOverLap,                     C_CAN_0x282_po_ctrl_msg1_rxBuf,                     &C_CAN_0x282_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x282_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x282_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x282_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/

	{0x2A1,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x2A1_po_ctrl_msg1_rxOverLap,                     C_CAN_0x2A1_po_ctrl_msg1_rxBuf,                     &C_CAN_0x2A1_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x2A1_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x2A1_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x2A1_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x2A2,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x2A2_po_ctrl_msg1_rxOverLap,                     C_CAN_0x2A2_po_ctrl_msg1_rxBuf,                     &C_CAN_0x2A2_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x2A2_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x2A2_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x2A2_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x2A5,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x2A5_po_ctrl_msg1_rxOverLap,                     C_CAN_0x2A5_po_ctrl_msg1_rxBuf,                     &C_CAN_0x2A5_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x2A5_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x2A5_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x2A5_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x2C2,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x2C2_po_ctrl_msg1_rxOverLap,                     C_CAN_0x2C2_po_ctrl_msg1_rxBuf,                     &C_CAN_0x2C2_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x2C2_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x2C2_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x2C2_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x2CE,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x2CE_po_ctrl_msg1_rxOverLap,                     C_CAN_0x2CE_po_ctrl_msg1_rxBuf,                     &C_CAN_0x2CE_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x2CE_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x2CE_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x2CE_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x2FE,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                  &C_CAN_0x2FE_po_ctrl_msg1_rxOverLap,                     C_CAN_0x2FE_po_ctrl_msg1_rxBuf,                     &C_CAN_0x2FE_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x2FE_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x2FE_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x2FE_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x319,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                 &C_CAN_0x319_po_ctrl_msg1_rxOverLap,                     C_CAN_0x319_po_ctrl_msg1_rxBuf,                     &C_CAN_0x319_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x319_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x319_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x319_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x341,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                 &C_CAN_0x341_po_ctrl_msg1_rxOverLap,                     C_CAN_0x341_po_ctrl_msg1_rxBuf,                     &C_CAN_0x341_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x341_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x341_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x341_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x380,     m_can_norMsg,     m_can_peMsg,      	FALSE,         FALSE,           m_can_dirRx,     8,          5,             0,                  &C_CAN_0x380_po_ctrl_msg1_rxOverLap,                     C_CAN_0x380_po_ctrl_msg1_rxBuf,                     &C_CAN_0x380_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x380_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x380_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x380_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x39E,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                 &C_CAN_0x39E_po_ctrl_msg1_rxOverLap,                     C_CAN_0x39E_po_ctrl_msg1_rxBuf,                     &C_CAN_0x39E_po_ctrl_msg1_rxTimeout,                    &C_CAN_0x39E_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &C_CAN_0x39E_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x39E_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/

};

static const canMboxCfg_t PT_CANMsgCfgGrp[m_PT_CAN_MsgCnt] = 
{
   /*id         msgType           msgMode              intIsr         extFrame         dir              dlc         period         txFirstDelay         rxOverlap                                                 buf                                                  rxTimeout                                                received                                                txRdy                                                  txFailed                                              pack_unpcak                                                cbHook                                               txSuccessCb                                          */
    {0x301,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          10,             0,                   NULL,                                                     PT_CAN_0x301_di_display_msg1_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x301_di_display_msg1_txReady,                 &PT_CAN_0x301_di_display_msg1_txFail,                 &PT_CAN_0x301_di_display_msg1_txPack,                      NULL,                                                NULL                                                      },/*1[di_display_msg1]*/
	{0x302,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                   &PT_CAN_0x302_do_ctrl_msg_rxOverLap,                      PT_CAN_0x302_do_ctrl_msg_rxBuf,                      &PT_CAN_0x302_do_ctrl_msg_rxTimeout,                     &PT_CAN_0x302_do_ctrl_msg_received,                     NULL,                                                  NULL,                                                 &PT_CAN_0x302_do_ctrl_msg_rxUnpack,                        &PT_CAN_0x302_do_ctrl_msg_rx_cb,                     NULL                                                      },/*0[do_ctrl_msg]*/
	{0x303,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                   &PT_CAN_0x303_do_ctrl_msg_rxOverLap,                      PT_CAN_0x303_do_ctrl_msg_rxBuf,                      &PT_CAN_0x303_do_ctrl_msg_rxTimeout,                     &PT_CAN_0x303_do_ctrl_msg_received,                     NULL,                                                  NULL,                                                 &PT_CAN_0x303_do_ctrl_msg_rxUnpack,                        &PT_CAN_0x303_do_ctrl_msg_rx_cb,                     NULL                                                      },/*0[do_ctrl_msg]*/
	{0x304,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                   &PT_CAN_0x304_do_ctrl_msg_rxOverLap,                      PT_CAN_0x304_do_ctrl_msg_rxBuf,                      &PT_CAN_0x304_do_ctrl_msg_rxTimeout,                     &PT_CAN_0x304_do_ctrl_msg_received,                     NULL,                                                  NULL,                                                 &PT_CAN_0x304_do_ctrl_msg_rxUnpack,                        &PT_CAN_0x304_do_ctrl_msg_rx_cb,                     NULL                                                      },/*0[do_ctrl_msg]*/
	{0x5A4,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          50,             0,                   NULL,                                                     PT_CAN_0x5A4_ai_display_msg2_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x5A4_ai_display_msg2_txReady,                 &PT_CAN_0x5A4_ai_display_msg2_txFail,                 &PT_CAN_0x5A4_ai_display_msg2_txPack,                      NULL,                                                NULL                                                      },/*5[ai_display_msg2]*/
	{0x1B1,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          2,             0,                    NULL,                                                     PT_CAN_0x1B1_ai_display_msg3_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x1B1_ai_display_msg3_txReady,                 &PT_CAN_0x1B1_ai_display_msg3_txFail,                 &PT_CAN_0x1B1_ai_display_msg2_txPack,                      NULL,                                                NULL                                                      },/*6[ai_display_msg3]*/
	{0x2FA,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          10,             0,                   NULL,                                                     PT_CAN_0x2FA_ai_display_msg4_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x2FA_ai_display_msg4_txReady,                 &PT_CAN_0x2FA_ai_display_msg4_txFail,                 &PT_CAN_0x2FA_ai_display_msg4_txPack,                      NULL,                                                NULL                                                      },/*7[ai_display_msg4]*/
	{0x5A5,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          50,             0,                   NULL,                                                     PT_CAN_0x5A5_ai_display_msg2_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x5A5_ai_display_msg2_txReady,                 &PT_CAN_0x5A5_ai_display_msg2_txFail,                 &PT_CAN_0x5A5_ai_display_msg2_txPack,                      NULL,                                                NULL                                                      },/*5[ai_display_msg2]*/
	{0x331,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          10,             0,                   NULL,                                                     PT_CAN_0x331_ai_display_msg4_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x331_ai_display_msg4_txReady,                 &PT_CAN_0x331_ai_display_msg4_txFail,                 &PT_CAN_0x331_ai_display_msg4_txPack,                      NULL,                                                NULL                                                      },/*7[ai_display_msg4]*/
	{0x330,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          10,             0,                   NULL,                                                     PT_CAN_0x330_ai_display_msg4_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x330_ai_display_msg4_txReady,                 &PT_CAN_0x330_ai_display_msg4_txFail,                 &PT_CAN_0x330_ai_display_msg4_txPack,                      NULL,                                                NULL                                                      },/*7[ai_display_msg4]*/

	{0x32E,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          10,             0,                   NULL,                                                     PT_CAN_0x32E_ai_display_msg4_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x32E_ai_display_msg4_txReady,                 &PT_CAN_0x32E_ai_display_msg4_txFail,                 &PT_CAN_0x32E_ai_display_msg4_txPack,                      NULL,                                                NULL                                                      },/*7[ai_display_msg4]*/
	{0x321,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          10,             0,                   NULL,                                                     PT_CAN_0x321_ai_display_msg4_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x321_ai_display_msg4_txReady,                 &PT_CAN_0x321_ai_display_msg4_txFail,                 &PT_CAN_0x321_ai_display_msg4_txPack,                      NULL,                                                NULL                                                      },/*7[ai_display_msg4]*/
	{0x561,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          100,             0,                  NULL,                                                     PT_CAN_0x561_ai_display_msg4_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x561_ai_display_msg4_txReady,                 &PT_CAN_0x561_ai_display_msg4_txFail,                 &PT_CAN_0x561_ai_display_msg4_txPack,                      NULL,                                                NULL                                                      },/*7[ai_display_msg4]*/
	{0x551,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          1000,             0,                 NULL,                                                     PT_CAN_0x551_ai_display_msg4_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x551_ai_display_msg4_txReady,                 &PT_CAN_0x551_ai_display_msg4_txFail,                 &PT_CAN_0x551_ai_display_msg4_txPack,                      NULL,                                                NULL                                                      },/*7[ai_display_msg4]*/
	{0x111,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          1,             0,                    NULL,                                                     PT_CAN_0x111_ai_display_msg4_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x111_ai_display_msg4_txReady,                 &PT_CAN_0x111_ai_display_msg4_txFail,                 &PT_CAN_0x111_ai_display_msg4_txPack,                      NULL,                                                NULL                                                      },/*7[ai_display_msg4]*/
	{0x401,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          20,             0,                   NULL,                                                     PT_CAN_0x401_ai_display_msg4_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x401_ai_display_msg4_txReady,                 &PT_CAN_0x401_ai_display_msg4_txFail,                 &PT_CAN_0x401_ai_display_msg4_txPack,                      NULL,                                                NULL                                                      },/*7[ai_display_msg4]*/
	{0x511,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          1000,             0,                 NULL,                                                     PT_CAN_0x511_ai_display_msg4_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x511_ai_display_msg4_txReady,                 &PT_CAN_0x511_ai_display_msg4_txFail,                 &PT_CAN_0x511_ai_display_msg4_txPack,                      NULL,                                                NULL                                                      },/*7[ai_display_msg4]*/
	{0x521,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          100,             0,                  NULL,                                                     PT_CAN_0x521_ai_display_msg4_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x521_ai_display_msg4_txReady,                 &PT_CAN_0x521_ai_display_msg4_txFail,                 &PT_CAN_0x521_ai_display_msg4_txPack,                      NULL,                                                NULL                                                      },/*7[ai_display_msg4]*/
	{0x531,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          100,             0,                  NULL,                                                     PT_CAN_0x531_ai_display_msg4_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x531_ai_display_msg4_txReady,                 &PT_CAN_0x531_ai_display_msg4_txFail,                 &PT_CAN_0x531_ai_display_msg4_txPack,                      NULL,                                                NULL                                                      },/*7[ai_display_msg4]*/
	{0x5F1,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          100,             0,                  NULL,                                                     PT_CAN_0x5F1_ai_display_msg4_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x5F1_ai_display_msg4_txReady,                 &PT_CAN_0x5F1_ai_display_msg4_txFail,                 &PT_CAN_0x5F1_ai_display_msg4_txPack,                      NULL,                                                NULL                                                      },/*7[ai_display_msg4]*/

	{0x101,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          1,             0,                    NULL,                                                     PT_CAN_0x101_ai_display_msg4_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x101_ai_display_msg4_txReady,                 &PT_CAN_0x101_ai_display_msg4_txFail,                 &PT_CAN_0x101_ai_display_msg4_txPack,                      NULL,                                                NULL                                                      },/*7[ai_display_msg4]*/
	{0x441,     m_can_norMsg,     m_can_periodMsg,     FALSE,         FALSE,           m_can_dirTx,     8,          10,             0,                   NULL,                                                     PT_CAN_0x441_ai_display_msg4_txBuf,                  NULL,                                                    NULL,                                                   &PT_CAN_0x441_ai_display_msg4_txReady,                 &PT_CAN_0x441_ai_display_msg4_txFail,                 &PT_CAN_0x441_ai_display_msg4_txPack,                      NULL,                                                NULL                                                      },/*7[ai_display_msg4]*/
	{0x3A6,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                  &PT_CAN_0x3A6_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x3A6_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x3A6_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x3A6_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x3A6_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x3A6_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
    {0x731,     m_can_udsMsg,     m_can_eventMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          0,             0,                    &PT_CAN_0x731_msg_rxOverLap,        				   	   PT_CAN_0x731_msg_rxBuf,        						&PT_CAN_0x731_msg_rxTimeout,       					 	 &PT_CAN_0x731_msg_received,       					 	 NULL,                                                  NULL,                                                 NULL,                                                      &PT_CAN_0x731_msg_rx_cb,       					  NULL                                                      },/*2[can3Can_ComTestRec_msg]*/
    {0x7E7,     m_can_udsMsg,     m_can_eventMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          0,             0,                    &PT_CAN_0x7E7_msg_rxOverLap,        				   	   PT_CAN_0x7E7_msg_rxBuf,        						&PT_CAN_0x7E7_msg_rxTimeout,       					 	 &PT_CAN_0x7E7_msg_received,       					 	 NULL,                                                  NULL,                                                 NULL,                                                      &PT_CAN_0x7E7_msg_rx_cb,      					  NULL                                                      },/*2[can3Can_ComTestRec_msg]*/
    {0x7DF,     m_can_udsMsg,     m_can_eventMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          0,             0,                    &PT_CAN_0x7DF_msg_rxOverLap,        				   	   PT_CAN_0x7DF_msg_rxBuf,        						&PT_CAN_0x7DF_msg_rxTimeout,       					 	 &PT_CAN_0x7DF_msg_received,       					 	 NULL,                                                  NULL,                                                 NULL,                                                      &PT_CAN_0x7DF_msg_rx_cb,       					  NULL                                                      },/*2[can3Can_ComTestRec_msg]*/
	{0x79A,     m_can_udsMsg,     m_can_eventMsg,      FALSE,         FALSE,           m_can_dirTx,     8,          0,             0,                    NULL,                                  				   PT_CAN_0x79A_msg_txBuf,         					NULL,                                 					 NULL,                                					 &PT_CAN_0x79A_msg_txReady,        						&PT_CAN_0x79A_msg_txFail,        					  NULL,                                   					 NULL,                             					  NULL                                   },/*29[UDS_TX]*/
	{0x797,     m_can_udsMsg,     m_can_eventMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          0,             0,                    &PT_CAN_0x797_msg_rxOverLap,         					   PT_CAN_0x797_msg_rxBuf,         						&PT_CAN_0x797_msg_rxTimeout,        					 &PT_CAN_0x797_msg_received,        					 NULL,                               					NULL,                              					  NULL,                                   					 NULL,                             					  NULL                                   },/*30[UDS_RX]*/

	{0x3F3,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          100,             0,                 &PT_CAN_0x3F3_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x3F3_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x3F3_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x3F3_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x3F3_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x3F3_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x492,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          50,             0,                  &PT_CAN_0x492_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x492_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x492_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x492_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x492_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x492_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x394,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                  &PT_CAN_0x394_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x394_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x394_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x394_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x394_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x394_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x407,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          20,             0,                  &PT_CAN_0x407_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x407_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x407_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x407_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x407_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x407_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/

	{0x487,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          50,             0,                  &PT_CAN_0x487_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x487_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x487_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x487_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x487_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x487_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x075,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                  &PT_CAN_0x075_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x075_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x075_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x075_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x075_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x075_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x07E,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                  &PT_CAN_0x07E_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x07E_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x07E_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x07E_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x07E_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x07E_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x4FE,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          100,             0,                 &C_CAN_0x4FE_po_ctrl_msg1_rxOverLap,                      C_CAN_0x4FE_po_ctrl_msg1_rxBuf,                      &C_CAN_0x4FE_po_ctrl_msg1_rxTimeout,                     &C_CAN_0x4FE_po_ctrl_msg1_received,                     NULL,                                                  NULL,                                                 &C_CAN_0x4FE_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x4FE_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x514,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          100,             0,                 &PT_CAN_0x514_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x514_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x514_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x514_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x514_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x514_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x574,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          100,             0,                 &PT_CAN_0x574_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x574_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x574_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x574_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x574_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x574_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
    {0x632,     m_can_nmMsg,     m_can_eventMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          0,             0,                    &PT_CAN_0x632_msg_rxOverLap,        				   	   PT_CAN_0x632_msg_rxBuf,        						&PT_CAN_0x632_msg_rxTimeout,       					 	 &PT_CAN_0x632_msg_received,       					 	 NULL,                                                  NULL,                                                 NULL,                                                      &PT_CAN_0x632_msg_rx_cb,       					  NULL                                                      },/*2[can3Can_ComTestRec_msg]*/
    {0x634,     m_can_nmMsg,     m_can_eventMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          0,             0,                    &PT_CAN_0x634_msg_rxOverLap,        				   	   PT_CAN_0x634_msg_rxBuf,        						&PT_CAN_0x634_msg_rxTimeout,       					 	 &PT_CAN_0x634_msg_received,       					 	 NULL,                                                  NULL,                                                 NULL,                                                      &PT_CAN_0x634_msg_rx_cb,       					  NULL                                                      },/*2[can3Can_ComTestRec_msg]*/
    {0x637,     m_can_nmMsg,     m_can_eventMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          0,             0,                    &PT_CAN_0x637_msg_rxOverLap,        				   	   PT_CAN_0x637_msg_rxBuf,        						&PT_CAN_0x637_msg_rxTimeout,       					 	 &PT_CAN_0x637_msg_received,       					 	 NULL,                                                  NULL,                                                 NULL,                                                      &PT_CAN_0x637_msg_rx_cb,       					  NULL                                                      },/*2[can3Can_ComTestRec_msg]*/
    {0x63E,     m_can_nmMsg,     m_can_eventMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          0,             0,                    &PT_CAN_0x63E_msg_rxOverLap,        				   	   PT_CAN_0x63E_msg_rxBuf,        						&PT_CAN_0x63E_msg_rxTimeout,       					 	 &PT_CAN_0x63E_msg_received,       					 	 NULL,                                                  NULL,                                                 NULL,                                                      &PT_CAN_0x63E_msg_rx_cb,       					  NULL                                                      },/*2[can3Can_ComTestRec_msg]*/

	{0x102,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          1,             0,                   &PT_CAN_0x102_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x102_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x102_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x102_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x102_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x102_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x103,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          1,             0,                   &PT_CAN_0x103_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x103_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x103_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x103_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x103_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x103_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x104,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          1,             0,                   &PT_CAN_0x104_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x104_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x104_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x104_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x104_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x104_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x112,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          1,             0,                   &PT_CAN_0x112_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x112_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x112_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x112_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x112_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x112_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x113,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          1,             0,                   &PT_CAN_0x113_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x113_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x113_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x113_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x113_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x113_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x114,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          1,             0,                   &PT_CAN_0x114_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x114_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x114_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x114_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x114_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x114_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x126,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                  &PT_CAN_0x126_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x126_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x126_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x126_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x126_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x126_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x134,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          1,             0,                   &PT_CAN_0x134_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x134_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x134_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x134_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x134_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x134_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x2C1,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                   &PT_CAN_0x2C1_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x2C1_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x2C1_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x2C1_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x2C1_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x2C1_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x309,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          2,             0,                   &PT_CAN_0x309_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x309_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x309_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x309_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x309_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x309_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/

	{0x312,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                  &PT_CAN_0x312_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x312_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x312_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x312_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x312_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x312_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x313,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                  &PT_CAN_0x313_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x313_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x313_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x313_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x313_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x313_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x354,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                  &PT_CAN_0x354_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x354_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x354_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x354_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x354_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x354_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x387,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                  &PT_CAN_0x387_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x387_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x387_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x387_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x387_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x387_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x38D,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                  &PT_CAN_0x38D_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x38D_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x38D_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x38D_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x38D_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x38D_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x38E,     m_can_norMsg,     m_can_peMsg,      	FALSE,         FALSE,           m_can_dirRx,     8,          5,             0,                   &PT_CAN_0x38E_po_ctrl_msg1_rxOverLap,                     PT_CAN_0x38E_po_ctrl_msg1_rxBuf,                     &PT_CAN_0x38E_po_ctrl_msg1_rxTimeout,                    &PT_CAN_0x38E_po_ctrl_msg1_received,                    NULL,                                                  NULL,                                                 &PT_CAN_0x38E_po_ctrl_msg1_rxUnpack,                       &PT_CAN_0x38E_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/
	{0x39E,     m_can_norMsg,     m_can_periodMsg,      FALSE,         FALSE,           m_can_dirRx,     8,          10,             0,                  &C_CAN_0x39E_po_ctrl_msg1_rxOverLap,                      C_CAN_0x39E_po_ctrl_msg1_rxBuf,                      &C_CAN_0x39E_po_ctrl_msg1_rxTimeout,                     &C_CAN_0x39E_po_ctrl_msg1_received,                     NULL,                                                  NULL,                                                 &C_CAN_0x39E_po_ctrl_msg1_rxUnpack,                       &C_CAN_0x39E_po_ctrl_msg1_rx_cb,                    NULL                                                      },/*10[po_ctrl_msg1]*/

};


/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/
const hs_u1_t canModuleNum = m_canMod_cnt;
const canMboxCfgInfo_t canMsgCfgInfoGrp[m_canMod_cnt] = 
{
    {CLASS_CANMsgCfgGrp,m_C_CAN_MsgCnt},
    {RESERVED_CANMsgCfgGrp,m_RESERVED_CAN_MsgCnt},
    {PT_CANMsgCfgGrp,m_PT_CAN_MsgCnt}
};

canMsgScheData_t * const canMsgScheCfgInfoGrp[m_canMod_cnt] = 
{
    CLASS_CANMsgScheData,RESERVED_CANMsgScheData,PT_CANMsgScheData
};


canMsgSortInfo_t * const canMsgSortInfoGrp[m_canMod_cnt] = 
{
    CLASS_CANMsgSortOrder,RESERVED_CANMsgSortOrder,PT_CANMsgSortOrder
};

uint16_t * const canTxDynMsgIndxBuf[m_canMod_cnt] =
{
	NULL,NULL,NULL
};


const hs_u1_t udsRxCanBus = m_can_udsRxBus;
const hs_u1_t udsTxCanBus = m_can_udsTxBus;
const hs_u1_t ccpRxCanBus = m_can_ccpRxBus;
const hs_u1_t ccpTxCanBus = m_can_ccpTxBus;
const hs_u2_t udsRxMsgIndx = m_can_udsRxMsgIndx;
const hs_u2_t udsTxMsgIndx = m_can_udsTxMsgIndx;
const hs_u2_t ccpRxMsgIndx = m_can_ccpRxMsgIndx;
const hs_u2_t ccpTxMsgIndx = m_can_ccpTxMsgIndx;
const hs_u1_t canBaudCfg[m_canMod_cnt] = {m_canBaud_500kps,m_canBaud_500kps,m_canBaud_500kps};


/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/


/* ---- Source switch off ---------------------------------------------------- */

/*--- End of file ------------------------------------------------------------*/
