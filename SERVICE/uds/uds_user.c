/*
* Copyright (c) 2016,SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File: uds_user.c
* Description: 
* History: 
* 2016-5-23, 1007040, original
*/


#include "uds_types.h"
#include "global_var.h"
#include "uds_cfg.h"
#include "uds.h"
#include "utils.h"
#include "nt_export.h"
#include "dataLink.h"
#include "uds_user.h"
/* ------------------- User define variables and functions for UDS ------------------ */

/* User export variables */
/* DID related temp export variables */
uint8_t didTemp_actSession = 0x01;
uint16_t didTemp_waterTemprature = 0x0000;
uint16_t didTemp_batVol = 0x0000;
uint16_t didTemp_qTemp1 = 0x0000;
uint16_t didTemp_writeTest = 0x0000;




bool normalRxEnable = TRUE;
bool normalTxEnable = TRUE;
bool networkManagerRxEnable = TRUE;
bool networkManagerTxEnable = TRUE;

bool uds_dtcSettingOn = TRUE;

bool uds_prog_allow_flag = 0;

/* User private function declare */
static void ecuResetTrig(void);

/* ----------------- Function to fit UDS service ---------------- */

/* Service: ECU reset */
void serv_ecuReset(void)
{
	uint8_t resetType = 0xFF;
	if(0x01 == uds_remainRequestDLC())
	{
		resetType = uds_popRequestData(0x00);
		switch(resetType)
		{
		    case servm_resetType_hardware:
		    //case servm_resetTYpe_keyOffOn:
		    case servm_resetType_software:
		        uds_pushRspData(resetType);
		        uds_sendResponse();
		        ecuResetTrig();
		        break;
		    default:
		        uds_negativeRsp(nrsp_subFunctionNotSupported);
		        break;
		}
	}
	else
	{
		uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
	} 
}

/* Service: communication control */
void serv_comControl(void)
{
    uint8_t ctrlType = 0x00;
    uint8_t comType = 0x00;

    if(TRUE != uds_28serviceAllowed())
    {
    	uds_negativeRsp(nrsp_conditionsNotCorrect);
    	return;
    }

	if(0x02 == uds_remainRequestDLC())
	{
		ctrlType = uds_popRequestData(0x00);
		comType = uds_popRequestData(0x00);
		/* Control type check */

	    if((ctrlType == 0) || (ctrlType == 3) || \
	       (ctrlType == 0x80) || (ctrlType == 0x83)	)
	    {
	        /* Check communication type */
	        if ((comType >= servm_comType_normal)
	            && (comType <= servm_comType_normalAndNm))
	        {
	            /* Communication supported, send positive response */
	            /* For fit program from application only no any real porocess */
				switch(comType)
				{
					case servm_comType_normal:
						if((ctrlType == 0x3) || (ctrlType == 0x83))
						{
							/*disable rxtx*/
							hcan_setILMode(0,CAN_IL_APP_SEND|CAN_IL_APP_REC,0);
							hcan_setILMode(2,CAN_IL_APP_SEND|CAN_IL_APP_REC,0);
						}
						else
						{
							/*enable rxtx*/
							hcan_setILMode(0,CAN_IL_APP_SEND|CAN_IL_APP_REC,1);
							hcan_setILMode(2,CAN_IL_APP_SEND|CAN_IL_APP_REC,1);
						}
						break;
					case servm_comType_networkManagement:
						if((ctrlType == 0x3) || (ctrlType == 0x83))
						{
							/*disable rxtx*/
							autosar_dirNm_setTxRxMode(0, 0);
						}
						else
						{
							/*enable rxtx*/
							autosar_dirNm_setTxRxMode(0, 1);
						}
						break;
					case servm_comType_normalAndNm:
						if((ctrlType == 0x3) || (ctrlType == 0x83))
						{
							/*disable rxtx*/
							hcan_setILMode(0,CAN_IL_APP_SEND|CAN_IL_APP_REC,0);
							hcan_setILMode(2,CAN_IL_APP_SEND|CAN_IL_APP_REC,0);

							autosar_dirNm_setTxRxMode(0, 0);
						}
						else
						{
							/*enable rxtx*/
							hcan_setILMode(0,CAN_IL_APP_SEND|CAN_IL_APP_REC,1);
							hcan_setILMode(2,CAN_IL_APP_SEND|CAN_IL_APP_REC,1);
							autosar_dirNm_setTxRxMode(0, 1);
						}
						break;
					default:
					return;
				}

				if (ctrlType & 0x80)
				{
					nt_rsp_reset();
				}
				else
				{
		            uds_pushRspData(ctrlType);
		            uds_sendResponse();
				}
	        }
	        else
	        {
	            /* communication type not supported */
	            uds_negativeRsp(nrsp_requestOutOfRange);
	        }
	    }
	    else
	    {
	        /* sub-function not supported */
	        uds_negativeRsp(nrsp_subFunctionNotSupported);
	    }
	}
	else
	{
		uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
	}
    
}

void serv_dtcSetting(void)
{
    uint8_t ctrlType = uds_popRequestData(0x00);

    if(0x00 != uds_remainRequestDLC())
    {
    	uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
    	return;
    }

    /* DTC setting type check */
    if ((servm_dtcSetting_on == (ctrlType & 0x0f) )
        || (servm_dtcSetting_off == (ctrlType & 0x0f)))
    {
    	if(ctrlType & 0x80)
    	{
    		nt_rsp_reset();
    	}
    	else
    	{
            /* is support type, send positive response */
            /* For fit program from application only no any real porocess */
            uds_pushRspData(ctrlType);
            uds_sendResponse();
    	}

    	 if(servm_dtcSetting_on == (ctrlType & 0x0f))
    	 {
    		 uds_dtcSettingOn = TRUE;
    	 }
    	 else
    	 {
    		 uds_dtcSettingOn = FALSE;
    	 }
    }
    else
    {
		/* un-support type */
		uds_negativeRsp(nrsp_subFunctionNotSupported);
    }
}



void serv_requestDownload(void)
{
	uds_negativeRsp(nrsp_serviceNotSupported);
}


void serv_transferData(void)
{
 	uds_negativeRsp(nrsp_serviceNotSupported);   
}

void serv_requestTransferExit(void)
{
	uds_negativeRsp(nrsp_serviceNotSupported);
}

bool uds_updateAppAllowed(void)
{
	uds_prog_allow_flag = 1;
	return uds_prog_allow_flag;
}
static void ecuResetTrig(void)
{
    disableInterrupts;
    /* Makesure watchdog is enable */
    for (;;)
    {
        /* Waiting watchdog timeout reset */
    }
}

/* Service: DID read, write */
/*
 * Function:didrd_fblFingerprint
 * Param<void>:
 * Return<uint8_t>:
 * Discription: FBL fingerprint read access check
 * Note: FBL fringerprint can be read only when program session unlocked
*/
uint8_t didrd_fblFingerprint(void)
{
    /* Support session check */
    if (sessionType_prog != uds_getActiveSession())
    {
        /* Not in program session, not allowed access */
        return nrsp_requestOutOfRange;
    }
    else
    {
        /* unlock state check */
        if (FALSE == uds_saLevelUnlockCheckByIndex(saLvSpt_fbl))
        {
            /* SA locked, not allowed access */
            return nrsp_securityAccessDenied;
        }
        else
        {
            /* In program session and unlocked, allowed access */
            return nrsp_positiveResponse;
        }
    }
}



/* ------ EOF ------ */


