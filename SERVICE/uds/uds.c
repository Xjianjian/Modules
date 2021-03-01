/*
* Copyright (c) 2016,SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File: uds.c
* Description:
* History:
* 2016-5-19, 1007040, original
*/

#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "swt.h"
#include "nt_export.h"
#include "mc33907.h"
#include "uds_types.h"
#include "uds_pri.h"
#include "uds_cfg.h"
#include "udsDidCfg.h"
#include "uds_readDtcInfo.h"
#include "uds.h"
#include "obd.h"
#include "sysClk.h"
#include "flexCan.h"
#include "flash.h"
#include "MemIf.h"


/* ------------------- Global variables   ------------------------------------------- */

/* ------------ UDS service session and security management ------- */
static uint8_t actSessionIndex = 0;
static uint8_t actSaLockerMask  = servm_saLv_allLocked; /* 1 for locked, 0 for unlocked */
/* UDS current request service ID */
static uint8_t curRequestServID = servm_servID_empty; /* Current request service ID */
static uint8_t curRequestSubFunc = 0u;   /* Current request service sub-function */
static bool rspSizeValid = TRUE;         /* Response message size valid flag (Not out range of NT response buffer size) */

/* ------------ RAM variables for [pre-define service] ------------ */
/* Automatic switch and locked session time */
static uint32_t sv_swStdAutoLockTick = 0;
/* security access protect related */
static uint8_t sv_saOrgType = 0; /* Log the last security access type. It will reset to 0 when session switch */
static uint8_t sv_saSeedUnCheckMask = servm_saLv_allSeedChecked; /* Mask the target access, Mask position as same as SA level mask */
/* DID read, write */
static udsDIDrwBkc_t sv_didRwBkcItem =
{
    /* bkcState */ servm_didRwBkcState_idle,
    /* index    */ servm_didRwBkcInvalidIndex
};
/* Routine ID control */
uint32_t rtCtrlBkcMask = 0ul; /* Max support first 32 routine to have task */

/*memory read/write */
static udsMemBkc_t sv_memBkcItem = 
{
	servm_memRwBkcState_idle,
	servm_memRwBkcInvalidIndx
};

/*Io control*/
udsIoCtrlDt_t udsIoCtrlDt[udsIoCtrl_itemCnt]=
{
	{
		0,		/*tick*/
		0,		/*elapseTick*/
		0,		/*targetVal*/
		0		/*initVal*/
	}
};


/*read DTC information*/

const static uds_readDtcInfo_t  uds_readDtcInfoCfgTable[uds_reportDtcSupportSubFunNum] = 
{
	{UDS_reportNumberOfDTCByStatusMask,						&serv_reportNumberOfDtcByStatusMask},
	{UDS_reportDTCByStatusMask,								&serv_reportDtcByStatusMask},
	{UDS_reportDTCSnapshotIdentification,					&serv_reportDtcSnapshotId},
	{UDS_reportDTCSnapshotRecordByDTCNumber,				&serv_reportDtcSnapshotByDtcNo},
	{UDS_reportDTCSnapshotRecordByRecordNumber,				NULL},
	{UDS_reportDTCExtendedDataRecordByDTCNumber,			NULL},
	{UDS_reportNumberOfDTCBySeverityMaskRecord,				NULL},
	{UDS_reportSeverityInformationOfDTC,					NULL},
	{UDS_reportSupportedDTC,								&serv_reportSupportDtc},
	{UDS_reportFirstTestFailedDTC,							NULL},
	{UDS_reportFirstConfirmedDTC,							NULL},
	{UDS_reportMostRecentTestFailedDTC,						NULL},
	{UDS_reportMostRecentConfirmedDTC,						NULL},
	{UDS_reportMirroyDTCByStatusMask,						NULL},
	{UDS_reportMirrorMemoryDTCExtendedDtatRecordByDTCNumber,NULL},
	{UDS_reportNumberOfMirrorMemoryDTCByStatusMask,			NULL},
	{UDS_reportNumberOfEmissionsRelatedOBDDTCByStatusMask,	NULL},
	{UDS_reportEmissionRelatedOBDDTCByStatusMask,			NULL},
	{UDS_reportDTCFaultDetectionCounter,					NULL},
	{UDS_reportDTCWithPermanentStatus,						NULL},
	{uds_reportDtcSupportSubFunNum,							NULL}
};

/* ------------ End RAM variables for [pre-define service] -------- */

/* -------- Non-Volatile varialbes for [pre-define service] ------- */
/* SA service invalid access count */
uint8_t sv_saInvalidAccessCnt = 0u;
/* SA service protect delay tick
 * When delay tick is zero(0) it will can allowed access SA service */
uint32_t sv_saProtectDelayTick = 0u;
/* -------- End Non-Volatile varialbes for [pre-define service] --- */

/* ------------------- End Global variables   --------------------------------------- */

/* ------------------- Pre-set service process function declare --------------------- */
static void uds_enterDefaultSession(void);
static void uds_autoRtSaProtect_task(void); /* auto sw default ss, Sa protect */
static void uds_didRwBkc_task(void);        /* DID read, write background callback task  */
static void serv_pushDIDtoRsp(uint16_t did_i);
static void serv_didWriteActiveRsp(uint16_t did_i);
static void uds_didRwBkc_clear(void);
static void uds_rtCtrBkc_task(void);
static void serv_memoryReadPositiveRsp(uint32_t memoryAddr,uint16_t memorySize);
static void serv_memoryWritePositiveRsp(uint32_t memoryAddr,uint16_t memorySize);
static void uds_memRwBck_clear(void);
static void uds_memRwBkc_task(void);
static void uds_ioCtrlBkc_task(void);
static void uds_requestValid(void);


/* ------------------- Pre-set service process function declare --------------------- */

/*
 * Function: uds_requestProcess
 * Param<void>: 
 * Return<void>: 
 * Discription: UDS service request process
 * Note: Call by NT layer, when call this function the NT is been locked.
 *       It will can not handle any NT frame before NT unlock
 *       We can call nt_rsp_start or nt_unlock function to unlock NT layer
*/
void uds_requestProcess(void)
{
    uint8_t serv_i = 0;

    /* Get request service basic information */
    curRequestServID = nt_req_pop_dt(0u);
    curRequestSubFunc = nt_req_pop_dt(0u);
    /* Set the load data pointer to SID + 1 (SID is store in 0 index) */
    nt_req_pos_set(1); /* Make sure the service process function first read data is the service request first data or sub-function */

    /* Set response size to valid */
    rspSizeValid = TRUE;
    /* Pre-push positive response service id */
    nt_rsp_reset();
    nt_rsp_push_dt(servm_positiveResponeCode(curRequestServID));

    /* Search service, and get service index if it is support */

    for (; serv_i < udsServCnt; serv_i++)
    {
        if (udsServCfg[serv_i].sid == curRequestServID)
        {
            /* Find service */
            break;
        }
    }
    /* check search result */
    if (serv_i < udsServCnt)
    {
    	uds_requestValid();
        /* Find service */
        /* Session Support Check */
        if (0 != (udsServCfg[serv_i].ssSptMask & servm_ssSptMask(actSessionIndex)))
        {
            /* Service support in active session */
            /* Security unlock denied check */
            if ((0u == udsServCfg[serv_i].saDeniedMask)
                || ((udsServCfg[serv_i].saDeniedMask & actSaLockerMask) != udsServCfg[serv_i].saDeniedMask))
            {
                /* Have one or more SA level was be unlocked of this request service ID */
                /* Min. data length check. */
                if (nt_req_remain_len() >= udsServCfg[serv_i].minDLC)
                {
                    /* Request data length is above or equre the min. length limit */
                    /* Call back function valid check */
                    if (NULL != udsServCfg[serv_i].processCallBack)
                    {
                        /* call service process function */
                        udsServCfg[serv_i].processCallBack();
                    }
                    else
                    {
                        uds_negativeRsp(nrsp_serviceNotSupported);
                    }
                }
                else
                {
                    uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
                }
            }
            else
            {
                uds_negativeRsp(nrsp_securityAccessDenied);
            }
        }
        else
        {
            uds_negativeRsp(nrsp_serviceNotSupportedInActiveSession);
        }
    }
    else
    {
        uds_negativeRsp(nrsp_serviceNotSupported);
    }

    /* dead lock detect */
    if ((TRUE == uds_isNotYetRsp())
        && (FALSE == uds_isRspPending()))
    {
        /* If not send real response or no response pending, unlock NT layer */
        nt_unlock();
    }
        
}

/*
 * Function:uds_init
 * Param<void>:
 * Return<void>:
 * Discription: Initial UDS service to default state.
 * Note: Will check SA protect denied.
*/
void uds_init(void)
{
    /* initial service and security access locker to default state */
    /* Clear uds session auto switch timer */
    sv_swStdAutoLockTick = 0;
    /* Reset session to default and locked all SA */
    actSessionIndex = 0;    /* set session to default again */
    /* Clear seed un-check mask */
    sv_saSeedUnCheckMask = servm_saLv_allSeedChecked;
    /* re-locked all SA levels */
    actSaLockerMask = servm_saLv_allLocked;
    /* reset uds current request service ID log */
    curRequestServID = servm_servID_empty;
    
    /* clear background task */
    /* clear DID read,write background task */
    sv_didRwBkcItem.bkcState = servm_didRwBkcState_idle;
    sv_didRwBkcItem.index = servm_didRwBkcInvalidIndex;
    /* clear routine control background task */
    rtCtrlBkcMask = 0ul;

	sv_memBkcItem.memIndx = servm_memRwBkcInvalidIndx;
	sv_memBkcItem.memOperation = servm_memRwBkcState_idle;

	normalRxEnable = TRUE;
	normalTxEnable = TRUE;
	networkManagerRxEnable = TRUE;
	networkManagerTxEnable = TRUE;
    /* SA protect denied check */
    if (sv_saInvalidAccessCnt >= saProtectLimitCnt)
    {
        /* SA invalid access times is above or equre, start protect */
        sv_saProtectDelayTick = saProtectDelayTime;
    }

    /* Call NT initial */
    nt_init();
}

