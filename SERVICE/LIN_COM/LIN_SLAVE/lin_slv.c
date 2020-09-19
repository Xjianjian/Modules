

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "OS_Types.h"
#include "utils.h"

#include "Lin.h"
#include "LinCfg.h"

#include "lin_slv.h"
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
static void lin_slv_rx_msg_indication(uint8_t module, uint8_t msg_id, uint8_t * buf, uint8_t dlc);
static void lin_slv_rx_header_indication(uint8_t module, uint8_t msg_id);
/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void lin_slv_init(void)
{
	uint8_t module;

	for(module = 0;module < LIN_SLAVE_MODULE_NUM;module++)
	{
		if(lin_slv[module].isen)
		{
			lin_register_rx_msg_cb(lin_slv[module].module,lin_slv_rx_msg_indication);
			lin_register_rx_head_cb(lin_slv[module].module,lin_slv_rx_header_indication);
		}
	}
}

void lin_slv_task(void)
{

}


/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static void lin_slv_rx_msg_indication(uint8_t module, uint8_t msg_id, uint8_t * buf, uint8_t dlc)
{
	uint16_t msg_indx = 0;
	uint8_t* BUF;
	uint8_t DLC;

	if(msg_id > 0x3F) /* LIN ID is 6bit, Max value is 0x3F */
	{
		return;
	}

	for (msg_indx = 0; msg_indx < lin_slv[module].rxnum; msg_indx++)
	{
		if (msg_id == lin_slv[module].rxmsg[msg_indx].id)
		{
			if(NULL_PTR != lin_slv[module].rxmsg[msg_indx].rbuf)
			{
				lin_slv[module].rxmsg[msg_indx].rbuf->received = TRUE;
				lin_slv[module].rxmsg[msg_indx].rbuf->timeout = FALSE;
				BUF = lin_slv[module].rxmsg[msg_indx].rbuf->rxbuf;
				DLC = lin_slv[module].rxmsg[msg_indx].dlc;
				if(NULL_PTR != BUF && DLC == dlc)
				{
					util_memoryCpy8(buf, BUF, dlc);
					if(NULL_PTR != lin_slv[module].rxmsg[msg_indx].pack_unpack)
					{
						lin_slv[module].rxmsg[msg_indx].pack_unpack(module,msg_id);
					}

					if(NULL_PTR != lin_slv[module].rxmsg[msg_indx].cb)
					{
						lin_slv[module].rxmsg[msg_indx].cb(module,msg_id);
					}
				}
			}

			break;
		}
	}
}

static void lin_slv_rx_header_indication(uint8_t module, uint8_t msg_id)
{
	uint8_t msg_indx;

	if(msg_id > 0x3F) /* LIN ID is 6bit, Max value is 0x3F */
	{
		return;
	}

	for (msg_indx = 0; msg_indx < lin_slv[module].txnum; msg_indx++)
	{
		if (msg_id == lin_slv[module].txmsg[msg_indx].id)
		{
			if(NULL_PTR != lin_slv[module].txmsg[msg_indx].cb)
			{
				lin_slv[module].txmsg[msg_indx].cb(module,msg_id);
			}

			break;
		}
	}
}




