/*
* Copyright (c) 2016,SHENZHEN HANGSHENG ELECTRONICS Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File: uds_cfg.c
* Description: 
* History: 
* 2016-5-20, 1007040, original
*/

#include "uds_types.h"
#include "global_var.h"
#include "sysClk.h"
#include "uds.h"
#include "uds_cfg.h"
#include "uds_user.h"

/* -------------- user addition functions, macros, variables declare ----------- */
static uint8_t uds_getSaLvIndex(uint8_t saLevel);
static bool uds_seedKeyMatchCheck(uint32_t seed,uint32_t key);

static uint8_t uds_testRotutineStart(void);
static uint8_t uds_testRotutineStop(void);
static uint8_t uds_testRotutineQuery(void);

	




/* -------------- End user addition functions, macros, variables declare ------- */



/* ---------------------------------- UDS service support config ----------------------------------------------- */

/* ------------------- UDS Session map define ------------------- */
const uint8_t salevelXOR[4] = {0xF4,0x61,0xF6,0xA1};
/* Map order mast fit with ssSpt_t enum item define order */
const uint8_t sessionTypeMap[ssSptCnt] =
{
    /* ssSpt_std  --> */ sessionType_std,
    /* ssSpt_prog --> */ sessionType_prog,
    /* ssSpt_ext  --> */ sessionType_ext
};

/* Session support count */
const uint8_t sessionTypeCnt = ssSptCnt;
/* ----------------- End UDS Session map define ---------------------- */

/* ----------------- UDS security access level map ------------------- */
#define m_fblseed_lng	4
#define m_fblkey_lng	4
#define m_caliseed_lng	4
#define m_calikey_lng	4
#define m_lvl1seed_lng	4
#define m_lvl1key_lng	4

uint8_t saLvSeedBuf_fbl[m_fblseed_lng] = {0, 0, 0, 0};
uint8_t saLvKeyBuf_fbl[m_fblkey_lng] = {0, 0, 0, 0};

uint8_t saLvSeedBuf_cali[m_caliseed_lng] = {0, 0, 0, 0};
uint8_t saLvKeyBuf_cali[m_calikey_lng] = {0, 0, 0, 0};

uint8_t saLvSeedBuf_lvl1[m_lvl1seed_lng] = {0, 0, 0, 0};
uint8_t saLvKeyBuf_lvl1[m_lvl1key_lng] = {0, 0, 0, 0};

const saLevelMap_t saLevelMap[saLvSptCnt] =
{
                       /* SA Level,   Seed Buffer,   Buf DLC  */
    /* saLvSpt_fbl  --> */ {saLv_fbl_lvl,       saLvSeedBuf_fbl,    m_fblseed_lng,	saLvKeyBuf_fbl,		m_fblkey_lng},
	/* saLvSpt_cali --> */ {saLv_cali_lvl,      saLvSeedBuf_cali,  m_caliseed_lng,	saLvKeyBuf_cali, 	m_calikey_lng},
	/* saLvSpt_lvl1 --> */ {saLv_lvl1_lvl,      saLvSeedBuf_lvl1,  m_lvl1seed_lng,	saLvKeyBuf_lvl1, 	m_lvl1key_lng},
	
};

/* Security access suport levels count */
const uint8_t saLevelCnt = saLvSptCnt;
/* ----------------- End UDS security access level map --------------- */

/* ----------------- Session Control and Security Access related configs -------------- */
/* Automatic switch and locked session time */
const uint32_t ssSwStd_saLockTime = 500ul; /* [UDS task cycle] */
/* Security access protect paramters */
const uint8_t saProtectLimitCnt = 3;
const uint32_t saProtectDelayTime = 10000ul; /* [UDS task cycle], Delay to access unlock forbid after protect active */
const uint32_t saProtectPowerOnDelayTime = 100ul; /* [UDS task cycle, Delay to access unlock forbit after reset or power-on  */
/* ----------------- End Session Control and Security Access related configs ---------- */

/* ----------------- UDS service support config ---------------------- */
/* Note: Set callback funtion to serv_sessionControl and serv_securityAccess 
 *       if need used preset Session & Security Management of UDS module */