/*
 * Function:uds_task
 * Param<void>:
 * Return<void>:
 * Discription: UDS backgroud task process
 * Note: call it by cycle (eg. 1ms). Some timer prameter is calculate with this function call cycle (eg. sv_swStdAutoLockTick, sv_saProtectDelayTick).
*/
void uds_task(void)
{
    /* Call NT layer task */
    nt_task();
    /* Auto return to default session(& locked SA) and SA protect task */
    uds_autoRtSaProtect_task();
    /* DID read, write background process task */
    uds_didRwBkc_task();
    /* Routine control background process (Not need for FBL) */
    uds_rtCtrBkc_task();

	/*uds read write background task process*/
	uds_memRwBkc_task();

	/*IO control background task*/
	uds_ioCtrlBkc_task();
}

/* ------------------ Preset Service Process ---------------------------------------- */
/*
 * Function:uds_autoRtSaProtect_task
 * Param<void>:
 * Return<static void>:
 * Discription: Auto return to default session and locked SA and SA protect task
 * Note: 
*/
static void uds_autoRtSaProtect_task(void)
{
    /* Auto return to default session and relocked process
     * Counter down of each task if tick above 0
     * When decrase from 1 to 0, set to default session and relock SA */
    if (sv_swStdAutoLockTick > 0)
    {
        /* Response pending check */
        if (TRUE == uds_isRspPending())
        {
            /* Stop counter down. Not call request valid becouse the result is unkown */
        }
        else
        {
            /* Tick counter down */
            sv_swStdAutoLockTick--;
            /* If conter down to zero (timeout) */
            if (0 == sv_swStdAutoLockTick)
            {
                /* Switch to default session */
                uds_enterDefaultSession();
                /* Notice background to enter default session */
                uds_sessionTimeout();
            }
        }
    }

    /* SA Protect handle
     * Counter down of each task if tick above 0
     * When decrease from 1 to 0, decrease invalid access counter if it above 0 */
    if (sv_saProtectDelayTick > 0)
    {
        sv_saProtectDelayTick--;
        if ((0 == sv_saProtectDelayTick)
            && (0 < sv_saInvalidAccessCnt))
        {
            sv_saInvalidAccessCnt--;
        }
    }
}

static void uds_enterDefaultSession(void)
{
    /* Clear uds session auto switch timer */
    sv_swStdAutoLockTick = 0;
    /* Reset session to default and locked all SA */
    actSessionIndex = 0;    /* set session to default again */
    /* check SA state */
    if (0xFF != actSaLockerMask)
    {
        /* sa is unlocked */
        /* Clear seed un-check mask */
        sv_saSeedUnCheckMask = 0u;
        /* re-locked all SA levels */
        actSaLockerMask = 0xFF;
    }
    uds_dtcSettingOn = TRUE;
    uds_clearDefSession();
}

/*
 * Function:uds_requestValid
 * Param<void>:
 * Return<static void>:
 * Discription: Function call for request can be get positive response
 * Note: This function will force the current active session and security access locker state stay on require time
*/
static void uds_requestValid(void)
{
    /* Un-default session check */
    if (actSessionIndex > 0)
    {
        /* Not default session */
        sv_swStdAutoLockTick = ssSwStd_saLockTime; /* Set tick to max */
    }
#if 0 /* handle in uds_task */
    else
    {
        /* Default session */
        sv_swStdAutoLockTick = 0; /* Set tick to 0 */
        actSaLockerMask = 0xFF;   /* relock SA */
    }
#endif
}

/* Service management (Session control & Security Access) */
void serv_sessionControl(void)
{
    uint8_t targetSessionType = 0; /* Get request target session */
    uint8_t actSessionType = sessionType_std;         /* active session ID */
    uint8_t switchRst = nrsp_positiveResponse;
    uint8_t  ss_i = 0; /* session index */

    uint8_t writeUpdtReqFlag[8] = {0xDE,0xAD,0xBE,0xEF,0xBE,0xEF,0xDE,0xAD};

    /* Get real active session ID */
    if (actSessionIndex < sessionTypeCnt)
    {
        actSessionType = sessionTypeMap[actSessionIndex];
    }
	if(0x01 == uds_remainRequestDLC())
	{
		/* Call session switch callback function */
		targetSessionType  = uds_popRequestData(0x00);
		/* Pre-push active response first data (request switch session)  */
		uds_pushRspData(targetSessionType&0x0f);
	    switchRst = uds_sessionSwitch(actSessionType, targetSessionType&0x0f);
	    if (nrsp_positiveResponse == switchRst)
	    {
	    	uds_pushRspData(0x00);
			uds_pushRspData(0x32);/*P2_sever_max=(00 32）h : 50ms for P2max [unit:1ms]*/
			uds_pushRspData(0x01);
			uds_pushRspData(0xF4);/*P2*_sever_max=(01 F4）h : 5s for P2*max [unit:10ms]*/
	        /* Request switch to default session check */
	        if ((actSessionType != targetSessionType)
	            &&  (sessionType_std == targetSessionType))
	        {
	            /* Enter default session */
	            uds_enterDefaultSession();
	        }

	        /* Switch to target session (modify session index) */
	        /* session index search */
	        for (; ss_i < sessionTypeCnt; ss_i++)
	        {
	            if (sessionTypeMap[ss_i] == targetSessionType)
	            {
	                /* Find session index */
	                /* Modify active session index to target session index */
	                actSessionIndex = ss_i;
	                break;
	            }
	        }

	        /* Positive response */
	        

	        if ((actSessionType != targetSessionType)
	        	&&  (sessionType_prog == targetSessionType))
	        {

	        	uds_rspPending();

				swt_feedDog();
				mc33907_changeWdWindow(WD_DISABLE);
				swt_feedDog();
				mc33907_refreshWd();
				swt_feedDog();
				disableInterrupts;
#ifdef  GLOBAL_FBL_IN_EE

				MemIf_mainFunction();
				(void)MemIf_WriteEE(EepromCfg_wrUpdtFlag,writeUpdtReqFlag,8,1);

#else
				while(m_ee256_idle != ee256_getSt())
				{
					swt_feedDog();
					ee256_cyclic();
				}
				ee256_write(0x7FF8,(uint32_t)writeUpdtReqFlag,8);

				while(m_ee256_idle != ee256_getSt())
				{
					swt_feedDog();
					ee256_cyclic();
				}
#endif


				swt_feedDog();
				/* Makesure watchdog is enable */
				for (;;)
				{
					/* Waiting watchdog timeout reset */
				}
	        }

			if(targetSessionType & 0x80)
			{
				nt_rsp_reset();
				return;
			}
	        /* Positive response */
	        uds_sendResponse();
	    }
	    else
	    {
	        /* Switch fail, response negative code */
	        uds_negativeRsp(switchRst);
	    }
	}
	else
	{
		uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);	
	}
    
}

void serv_securityAccess(void)
{
    uint8_t saType = uds_popRequestData(0x00); /* Load security access type */
    uint8_t saLevel_i = 0; /* security access level */
    bool    saGetSeed = TRUE;

    /* Pre-push security access type for active response */
    uds_pushRspData(saType);

    /* Security access level search */
    for (; saLevel_i < saLevelCnt; saLevel_i++)
    {
        if (saLevelMap[saLevel_i].level == saType)
        {
            /* Find support level and get seed */
            break;
        }
        else if ((saLevelMap[saLevel_i].level + 1) == saType)
        {
            /* Find support level and check key */
            saGetSeed = FALSE;
            break;
        }
        else
        {
            /* continue search */
        }
    }

    /* Check search result */
    if (saLevel_i < saLevelCnt)
    {
        /* request security access level is been support */
        /* SA Protect check */
        if (0 == sv_saProtectDelayTick)
        {
			/* Security access step check (Get seed or check key) */
			if (TRUE == saGetSeed)
			{
				/* Get seed */
				if(0x00 == uds_remainRequestDLC())
				{
					/* Update seed condition check */
					if (0 == (sv_saSeedUnCheckMask & servm_saLvSptMask(saLevel_i)))
					{
						/* Call update seed position */
						uds_saUpdateSeed(saLevelMap[saLevel_i].level);
						actSaLockerMask |= servm_saLvSptMask(saLevel_i);
					}
					/* response seed */
					uds_pushRspBuf(saLevelMap[saLevel_i].seedBuf, saLevelMap[saLevel_i].seedLen);
					uds_sendResponse();
				}
				else
				{
					uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
				}
            }
            else
            {
                /* Check key */
                if (sv_saOrgType != saLevelMap[saLevel_i].level)
                {
                    /* Check key is un-match  */
                    /* SA invalid access times count increase */
                    if (sv_saInvalidAccessCnt < saProtectLimitCnt)
                    {
                        sv_saInvalidAccessCnt++;
                    }
                    /* Protect check */
                    if (sv_saInvalidAccessCnt >= saProtectLimitCnt)
                    {
                        /* SA invalid access times has exceeded the limited times, start protect */
                        sv_saProtectDelayTick = saProtectDelayTime;
                    }
                    /* negative response */
                    uds_negativeRsp(nrsp_requestSequenceError);
                }
                else
                {
                    /* Check Key */
                	if(saLevelMap[saLevel_i].seedLen ==  uds_remainRequestDLC())
                	{
						if (TRUE == uds_saCheckKey(saLevelMap[saLevel_i].level))
	                    {
	                        /* Check pass, unlock target level and send positive response */
							sv_saInvalidAccessCnt = 0;
	                        actSaLockerMask &= (~(servm_saLvSptMask(saLevel_i)));
	                        uds_sendResponse();
	                    }
	                    else
	                    {
	                        /* Check fail, (re-)locked all and send negative response */
	                        actSaLockerMask = 0xFFu;
	                        /* SA invalid access times count increase */
							if (sv_saInvalidAccessCnt < saProtectLimitCnt)
							{
								sv_saInvalidAccessCnt++;
							}
							/* Protect check */
							if (sv_saInvalidAccessCnt >= saProtectLimitCnt)
							{
								/* SA invalid access times has exceeded the limited times, start protect */
								sv_saProtectDelayTick = saProtectDelayTime;
								uds_negativeRsp(nrsp_exceedNumberOfAttempts);
							}
							else
							{
		                        uds_negativeRsp(nrsp_invalidKey);
							}
	                    }
					}
					else
					{
						uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
					}
                   
                }
            }
        }
        else
        {
            /* Delay not complete, negative response */
            uds_negativeRsp(nrsp_requiredTimeDelayNotExpired);
        }
    }
    else
    {
        /* not support request security access level */
        uds_negativeRsp(nrsp_subFunctionNotSupported); /* negative response */
    }

    /* Log last SA type */
    sv_saOrgType = saType;
}

