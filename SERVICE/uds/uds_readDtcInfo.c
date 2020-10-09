/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:udsReadDtcInfo.c
* Description: the sub-functions of the 0x19 -readDtcInfomation service
* REQ IDs:
* History:
* 2017-02-22, Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#define UDS_READ_DTC_INFO_C_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "uds.h"
#include "uds_readDtcInfo.h"
#include "udsDidCfg.h"
#include "obd.h"

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
                      Implementation of global functions
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
uint8_t serv_reportNumberOfDtcByStatusMask(void)
{
	uint8_t nrspCode = nrsp_positiveResponse;
	uint8_t statusMask = 0;
	uint16_t dtc_i = 0;
	uint16_t matchNum = 0;
	uds_pushRspData(uds_supprotDtcStatusMask);
	uds_pushRspData(uds_iso14229DtcFormat);
	if(0x01 == uds_remainRequestDLC())
	{
		statusMask = uds_popRequestData(0x00);
		if(0x00 == statusMask)
		{
			/*surely in this situation, no dtc can match this status mask*/
			uds_pushRspData(0x00);
			uds_pushRspData(0x00);
		}
		else
		{
			for(;dtc_i < dtcItemCnt; dtc_i ++)
			{
				if(0 != (obdDiagDtGrp[dtc_i].dtcStatus.all & statusMask))
				{
					matchNum ++;	
				}
			}
			uds_pushRspData((uint8_t)((matchNum >> 8) & 0xFF));
			uds_pushRspData((uint8_t)(matchNum  & 0xFF));
		}
	}
	else
	{
		nrspCode = nrsp_incorrectMessageLengthOrInvalidFormat;
	}
	return nrspCode;
}


uint8_t serv_reportDtcByStatusMask(void)
{
	uint8_t nrspCode = nrsp_positiveResponse;
	uint8_t statusMask = 0;
	uint16_t dtc_i = 0;	
	uds_pushRspData(uds_supprotDtcStatusMask);
	if(0x01 == uds_remainRequestDLC())
	{
		statusMask = uds_popRequestData(0x00);
		if(0x00 != statusMask)
		{
			for(;dtc_i < dtcItemCnt; dtc_i ++)
			{
				if(0 != (obdDiagDtGrp[dtc_i].dtcStatus.all & statusMask))
				{
					uds_pushRspData((uint8_t)((dtcInfoCfgGrp[dtc_i].dtcCode >> 16) & 0xFF));
					uds_pushRspData((uint8_t)((dtcInfoCfgGrp[dtc_i].dtcCode >> 8) & 0xFF));
					uds_pushRspData((uint8_t)(dtcInfoCfgGrp[dtc_i].dtcCode  & 0xFF));
					uds_pushRspData(obdDiagDtGrp[dtc_i].dtcStatus.all);
				}
			}
		}
		else
		{
			/*no dtc can be matched, so nothing more need to done here*/
		}
	}
	else
	{
		nrspCode = nrsp_incorrectMessageLengthOrInvalidFormat;
	}
	return nrspCode;
}

uint8_t serv_reportDtcSnapshotId(void)
{
	uint8_t nrspCode = nrsp_positiveResponse;
	uint16_t fault_i = 0;
	uint32_t dtcCode = 0;
	uint8_t snapId = 0;
	if(0x00 == uds_remainRequestDLC())
	{
		for(;fault_i < dtcItemCnt; fault_i ++)
		{
			if(TRUE ==  obdDiagDtGrp[fault_i].isStored)	
			{
				dtcCode = dtcInfoCfgGrp[fault_i].dtcCode;
				snapId = (uint8_t)((fault_i + 1) & 0xFF);
				uds_pushRspData((uint8_t)((dtcCode >> 16) & 0xFF));
				uds_pushRspData((uint8_t)((dtcCode >> 8) & 0xFF));
				uds_pushRspData((uint8_t)(dtcCode & 0xFF));
				uds_pushRspData(snapId);
			}
		}
	}
	else
	{
		nrspCode = nrsp_incorrectMessageLengthOrInvalidFormat;
	}
	return nrspCode;
}


