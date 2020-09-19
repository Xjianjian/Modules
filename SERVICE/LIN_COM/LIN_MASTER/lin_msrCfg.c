

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "Lin.h"
#include "lin_msrCfg.h"
/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

static void lin_msr_pack_msg(uint8_t module,uint8_t msgid);
static void lin_msr_tx_cb(uint8_t module,uint8_t msgid);
static void lin_msr_unpack_msg(uint8_t module,uint8_t msgid);
static void lin_msr_rx_cb(uint8_t module,uint8_t msgid);

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
static lin_msr_txbuf_t lin0_msr_txbuf[LIN0_TX_MSG_CNT];
static lin_msr_rxbuf_t lin0_msr_rxbuf[LIN0_RX_MSG_CNT];

static lin_msr_txbuf_t lin1_msr_txbuf[LIN1_TX_MSG_CNT];
static lin_msr_rxbuf_t lin1_msr_rxbuf[LIN1_RX_MSG_CNT];

/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/
const lin_msr_msg_cfg_t lin0_msr_txmsg[LIN0_TX_MSG_CNT] =
{
	/*id,		tx,		dlc,tbuf,				rbuf	  			pack_unpack,		cb*/
	{0x13,		TRUE,	2,	&lin0_msr_txbuf[0], NULL_PTR, 			lin_msr_pack_msg,	lin_msr_tx_cb},
	{0x14,		TRUE,	2,	&lin0_msr_txbuf[1], NULL_PTR, 			lin_msr_pack_msg,	lin_msr_tx_cb},
};

const lin_msr_msg_cfg_t lin0_msr_rxmsg[LIN0_RX_MSG_CNT] =
{
	/*id,		tx,		dlc,tbuf,     			rbuf,	  			pack_unpack,	    cb*/
	{0x3,		FALSE,	2,	NULL_PTR, 			&lin0_msr_rxbuf[0], lin_msr_unpack_msg, lin_msr_rx_cb},
	{0x4,		FALSE,	2,	NULL_PTR, 			&lin0_msr_rxbuf[1], lin_msr_unpack_msg, lin_msr_rx_cb},

};

const lin_msr_schedule_t lin0_schedule[LIN0_SCHEDULE_CNT] =
{  /*tx, tx indx,	gap time	*/
	{1,		0,		0x01},
	{1,		1,		0x01},
	{0,		0,		0x01},
	{0,		1,		0x01},
};


const lin_msr_msg_cfg_t lin1_msr_txmsg[LIN1_TX_MSG_CNT] =
{
	/*id,		tx,		dlc,tbuf,				 rbuf,				pack_unpack,		cb*/
	{0x15,		TRUE,	4,	&lin1_msr_txbuf[0],  NULL_PTR,			lin_msr_pack_msg,	lin_msr_tx_cb},
};

const lin_msr_msg_cfg_t lin1_msr_rxmsg[LIN1_RX_MSG_CNT] =
{
	/*id,		tx,		dlc,tbuf, 				rbuf,	 			 pack_unpack,	    cb*/
	{0x16,		FALSE,	8,	NULL_PTR, 			&lin1_msr_rxbuf[0],  lin_msr_unpack_msg,lin_msr_rx_cb},
};

const lin_msr_schedule_t lin1_schedule[LIN1_SCHEDULE_CNT] =
{  /*tx, tx indx,	gap time	*/
	{1,		0,		0x01},
	{0,		0,		0x04},
};

const lin_msr_t lin_msr[LIN_MODULE_MASTER_NUM]=
{
	{	0,
		1,
		LIN0_TX_MSG_CNT,
		LIN0_RX_MSG_CNT,
		lin0_msr_txmsg,
		lin0_msr_rxmsg,
		lin0_schedule,
	},
	{	1,
		1,
		LIN1_TX_MSG_CNT ,
		LIN1_RX_MSG_CNT,
		lin1_msr_txmsg,
		lin1_msr_rxmsg,
		lin1_schedule,
	},
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
static void lin_msr_pack_msg(uint8_t module,uint8_t msgid)
{
	uint8_t tx_idx;


	switch(module)
	{
		case 0:
		{
			switch(msgid)
			{
				case 0x13:
				{
					//lin_msr[module].txmsg[tx_idx].tbuf->txbuf[0] = lin_msr[module].rxmsg[tx_idx].tbuf->txbuf[0];
				}
				break;
				case 0x14:
				{
					//lin_msr[module].txmsg[tx_idx].tbuf->txbuf[0] = lin_msr[module].rxmsg[tx_idx].tbuf->txbuf[0];
				}
				break;
				default:
				{

				}
				break;
			}
		}
		break;
		case 1:
		{

		}
		break;
		default:
		break;
	}
}

static void lin_msr_tx_cb(uint8_t module,uint8_t msgid)
{

}


static void lin_msr_unpack_msg(uint8_t module,uint8_t msgid)
{

}

static void lin_msr_rx_cb(uint8_t module,uint8_t msgid)
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




