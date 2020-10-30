/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:obd.c
* Description: on board diagnose
* REQ IDs:
* History:
* 2017-02-20, Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#define OBD_C_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "uds_user.h"
#include "obd.h"
#include "udsDidCfg.h"
#include "uds_cfg.h"
/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/
/*if put the elements with a most recently modification into  the first position of the  frame order buffer */


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
static void obd_storeFreezeFrame(uint16_t fault_i);
static void obd_updateSnapshot(uint16_t fault_i);
static void obdClearff(uint16_t fault_i);
static bool obd_isEnable(void);


/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/



void obd_defDtInit(void)
{
#if 0
	uint16_t indx = 0;
	for(indx = 0; indx < dtcItemCnt; indx ++)
	{
		obdDiagDtGrp[indx].faultCnt = 0;
		obdDiagDtGrp[indx].ageCnt = 0;
		obdDiagDtGrp[indx].dcCnt = 0;
		obdDiagDtGrp[indx].isStored = FALSE;
		obdDiagDtGrp[indx].dtcStatus.all = 0X50;
	}

	for(indx = 0; indx < dtcFreezeFrameCnt; indx ++)
	{
		m_ff_setOrder(indx,m_ff_emptyMask | indx);
		obdFFDtGrp[indx].fault_i = 0xFFFF;
		obdFFDtGrp[indx].ffPrio = m_dtcPrio_none;
		
	}
	obd_snapShotDefDtInit();
#endif
}


void obd_init(void)
{
	uint16_t ft_i = 0;
	for(;ft_i < dtcItemCnt; ft_i ++)
	{
		obdDiagDtGrp[ft_i].dtcStatus.b.testFailed = 0;
		obdDiagDtGrp[ft_i].dtcStatus.b.testFailedThisOperationCycle = 0;
		obdDiagDtGrp[ft_i].dtcStatus.b.testNotCompleteThisOperationCycle = 1;
		obdDiagDtGrp[ft_i].faultCnt = 0;

	}
#if 0
	uint16_t fault_i = 0;
	if(((m_eehBlkSt_Ok == eeh_getBlkSt(m_eeBlk_diagDt)) || (m_eehBlkSt_Ok == eeh_getBlkSt(m_eeBlk_diagDtBkup))) &&
	  ((m_eehBlkSt_Ok == eeh_getBlkSt(m_eeBlk_ff)) 		 || (m_eehBlkSt_Ok == eeh_getBlkSt(m_eeBlk_ffBkup))) &&
	  ((m_eehBlkSt_Ok == eeh_getBlkSt(m_eeBlk_ffOrder)) || (m_eehBlkSt_Ok == eeh_getBlkSt(m_eeBlk_ffOrderBkup))))
	{
		for(;fault_i < dtcItemCnt; fault_i++)
	    {

			if((0 != obdDiagDtGrp[fault_i].dtcStatus.b.testFailed) ||
			(0 != obdDiagDtGrp[fault_i].dtcStatus.b.testFailedThisOpearionCycle) ||	\
			(1 != obdDiagDtGrp[fault_i].dtcStatus.b.testNotCompleteThisOperationCycle) ||	\
			(0 != obdDiagDtGrp[fault_i].faultCnt))
			{
				anv_modifyReq(m_eeBlk_diagDt);
			}
			obdDiagDtGrp[fault_i].dtcStatus.b.testFailed = 0;
			obdDiagDtGrp[fault_i].dtcStatus.b.testFailedThisOpearionCycle = 0;
			obdDiagDtGrp[fault_i].dtcStatus.b.testNotCompleteThisOperationCycle = 1;
			obdDiagDtGrp[fault_i].faultCnt = 0;

	    }
	}
	else
	{
		obd_clearAll();
		anv_modifyReq(m_eeBlk_diagDt);
		anv_modifyReq(m_eeBlk_ff);
		anv_modifyReq(m_eeBlk_ffOrder);
	}
#endif
}