const udsServCfg_t udsServCfg[udsServiceSptCnt] =
{
    /* SID,                                            Session Support Mask,                               saDeniedMask,                    minDLC,         process function */
/* Pre-define service */
    {sid_sessionControl,             	ssSptMask_all,                     saLvSptMask_none,            1,      &serv_sessionControl},
    {sid_securityAccess,             	ssSptMask_ext | ssSptMask_prog,    saLvSptMask_none,            1,      &serv_securityAccess},
    {sid_testPresent,                	ssSptMask_all,                     saLvSptMask_none,            1,      &serv_testPresent},
    {sid_readDataByIdentifier,       	ssSptMask_all,                     saLvSptMask_fbl,            2,      &serv_didRead},
    {sid_writeDataByIdentifier,      	ssSptMask_all,                     saLvSptMask_fbl,  			3,      &serv_didWrite},
    {sid_readMemoryByAddress,        	ssSptMask_all,                     saLvSptMask_none,            3,      &serv_readMemoryByAddr},
    {sid_writeMemoryByAddress,       	ssSptMask_all,                     saLvSptMask_none,            4,      &serv_writeMemoryByAddr},
    {sid_routineControl,             	ssSptMask_all,                     saLvSptMask_none,            3,      &serv_routineControl},
/* User define service */
    {sid_ecuReset,                   	ssSptMask_all,                     saLvSptMask_none,            1,      &serv_ecuReset},
    {sid_communicationControl,       	ssSptMask_ext | ssSptMask_prog,    saLvSptMask_none,            2,      &serv_comControl},
    {sid_controlDTCSetting,          	ssSptMask_ext | ssSptMask_prog,    saLvSptMask_none,            1,      &serv_dtcSetting},
    {sid_inputOutputControlByIdentifier,ssSptMask_all,    				   saLvSptMask_cali,            3,      &serv_ioControl},
    {sid_requestDownload,            	ssSptMask_prog,                    saLvSptMask_fbl,             5,      &serv_requestDownload},
    {sid_transferData,               	ssSptMask_prog,                    saLvSptMask_fbl,             3,      &serv_transferData},
    {sid_requestTransferExit,        	ssSptMask_prog,                    saLvSptMask_fbl,             0,      &serv_requestTransferExit},
/*dtc Control service*/
	{sid_clearDiagnosticInformation,    ssSptMask_all,                     saLvSptMask_none,            3,      &serv_dtcInforClear},
	{sid_readDTCInformation,    		ssSptMask_all,                     saLvSptMask_none,            1,      &serv_dtcInforRead}
	
};
/* Service support count */
const uint8_t udsServCnt = udsServiceSptCnt;
/* ----------------- End UDS service support config ------------------ */

/* ----------------- DID config -------------------------------------- */






/* DID support count */

/* ----------------- End DID config ---------------------------------- */

/* ----------------- Routine Control Global Mask --------------------- */
const uint8_t udsRtGlobalMask = servm_rtGlobalMask_all;
/* ----------------- End Routine Control Global Mask ----------------- */

/* ----------------- Routine control config -------------------------- */
const udsRtIDcfg_t udsRtIDcfg[udsRtIDsptCnt] =
{
    /* RID value,                       Start routine,  Stop routine,         Get routine result, 		Routine Task */
    {rtid_eraseMemory,                  NULL,        	NULL,                 NULL,               NULL},
    {rtid_checkProgrammingDependencies, NULL,		 	NULL,                 NULL,               NULL},
    {rtid_hs_stayInBoot,                NULL,        	NULL,                 NULL,               NULL},
    {rtid_testProcess,					&uds_testRotutineStart,        &uds_testRotutineStop,                 &uds_testRotutineQuery,               NULL}
};

const uint16_t udsRtIDcnt = udsRtIDsptCnt;

/* ----------------- End Routine control config ---------------------- */

/*chip memory range configure*/
const chipMemCfg_t chipMemCfg[chipMem_cnt] = 
{
	{0x400000,	0x403FFF,	NULL,	NULL},
	{0x800000,	0x817FFF,	NULL,	NULL},
	{0xF98000,	0x11FFFFF,	NULL,	NULL},
	{0x40000000,0x4005FFFF,	NULL,	NULL}
};

/*end chip memory range configure*/

/*IO control parameters*/

static const uint16_t hld_ai_c1Def = 0;