void serv_testPresent(void)
{
	uint8_t reqCode;
    /* Check sub-function */
	if(0x01 != uds_remainRequestDLC())
	{
		uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
		return;
	}

	reqCode = uds_popRequestData(0xFF);
	if(reqCode)
	{
		if(reqCode == 0x80)
		{

		}
		else
		{
			/* Negative response */
			 uds_negativeRsp(nrsp_subFunctionNotSupported);
		}
	}
    else
    {
    	/* Sub-function valid (Only support sub-function 0x00) */
		/* Positive response */
		uds_pushRspData(0x00);
		uds_sendResponse();
    }
}

/* DID Process */
void serv_didRead(void)
{
    uint16_t curDID = 0; /* Current need read DID */
    uint16_t did_i = 0;  /* For search current DID in DID config index */
	uint8_t did_high = 0;
	uint8_t did_low = 0;
	bool did_find =  FALSE;
    uint8_t  didAcNrspRst = nrsp_positiveResponse; /* DID read callback result */

    /* clear bkc */
    uds_didRwBkc_clear();
    /* Notice DID read/write ready */
    uds_didRwReady();

    /* Request data length check */
    if (0 != (uds_remainRequestDLC() & 0x01))
    {
        /* DID is not aligned with 2 bytes */
        uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
    }
    else
    {
        /* Request data length valid */
        /* Read DIDs */
        while ((uds_remainRequestDLC() >= 2)
            && (nrsp_positiveResponse == didAcNrspRst))
        {
        	did_find = FALSE;
            /* Load current need read DID */
            did_high = uds_popRequestData(0x00);
			curDID = did_high;
            curDID <<= 8;
			did_low = uds_popRequestData(0x00);
            curDID |= did_low;
            /* DID support check */
            for (did_i = 0; did_i < uds_didRdCnt; did_i++)
			{
				if(uds_didRdCfgGrp[did_i].did == curDID)
				{
					did_find = TRUE;
					break;
				}

			}
			
			if((TRUE == did_find) && ((saLvSptMask_none == uds_didRdCfgGrp[did_i].secLv)
			  || (uds_didRdCfgGrp[did_i].secLv != (uds_didRdCfgGrp[did_i].secLv & actSaLockerMask))))
			{
				
				if(NULL == uds_didRdCfgGrp[did_i].cb)
				{
					serv_pushDIDtoRsp(did_i);	
				}
				else
				{
					uds_didRdCfgGrp[did_i].cb();
					serv_pushDIDtoRsp(did_i);
				}
			}
        }

        /* Response check */
        if(TRUE == uds_isNotYetRsp())
        {
            /* Response pending check */
            if (nrsp_requestCorrectlyReceivedResponsePending != didAcNrspRst)
            {
                /* No response pending */
                /* Check data read result */
                if (uds_getResponseDataLen() == 0)
                {
                    /* No any DID was be read */
                    /* Send negative response */
                    uds_negativeRsp(nrsp_requestOutOfRange);
                }
                else
                {
                    /* Have DID be read */
                    /* Send response */
                    uds_sendResponse();
                }
            }
            /* Else run DID background call task */
        }
    }
}

void serv_didWrite(void)
{
    uint16_t curDID = 0;
    uint16_t did_i = 0;
    uint16_t dt_i = 0;
    uint8_t didAcNrspRst = nrsp_requestOutOfRange;

    /* clear bkc */
    uds_didRwBkc_clear();
    /* Notice DID read/write ready */
    uds_didRwReady();
    /* Load current need read DID */
    curDID = uds_popRequestData(0x00);
    curDID = curDID << 8;
    curDID = curDID | uds_popRequestData(0x00);

    /* DID support check */
    for (; did_i < uds_didWtCnt; did_i++)
    {
#if 1
        /* Find DID in support(config) list */
        if (curDID == uds_didWtCfgGrp[did_i].did)
        {
            /* Find DID */
			if((saLvSptMask_none == uds_didWtCfgGrp[did_i].secLv)
				|| (uds_didWtCfgGrp[did_i].secLv != (uds_didWtCfgGrp[did_i].secLv & actSaLockerMask)))
			{
				/* DLC valid check */
				if (uds_remainRequestDLC() == uds_didWtCfgGrp[did_i].dlc)
				{
					/* Length valid */
					/* Load request write data to DID data cache */
					for (; dt_i < uds_didWtCfgGrp[did_i].dlc; dt_i++)
					{
						uds_didWtCfgGrp[did_i].dtCache[dt_i] = uds_popRequestData(0x00);
					}
					/* Check callback denied */
					if (NULL != uds_didWtCfgGrp[did_i].cb)
					{
						/* Need callback for write */
						/* Call write callback function */
						uds_didWtCfgGrp[did_i].cb();
					}
					serv_didWriteActiveRsp(did_i);
				}
				else
				{
					/* Data length not match */
					/* Send negative response */
					uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
				}
			}
			else
			{
				uds_negativeRsp(nrsp_securityAccessDenied);
			}
            break; /* find DID, stop search */
        }
#endif
    }

    /* DID not support check
     * Check logic: If not send real response and not response pending
     *              Have response pending means DID find and result unkown.
     *              Have negative or positive response means DID find and result comfrim.
    */
    if (TRUE == uds_isNotYetRsp())
    {
        /* Response pending check */
        if (nrsp_requestCorrectlyReceivedResponsePending != didAcNrspRst) /* Can also used uds_isRspPending() to check */
        {
            uds_negativeRsp(nrsp_requestOutOfRange); /* DID not support */
        }
    }
}


void serv_readMemoryByAddr(void)
{
	uint8_t addrAndLenFormat = uds_popRequestData(0x00);
	uint8_t memAddrLen	= addrAndLenFormat & 0x0F;			/*the bytes number of the memory address parameters*/
	uint8_t memSizeLen	= (addrAndLenFormat >> 4) & 0x0F;	/*the bytes number of the memory size parameters*/
	uint32_t memAddr = 0;
	uint32_t memSize = 0;
	uint8_t byteIndx = 0;
	uint8_t memoryIndx = 0xFF;		/*0xFF: invalid memoryIndx*/
	uint8_t memAcNrspRst = nrsp_positiveResponse;
	uds_memRwBck_clear();
	if(uds_remainRequestDLC() == memAddrLen + memSizeLen)
	{
		if((memAddrLen >0) && (memAddrLen <= 4) && (memSizeLen > 0) && (memSizeLen <= 2))
		{
			for(byteIndx = 0;byteIndx < memAddrLen; byteIndx ++)
			{
				memAddr <<= 8;
				memAddr |= uds_popRequestData(0x00);
			}
			for(byteIndx = 0;byteIndx < memSizeLen; byteIndx ++)
			{
				memSize <<= 8;
				memSize |= uds_popRequestData(0x00);
			}
			memoryIndx = uds_getMemIndx(memAddr,memSize);
			if(memoryIndx < chipMem_cnt)
			{
				if(NULL != chipMemCfg[memoryIndx].readCallback)
				{
					memAcNrspRst = chipMemCfg[memoryIndx].readCallback();
					if(nrsp_positiveResponse == memAcNrspRst)
					{
						uds_sendResponse();						
					}
					else if(nrsp_requestCorrectlyReceivedResponsePending == memAcNrspRst)
					{
						if (FALSE == uds_isRspPending())
	                    {
	                        uds_rspPending();
	                    }
						sv_memBkcItem.memIndx = memoryIndx;
						sv_memBkcItem.memOperation = servm_memRwBkcState_read;
					}
					else
					{
						uds_negativeRsp(memAcNrspRst);
					}
				}
				else
				{
					 /* Not need callback */
	                 /* Send positive response */
	                 serv_memoryReadPositiveRsp(memAddr,memSize);
					 
				}
			}
			else
			{
				uds_negativeRsp(nrsp_requestOutOfRange);
			}
		}
		else
		{
			uds_negativeRsp(nrsp_requestOutOfRange);
		}
	}
	else
	{
		uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
	}
}