void obd_updateFault(uint16_t fault_i,bool occured)
{
	if(fault_i < dtcItemCnt)
	{
		if(TRUE == uds_dtcSettingOn && TRUE == obd_isEnable())
		{
			if(TRUE == occured)
			{
				if(obdDiagDtGrp[fault_i].faultCnt + dtcInfoCfgGrp[fault_i].fltInc < dtcInfoCfgGrp[fault_i].fltLimit)
				{
					obdDiagDtGrp[fault_i].faultCnt += dtcInfoCfgGrp[fault_i].fltInc;
				}
				else
				{
					obdDiagDtGrp[fault_i].faultCnt = dtcInfoCfgGrp[fault_i].fltLimit;
					obdDiagDtGrp[fault_i].dtcStatus.b.testFailed = 1;
					obdDiagDtGrp[fault_i].dtcStatus.b.pendingDtc = 1;
					obdDiagDtGrp[fault_i].dtcStatus.b.testNotCompleteSinceLastClear = 0;
					obdDiagDtGrp[fault_i].dtcStatus.b.testFailedSinceLastClear = 1;
					obdDiagDtGrp[fault_i].dtcStatus.b.testNotCompleteThisOperationCycle = 0;
					if(0 == obdDiagDtGrp[fault_i].dtcStatus.b.testFailedThisOperationCycle)
					{
						obdDiagDtGrp[fault_i].dtcStatus.b.testFailedThisOperationCycle = 1;
						if(obdDiagDtGrp[fault_i].dcCnt < dtcInfoCfgGrp[fault_i].dcLimit - 1)
						{
							obdDiagDtGrp[fault_i].dcCnt += 1;
						}
						else
						{
							obdDiagDtGrp[fault_i].dcCnt = dtcInfoCfgGrp[fault_i].dcLimit;
							if(0xFF != dtcInfoCfgGrp[fault_i].ageLimit)
							{
								obdDiagDtGrp[fault_i].ageCnt = 0;
								obdDiagDtGrp[fault_i].dtcStatus.b.confirmDtc = 1;
								obdDiagDtGrp[fault_i].dtcStatus.b.warnIndi = 1;
								if(m_dtcPrio_none != dtcInfoCfgGrp[fault_i].prio)
								{
									obd_storeFreezeFrame(fault_i);
								}
								
							}
							
						}
						
					}
				}
			}
			else
			{
				if(obdDiagDtGrp[fault_i].faultCnt > dtcInfoCfgGrp[fault_i].fltDec)
				{
					obdDiagDtGrp[fault_i].faultCnt -= dtcInfoCfgGrp[fault_i].fltDec;
				}
				else
				{
					obdDiagDtGrp[fault_i].faultCnt = 0;
					obdDiagDtGrp[fault_i].dtcStatus.b.testFailed = 0;
					obdDiagDtGrp[fault_i].dtcStatus.b.testNotCompleteSinceLastClear = 0;
					obdDiagDtGrp[fault_i].dtcStatus.b.testNotCompleteThisOperationCycle = 0;
				}
			}
		}
	}	

}


void obd_shutdown(void)
{

	uint16_t ft_i = 0;
	for(;ft_i < dtcItemCnt; ft_i ++)
	{
		if(0 == obdDiagDtGrp[ft_i].dtcStatus.b.testFailedThisOperationCycle)
		{
			if(0 == obdDiagDtGrp[ft_i].dtcStatus.b.testNotCompleteThisOperationCycle)
			{
				 obdDiagDtGrp[ft_i].dtcStatus.b.pendingDtc = 0;
				 if(1 == obdDiagDtGrp[ft_i].dtcStatus.b.confirmDtc)
				 {
					if(obdDiagDtGrp[ft_i].ageCnt < dtcInfoCfgGrp[ft_i].ageLimit)
					{
						obdDiagDtGrp[ft_i].ageCnt ++;
						if(dtcInfoCfgGrp[ft_i].clearWarnCycle == obdDiagDtGrp[ft_i].ageCnt)
						{
							obdDiagDtGrp[ft_i].dtcStatus.b.warnIndi = 0;
						}
						if((dtcInfoCfgGrp[ft_i].ageLimit != 0xFF) &&
						   (dtcInfoCfgGrp[ft_i].ageLimit == obdDiagDtGrp[ft_i].ageCnt))
						{
							obd_clearFault(ft_i,FALSE);
						}
					}
				 }
				 else
				 {
					if(obdDiagDtGrp[ft_i].dcCnt > 0)
					{
						obdDiagDtGrp[ft_i].dcCnt --;
					}
				 }
			}
		}
		else
		{
			obdDiagDtGrp[ft_i].ageCnt = 0;
		}
		
	}
}