const udsIoCtrlCfg_t udsIoCtrlCfg[udsIoCtrl_itemCnt] = 
{
	

};




/*end IO control parameters*/



/* ---------------------------------- End UDS service support config ----------------------------------------------- */

/* ----------------- UDS basic support callback functions ------------ */
/*
 * Function:uds_sessionTimeout
 * Param<void>:
 * Return<void>:
 * Discription: This callback is be called when UDS detect no any service request in not-default session.
 * Note: Before call this function, the session is auto switch to default, and SA was been auto locked.
*/
void uds_sessionTimeout(void)
{
}
/* ----------------- End UDS basic support callback functions -------- */


/* ---------------------------------- UDS pre-set service support callback functions ---------------------------------- */
/*
 * Function: uds_sessionSwitch
 * Param<uint8_t actSession>: Current active session ID (Not index)
 * Param<uint8_t tragetSession>: Target Session ID (Not index)
 * Return<bool>: 0x00 for active response (Accept), Others for negtive response (Reject, Used NRSP Code)
 *               See uds_types.h Standard Negative Response Code
 * Discription: 
 * Note: 1. For read more request data, used uds_popRequestData. It read start from next data of request switch session
 *       2. For response more data after request switch session, used uds_pushRspData function to push data to response after request switch session.
*/
uint8_t uds_sessionSwitch(uint8_t actSession, uint8_t tragetSession)
{
    uint16_t swMask = ssSwitchMask(actSession, tragetSession);
    uint8_t  nrspCode = nrsp_conditionsNotCorrect;
    uint8_t  ss_i = 0; /* session index */

    /* Sub-function search */
    for (; ss_i < sessionTypeCnt; ss_i++)
    {
        if (tragetSession == sessionTypeMap[ss_i])
        {
            break;
        }
    }
    /* Search result check */
    if (ss_i >= sessionTypeCnt)
    {
        /* Sub-function not support */
        nrspCode = nrsp_subFunctionNotSupported;
    }
	else
	{
		/* Switch support check */
	    switch(swMask)
	    {
	        case ssSwitch_stdToExt:
	        case ssSwitch_extToStd:
	        case ssSwitch_progToStd:
	            nrspCode = nrsp_positiveResponse; /* Accept switch */
	            break;
	        case ssSwitch_extToProg:
	            /* You may need check the program condition here
	             * Eg1. Check speed, ign state, High-Voltage system state for ensure program safety in application
	             * Eg2. Check ign state, power supply voltage in bootloader */
	            if(uds_updateAppAllowed())
	            {
	            	
					nrspCode = nrsp_positiveResponse; /* Accept switch */
				}
				else
				{
					nrspCode = nrsp_conditionsNotCorrect; /* NOT Accept switch */
				}
	            
	            break;
	        default:
	            //nrspCode = nrsp_subFunctionNotSupportedInActiveSession; /* Not supported switch */
	            nrspCode = nrsp_positiveResponse;
	            break;
	    }
	}
    return nrspCode;
}

/*
 * Function:uds_saUpdateSeed
 * Param<uint8_t saLevel>: update seed SA level
 * Return<void>: 
 * Discription: Update the target level seed buffer (The seed buffer will be response to client)
 * Note: You may need call the timer module counter value to random some bytes.
*/
void uds_saUpdateSeed(uint8_t saLevel)
{
    uint8_t saLv_i = uds_getSaLvIndex(saLevel); /* get SA level index */
    uint8_t seedDt_i = 0; /* seed data index */

    if (saLv_i < saLevelCnt) /* SA level index valid check */
    {
        /* SA level index is valid (SA Level is be support) */
        if ((NULL != saLevelMap[saLv_i].seedBuf) /* seed buffer un-empty */
            && saLevelMap[saLv_i].seedLen > 0)   /* and seed buffer size above zero(0) */
        {
            if (TRUE == uds_saLevelUnlockCheckByIndex(saLv_i)) /* is in unlock state */
            {
                /* update seed buffer with 0x00 */
                for (; seedDt_i < saLevelMap[saLv_i].seedLen; seedDt_i++)
                {
                    saLevelMap[saLv_i].seedBuf[seedDt_i] = 0x00u;
                }
            }
            else
            {
                /* initial random seed */
                (void)sys_random(TRUE);
                /* generate seed to target(saLevelMap indicate) buffer */
                for (; seedDt_i < saLevelMap[saLv_i].seedLen; seedDt_i++)
                {
                    saLevelMap[saLv_i].seedBuf[seedDt_i] = (uint8_t)(sys_random(FALSE) & 0xFFul);
                }
            }
        }
    }
}