void serv_writeMemoryByAddr(void)
{
	uint8_t addrAndLenFormat = uds_popRequestData(0x00);
	uint8_t memAddrLen	= addrAndLenFormat & 0x0F;			/*the bytes number of the memory address parameters*/
	uint8_t memSizeLen	= (addrAndLenFormat >> 4) & 0x0F;	/*the bytes number of the memory size parameters*/ 
	uint8_t byteIndx = 0;
	uint32_t memAddr = 0;
	uint16_t memSize = 0;
	uint8_t memoryIndx = 0xFF;
	uint8_t memAcNrspRst = nrsp_positiveResponse;
	uint8_t popDt = 0;
	uds_pushRspData(addrAndLenFormat);
	uds_memRwBck_clear();
	if((memAddrLen >0) && (memAddrLen <= 4) && (memSizeLen > 0) && (memSizeLen <= 2))
	{
		for(byteIndx = 0;byteIndx < memAddrLen; byteIndx ++)
		{
			popDt = uds_popRequestData(0x00);
			memAddr <<= 8;
			memAddr |= popDt;
			uds_pushRspData(popDt);
		}
		for(byteIndx = 0;byteIndx < memSizeLen; byteIndx ++)
		{
			popDt = uds_popRequestData(0x00);
			memSize <<= 8;
			memSize |= popDt;
			uds_pushRspData(popDt);
		}
		memoryIndx = uds_getMemIndx(memAddr,memSize);
		if(memoryIndx < chipMem_cnt)
		{
			if(memSize == uds_remainRequestDLC())
			{
				if(NULL != chipMemCfg[memoryIndx].writeCallback)
				{
					memAcNrspRst = chipMemCfg[memoryIndx].writeCallback();
					if(nrsp_positiveResponse == memAcNrspRst)
					{
						uds_sendResponse();						
					}
					else if(nrsp_requestCorrectlyReceivedResponsePending == memAcNrspRst)
					{
						if (FALSE == uds_isRspPending())
	                    {
	                        uds_rspPending();
	                    }
						sv_memBkcItem.memIndx = memoryIndx;
						sv_memBkcItem.memOperation = servm_memRwBkcState_write;
					}
					else
					{
						uds_negativeRsp(memAcNrspRst);
					}
				}
				else
				{
					 /* Not need callback */
	                 /* Send positive response */
	                 serv_memoryWritePositiveRsp(memAddr,memSize);			
				}		
			}
			else
			{
				uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
			}
		}
		else
		{
			uds_negativeRsp(nrsp_requestOutOfRange);
		}
	}
	else
	{
		uds_negativeRsp(nrsp_requestOutOfRange);
	}
	
}



/*
 * Function:serv_pushDIDtoRsp
 * Param<uint16_t did_i>:
 * Return<static void>:
 * Discription:
 * Note: This function not check acFlag (access type flag)
*/
static void serv_pushDIDtoRsp(uint16_t did_i)
{
    /* DID index valid check */
    if (did_i < uds_didRdCnt)
    {
        /* DID is in list */
        /* Check data cache */
        if (NULL != uds_didRdCfgGrp[did_i].dtCache)
        {
            /* Data cache valid */
            /* Push DID and Data to response buffer */
			uds_pushRspData((uint8_t)(uds_didRdCfgGrp[did_i].did >> 8));
			uds_pushRspData((uint8_t)(uds_didRdCfgGrp[did_i].did & 0x00FFu));

            uds_pushRspBuf((uint8_t *)uds_didRdCfgGrp[did_i].dtCache, (uint16_t)(uds_didRdCfgGrp[did_i].dlc));
        }
    }
}

/*
 * Function:serv_didWriteActiveRsp
 * Param<uint16_t did_i>:
 * Return<static void>:
 * Discription: 
 * Note: THis function not check acFlag (access type flag)
*/
static void serv_didWriteActiveRsp(uint16_t did_i)
{
    /* DID index valid check */
    if (did_i < uds_didWtCnt)
    {
        uds_pushRspData((uint8_t)(uds_didWtCfgGrp[did_i].did >> 8));
        uds_pushRspData((uint8_t)(uds_didWtCfgGrp[did_i].did & 0x00FFu));
        uds_sendResponse();
    }
}

static void uds_didRwBkc_clear(void)
{
    sv_didRwBkcItem.index = servm_didRwBkcInvalidIndex;
    sv_didRwBkcItem.bkcState = servm_didRwBkcState_idle;
}

static void uds_didRwBkc_task(void)
{
#if 0
    uint8_t didAcNrspRst = nrsp_positiveResponse;
    /* RW background call process check */
	if (servm_didRwBkcState_Read == sv_didRwBkcItem.bkcState)
	{
		if (sv_didRwBkcItem.index < uds_didRdCnt)
		{
			didAcNrspRst = uds_didRdCfgGrp[sv_didRwBkcItem.index].readCallBack();
			uds_requestValid();
	        /* Callback result check (NRSP code) */
	        if (nrsp_positiveResponse == didAcNrspRst)
	        {
	            /* Read process compelete */
	            /* Push read result to response buffer */
	            serv_pushDIDtoRsp(sv_didRwBkcItem.index);
	            /* Clear bkc */
	            uds_didRwBkc_clear();
	            /* continue to read DIDs */
	            if(uds_remainRequestDLC()>0)
	            {
	            	 serv_didRead();
	            }
	            else
	            {
	            	if (uds_getResponseDataLen() == 0)
					{
						/* No any DID was be read */
						/* Send negative response */
						uds_negativeRsp(nrsp_requestOutOfRange);
					}
					else
					{
						/* Have DID be read */
						/* Send response */
						uds_sendResponse();
					}
	            }
	        }
	        else if (nrsp_requestCorrectlyReceivedResponsePending != didAcNrspRst)
	        {
	            /* not in-processing (Not pending) */
	            /* Clear bkc */
	            uds_didRwBkc_clear();
	            /* Send negative response with return NRSP code */
	            uds_negativeRsp(didAcNrspRst);
	        }
	        else
	        {
	            /* Already in process (response pending) */
	            /* Do nothing */
	        }
		}
		
	}
	else
	{

	}






    if (sv_didRwBkcItem.index < udsDIDcnt)
    {
        /* Have DID need background process */
        /* Read or write check */
        if (servm_didRwBkcState_Read == sv_didRwBkcItem.bkcState)
        {
            /* is DID read */
            /* Run read callback */
            didAcNrspRst = udsDIDcfg[sv_didRwBkcItem.index].readCallBack();
            uds_requestValid();
            /* Callback result check (NRSP code) */
            if (nrsp_positiveResponse == didAcNrspRst)
            {
                /* Read process compelete */
                /* Clear bkc */
                uds_didRwBkc_clear();
                /* Push read result to response buffer */
                serv_pushDIDtoRsp(sv_didRwBkcItem.index);
                /* continue to read DIDs */
                serv_didRead();
            }
            else if (nrsp_requestCorrectlyReceivedResponsePending != didAcNrspRst)
            {
                /* not in-processing (Not pending) */
                /* Clear bkc */
                uds_didRwBkc_clear();
                /* Send negative response with return NRSP code */
                uds_negativeRsp(didAcNrspRst);
            }
            else
            {
                /* Already in process (response pending) */
                /* Do nothing */
            }
        }
        else if (servm_didRwBkcState_Write == sv_didRwBkcItem.bkcState)
        {
            /* is DID write */
            /* Run read callback */
            didAcNrspRst = udsDIDcfg[sv_didRwBkcItem.index].writeCallBack();
            /* Callback result check (NRSP code) */
            if (nrsp_positiveResponse == didAcNrspRst)
            {
                /* Read process compelete */
                /* Clear bkc */
                uds_didRwBkc_clear();
                /* send positive response for notice write success */
                serv_didWriteActiveRsp(sv_didRwBkcItem.index);
                /* Write compelete, no more things need to do */
            }
            else if (nrsp_requestCorrectlyReceivedResponsePending != didAcNrspRst)
            {
                /* not in-processing (Not pending) */
                /* Clear bkc */
                uds_didRwBkc_clear();
                /* Send negative response with return NRSP code */
                uds_negativeRsp(didAcNrspRst);
            }
            else
            {
                /* Already in process (response pending) */
                /* Do nothing */
            }
        }
        else
        {
            /* Idle state, Do nothing */
        }
    }
	#endif
}

/*memory read,write*/
static void serv_memoryReadPositiveRsp(uint32_t memoryAddr,uint16_t memorySize)
{
	uint16_t memoryByteIndx = 0;
	for(;memoryByteIndx < memorySize; memoryByteIndx++)
	{
		uds_pushRspData((uint8_t)(*((uint8_t *)(memoryAddr + memoryByteIndx))));
		
	}
	uds_sendResponse();
}

static void serv_memoryWritePositiveRsp(uint32_t memoryAddr,uint16_t memorySize)
{
	uint16_t memoryByteIndx = 0;
	for(;memoryByteIndx < memorySize; memoryByteIndx++)
	{
		(*((uint8_t *)(memoryAddr + memoryByteIndx))) = uds_popRequestData(0xFF);
	}
	uds_sendResponse();
}



static void uds_memRwBck_clear(void)
{
	sv_memBkcItem.memOperation = servm_memRwBkcState_idle;	
	sv_memBkcItem.memIndx = servm_memRwBkcInvalidIndx;
}

