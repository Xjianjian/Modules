/*
* Copyright (c) File 2016-08-11,SHENZHEN HANGSHENG NEW ENERGY Co.,Ltd.
* All Rights Reserved.
* Dept.: Software Department
* File:hdio.c
* Description:
* REQ IDs:
* History:
* 2016-08-12, Devon Draft version
*/

/* ---- Source switch on ------------------------------------------------------*/
#define HDIO_C_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "dio.h"
#include "hdioCfg.h"
#include "hdio.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/
#define m_hdio_chanId		(hdioCfgGrp[indx].chanId)
#define m_hdio_dir			(hdioCfgGrp[indx].dir)
#define m_hdio_pull			(hdioCfgGrp[indx].pullType)
#define m_hdio_safe			(hdioCfgGrp[indx].safeMode)
#define m_hdio_ioStd		(hdioCfgGrp[indx].ioStd)
#define m_hdio_slewFast		(hdioCfgGrp[indx].slewFast)
#define m_hdio_appLock		(hdioCfgGrp[indx].appLock)

#define m_hdio_openDrain 	(hdioCfgGrp[indx].openDrain)
#define m_hdio_wtCheck		(hdioCfgGrp[indx].writeCheck)
#define m_hdio_initVal		(hdioCfgGrp[indx].initVal)
#define m_hdio_lockBuf		(hdioCfgGrp[indx].lockBuf)
#define m_hdio_readDeb		(hdioCfgGrp[indx].readDeb)
#define m_hdio_signal		(hdioCfgGrp[indx].sig)
#define m_hdio_debBuf		(hdioCfgGrp[indx].debBuf)
#define m_hdio_updtFun		(hdioCfgGrp[indx].fun)


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/
static diDebSig_t diDebSigGrp[m_hdio_diSigCnt] = 
{
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3	
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3	
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3	
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3	
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3	
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3	
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3	
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3	
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3	
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3
	},
	{
		m_hdio_stUnknown,
		0,
		m_hdio_stUnknown,
		3
	}
};

#if m_hdio_debRuler
uint8_t diDebRulerReady = FALSE;

static diDebSig_t diRulerDebSig = 
{
	m_hdio_stUnknown,
	0,
	m_hdio_stUnknown,
	10	
};
#endif

static uint8_t hdioLastDebSt[m_hdio_diSigCnt] = 
{
	m_hdio_stUnknown,m_hdio_stUnknown,m_hdio_stUnknown,m_hdio_stUnknown,m_hdio_stUnknown,
	m_hdio_stUnknown,m_hdio_stUnknown,m_hdio_stUnknown,m_hdio_stUnknown,m_hdio_stUnknown,
	m_hdio_stUnknown,m_hdio_stUnknown,m_hdio_stUnknown,m_hdio_stUnknown,m_hdio_stUnknown,
	m_hdio_stUnknown,m_hdio_stUnknown,m_hdio_stUnknown,m_hdio_stUnknown,m_hdio_stUnknown
};

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
static void hdio_readDebInit(uint8_t indx);

/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void hdio_init(void)
{
	uint8_t indx = 0;
	#if m_hdio_debRuler
	uint16_t debCntMax = 0;
	#endif
	for(indx = m_hdio_firstDiIndex;indx <= m_hdio_LastDiIndex;indx ++)
	{
		dio_setHysteresis(m_hdio_chanId,em_funSt_off);
		dio_setSlewCtrl(m_hdio_chanId,((TRUE == m_hdio_slewFast)?(em_funSt_on):(em_funSt_off)));
		dio_setDir(m_hdio_chanId,		m_hdio_dir);
		dio_setPullType(m_hdio_chanId,	m_hdio_pull);
		dio_safeModeCmd(m_hdio_chanId,((TRUE == m_hdio_safe)?(em_funSt_on):(em_funSt_off)));
		hdio_readDebInit(indx - m_hdio_firstDiIndex);
		#if m_hdio_debRuler
		if(debCntMax < diDebSigGrp[indx - m_hdio_firstDiIndex].debCall)
		{
			debCntMax = diDebSigGrp[indx - m_hdio_firstDiIndex].debCall;
		}
		#endif
	}
	#if m_hdio_debRuler
	diDebRulerReady = FALSE;
	diRulerDebSig.sig = m_hdio_stUnknown;
	diRulerDebSig.debSig = m_hdio_stUnknown;
	diRulerDebSig.currCall = 0;
	diRulerDebSig.debCall = debCntMax; 
	#endif
	
	for(indx = m_hdio_firstDoIndex;indx <= m_hdio_LastDoIndex;indx ++)
	{
		dio_setHysteresis(m_hdio_chanId,em_funSt_off);
		dio_setSlewCtrl(m_hdio_chanId,((TRUE == m_hdio_slewFast)?(em_funSt_on):(em_funSt_off)));
		dio_setDir(m_hdio_chanId,		m_hdio_dir);
		dio_setPullType(m_hdio_chanId,	m_hdio_pull);
		dio_safeModeCmd(m_hdio_chanId,((TRUE == m_hdio_safe)?(em_funSt_on):(em_funSt_off)));
		dio_setOutAbility(m_hdio_chanId,((FALSE == m_hdio_openDrain)?(m_dio_outNormal):(m_dio_outOpenDrain)));
		dio_setDir(m_hdio_chanId,	((FALSE == m_hdio_wtCheck)?(m_dio_output):(m_dio_biDir)));
		hdio_writeSig(indx,m_hdio_initVal);
	}
}