/*
 * Function:uds_saCheckKey
 * Param<uint8_t saLevel>:
 * Return<uint8_t>: Check result, 0x00 for positive response (Accept), Others for negtive response (Reject, Used NRSP Code).
 * Discription: The key data need use uds_popRequestData function to get. First call return key[0]
 *              The key lenth need used uds_remainRequestDLC function to get before call uds_popRequestData function,
 *              or used uds_getRequestDataLen() - 1 to get.
 * Note: If the current saLevel is already in unlocked state, it will not call this function and any key check after seed get will be accept.
*/
uint8_t uds_saCheckKey(uint8_t saLevel)
{
	uint8_t nrspCode =  nrsp_positiveResponse;
	uint8_t keyByteIndx = 0;
	uint32_t seed = 0;
	uint32_t remotekey = 0;
	uint32_t localkey = 0;
	uint8_t salevelCAL[4];
	uint8_t keyBuf[4];
	char sa_i;

	switch(saLevel)
	{
		case saLv_fbl_lvl:
			if(TRUE != uds_saLevelUnlockCheckByIndex(saLvSpt_fbl)) /* is in unlock state */
			{
				if(saLevelMap[saLvSpt_fbl].keyLen!= uds_remainRequestDLC())
				{
					nrspCode = nrsp_invalidKey;
				}
				else
				{
					for(sa_i = 0;sa_i < saLevelMap[saLvSpt_fbl].keyLen;sa_i++)
					{
						saLevelMap[saLvSpt_fbl].keyBuf[sa_i] = uds_popRequestData(0x00);
					}

					remotekey = *((uint32_t *)saLevelMap[saLvSpt_fbl].keyBuf);

					for(sa_i = 0;sa_i < 4;sa_i++)
					{
						salevelCAL[sa_i] = saLevelMap[saLvSpt_fbl].seedBuf[sa_i]^salevelXOR[sa_i];
					}

					keyBuf[0] = ((salevelCAL[0]&0xf)<<4)|(salevelCAL[1]&0xf);
					keyBuf[1] = ((salevelCAL[1]&0xf0)>>4)|((salevelCAL[2]&0xf)<<4);
					keyBuf[2] = ((salevelCAL[2]&0xf0)>>4)|(salevelCAL[3]&0xf0);
					keyBuf[3] = (salevelCAL[3]&0xf)|((salevelCAL[0]&0xf0)>>4);
					localkey = *((uint32_t *)keyBuf);

					if(TRUE != uds_seedKeyMatchCheck(localkey,remotekey))
					{
						nrspCode = nrsp_invalidKey;
					}
				}
			}
			else
			{
				/*already in unlocked state, any key check is considered as pass*/
			}
			
			break;
		case saLv_cali_lvl:
			if(TRUE != uds_saLevelUnlockCheckByIndex(saLvSpt_cali)) /* is in unlock state */
			{
				if(saLevelMap[saLvSpt_cali].keyLen!= uds_remainRequestDLC())
				{
					nrspCode = nrsp_invalidKey;
				}
				else
				{
					while(keyByteIndx < saLevelMap[saLvSpt_cali].keyLen)
					{
						saLevelMap[saLvSpt_cali].keyBuf[keyByteIndx ++] = uds_popRequestData(0x00);
					}
					seed = *((uint32_t *)saLevelMap[saLvSpt_cali].seedBuf);
					remotekey = *((uint32_t *)saLevelMap[saLvSpt_cali].keyBuf);
					if(TRUE != uds_seedKeyMatchCheck(seed,remotekey))
					{
						nrspCode = nrsp_invalidKey;
					}
				}
			}
			else
			{
				/*already in unlocked state, any key check is considered as pass*/
			}
			break;
		case saLv_lvl1_lvl:
		{
			if(TRUE != uds_saLevelUnlockCheckByIndex(saLvSpt_lvl1)) /* is in unlock state */
			{
				if(saLevelMap[saLvSpt_lvl1].keyLen!= uds_remainRequestDLC())
				{
					nrspCode = nrsp_invalidKey;
				}
				else
				{
					for(sa_i = 0;sa_i < saLevelMap[saLvSpt_lvl1].keyLen;sa_i++)
					{
						saLevelMap[saLvSpt_lvl1].keyBuf[sa_i] = uds_popRequestData(0x00);
					}

					remotekey = *((uint32_t *)saLevelMap[saLvSpt_lvl1].keyBuf);

					for(sa_i = 0;sa_i < 4;sa_i++)
					{
						salevelCAL[sa_i] = saLevelMap[saLvSpt_lvl1].seedBuf[sa_i]^salevelXOR[sa_i];
					}

					keyBuf[0] = ((salevelCAL[0]&0xf)<<4)|(salevelCAL[1]&0xf0);
					keyBuf[1] = ((salevelCAL[1]&0xf)<<4)|((salevelCAL[2]&0xf0)>>4);
					keyBuf[2] = (salevelCAL[2]&0xf0)|((salevelCAL[3]&0xf0)>>4);
					keyBuf[3] = ((salevelCAL[3]&0xf)<<4)|(salevelCAL[0]&0xf);
					localkey = *((uint32_t *)keyBuf);

					if(TRUE != uds_seedKeyMatchCheck(localkey,remotekey))
					{
						nrspCode = nrsp_invalidKey;
					}
				}
			}
			else
			{
				/*already in unlocked state, any key check is considered as pass*/
			}
		}
		break;
		default:
			nrspCode = nrsp_subFunctionNotSupported;
			break;
	}
    return nrspCode; 
}