static void uds_memRwBkc_task(void)
{
	uint8_t memRwBkcNrspRst = nrsp_positiveResponse;
	if (sv_memBkcItem.memIndx < chipMem_cnt)
    {
        /* Have DID need background process */
        /* Read or write check */
        if (servm_memRwBkcState_read == sv_memBkcItem.memOperation)
        {
            /* is DID read */
            /* Run read callback */
            memRwBkcNrspRst = chipMemCfg[sv_memBkcItem.memIndx].readCallback();
            uds_requestValid();
            /* Callback result check (NRSP code) */
            if (nrsp_positiveResponse == memRwBkcNrspRst)
            {
                /* Read process compelete */
                /* Clear bkc */
                uds_memRwBck_clear();
				uds_sendResponse();
            }
            else if (nrsp_requestCorrectlyReceivedResponsePending != memRwBkcNrspRst)
            {
                /* not in-processing (Not pending) */
                /* Clear bkc */
                uds_memRwBck_clear();
                /* Send negative response with return NRSP code */
                uds_negativeRsp(memRwBkcNrspRst);
            }
            else
            {
                /* Already in process (response pending) */
                /* Do nothing */
            }
        }
        else if (servm_memRwBkcState_write == sv_memBkcItem.memOperation)
        {
            /* is DID write */
            /* Run read callback */
            memRwBkcNrspRst = chipMemCfg[sv_memBkcItem.memIndx].writeCallback();
            /* Callback result check (NRSP code) */
            if (nrsp_positiveResponse == memRwBkcNrspRst)
            {
                /* Read process compelete */
                /* Clear bkc */
                uds_memRwBck_clear();
				uds_sendResponse();
            }
            else if (nrsp_requestCorrectlyReceivedResponsePending != memRwBkcNrspRst)
            {
                /* not in-processing (Not pending) */
                /* Clear bkc */
                uds_didRwBkc_clear();
                /* Send negative response with return NRSP code */
                uds_negativeRsp(memRwBkcNrspRst);
            }
            else
            {
                /* Already in process (response pending) */
                /* Do nothing */
            }
        }
        else
        {
            /* Idle state, Do nothing */
        }
    }
}

static void uds_ioCtrlBkc_task(void)
{
	uint8_t ioCtrl_i = 0;
	int32_t diff = 0;
	uint32_t lockVal = 0;
	for(; ioCtrl_i < udsIoCtrl_itemCnt; ioCtrl_i ++)
	{
		if((NULL != udsIoCtrlCfg[ioCtrl_i].lock) && (NULL != udsIoCtrlCfg[ioCtrl_i].lockVal) &&	\
		   (TRUE == *udsIoCtrlCfg[ioCtrl_i].lock))
		{
			if(0 != udsIoCtrlDt[ioCtrl_i].tick)
			{
				if(udsIoCtrlDt[ioCtrl_i].elapseTick < udsIoCtrlDt[ioCtrl_i].tick)
				{
					udsIoCtrlDt[ioCtrl_i].elapseTick ++;
					if(udsIoCtrlDt[ioCtrl_i].elapseTick == udsIoCtrlDt[ioCtrl_i].tick)
					{
						if(1 == udsIoCtrlCfg[ioCtrl_i].dtSize)
						{
							*(uint8_t *)udsIoCtrlCfg[ioCtrl_i].lockVal = (uint8_t)(udsIoCtrlDt[ioCtrl_i].targetVal & 0xFF);
						}
						else if(2 == udsIoCtrlCfg[ioCtrl_i].dtSize)
						{
							*(uint16_t *)udsIoCtrlCfg[ioCtrl_i].lockVal = (uint16_t)(udsIoCtrlDt[ioCtrl_i].targetVal & 0xFFFF);
						}
						else
						{
							*(uint32_t *)udsIoCtrlCfg[ioCtrl_i].lockVal = udsIoCtrlDt[ioCtrl_i].targetVal;
						}
					}
					else
					{
						diff = udsIoCtrlDt[ioCtrl_i].targetVal - udsIoCtrlDt[ioCtrl_i].initVal;
						lockVal = udsIoCtrlDt[ioCtrl_i].initVal + (diff * udsIoCtrlDt[ioCtrl_i].elapseTick / udsIoCtrlDt[ioCtrl_i].tick);
						if(1 == udsIoCtrlCfg[ioCtrl_i].dtSize)
						{
							*(uint8_t *)udsIoCtrlCfg[ioCtrl_i].lockVal = (uint8_t)(lockVal & 0xFF);
						}
						else if(2 == udsIoCtrlCfg[ioCtrl_i].dtSize)
						{
							*(uint16_t *)udsIoCtrlCfg[ioCtrl_i].lockVal = (uint16_t)(lockVal & 0xFFFF);
						}
						else
						{
							/*4 bytes*/
							*(uint32_t *)udsIoCtrlCfg[ioCtrl_i].lockVal = lockVal;
						}
					}
				}
			}
		}
		else
		{

		}
	}
}

/* Routine Control */
void serv_routineControl(void)
{
    uint8_t rtCtrlType = uds_popRequestData(0x00); /* load request routine control type (sub-function) */
    uint16_t curRTID = 0; /* Current request routine ID */
    uint16_t rtid_i = 0; /* Routine ID index */
    uint8_t rtRst = nrsp_positiveResponse; /* Pre set result to positive response state */
    
    if(TRUE != uds_31serviceAllowed())
    {
    	uds_negativeRsp(nrsp_conditionsNotCorrect);
    	return;
    }

    /* Routine control type check */
    if (0 != (udsRtGlobalMask & (0x01 << rtCtrlType)))
    {
        /* Sub function support */
        /* Get routine ID */
        curRTID = (uint16_t)(uds_popRequestData(0x00));
        curRTID = curRTID << 8;
        curRTID = curRTID | (uint16_t)(uds_popRequestData(0x00));

        /* Search routine */
        for (rtid_i = 0; rtid_i < udsRtIDcnt; rtid_i++)
        {
            if (curRTID == udsRtIDcfg[rtid_i].rtid)
            {
                /* Find RID */
                /* Pre-set active response information for callback function append response data */
                uds_pushRspData(rtCtrlType);
                uds_pushRspData((uint8_t)(curRTID >> 8));
                uds_pushRspData((uint8_t)(curRTID & 0x00FFu));
                
                /* check rtCtrl type */
                switch (rtCtrlType)
                {
                    case servm_rtidStartRoutine:
                        /* Check routine sub-function support (Callback valid check) */
                        if (NULL != udsRtIDcfg[rtid_i].rtStart)
                        {
                            /* sub-function support, call it */
                            rtRst = udsRtIDcfg[rtid_i].rtStart();
                            /* Check result */
                            if ((nrsp_positiveResponse == rtRst)
                                || (nrsp_requestCorrectlyReceivedResponsePending == rtRst))
                            {
                                /* routine start success */
                                /* Check rtTask call denied (Routine task callback valid check) */
                                if (NULL != udsRtIDcfg[rtid_i].rtTask)
                                {
                                    /* Set routine task */
                                    rtCtrlBkcMask = rtCtrlBkcMask | ((uint32_t)(0x01ul << rtid_i));
                                }
                            }
                        }
                        else
                        {
                            /* Sub-function not supported, set call result to requestOutOfRange NRSP */
                            /* Note: for start routine, please make sure never run to here */
                            rtRst = nrsp_requestOutOfRange;
                        }
                        break;
                    case servm_rtidStopRoutine:
                        /* Check routine sub-function support (Callback valid check) */
                        if (NULL != udsRtIDcfg[rtid_i].rtStop)
                        {
                            /* sub-function support, call it */
                            rtRst = udsRtIDcfg[rtid_i].rtStop();
                            /* Check task running flag */
                            if (0ul != (rtCtrlBkcMask & ((uint32_t)(0x01ul << rtid_i))))
                            {
                                /* Clear task */
                                rtCtrlBkcMask = rtCtrlBkcMask & (~((uint32_t)(0x01ul << rtid_i)));
                            }
                        }
                        else
                        {
                            /* Sub-function not supported, set call result to requestOutOfRange NRSP */
                            rtRst = nrsp_requestOutOfRange;
                        }
                        break;
                    case servm_rtidGetRoutineResult:
                        /* Check routine sub-function support (Callback valid check) */
                        if (NULL != udsRtIDcfg[rtid_i].rtGetResult)
                        {
                            /* sub-function support, call it */
                            rtRst = udsRtIDcfg[rtid_i].rtGetResult();
                        }
                        else
                        {
                            /* Sub-function not supported, set call result to requestOutOfRange NRSP */
                            rtRst = nrsp_requestOutOfRange;
                        }
                        break;
                    default:
                        /* Sub-function support flag set error, ignore and set rtRst (routine execute result) to sub function not support code */
                        rtRst = nrsp_subFunctionNotSupported;
                        break;
                }

                /* Routine call response check.
                 * If not start routine or routine task call back is NULL.
                 * Then if response pending, instead with nrsp_conditionsNotCorrect.
                 * (Only start routine can support response pending NRSP. */
                if ((servm_rtidStartRoutine != rtCtrlType)
                    || (NULL == udsRtIDcfg[rtid_i].rtTask))
                {
                    if (nrsp_requestCorrectlyReceivedResponsePending == rtRst)
                    {
                        rtRst = nrsp_conditionsNotCorrect;
                    }
                }

                /* routine call result check */
                if (nrsp_positiveResponse == rtRst )
                {
                    /* Active response */
                    uds_sendResponse();
                }
                else
                {
                    /* Negative response */
                    uds_negativeRsp(rtRst);
                }

                break; /* Find routine ID, stop search */
            }
        }

        /* Search result check */
        if (curRTID >= udsRtIDcnt)
        {
            /* Not find RID */
            uds_negativeRsp(nrsp_requestOutOfRange);
        }
    }
    else
    {
        uds_negativeRsp(nrsp_subFunctionNotSupported);
    }
}

