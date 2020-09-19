
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "OS_Types.h"
#include "utils.h"

#include "Lin.h"
#include "LinCfg.h"

#include "lin_msr.h"
/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/
static volatile uint16_t s_lin_schedule_item_index[LIN_MODULE_MASTER_NUM] = {0};
static uint16_t s_gap_tick[LIN_MODULE_MASTER_NUM] =  {0};

/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at RAM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/
static void lin_msr_rx_msg_indication(uint8_t module, uint8_t msg_id, uint8_t * buf, uint8_t dlc);
static void lin_msr_rx_header_indication(uint8_t module, uint8_t msg_id);
/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void lin_msr_init(void)
{
	uint8_t module;
	for(module = 0;module < LIN_MODULE_MASTER_NUM;module++)
	{
		if(lin_msr[module].isen)
		{
			lin_register_rx_msg_cb(lin_msr[module].module,lin_msr_rx_msg_indication);
		}
	}
}

void lin_msr_task(void)
{
	uint8_t module;
	uint8_t msg_indx = 0;
	uint8_t msg_id = 0;
	uint8_t* BUF;
	uint8_t DLC;

	for(module = 0;module < LIN_MODULE_MASTER_NUM;module++)
	{
		if(!lin_msr[module].isen)
		{
			continue;
		}

		if(s_gap_tick[module] > 0)
		{
			s_gap_tick[module]--;
		}

		if(0 == s_gap_tick[module])
		{
			msg_indx = lin_msr[module].scheduleList[s_lin_schedule_item_index[module]].p_msg_indx;
			if(lin_msr[module].scheduleList[s_lin_schedule_item_index[module]].is_tx)
			{
				msg_id = lin_msr[module].txmsg[msg_indx].id;

				if(NULL_PTR != lin_msr[module].txmsg[msg_indx].pack_unpack)
				{
					lin_msr[module].txmsg[msg_indx].pack_unpack(module,msg_id);
				}

				if(NULL_PTR !=lin_msr[module].txmsg[msg_indx].tbuf)
				{
					BUF = lin_msr[module].txmsg[msg_indx].tbuf->txbuf;
					DLC = lin_msr[module].txmsg[msg_indx].dlc;
					lin_msr[module].txmsg[msg_indx].tbuf->txOk = \
											lin_master_sendMsg(module,msg_id,BUF,DLC,FALSE);
				}

				if(NULL_PTR != lin_msr[module].txmsg[msg_indx].cb)
				{
					lin_msr[module].txmsg[msg_indx].cb(module,msg_id);
				}
			}
			else
			{
				if(NULL_PTR != lin_msr[module].rxmsg[msg_indx].rbuf)
				{
					if(TRUE == lin_msr[module].rxmsg[msg_indx].rbuf->received)
					{
						lin_msr[module].rxmsg[msg_indx].rbuf->received = FALSE;
						lin_msr[module].rxmsg[msg_indx].rbuf->timeout = FALSE;
					}
					else
					{
						lin_msr[module].rxmsg[msg_indx].rbuf->timeout = TRUE;
					}
				}
				msg_id = lin_msr[module].rxmsg[msg_indx].id;
				DLC = lin_msr[module].rxmsg[msg_indx].dlc;
				lin_master_sendHeader(module,msg_id,DLC);
			}
			
			s_lin_schedule_item_index[module] += 1;
			if(s_lin_schedule_item_index[module] >= (lin_msr[module].rxnum+lin_msr[module].txnum))
			{
				s_lin_schedule_item_index[module] = 0;
			}
            s_gap_tick[module] = lin_msr[module].scheduleList[s_lin_schedule_item_index[module]].p_time_gap;
		}
	}
}


/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static void lin_msr_rx_msg_indication(uint8_t module, uint8_t msg_id, uint8_t * buf, uint8_t dlc)
{
	uint8_t msg_indx = 0;
	uint8_t* BUF;
	uint8_t DLC;

	if(msg_id > 0x3F) /* LIN ID is 6bit, Max value is 0x3F */
	{
		return;
	}

	for (msg_indx = 0; msg_indx < lin_msr[module].rxnum; msg_indx++)
	{
		if (msg_id == lin_msr[module].rxmsg[msg_indx].id)
		{
			if(NULL_PTR != lin_msr[module].rxmsg[msg_indx].rbuf)
			{
				lin_msr[module].rxmsg[msg_indx].rbuf->received = TRUE;
				lin_msr[module].rxmsg[msg_indx].rbuf->timeout = FALSE;
				BUF = lin_msr[module].rxmsg[msg_indx].rbuf->rxbuf;
				DLC = lin_msr[module].rxmsg[msg_indx].dlc;
				if(NULL_PTR != BUF && DLC == dlc)
				{
					util_memoryCpy8(buf, BUF, dlc);
					if(NULL_PTR != lin_msr[module].rxmsg[msg_indx].pack_unpack)
					{
						lin_msr[module].rxmsg[msg_indx].pack_unpack(module,msg_id);
					}

					if(NULL_PTR != lin_msr[module].rxmsg[msg_indx].cb)
					{
						lin_msr[module].rxmsg[msg_indx].cb(module,msg_id);
					}
				}
			}

			break;
		}
	}
}

static void lin_msr_rx_header_indication(uint8_t module, uint8_t msg_id)
{
	uint8_t msg_i;

	if(msg_id > 0x3F) /* LIN ID is 6bit, Max value is 0x3F */
	{
		return;
	}
}

/* ---- Source switch off ---------------------------------------------------- */
/*--- End of file ------------------------------------------------------------*/