void obd_clearAll(void)
{
	uint16_t fault_i = 0;
	uint16_t ff_i = 0;
	for(;fault_i < dtcItemCnt; fault_i ++)
	{
		obdDiagDtGrp[fault_i].ageCnt = 0;
		obdDiagDtGrp[fault_i].dcCnt = 0;
		obdDiagDtGrp[fault_i].dtcStatus.all = 0x50;
		/*same function as below sentences*/
		/*
			obdDiagDtGrp[fault_i].dtcStatus.b.warnIndi = 0;
			obdDiagDtGrp[fault_i].dtcStatus.b.testNotCompleteThisOperationCycle = 1;
			obdDiagDtGrp[fault_i].dtcStatus.b.testFailedSinceLastClear = 0;
			obdDiagDtGrp[fault_i].dtcStatus.b.testNotCompleteSinceLastClear = 1;
			obdDiagDtGrp[fault_i].dtcStatus.b.confirmDtc = 0;
			obdDiagDtGrp[fault_i].dtcStatus.b.pendingDtc = 0;
			obdDiagDtGrp[fault_i].dtcStatus.b.testFailedThisOpearionCycle = 0;
			obdDiagDtGrp[fault_i].dtcStatus.b.testFailed = 0;
		*/
		obdDiagDtGrp[fault_i].faultCnt = 0;
		obdDiagDtGrp[fault_i].isStored = FALSE;
		
	}
	for(;ff_i < dtcFreezeFrameCnt;  ff_i ++)
	{
		obdFFDtGrp[ff_i].fault_i = 0xFFFF;		/*invalid fault index*/
		obdFFDtGrp[ff_i].ffPrio = m_dtcPrio_none;
		obdFFOrder[ff_i] = (m_ff_emptyMask | ff_i);
	}
	

	//anv_modifyReq(m_eeBlk_snapShotRecord);
}



void obd_clearFault(uint16_t fault_i, bool udsClear)
{
	if(fault_i < dtcItemCnt)
	{
		obdDiagDtGrp[fault_i].faultCnt = 0;
		obdDiagDtGrp[fault_i].ageCnt = 0;
		obdDiagDtGrp[fault_i].dcCnt = 0;
		obdDiagDtGrp[fault_i].dtcStatus.b.testFailed = 0;
		obdDiagDtGrp[fault_i].dtcStatus.b.testFailedThisOperationCycle = 0;
		obdDiagDtGrp[fault_i].dtcStatus.b.pendingDtc = 0;
		obdDiagDtGrp[fault_i].dtcStatus.b.confirmDtc = 0;
		obdDiagDtGrp[fault_i].dtcStatus.b.testNotCompleteThisOperationCycle = 1;
		obdDiagDtGrp[fault_i].dtcStatus.b.warnIndi = 0; 
		if(TRUE == udsClear)
		{
			obdDiagDtGrp[fault_i].dtcStatus.b.testFailedSinceLastClear = 0;
			obdDiagDtGrp[fault_i].dtcStatus.b.testNotCompleteSinceLastClear = 1;
		}
		if(TRUE == obdDiagDtGrp[fault_i].isStored)
		{
			obdClearff(fault_i);
			obdDiagDtGrp[fault_i].isStored = FALSE;
		}
	}
}

void obd_diag_update(void)
{
	uint16_t dtc_i = 0;
	for(; dtc_i < dtcItemCnt; dtc_i ++)
	{
		if(NULL != dtcInfoCfgGrp[dtc_i].p_ok)
		{
			if(*(dtcInfoCfgGrp[dtc_i].p_ok) > 0)
			{
				obd_updateFault(dtc_i,FALSE);
			}
			else if(NULL != dtcInfoCfgGrp[dtc_i].p_flt && *(dtcInfoCfgGrp[dtc_i].p_flt) > 0)
			{
				obd_updateFault(dtc_i,TRUE);
			}
			else
			{
			}
		}
		else
		{
			if(NULL != dtcInfoCfgGrp[dtc_i].p_flt)
			{
				if(*(dtcInfoCfgGrp[dtc_i].p_flt) > 0)
				{
					obd_updateFault(dtc_i,TRUE);
				}
				else
				{
					obd_updateFault(dtc_i,FALSE);
				}
			}
			else
			{

			}
		}
	}
}