/*
 * Function:uds_didRwReady
 * Param<void>:
 * Return<void>:
 * Discription: This function used to initialize before call UDS DID read/write process function.
 * Note: Normally is empty function if not used background process for DID read/write.
*/
void uds_didRwReady(void)
{
    return;
}


uint8_t  uds_getMemIndx(uint32_t memory, uint16_t size)
{
	uint8_t chipMemIndx = 0xFF;
	if(size > 0)
	{
		for(chipMemIndx = 0;chipMemIndx < chipMem_cnt; chipMemIndx ++)
		{
			if((memory >= chipMemCfg[chipMemIndx].startAddr) && ((memory + size - 1) <= chipMemCfg[chipMemIndx].endAddr))
			{
				break;
			}
		}
	}
	return((chipMemIndx < chipMem_cnt)?(chipMemIndx):(0xFF));
}

/*UDS IO control function region*/
bool uds_ioCtrlAllowed(void)
{
	bool allowed = TRUE;
	/*<todo> here need to judge the pre-condition of the IO-control*/
	return allowed;
	
}


/* ------------------- --------------- End UDS pre-set service support callback functions ---------------------------------- */

/* -------------- user addition functions, macros, variables define ------------- */
/*
 * Function:uds_getSaLvIndex
 * Param<uint8_t saLevel>:
 * Return<static uint8_t>: 0xFF for not find, others for active SA level index
 * Discription:
 * Note: 
*/
static uint8_t uds_getSaLvIndex(uint8_t saLevel)
{
    uint8_t saLv_i = 0;
    
    /* Search level */
    for (; saLv_i < saLevelCnt; saLv_i++)
    {
        if (saLevel == saLevelMap[saLv_i].level)
        {
            break;
        }
    }

    /* if not find, set index to 0xFF */
    if (saLv_i >= saLevelCnt)
    {
        saLv_i = 0xFFu;
    }

    return saLv_i;
}


static bool uds_seedKeyMatchCheck(uint32_t seed,uint32_t key)
{
	/*below as an example, <todo> the final algorithm should be supplied by DNTC*/
	return ((seed == key)?(TRUE):(FALSE));
}


static uint8_t uds_testRotutineStart(void)
{
	uint8_t nrspCode = 	nrsp_positiveResponse;
	return nrspCode;
};

static uint8_t uds_testRotutineStop(void)
{
	uint8_t nrspCode = 	nrsp_positiveResponse;
	return nrspCode;
};

static uint8_t uds_testRotutineQuery(void)
{
	uint8_t nrspCode = 	nrsp_positiveResponse;
	return nrspCode;
};


/* -------------- End user addition functions, macros, variables define --------- */


