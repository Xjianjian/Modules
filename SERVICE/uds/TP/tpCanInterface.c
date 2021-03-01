 /*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:tpCanInterface.c
* Description:
* REQ IDs:
* History:
* 2017-02-07,  Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#define TP_CAN_INTERFACE_C_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "tpCanInterface.h"
#include "canCfg_export.h"
#include "nt_cfg.h"
#include "uds.h"


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


/*------------------------------------------------------------------------------
                      Implementation of global functio   ns
------------------------------------------------------------------------------*/
bool nt_pdu_transmit(uint8_t * nt_pdu_buf, uint8_t buf_len)
{
	bool transmitOk = FALSE;
	if(8 == buf_len)
	{
		transmitOk = hcan_ntTransmit(nt_pdu_buf);
	}
	return transmitOk;
}

/*
 * Function:nt_request_cb
 * Param<void>:
 * Return<void>:
 * REQ IDs:
 * Discription: Callback function
 *              This function is used to call upper layer (eg. UDS service), a integrity request was be received by NT.
 * Note:
*/
void nt_request_cb(void)
{
    uds_requestProcess();
}


void nt_txToCb(void)
{
#if PARAM_NT_TM_As > 0
	nt_pdu_send_fail_cb();
#endif
}

void nt_txConfirmCb(void)
{
#if PARAM_NT_TM_As > 0
	nt_pdu_send_ok_cb();
#endif
}



void nt_recIndication(uint32_t rxId, uint8_t rxDLC,uint8_t * rxBuf)
{
	if(m_udsRx_phyId == rxId)
	{
		nt_pdu_receive_cb(FALSE, rxBuf, rxDLC);
	}
	else if(m_udsRx_funId == rxId)
	{
		nt_pdu_receive_cb(TRUE, rxBuf, rxDLC);
	}
	else
	{

	}
}


void nt_busErrInit(void)
{
	hcan_ntTransmitErrInit();
}





/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/



/* ---- Source switch off ---------------------------------------------------- */
#undef TP_CAN_INTERFACE_C_
/*--- End of file ------------------------------------------------------------*/