/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static void obd_storeFreezeFrame(uint16_t fault_i)
{

	uint16_t foIndx = 0;
	uint16_t ffOrder = 0xFFFF;				  /*0xFFFF means an invalid value*/
	uint16_t foEmptyIndx = 0xFFFF;			  /*the first empty index in frame order,0xFFFF means an invalid value*/
	uint16_t foLowLevelIndx = 0xFFFF;		  /*the first low-level index in frame order,0xFFFF means an invalid value*/
	uint16_t foMidLevelIndx = 0xFFFF;		  /*the first low-level index in frame order,0xFFFF means an invalid value*/
	uint16_t foUseIndx = 0xFFFF;		      /*the finally used  index in frame order,0xFFFF means an invalid value*/
	if((fault_i < dtcItemCnt) && (FALSE == obdDiagDtGrp[fault_i].isStored))
	{
		for(;foIndx < dtcFreezeFrameCnt;foIndx ++)
		{
			/*fetch the frame order,point to the freeze frame*/
			ffOrder = m_ff_getOrder(foIndx);
			if(ffOrder >= dtcFreezeFrameCnt)
			{
				/*it is the invalid value , reset the frame order and marked it as empty for future usage*/
				m_ff_setOrder(foIndx,m_ff_emptyMask | foIndx);
				ffOrder = foIndx;
			}
			if(TRUE == m_ff_getEmptyFlag(foIndx))
			{
				/*very lucky to find a empty freeze frame to use this time*/
				foEmptyIndx = foIndx;
				break;
			}
			else 
			{
				if((0xFFFF == foLowLevelIndx) && (m_dtcPrio_low == obdFFDtGrp[ffOrder].ffPrio))
				{
					foLowLevelIndx = foIndx;
				}
				else if((0xFFFF == foLowLevelIndx) && (0xFFFF == foMidLevelIndx) && (m_dtcPrio_middle == obdFFDtGrp[ffOrder].ffPrio))
				{
					foMidLevelIndx = foIndx;
				}
				else
				{
					/*nothing more need to be done, just continue searching the reset*/
				}
			}
		}
		if(foEmptyIndx < dtcFreezeFrameCnt)
		{
			foUseIndx = foEmptyIndx;	
		}
		else
		{
			if(foLowLevelIndx < dtcFreezeFrameCnt)
			{
				if((m_dtcPrio_middle == obdFFDtGrp[ffOrder].ffPrio) || (m_dtcPrio_high == obdFFDtGrp[ffOrder].ffPrio))
				{
					foUseIndx = foLowLevelIndx;	
				}
			}
			else if(foMidLevelIndx < dtcFreezeFrameCnt)
			{
				if(m_dtcPrio_high == obdFFDtGrp[ffOrder].ffPrio)
				{
					foUseIndx = foMidLevelIndx;	
				}
			}
		}

		if(foUseIndx < dtcFreezeFrameCnt)
		{
			/*very lucky to find a suitable freeze frame, either an empty or a low-priority one*/
			ffOrder = m_ff_getOrder(foUseIndx);
			if((FALSE == m_ff_getEmptyFlag(foUseIndx)) && (obdFFDtGrp[ffOrder].fault_i < dtcItemCnt))
			{
				/*this is a non-empty freeze frame,so sorry here we need to ocuppy it with a high-priority one*/
				/*in fact the following judgement sentence is not necessary at all, so we can just comment it out*/
				if(TRUE == obdDiagDtGrp[obdFFDtGrp[ffOrder].fault_i].isStored)
				{
					obdDiagDtGrp[obdFFDtGrp[ffOrder].fault_i].isStored = FALSE;
				}
				/*the fault whose freeze frame has been over-written cruelly, we must clear the confirmed DTC flag*/
				obdDiagDtGrp[obdFFDtGrp[ffOrder].fault_i].dtcStatus.b.confirmDtc = 0;
			}
			else
			{
				/*set the order and marked it as non-empty*/
				m_ff_setFull(foUseIndx);
			}
			obdFFDtGrp[ffOrder].fault_i = fault_i;
			obdFFDtGrp[ffOrder].ffPrio = dtcInfoCfgGrp[fault_i].prio;
			obd_updateSnapshot(fault_i);
			obdDiagDtGrp[fault_i].isStored = TRUE;
			#if m_frameOrderKeepFreshFirst
			for(foIndx = foUseIndx; foIndx >0; foIndx --)
			{
				m_ff_setOrder(foIndx,obdFFOrder[foIndx - 1]);
			}
			m_ff_setOrder(0,ffOrder);

			#endif
		}
		else
		{
			/*so sorry to find no suitable freeze frame to store this fault*/
		}
	}

	
}