void hdio_diSampleCyclic(void)
{
	uint8_t indx = 0;
	uint8_t readSt = m_hdio_stUnknown;
	uint8_t debSt = m_hdio_stUnknown;
	#if m_hdio_debRuler
	
	
	if(FALSE == diDebRulerReady)
	{
		util_diDebCal(&diRulerDebSig,m_hdio_stHigh);
		if(m_hdio_stHigh == diRulerDebSig.debSig)
		{
			diDebRulerReady = TRUE;	
		}
	}
	#endif
	
	for(indx = m_hdio_firstDiIndex;indx <= m_hdio_LastDiIndex;indx ++)
	{
		if((NULL != m_hdio_appLock) && (TRUE == *m_hdio_appLock) &&
			(NULL != m_hdio_lockBuf))
		{
			readSt = *m_hdio_lockBuf;	
		}
		else
		{
			readSt = dio_readVal(m_hdio_chanId);
		}
		if(FALSE == m_hdio_ioStd)
		{
			readSt = !readSt;
		}
		if(NULL != m_hdio_signal)
		{
			*m_hdio_signal = readSt;
		}
		if((NULL != m_hdio_readDeb) && (TRUE == *m_hdio_readDeb))
		{
			util_diDebCal(&diDebSigGrp[indx - m_hdio_firstDiIndex],readSt);
			if((NULL != m_hdio_debBuf) && (m_hdio_stUnknown != diDebSigGrp[indx - m_hdio_firstDiIndex].debSig))
			{
				*m_hdio_debBuf = diDebSigGrp[indx - m_hdio_firstDiIndex].debSig;
				debSt = diDebSigGrp[indx - m_hdio_firstDiIndex].debSig;
			}
		}
		else
		{
			*m_hdio_debBuf = readSt ;
			debSt = readSt;
		}
		#if m_hdio_debRuler
		if(TRUE == diDebRulerReady)
		{
		#endif
			if(hdioLastDebSt[indx - m_hdio_firstDiIndex] != debSt)
			{
				hdioLastDebSt[indx - m_hdio_firstDiIndex] = debSt;
				if(NULL != m_hdio_updtFun)
				{
					m_hdio_updtFun(debSt);
				}
			}

		#if m_hdio_debRuler
		}
		#endif
	}
	
}

void hdio_doUpdtCycle(void)
{
	uint8_t indx = 0;
	for(indx = m_hdio_firstDoIndex;indx <= m_hdio_LastDoIndex;indx ++)
	{
		if((NULL != m_hdio_appLock) && (TRUE == *m_hdio_appLock) && (NULL != m_hdio_lockBuf))
		{
			hdio_writeSig(indx,*m_hdio_lockBuf);
		}
		else
		{
			if(NULL != m_hdio_signal)
			{
				hdio_writeSig(indx,*m_hdio_signal);
			}
		}
	}
}


void hdio_doUpdtIndx(uint8_t indx)
{
	if((indx >= m_hdio_firstDoIndex) && (indx <= m_hdio_LastDoIndex))
	{
		if((NULL != m_hdio_appLock) && (TRUE == *m_hdio_appLock) && (NULL != m_hdio_lockBuf))
		{
			hdio_writeSig(indx,*m_hdio_lockBuf);
		}
		else
		{
			if(NULL != m_hdio_signal)
			{
				hdio_writeSig(indx,*m_hdio_signal);
			}
		}
	}
}





bool hdio_writeSig(uint8_t indx,uint8_t val)
{
	bool verifySuccess = TRUE;
	m_InParaCheck(m_hdio_outSigIndxInRange(indx),	m_plat_hdioModuleId,	m_hdio_writeSigFunId,	m_plat_hdioWtSigIndexErr);
	m_InParaCheck(m_hdio_wtValueInRange(val),		m_plat_hdioModuleId,	m_hdio_writeSigFunId,	m_plat_hdioWtValueErr);
	if(FALSE == m_hdio_ioStd)
	{
		val = !val;
	}
	verifySuccess = dio_writeVal(m_hdio_chanId,val,m_hdio_wtCheck);
	return verifySuccess;
	
}

bool hdio_toggleSig(uint8_t indx)
{
	bool verifySuccess = TRUE;
	m_InParaCheck(m_hdio_outSigIndxInRange(indx),	m_plat_hdioModuleId,	m_hdio_toggleSigFunId,	m_plat_hdioWtSigIndexErr);
	verifySuccess = dio_toggleChan(m_hdio_chanId,m_hdio_wtCheck);
	return verifySuccess;
}



/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static void hdio_readDebInit(uint8_t indx)
{
	m_InParaCheck(m_hdio_DebObjIndexInRange(indx),	m_plat_hdioModuleId,	m_hdio_readDebInitFunId,	m_plat_hdioDebIndexErr);
	if((NULL!= m_hdio_readDeb) && (TRUE == *m_hdio_readDeb))
	{
		diDebSigGrp[indx].sig = m_hdio_stUnknown;
		diDebSigGrp[indx].debSig = m_hdio_stUnknown;
		diDebSigGrp[indx].currCall = 0;
		//diDebSigGrp[indx].debCall = 10;
	}
	else
	{
		/*no debounce need at all*/
	}
}



/* ---- Source switch off ---------------------------------------------------- */
#undef HDIO_C_
/*--- End of file ------------------------------------------------------------*/




