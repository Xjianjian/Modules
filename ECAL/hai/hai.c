
 /*------------------------------------------------------------------------------
					 Include files
 ------------------------------------------------------------------------------*/
#include "typedefs.h"
#include "platformTypes.h"
#include "utils.h"
#include "adcCfg.h"
#include "adc.h"
#include "haiCfg.h"
#include "hai.h"

 /*-----------------------------------------------------------------------------
						 Module local defines and constants
 ------------------------------------------------------------------------------*/
#define m_hai_cfg				haiCfgGrp[sig]
#define m_hai_cfgModu			haiCfgGrp[sig].module
#define m_hai_cfgChan			haiCfgGrp[sig].chan
#define m_hai_cfgInj			haiCfgGrp[sig].inj
#define m_hai_cfgNor			haiCfgGrp[sig].nor
#define m_hai_cfgIsrEn			haiCfgGrp[sig].isrEn
#define m_hai_cfgIsrMsk		haiCfgGrp[sig].isrMask
#define m_hai_cfgBuf			haiCfgGrp[sig].buf
#define m_hai_cfgIsrFun		haiCfgGrp[sig].isrFun
#define m_hai_cfgUpdtHook		haiCfgGrp[sig].updtHook

static uint16_t hal_ai_voltval[e_ai_max];
static lpfDebSig_t HailpfDebGrp[e_ai_max];
static uint16_t hal_ai_raw[e_ai_max];
static uint16_t hal_ai_rawfltrVal[e_ai_max];
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
static void hal_ai_updt_hook(uint8_t obj,uint16_t val);
static void hai_sampleCyclic(uint8_t module);
static void hai_adcToVolt(void);
 /*------------------------------------------------------------------------------
					   Implementation of global functions
 ------------------------------------------------------------------------------*/
void hai_init(void) 
{
	uint16_t injChanMask[m_hai_adc_module_num] = {0,0};
	uint16_t norChanMask[m_hai_adc_module_num] = {0,0};
	uint16_t isrChanEnMask[m_hai_adc_module_num] = {0,0};
	uint16_t IsrTypeMask[m_hai_adc_module_num] = {0,0};
	uint8_t  listEleIndx = 0;
	uint8_t sig = 0;
	uint8_t module =0;
	uint8_t listEleCnt[m_hai_adc_module_num] = {0,0};

	for(listEleIndx = 0; listEleIndx < e_ai_max;listEleIndx ++)
	{
		HailpfDebGrp[listEleIndx].sig = 0;
		HailpfDebGrp[listEleIndx].debVal = 0;
		HailpfDebGrp[listEleIndx].head = 0;
		HailpfDebGrp[listEleIndx].memFullCnt = m_hai_mem_fullCnt;
		HailpfDebGrp[listEleIndx].memCnt = 0;
		HailpfDebGrp[listEleIndx].historyVal = &hal_ai_deb_buf[listEleIndx][0];
		HailpfDebGrp[listEleIndx].historyFact = &hal_ai_lpf_factor[listEleIndx][0];
		HailpfDebGrp[listEleIndx].totalFactor = m_hai_mem_fullCnt;
	}

	for(module=0; module<m_hai_adc_module_num; module ++)
	{
		for(listEleIndx = 0; listEleIndx < haiSigList_conf[module].cnt;listEleIndx ++)
		{
			haiSigList_conf[module].haiSigList[listEleIndx].sig = 0xFF;	/*means invalid signal*/
			haiSigList_conf[module].haiSigList[listEleIndx].next = NULL;	/*means next signal doesn't exist*/
			haiSigList_conf[module].haiSigList[listEleIndx].prv= NULL;		/*means last signal doesn't exist*/
		}

		for(sig =0;sig < e_ai_max;sig ++)
		{
			if(haiCfgGrp[sig].module == module)
			{
				if(listEleCnt[module] < haiSigList_conf[module].cnt)
				{
					if(0 == listEleCnt[module])
					{
						/*add the first signal to the list*/
						haiSigList_conf[module].haiSigList[listEleCnt[module]].sig = sig;
					}
					else
					{
						haiSigList_conf[module].haiSigList[listEleCnt[module]].sig    	= sig;
						haiSigList_conf[module].haiSigList[(listEleCnt[module]-1)].next = &haiSigList_conf[module].haiSigList[listEleCnt[module]];
						haiSigList_conf[module].haiSigList[listEleCnt[module]].prv    	= &haiSigList_conf[module].haiSigList[(listEleCnt[module]-1)];
					}
				}

				listEleCnt[module]++;
				
				if(TRUE == haiCfgGrp[sig].inj)
				{
					injChanMask[haiCfgGrp[sig].module] |= m_util_bitMask32(haiCfgGrp[sig].chan);
				}

				if(TRUE == haiCfgGrp[sig].nor)
				{
					norChanMask[haiCfgGrp[sig].module] |= m_util_bitMask32(haiCfgGrp[sig].chan);
				}

				if(TRUE == haiCfgGrp[sig].isrEn)
				{
					isrChanEnMask[haiCfgGrp[sig].module] |= m_util_bitMask32(haiCfgGrp[sig].chan);
					IsrTypeMask[haiCfgGrp[sig].module]	|= m_hai_cfgIsrMsk;
				}
			}
		}
	}


	for(module=0; module<m_hai_adc_module_num; module ++)
	{
		if((injChanMask[module] >0) || (norChanMask[module] >0))
		{
			adc_init(module,injChanMask[module],norChanMask[module]);
		}

		if((IsrTypeMask[module] >0) || (isrChanEnMask[module] >0))  
		{
			adc_setInt(module,IsrTypeMask[module],isrChanEnMask[module],em_funSt_on);
		}
		else
		{
			adc_setInt(module,0x1F,0xFFFF,em_funSt_off);
		}
	}

}
 