static void obd_updateSnapshot(uint16_t fault_i)
{
	uint8_t index = 0;
	uint16_t did_i = 0;
	uint8_t didDtIndx = 0;
	uint8_t snapDtByteIndx = 0;
	if(fault_i < dtcItemCnt)
	{
		for(;index < dtcInfoCfgGrp[fault_i].snapDidNum; index ++)
		{
			did_i  = dtcInfoCfgGrp[fault_i].pDidIndx[index];
			if(did_i < uds_snapCnt)
			{
				if(NULL != uds_snapListCfgGrp[did_i].cb)
				{
					uds_snapListCfgGrp[did_i].cb();
				}
				if((NULL != uds_snapListCfgGrp[did_i].dtCache) && (NULL != dtcInfoCfgGrp[fault_i].snapBuf))
				{
					for(didDtIndx = 0;didDtIndx < uds_snapListCfgGrp[did_i].dlc; didDtIndx ++)
					{
						dtcInfoCfgGrp[fault_i].snapBuf[snapDtByteIndx] = ((uint8_t *)uds_snapListCfgGrp[did_i].dtCache)[didDtIndx];
						snapDtByteIndx ++;
					}
				}

			}
		}	
	}
}


static void obdClearff(uint16_t fault_i)
{
	uint16_t foIndx = 0;
	uint16_t ffIndx = 0;
	bool     ffEmpty = FALSE;
	#if m_frameOrderKeepFreshFirst
	uint16_t frameOrderTemp = 0;
	#endif
	if(fault_i < dtcItemCnt)
	{
		/*at first we need to find the freeze frame of the given fault */
		for(;foIndx < dtcFreezeFrameCnt;foIndx ++)
		{
			#if m_frameOrderKeepFreshFirst
			if(TRUE == m_ff_getEmptyFlag(foIndx))
			{
				foIndx = dtcFreezeFrameCnt;
				break;
			}
			#endif
			ffIndx = m_ff_getOrder(foIndx);
			ffEmpty = m_ff_getEmptyFlag(foIndx);
			if((ffIndx < dtcFreezeFrameCnt) && (FALSE == ffEmpty) && (fault_i == obdFFDtGrp[ffIndx].fault_i))
			{
				break;	
			}
		}
		if(foIndx < dtcFreezeFrameCnt)
		{
			/*in fact, below one sentence has been executed in the above already , here  duplicate it just for a better understanding*/
			ffIndx = m_ff_getOrder(foIndx);
			obdFFDtGrp[ffIndx].fault_i = 0xFFFF;			/*means no fault*/
			obdFFDtGrp[ffIndx].ffPrio = m_dtcPrio_none;		/*reset it to the lowest priority*/
			/*mark the current freeze frame as empty*/
			m_ff_setEmpty(foIndx);

			#if m_frameOrderKeepFreshFirst
			/*move the  current empty freeze frame order to the back of all the non-empty ones*/	
			if(foIndx < (dtcFreezeFrameCnt - 1))
			{
				/*here to backup the frame order for next-step usage*/
				frameOrderTemp = obdFFOrder[foIndx];
				for(; (foIndx < (dtcFreezeFrameCnt - 1)) && (FALSE == m_ff_getEmptyFlag(foIndx)); foIndx ++)
				{
					obdFFOrder[foIndx]	 = obdFFOrder[foIndx + 1];
				}				
				obdFFOrder[dtcFreezeFrameCnt - 1] = frameOrderTemp;
				/*obdFFOrder[foIndx + 1] = frameOrderTemp;*/
			}
			else
			{
				/*it is the last one, nothing more need to be done*/
			}
			#endif
		}
	}
}


static bool obd_isEnable(void)
{
	/*for example, the ignition off or battery voltage <9v is not allowed to do OBD function*/
	return TRUE;
}


/* ---- Source switch off ---------------------------------------------------- */
#undef OBD_C_
/*--- End of file ------------------------------------------------------------*/


