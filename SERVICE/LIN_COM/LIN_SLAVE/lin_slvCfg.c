

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "Lin.h"
#include "lin_slvCfg.h"
/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

static void lin_slv_pack_msg(uint8_t module,uint8_t msgid);
static void lin_slv_unpack_msg(uint8_t module,uint8_t msgid);
static void lin_slv_rx_msg_cb(uint8_t module,uint8_t msgid);
static void lin_slv_tx_msg_cb(uint8_t module,uint8_t msgid);
/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/
static lin_slv_txbuf_t lin0_slv_txbuf[LIN0_SLV_TX_MSG_CNT];
static lin_slv_rxbuf_t lin0_slv_rxbuf[LIN0_SLV_RX_MSG_CNT];

static lin_slv_txbuf_t lin1_slv_txbuf[LIN1_SLV_TX_MSG_CNT];
static lin_slv_rxbuf_t lin1_slv_rxbuf[LIN1_SLV_RX_MSG_CNT];

/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/
const lin_slv_msg_t lin0_slv_txmsg[LIN0_SLV_TX_MSG_CNT] =
{
	/*id,		tx,		dlc,  tbuf,				rbuf,				pack_unpack,		cb*/
	{0x3A,		TRUE,	8,	  &lin0_slv_txbuf[0],NULL_PTR,			lin_slv_pack_msg,	lin_slv_tx_msg_cb},
};

const lin_slv_msg_t lin0_slv_rxmsg[LIN0_SLV_RX_MSG_CNT] =
{
	/*id,		tx,		dlc, tbuf, 				rbuf,				pack_unpack,		cb*/
	{0x31,		FALSE,	8,	 NULL_PTR, 			&lin0_slv_rxbuf[0],	lin_slv_unpack_msg,	lin_slv_rx_msg_cb}
};

const lin_slv_msg_t lin1_slv_txmsg[LIN1_SLV_TX_MSG_CNT] =
{
	/*id,		tx,		dlc, tbuf, 				rbuf,				pack_unpack,	 	cb*/
	{0x3A,		TRUE,	8,	 &lin1_slv_txbuf[0],NULL_PTR,			lin_slv_pack_msg,	lin_slv_tx_msg_cb},
};

const lin_slv_msg_t lin1_slv_rxmsg[LIN0_SLV_RX_MSG_CNT] =
{
	/*id,		tx,		dlc,  tbuf, 			rbuf,				pack_unpack,		cb*/
	{0x31,		FALSE,	8,	  NULL_PTR, 		&lin1_slv_rxbuf[0], lin_slv_unpack_msg,	lin_slv_rx_msg_cb}
};

const lin_slv_t lin_slv[LIN_SLAVE_MODULE_NUM]  =
{
	{	0,
		1,
		LIN0_SLV_TX_MSG_CNT,
		LIN0_SLV_RX_MSG_CNT,
		lin0_slv_txmsg,
		lin0_slv_rxmsg,
	},
	{	1,
		0,
		LIN1_SLV_TX_MSG_CNT,
		LIN1_SLV_RX_MSG_CNT,
		lin1_slv_txmsg,
		lin1_slv_rxmsg,
	}
};

/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
static void lin_slv_pack_msg(uint8_t module,uint8_t msgid)
{

}

static void lin_slv_unpack_msg(uint8_t module,uint8_t msgid)
{

}

static void lin_slv_rx_msg_cb(uint8_t module,uint8_t msgid)
{

}

static void lin_slv_tx_msg_cb(uint8_t module,uint8_t msgid)
{
	uint8_t ID = 0x3A;
	uint8_t	BUF[8] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};
	uint8_t DLC = 8;
	lin_slave_sendMsg(0,ID,BUF,DLC,FALSE);
}

/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/



/* ---- Source switch off ---------------------------------------------------- */

/*--- End of file ------------------------------------------------------------*/