void hai_adc_sampleTask(void)
{
	uint8_t module;
	for(module=0; module<m_hai_adc_module_num; module ++)
	{
		hai_sampleCyclic(module);
	}

	hai_adcToVolt();
}

static void hai_sampleCyclic(uint8_t module)
{
	uint8_t sig = 0;
	uint8_t listEleIndx = 0;
	uint16_t ChanEoc = 0;

	ChanEoc = adc_getChanEoc(module);
	for(listEleIndx=0;listEleIndx <  haiSigList_conf[module].cnt; listEleIndx++)
	{
		if(0xFF != haiSigList_conf[module].haiSigList[listEleIndx].sig)
		{
			sig = haiSigList_conf[module].haiSigList[listEleIndx].sig;
			if((FALSE == haiCfgGrp[sig].isrEn) && (0 < (ChanEoc & m_util_bitMask32(haiCfgGrp[sig].chan))))
			{	
				if(TRUE == adc_getDt(haiCfgGrp[sig].module,haiCfgGrp[sig].chan,&hal_ai_raw[sig],TRUE))
				{
					hal_ai_updt_hook(sig,hal_ai_raw[sig]);
				}

			}

			if(NULL == haiSigList_conf[module].haiSigList[listEleIndx].next)
			{
				break;
			}
		}
	}

	adc_clrEoc(module,ChanEoc);
	adc_startNormalConversion(module);
}


static void hai_adcToVolt(void)
{
	uint8_t indx;

	for(indx = 0;indx < e_ai_max;indx++)
	{
		hal_ai_voltval[indx] = (hal_ai_raw[indx]*5000) >> 12;
	}
}


uint16_t get_hai_VoltValue(uint8_t indx)
{
	return hal_ai_voltval[indx];
}

static void hal_ai_updt_hook(uint8_t obj,uint16_t val)
{
	if(TRUE == haiCfgGrp[obj].filter)
	{
		util_LPFCal(&HailpfDebGrp[obj],hal_ai_raw[obj]);
	}
	else
	{
		HailpfDebGrp[obj].debVal = hal_ai_raw[obj];
	}

	hal_ai_rawfltrVal[obj] = ((uint16_t)HailpfDebGrp[obj].debVal >> 2);
}
 
 /*------------------------------------------------------------------------------
					   Implementation of local functions
 ------------------------------------------------------------------------------*/


 