static void uds_rtCtrBkc_task(void)
{
    uint16_t rtid_i = 0; /* routine ID index in config */
    bool bkcRst = TRUE;
    /* routine task run denied check */
    if (0ul != rtCtrlBkcMask)
    {
        /* have one or more task need running */
        for (; (rtid_i < udsRtIDcnt) && (rtid_i < 32); rtid_i++)
        {
            /* Check task running flag and task callback function valid state */
            if ((0ul != (rtCtrlBkcMask & ((uint32_t)(0x01ul << rtid_i))))
                && (NULL != udsRtIDcfg[rtid_i].rtTask))
            {
                /* Need to run task and task valid */
                /* Call task and check task response */
#if 0 /* Do not used conditon order relative decision */
                if ((sessionType_std == uds_getActiveSession())
                    || (TRUE == udsRtIDcfg[rtid_i].rtTask()))
                {
                    /* Task run compelete or  */
                    /* Clear task */
                    rtCtrlBkcMask = rtCtrlBkcMask & (~((uint32_t)(0x01ul << rtid_i)));
                }
#else

#if 0           /* Not auto stop when UDS switch to default session */
                /* session auto back check */
                if (sessionType_std == uds_getActiveSession())
                {
                    /* Task run compelete or  */
                    /* Stop Task if support */
                    if (NULL != udsRtIDcfg[rtid_i].rtStop)
                    {
                        udsRtIDcfg[rtid_i].rtStop();
                    }
                    /* Clear task */
                    rtCtrlBkcMask = rtCtrlBkcMask & (~((uint32_t)(0x01ul << rtid_i)));
                }
                else
#endif
                {
                    /* Run task */
                    bkcRst = udsRtIDcfg[rtid_i].rtTask();
                    /* Task compelete check */
                    if (TRUE == bkcRst)
                    {
                        /* Clear task */
                        rtCtrlBkcMask = rtCtrlBkcMask & (~((uint32_t)(0x01ul << rtid_i)));
                    }
                }
#endif
                
            }

            /* If no more any task need to running, break search */
            if (0ul == (rtCtrlBkcMask >> rtid_i))
            {
                break;
            }
        }
    }
}

/* DTC Process */
void serv_dtcInforClear(void)
{
	uint32_t dtcCodeGrp = 0;
	uint16_t indx =0;
   	if(0x03 == uds_remainRequestDLC())
   	{
   		while(uds_remainRequestDLC()>0)
   		{
			dtcCodeGrp <<= 8;
			dtcCodeGrp |= uds_popRequestData(0x00);
		}
		switch(dtcCodeGrp)
		{
			case DTC_POWERTRAIN_GROUP:
			case DTC_CHASSIS_GROUP:
			case DTC_BODY_GROUP:
			case DTC_NETWORK_COMMUNICATION_GROUP:
				for(;indx < dtcItemCnt; indx ++)
				{
					if((dtcInfoCfgGrp[indx].dtcCode >=  dtcCodeGrp) && (dtcInfoCfgGrp[indx].dtcCode< dtcCodeGrp +  0x00400000))
					{
						obd_clearFault(indx,TRUE);
					}
				}
				break;
			case DTC_ALL_GROUP:
				obd_clearAll();
				break;
			default:
				uds_negativeRsp(nrsp_requestOutOfRange);
				break;
		}
		if(uds_isNotYetRsp())
		{
			uds_sendResponse();
		}
	}
   	else
   	{
		uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
   	}
}

void serv_dtcInforRead(void)
{
	uint8_t subFun_i = 0;
	uint8_t subCmd = uds_popRequestData(0xFF);
	uint8_t nrspCode = nrsp_subFunctionNotSupported;
	uds_pushRspData(subCmd);
	for(;subFun_i < uds_reportDtcSupportSubFunNum; subFun_i ++)
	{
		if(uds_readDtcInfoCfgTable[subFun_i].subCmd == subCmd)
		{
			break;
		}
	}
	if(subFun_i < uds_reportDtcSupportSubFunNum)
	{
		if(NULL != uds_readDtcInfoCfgTable[subFun_i].subFunProcess)
		{
			nrspCode = uds_readDtcInfoCfgTable[subFun_i].subFunProcess();
		}
		else
		{
			uds_negativeRsp(nrsp_subFunctionNotSupported);
		}
	}
	else
	{
		uds_negativeRsp(nrsp_subFunctionNotSupported);
	}

	if(nrsp_positiveResponse == nrspCode)
	{
		uds_sendResponse();
	}
	else if (nrsp_requestCorrectlyReceivedResponsePending == nrspCode)
	{
		/*todo, read diagnostic information service currently can not support NRSP_pending  code*/
		uds_negativeRsp(nrsp_requestOutOfRange);
	}
	else
	{
		uds_negativeRsp(nrspCode);
	}
	
}



