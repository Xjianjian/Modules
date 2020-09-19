
/*
* File:hdio.c
* Description:
*/
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

/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/
static diDebSig_t diDebSigGrp[e_di_max];
static uint8_t hdioLastDebSt[e_di_max];
static uint8_t hdioCurrDebSt[e_di_max];

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

	for(indx = 0;indx < e_di_max;indx ++)
	{
		dio_setHysteresis(diCfgGrp[indx].chanId,em_funSt_off);
		dio_setSlewCtrl(diCfgGrp[indx].chanId,((TRUE == diCfgGrp[indx].slewFast)?(em_funSt_on):(em_funSt_off)));
		dio_setDir(diCfgGrp[indx].chanId,		diCfgGrp[indx].dir);
		dio_setPullType(diCfgGrp[indx].chanId,	diCfgGrp[indx].pullType);
		dio_safeModeCmd(diCfgGrp[indx].chanId,((TRUE == diCfgGrp[indx].safeMode)?(em_funSt_on):(em_funSt_off)));
		hdio_readDebInit(indx);
	}
	
	for(indx = 0;indx < e_do_max;indx++)
	{
		dio_setHysteresis(doCfgGrp[indx].chanId,em_funSt_off);
		dio_setSlewCtrl(doCfgGrp[indx].chanId,((TRUE == doCfgGrp[indx].slewFast)?(em_funSt_on):(em_funSt_off)));
		dio_setDir(doCfgGrp[indx].chanId,		doCfgGrp[indx].dir);
		dio_setPullType(doCfgGrp[indx].chanId,	doCfgGrp[indx].pullType);
		dio_safeModeCmd(doCfgGrp[indx].chanId,((TRUE == doCfgGrp[indx].safeMode)?(em_funSt_on):(em_funSt_off)));
		dio_setOutAbility(doCfgGrp[indx].chanId,((FALSE == doCfgGrp[indx].openDrain)?(m_dio_outNormal):(m_dio_outOpenDrain)));
		dio_setDir(doCfgGrp[indx].chanId,	((FALSE == doCfgGrp[indx].writeCheck)?(m_dio_output):(m_dio_biDir)));
		hdio_writeSig(indx,doCfgGrp[indx].initVal);
	}
}


void hdio_diSampleCyclic(void)
{
	uint8_t indx = 0;
	uint8_t readSt = m_hdio_stUnknown;
	
	for(indx = 0;indx <= e_di_max;indx ++)
	{
		readSt = dio_readVal(diCfgGrp[indx].chanId);

		if(FALSE == diCfgGrp[indx].ioStd)
		{
			readSt = !readSt;
		}

		if(TRUE == diCfgGrp[indx].readDeb)
		{//filter
			util_diDebCal(&diDebSigGrp[indx],readSt);
			if(m_hdio_stUnknown != diDebSigGrp[indx].debSig)
			{
				hdioCurrDebSt[indx] = diDebSigGrp[indx].debSig;
			}
		}
		else
		{
			hdioCurrDebSt[indx] = readSt ;
		}

		if(hdioLastDebSt[indx] != hdioCurrDebSt[indx])
		{
			hdioLastDebSt[indx] = hdioCurrDebSt[indx];
		}
	}
}

bool hdio_writeSig(uint8_t indx,uint8_t val)
{
	bool verifySuccess = TRUE;

	if(FALSE == doCfgGrp[indx].ioStd)
	{
		val = !val;
	}

	verifySuccess = dio_writeVal(doCfgGrp[indx].chanId,val,doCfgGrp[indx].writeCheck);
	return verifySuccess;
	
}

bool hdio_toggleSig(uint8_t indx)
{
	bool verifySuccess = TRUE;

	verifySuccess = dio_toggleChan(doCfgGrp[indx].chanId,doCfgGrp[indx].writeCheck);
	return verifySuccess;
}

uint8_t	hdio_readSig(uint8_t indx)
{
	return hdioLastDebSt[indx];
}

/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static void hdio_readDebInit(uint8_t indx)
{
	diDebSigGrp[indx].sig = m_hdio_stUnknown;
	diDebSigGrp[indx].debSig = m_hdio_stUnknown;
	diDebSigGrp[indx].currCall = 0;
	diDebSigGrp[indx].debCall = 3;

	hdioLastDebSt[indx] = m_hdio_stUnknown;
}