uint8_t serv_reportDtcSnapshotByDtcNo(void)
{
	uint8_t nrspCode = nrsp_positiveResponse;
	uint32_t dtcCode = 0;
	uint8_t recordNo = 0;
	uint8_t flt_i = 0;
	uint8_t snapdid_i =0;
	uint8_t did_i =0;
	uint8_t snap_i= 0;
	uint8_t dt_i = 0;
	if(0x04 == uds_remainRequestDLC())
	{
		dtcCode |= uds_popRequestData(0x00);
		dtcCode <<= 8;
		dtcCode |= uds_popRequestData(0x00);
		dtcCode <<= 8;
		dtcCode |= uds_popRequestData(0x00);
		recordNo = uds_popRequestData(0x00);

		

		/*at first we need to search the given DTC is existing or not*/
		for(;flt_i < dtcItemCnt; flt_i ++)
		{
			if(dtcCode == dtcInfoCfgGrp[flt_i].dtcCode)
			{
				break;
			}
		}
		if(flt_i < dtcItemCnt)
		{
			uds_pushRspData((uint8_t)((dtcCode >> 16) & 0xFF));
			uds_pushRspData((uint8_t)((dtcCode >> 8) & 0xFF));
			uds_pushRspData((uint8_t)(dtcCode  & 0xFF));
			uds_pushRspData(obdDiagDtGrp[flt_i].dtcStatus.all);
			if(0xFF == recordNo)
			{
				for(flt_i = 0; flt_i < dtcItemCnt; flt_i ++)
				{
					if(obdDiagDtGrp[flt_i].isStored)
					{
						uds_pushRspData(1);/*only one copy of snap shot is supported*/
						uds_pushRspData((uint8_t)((flt_i + 1) & 0xFF));
						for(snapdid_i = 0;snapdid_i < dtcInfoCfgGrp[flt_i].snapDidNum; snapdid_i ++)
						{
							did_i = dtcInfoCfgGrp[flt_i].pDidIndx[snapdid_i];
							uds_pushRspData((uint8_t)((uds_didRdCfgGrp[did_i].did >> 8) & 0xFF));
							uds_pushRspData((uint8_t)(uds_didRdCfgGrp[did_i].did & 0xFF));
							for(dt_i = 0;dt_i < uds_didRdCfgGrp[did_i].dlc; dt_i ++)
							{
								uds_pushRspData(dtcInfoCfgGrp[flt_i].snapBuf[snap_i]);
								snap_i ++;
							}
						}
					}
				}
			}
			else
			{
				if(obdDiagDtGrp[flt_i].isStored)
				{
					uds_pushRspData(1);/*only one copy of snap shot is supported*/
					uds_pushRspData((uint8_t)((flt_i + 1) & 0xFF));
					for(snapdid_i = 0;snapdid_i < dtcInfoCfgGrp[flt_i].snapDidNum; snapdid_i ++)
					{
						did_i = dtcInfoCfgGrp[flt_i].pDidIndx[snapdid_i];
						uds_pushRspData((uint8_t)((uds_didRdCfgGrp[did_i].did >> 8) & 0xFF));
						uds_pushRspData((uint8_t)(uds_didRdCfgGrp[did_i].did & 0xFF));
						for(dt_i = 0;dt_i < uds_didRdCfgGrp[did_i].dlc; dt_i ++)
						{
							uds_pushRspData(dtcInfoCfgGrp[flt_i].snapBuf[snap_i]);
							snap_i ++;
						}
					}
				}
			}	
		}
		else
		{
			nrspCode = nrsp_requestOutOfRange;
		}
	}
	return nrspCode;
}


uint8_t serv_reportSupportDtc(void)
{
	uint8_t nrspCode = nrsp_positiveResponse;
	uint16_t fault_i = 0;
	uint32_t dtcCode = 0;
	if(0x00 == uds_remainRequestDLC())
	{
		uds_pushRspData(uds_supprotDtcStatusMask);
		for(;fault_i < dtcItemCnt; fault_i ++)
		{
			dtcCode = dtcInfoCfgGrp[fault_i].dtcCode;
			uds_pushRspData((uint8_t)((dtcCode >> 16) & 0xFF));
			uds_pushRspData((uint8_t)((dtcCode >> 8) & 0xFF));	
			uds_pushRspData((uint8_t)(dtcCode  & 0xFF));
			uds_pushRspData(obdDiagDtGrp[fault_i].dtcStatus.all);
		}
	}
	else
	{
		nrspCode = nrsp_incorrectMessageLengthOrInvalidFormat;
	}
	return nrspCode;
}

/* ---- Source switch off ---------------------------------------------------- */
#undef UDS_READ_DTC_INFO_C_
/*--- End of file ------------------------------------------------------------*/