/* IO Control */
void serv_ioControl(void)
{
	uint8_t did_i = 0;
	uint16_t currID = 0;
	uint8_t popDt = 0;
	uint8_t byteIndx = 0;
	uint32_t physicalVal = 0;
	uint8_t nrsp_code = nrsp_positiveResponse;
	//void * pCurrPhysical = NULL;
	if(TRUE == uds_ioCtrlAllowed())
	{
		popDt = uds_popRequestData(0xFF);
		currID = popDt;
		currID <<= 8;
		uds_pushRspData(popDt);
		
		popDt = uds_popRequestData(0xFF);
		currID |= popDt;
		uds_pushRspData(popDt);
		for(;did_i < udsIoCtrl_itemCnt; did_i ++)
		{
			if(udsIoCtrlCfg[did_i].did == currID)
			{
				break;
			}
		}
		if(did_i < udsIoCtrl_itemCnt)
		{
			if((1 == udsIoCtrlCfg[did_i].dtSize) || (2 == udsIoCtrlCfg[did_i].dtSize)	\
			   || (4 == udsIoCtrlCfg[did_i].dtSize))
			{
				popDt = uds_popRequestData(0xFF);
				switch(popDt)
				{
					case servm_ioCtrl_returnCtrlToEcu:
						if(0x00 == uds_remainRequestDLC())
						{
							uds_pushRspData(servm_ioCtrl_returnCtrlToEcu);
#if 0
							if(NULL != udsIoCtrlCfg[did_i].lock)
							{
								(*udsIoCtrlCfg[did_i].lock) = FALSE;
							}
							udsIoCtrlDt[did_i].tick = 0;
							udsIoCtrlDt[did_i].elapseTick = 0;
							
							if(NULL != udsIoCtrlCfg[did_i].phy2Comm)
							{
								for(byteIndx = 0; byteIndx < udsIoCtrlCfg[did_i].dtSize; byteIndx ++)
								{
									physicalVal <<= 8;
									physicalVal |= (uint8_t)(*(uint8_t *)((uint32_t)udsIoCtrlCfg[did_i].addr + byteIndx));
								}
								udsIoCtrlCfg[did_i].phy2Comm(physicalVal);
							}
							else
							{
								for(byteIndx=0; byteIndx < udsIoCtrlCfg[did_i].dtSize;byteIndx ++)
								{
									uds_pushRspData(*(uint8_t *)(udsIoCtrlCfg[did_i].addr + byteIndx));
								}
							}
#endif
							if(NULL != udsIoCtrlCfg[did_i].phy2Comm)
							{
								udsIoCtrlCfg[did_i].phy2Comm(0);
							}
							uds_sendResponse();
						}
						else
						{
							uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
						}
						break;
					case servm_ioCtrl_resetToDefault:
						if(0x00 == uds_remainRequestDLC())
						{
							if((NULL != udsIoCtrlCfg[did_i].lock) &&(NULL != udsIoCtrlCfg[did_i].lockVal))
							{
								uds_pushRspData(servm_ioCtrl_returnCtrlToEcu);
								(*udsIoCtrlCfg[did_i].lock) = TRUE;
								util_memoryCpy8((uint8_t *)udsIoCtrlCfg[did_i].defVal,(uint8_t *)udsIoCtrlCfg[did_i].lockVal,udsIoCtrlCfg[did_i].dtSize);
								udsIoCtrlDt[did_i].tick = 0;		
								udsIoCtrlDt[did_i].elapseTick = 0;
								if(NULL != udsIoCtrlCfg[did_i].phy2Comm)
								{
									for(byteIndx = 0; byteIndx < udsIoCtrlCfg[did_i].dtSize; byteIndx ++)
									{
										physicalVal <<= 8;
										physicalVal |= (uint8_t)(*(uint8_t *)((uint32_t)udsIoCtrlCfg[did_i].defVal + byteIndx));
									}
									udsIoCtrlCfg[did_i].phy2Comm(physicalVal);
								}
								else
								{
									for(byteIndx=0; byteIndx < udsIoCtrlCfg[did_i].dtSize;byteIndx ++)
									{
										uds_pushRspData(*(uint8_t *)((uint32_t)udsIoCtrlCfg[did_i].defVal + byteIndx));
									}
								}
								uds_sendResponse();
							}
							else
							{
								uds_negativeRsp(nrsp_conditionsNotCorrect);
							}
						}
						else
						{
							uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
						}
						break;
					case servm_ioCtrl_freezeCurrentSt:
						if(0x00 == uds_remainRequestDLC())
						{
							uds_pushRspData(servm_ioCtrl_freezeCurrentSt);
							/*find the current value, then freeze it*/
							if((NULL != udsIoCtrlCfg[did_i].lock) && (NULL != udsIoCtrlCfg[did_i].lockVal))
							{
								/*support lock action*/
								if(TRUE == *udsIoCtrlCfg[did_i].lock)
								{
									/*current value is locked value*/
									for(byteIndx = 0; byteIndx < udsIoCtrlCfg[did_i].dtSize; byteIndx ++)
									{
										physicalVal <<= 8;
										physicalVal |= (uint8_t)(*(uint8_t *)((uint32_t)udsIoCtrlCfg[did_i].lockVal + byteIndx));
									}
								}
								else
								{
									/*this parameter  has not been locked,treat the *addr value as the current value*/
									for(byteIndx = 0; byteIndx < udsIoCtrlCfg[did_i].dtSize; byteIndx ++)
									{
										physicalVal <<= 8;
										physicalVal |= (uint8_t)(*(uint8_t *)udsIoCtrlCfg[did_i].addr);
										*((uint8_t *)((uint32_t)udsIoCtrlCfg[did_i].lockVal + byteIndx)) = (uint8_t)(*(uint8_t *)((uint32_t)udsIoCtrlCfg[did_i].addr + byteIndx));
									}

									
								}
								if(NULL != udsIoCtrlCfg[did_i].phy2Comm)
								{
									udsIoCtrlCfg[did_i].phy2Comm(physicalVal);
								}
								else
								{
									if(TRUE == *udsIoCtrlCfg[did_i].lock)
									{
										for(byteIndx=0; byteIndx < udsIoCtrlCfg[did_i].dtSize;byteIndx ++)
										{
											uds_pushRspData(*(uint8_t *)((uint32_t)udsIoCtrlCfg[did_i].lockVal + byteIndx));
										}
									}
									else
									{
										for(byteIndx=0; byteIndx < udsIoCtrlCfg[did_i].dtSize;byteIndx ++)
										{
											uds_pushRspData(*(uint8_t *)((uint32_t)udsIoCtrlCfg[did_i].addr + byteIndx));
										}
									}
								}
								*udsIoCtrlCfg[did_i].lock = TRUE;
								udsIoCtrlDt[did_i].tick = 0;
								udsIoCtrlDt[did_i].elapseTick = 0;
								uds_sendResponse();
								
							}
							else
							{
								uds_negativeRsp(nrsp_conditionsNotCorrect);
							}
						}
						else
						{
							uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
						}
						break;
					case servm_ioCtrl_shortTermAdjust:
#if 0
						if(0x02 < uds_remainRequestDLC())
						{
							uds_pushRspData(servm_ioCtrl_shortTermAdjust);
							if((NULL != udsIoCtrlCfg[did_i].lock) && (NULL != udsIoCtrlCfg[did_i].lockVal))
							{
								udsIoCtrlDt[did_i].elapseTick = 0;
								udsIoCtrlDt[did_i].tick = 0;
								popDt = uds_popRequestData(0x00);	
								udsIoCtrlDt[did_i].tick = popDt;
								uds_pushRspData(popDt);
								popDt = uds_popRequestData(0x00);	
								udsIoCtrlDt[did_i].tick <<= 8;
								udsIoCtrlDt[did_i].tick |= popDt;
								uds_pushRspData(popDt);
								/*at first read out the commnucation target value*/
								if(NULL != udsIoCtrlCfg[did_i].comm2Phy)
								{
									nrsp_code = udsIoCtrlCfg[did_i].comm2Phy((void *)&udsIoCtrlDt[did_i].targetVal);
									if(nrsp_positiveResponse == nrsp_code)
									{
										
									}
									else if(nrsp_requestCorrectlyReceivedResponsePending == nrsp_code)
									{
										/*don't support pending NRSP code in this service*/
										uds_negativeRsp(nrsp_conditionsNotCorrect);
									}
									else
									{
										uds_negativeRsp(nrsp_code);	
									}
									
								}
								else
								{
									if(udsIoCtrlCfg[did_i].dtSize == uds_remainRequestDLC())
									{
										udsIoCtrlDt[did_i].targetVal = 0;	
										for(byteIndx = 0; byteIndx < udsIoCtrlCfg[did_i].dtSize; byteIndx ++)
										{	
											udsIoCtrlDt[did_i].targetVal <<= 8;
											udsIoCtrlDt[did_i].targetVal |= uds_popRequestData(0x00);
										}
									}
									else
									{
										uds_negativeRsp(nrsp_requestOutOfRange);
									}
								}

								if(TRUE == uds_isNotYetRsp())
								{
									/*find the current physical value*/
									if(TRUE == *udsIoCtrlCfg[did_i].lock)
									{
										/*current value is locked value*/
										for(byteIndx = 0; byteIndx < udsIoCtrlCfg[did_i].dtSize; byteIndx ++)
										{
											physicalVal <<= 8;
											physicalVal |= (uint8_t)(*(uint8_t *)((uint32_t)udsIoCtrlCfg[did_i].lockVal + byteIndx));
										}
										pCurrPhysical = udsIoCtrlCfg[did_i].lockVal;
									}
									else
									{
										/*this parameter  has not been locked,treat the *addr value as the current value*/
										for(byteIndx = 0; byteIndx < udsIoCtrlCfg[did_i].dtSize; byteIndx ++)
										{
											physicalVal <<= 8;
											physicalVal |= (uint8_t)(*(uint8_t *)((uint32_t)udsIoCtrlCfg[did_i].addr + byteIndx));
										}
										pCurrPhysical = udsIoCtrlCfg[did_i].addr;
									}
									if(NULL != udsIoCtrlCfg[did_i].phy2Comm)
									{
										udsIoCtrlCfg[did_i].phy2Comm(physicalVal);
									}
									else
									{
										if(NULL != pCurrPhysical)
										{
											for(byteIndx = 0; byteIndx < udsIoCtrlCfg[did_i].dtSize; byteIndx ++)
											{
												uds_pushRspData(*(uint8_t *) ((uint32_t)pCurrPhysical + byteIndx));
											}	
										}
										else
										{
											uds_negativeRsp(nrsp_conditionsNotCorrect);
										}
										
									}
									
									if(TRUE == uds_isNotYetRsp())
									{
										if(0 == udsIoCtrlDt[did_i].tick)
										{
											if(1 == udsIoCtrlCfg[did_i].dtSize)
											{
												*(uint8_t *)udsIoCtrlCfg[did_i].lockVal = (uint8_t)(udsIoCtrlDt[did_i].targetVal & 0xFF);
											}
											else if(2 == udsIoCtrlCfg[did_i].dtSize)
											{
												*(uint16_t *)udsIoCtrlCfg[did_i].lockVal = (uint16_t)(udsIoCtrlDt[did_i].targetVal & 0xFFFF);
											}
											else
											{
												*(uint32_t *)udsIoCtrlCfg[did_i].lockVal = udsIoCtrlDt[did_i].targetVal;
											}
										}
										else
										{
											udsIoCtrlDt[did_i].initVal = physicalVal;
											/*the adjustment will be finished in the background task*/
										}
										*udsIoCtrlCfg[did_i].lock = TRUE;
										uds_sendResponse();
									}
								}
							}
							else
							{
								uds_negativeRsp(nrsp_conditionsNotCorrect);
							}
						}
#endif
						if(0x01 == uds_remainRequestDLC())
						{
							uint8_t ctrlRequest = 0;
							uds_pushRspData(servm_ioCtrl_shortTermAdjust);
							ctrlRequest = uds_popRequestData(0x00);
							uds_pushRspData(ctrlRequest);

							if(NULL != udsIoCtrlCfg[did_i].comm2Phy)
							{
								nrsp_code = udsIoCtrlCfg[did_i].comm2Phy((void *)&ctrlRequest);
								if(nrsp_positiveResponse == nrsp_code)
								{
									uds_sendResponse();
								}
								else
								{
									uds_negativeRsp(nrsp_code);
								}
							}
							else
							{
								uds_negativeRsp(nrsp_subFunctionNotSupported);
							}
						}
						else
						{
							uds_negativeRsp(nrsp_incorrectMessageLengthOrInvalidFormat);
						}
						break;
					default:
						uds_negativeRsp(nrsp_requestOutOfRange);	
						break;
				}
			}
			else
			{
				uds_negativeRsp(nrsp_conditionsNotCorrect);	
			}
			
		}
		else
		{
			uds_negativeRsp(nrsp_requestOutOfRange);
		}
	}
	else
	{
		uds_negativeRsp(nrsp_conditionsNotCorrect);
	}
}
/* ------------------ End Preset Service Process ------------------------------------ */


/* ------------------ Export UDS functions for service process ---------------------- */
uint8_t uds_getActiveSession(void)
{
    uint8_t actSession = sessionType_std;
    
    if (actSessionIndex < udsServCnt)
    {
        actSession = sessionTypeMap[actSessionIndex];
    }

    return actSession;
}

bool uds_saLevelUnlockCheck(uint8_t saLevel)
{
    uint8_t saLv_i = 0; /* SA level index */
    bool isUnlock = FALSE;

    /* Search level */
    for (; saLv_i < saLevelCnt; saLv_i++)
    {
        if (saLevel == saLevelMap[saLv_i].level)
        {
            /* Find check level */
            /* Unlock state check */
            if (0 == (actSaLockerMask & servm_saLvSptMask(saLv_i)))
            {
                isUnlock = TRUE;
                break;
            }
        }
    }
	
    return isUnlock;
}

bool uds_saLevelUnlockCheckByIndex(uint8_t saLv_index)
{
    bool isUnlock = FALSE;

    /* index range check */
    if (saLv_index < saLevelCnt)
    {
        /* index valid, check unlock state */
        if (0 == (actSaLockerMask & servm_saLvSptMask(saLv_index)))
        {
            /* is unlocked, set result to TRUE */
            isUnlock = TRUE;
        }
    }

    return isUnlock;
}

/* Re-make NT functions for service process */
/*
 * Function: uds_setPopDataIndex
 * Param<uint16_t index>: The request service data index (0 is the first data or sub-function after service ID)
 * Return<void>: 
 * Discription: 
 * Note: This function will no any response warning, if set out of range, it will always response defaltDt when pop data
*/
void uds_setPopDataIndex(uint16_t index)
{
    nt_req_pos_set(index + 1);
}

/*
 * Function: uds_popRequestData
 * Param<uint8_t defaultDt>: Return if no more data
 * Return<uint8_t>: The current index data or defaultDt if is end to request data buffer
 * Discription: After pop, index to next data
 * Note: 
*/
uint8_t uds_popRequestData(uint8_t defaultDt)
{
    return nt_req_pop_dt(defaultDt);
}

/*
 * Function:uds_remainRequestDLC
 * Param<void>:
 * Return<uint16_t>: The remain request data length after request data pot index
 * Discription: Get remain request bytes of not be read datas.
 * Note: 
*/
uint16_t uds_remainRequestDLC(void)
{
    return nt_req_remain_len();
}

/* Request process support functions */

/*
 * Function: uds_curRequestSID
 * Param<void>: 
 * Return<uint8_t>: return curRequestServID
 * Discription: 
 * Note: 
*/
uint8_t uds_curRequestSID(void)
{
    return curRequestServID;
}

/*
 * Function: uds_getRequestDataLen
 * Param<void>: 
 * Return<uint16_t>: The request data length (include sub-function, but not include service ID)
 * Discription: 
 * Note: 
*/
uint16_t uds_getRequestDataLen(void)
{
    return (nt_req_len() - 1);
}

/* Response proces support functions */


/*
 * Function:uds_rspPending
 * Param<void>:
 * Return<void>:
 * Discription:
 * Note: 1. Response Pending only active when NT is locked and can not auto unlock NT Layer after call this function.
 *       2. It will auto send response pengding service with uds_rsp_pd_cycle cycle in background before give the 
 *          valid (Negative or Positive) response.
 *       3. This function have no effect of the original response data (The push to response buffer data is remain valid).
*/
void uds_rspPending(void)
{
    uds_rsp_pending(curRequestServID);
}


/*
 * Function: uds_isRspPending
 * Param<void>:
 * Return<bool>: Result of current request is in auto send response pending NRSP or not. FALSE for Not.
 * Discription: Check the auto response pending notice state.
 * Note: 
*/
bool uds_isRspPending(void)
{
    return (bool)(uds_rsp_is_pending());
}

/*
 * Function:uds_isNotYetRsp
 * Param<void>:
 * Return<bool>: Service response state.
 *               TRUE: not send response(Positive, hide positive, negative, but not include response pending)
 *               FALSE: response triggled
 * Discription:
 * Note: 
*/
bool uds_isNotYetRsp(void)
{
    return (bool)(nt_is_locked());
}

/*
 * Function: uds_preActiveResponse
 * Param<uint8_t servID>: 
 * Return<void>: 
 * Discription: If send active response, is un-necessary to call this function before push response data.
 * Note: 
*/
void uds_preActiveResponse(uint8_t servID)
{
    /* Set response size to valid */
    rspSizeValid = TRUE;
    /* Pre-push positive response service id */
    nt_rsp_reset();
    nt_rsp_push_dt(servm_positiveResponeCode(servID));
}

/*
 * Function:uds_pushRspData
 * Param<uint8_t data>: data value
 * Return<void>:
 * Discription: Push a data to resonpse buffer
 * Note: 
*/
void uds_pushRspData(uint8_t data)
{
    rspSizeValid = nt_rsp_push_dt(data);
}

/*
 * Function:uds_pushRspBuf
 * Param<uint8_t buf[]>: Push buffer start pointer
 * Param<uint16_t len>: Push buffer length
 * Return<void>:
 * Discription:
 * Note: 
*/
void uds_pushRspBuf(uint8_t buf[], uint16_t len)
{
    rspSizeValid = nt_rsp_push_buf(buf, len);
}

/*
 * Function:uds_getResponseDataLen
 * Param<void>:
 * Return<uint16_t>: The response data length (not include response service ID)
 * Discription:
 * Note: 
*/
uint16_t uds_getResponseDataLen(void)
{
    return (nt_rsp_len() - 1);
}

/* Define the uds have sub-function service count (based ISO 14229-1) */
#define uds_haveSubFuncServCnt 12

/*
 * Function: uds_sendResponse
 * Param<void>: 
 * Return<void>: 
 * Discription: Start send response
 * Note: 
*/
void uds_sendResponse(void)
{
    /* Have sub-function service list define */
    const uint8_t subFunServList[uds_haveSubFuncServCnt] =
    {
        sid_sessionControl,
        sid_ecuReset,
        sid_securityAccess,
        sid_communicationControl,
        sid_testPresent,
        sid_accessTimingParamenter,
        sid_controlDTCSetting,
        sid_responseOnEvent,
        sid_linkControl,
        sid_dynamicallyDefineDataIdentifier,
        sid_readDTCInformation,
        sid_routineControl
    };

    uint8_t subFunServ_i = 0; /* subFunServList item index */
    bool actRspHide = FALSE;  /* active response hide check result */
    uint8_t nrspCode = 0;
    /* Response pending check (If response pending, always send real response) */
    if (FALSE == uds_isRspPending())
    {
    	/* Positive response check */
		if (0x7F != nt_rsp_get_data(0))
		{
			/* Not negative response */
			/* request valid */
			uds_requestValid();

			/* have sub-function service search */
			for (; subFunServ_i < uds_haveSubFuncServCnt; subFunServ_i++)
			{
				/* check request service */
				if (subFunServList[subFunServ_i] == curRequestServID)
				{
					/* request service have sub-function */
					/* check active response hide flag in sub-function code */
					if (0 != (curRequestSubFunc & uds_actResponseHideBySubFunctionMask))
					{
						/* Set hide active response flag */
						actRspHide = TRUE;
						break;
					}
				}
			}
		}
		else
		{
			nrspCode = nt_rsp_get_data(2);
			if (TRUE == nt_req_is_functional_check())
			{
				if ((nrsp_serviceNotSupported == nrspCode)
					|| (nrsp_subFunctionNotSupported == nrspCode)
					|| (nrsp_requestOutOfRange == nrspCode))
				{

					actRspHide = TRUE;
				}
			}
		}
    }


    /* Check active response hide flag */
    if (TRUE == actRspHide)
    {
        /* Not need response for active response */
        nt_unlock();
    }
    else
    {
        /* Check response size */
        if (FALSE == rspSizeValid)
        {
            /* Response data out of size */
            /* Send negative response with response too long information */
            uds_negativeRsp(nrsp_responseTooLong);
        }
        else
        {
            /* Response data in size */
            /* Send real response */
            nt_rsp_start();
        }
    }
}

/*
 * Function:uds_noRsp
 * Param<void>:
 * Return<void>:
 * Discription: The request not need send active response
 * Note: 
*/
void uds_noRsp(void)
{
    nt_unlock();
}

/*
 * Function: uds_negativeRsp
 * Param<uint8_t nrspCode>: negative response code
 * Return<void>: 
 * Discription: Transmit negative response and auto unlock NT Layser
 * Note: The request service ID is auto set.
*/
void uds_negativeRsp(uint8_t nrspCode)
{
    uint8_t nrspBuf[3];

    /* NRSP type check, if is response pending, used special response function */
    if (nrsp_requestCorrectlyReceivedResponsePending == nrspCode)
    {
        /* Is response pending */
        /* Call NT special function for response pending */
        uds_rspPending();
    }
    else
    {
        /* Other NRSP */
		if ((TRUE == nt_req_is_functional_check())
			&& (FALSE == uds_isRspPending())
			&& ((nrsp_serviceNotSupported == nrspCode)
				|| (nrsp_subFunctionNotSupported == nrspCode)
				|| (nrsp_requestOutOfRange == nrspCode)))
		{
			nt_unlock();
		}
		else
		{
			/* Set negative response value */
			nrspBuf[0] = servm_negativeResponseSID;
			nrspBuf[1] = curRequestServID;
			nrspBuf[2] = nrspCode;
			/* Push to NT response buffer */
			nt_rsp_set(nrspBuf, 3);
			/* Transmit response and unlock NT receive buffer */
			nt_rsp_start(); /* unlock NT layer after call this function */
		}
    }
}

#if 0 /* For test only */

/*
 * Function: uds_positiveRsp
 * Param<uint8_t rspDts[]>: response data
 * Param<uint16_t rspLen>: response data length (bytes)
 * Return<void>:
 * Discription: Transmit negative response and auto unlock NT Layser
 * Note: The request service ID is auto set.
*/
void uds_positiveRsp(uint8_t rspDts[], uint16_t rspLen)
{
    /* request valid */
    uds_requestValid();
    /* Pre-push positive response service id */
    nt_rsp_reset();
    nt_rsp_push_dt(curRequestServID);
    /* Push response data to NT send buffer */
    rspSizeValid = nt_rsp_push_buf(rspDts, rspLen);

    /* Check response size */
    if (TRUE == rspSizeValid)
    {
        /* Response data out of size */
        /* Send negative response with response too long information */
        uds_negativeRsp(nrsp_responseTooLong);
    }
    else
    {
        /* Response data in size */
        /* Send real response */
        nt_rsp_start();
    }
}

/*
 * Function: uds_positiveResponse
 * Param<uint8_t servID>: request service ID for positive response
 * Param<uint8_t rspDts[]>: response data
 * Param<uint16_t rspLen>: response data length (bytes)
 * Return<void>: 
 * Discription: Transmit positive response and auto unlock NT Layser
 * Note: 
*/
void uds_positiveResponse(uint8_t servID, uint8_t rspDts[], uint16_t rspLen)
{
    bool pushSuccess = FALSE;

#warning you need check the suppressPosRspMsgIndicationBit (bit 7) for all service of based UDS service define

    nt_rsp_reset();
    nt_rsp_push_dt(servID);
    pushSuccess = nt_rsp_push_buf(rspDts, rspLen);

    if (TRUE == pushSuccess)
    {
        nt_rsp_start();
    }
    else
    {
        uds_negativeResponse(servID, nrsp_responseTooLong);
    }
}

/*
 * Function: uds_negativeResponse
 * Param<uint8_t servID>: service ID for negative response
 * Param<uint8_t nrspCode>: negative response code
 * Return<void>: 
 * Discription: Transmit negative response and auto unlock NT Layser
 * Note: 
*/
void uds_negativeResponse(uint8_t servID, uint8_t nrspCode)
{
    uint8_t nrspBuf[3] = {servm_negativeResponseSID, servID, nrspCode};

    nt_rsp_set(nrspBuf, 3);
    nt_rsp_start(); /* unlock NT layer after call this function */
}
#endif

/* ------------------ End Export UDS functions for service process ------------------ */

