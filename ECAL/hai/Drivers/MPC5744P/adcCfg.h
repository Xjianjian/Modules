
#ifndef ADC_CFG_H_
#define ADC_CFG_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_adc_modu0PortCnt  13
#define m_adc_modu1PortCnt  13


#define m_adc_cpuModuCnt    4
#define m_adc_cfgModuCnt    2

#if m_adc_cfgModuCnt > m_adc_cpuModuCnt
#error "The configure ADC module count is error"
#endif

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef struct __ADC_MODULECONF_T__
{
	bool isUsed;
    bool overWriteEn;
    bool convertAligned;
	bool scanEn;  /*0: one shot, 1: scan*/
	bool injectExtTriEn;   /*Injection external trigger enable.*/
	bool injectExtTriEdge;   /*0: falling, 1:rising*/
	bool ctuEn;            /*0: disable CTU, 1:enable CTU*/
	bool clockSel;         /*0: half of fp, 1: equal to fp*/
	bool autoClockOff;     /*0: dis,  1: en*/
}adc_moduleConf_t;

/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
extern const adc_moduleConf_t adcModuleCfg[m_adc_cfgModuCnt];
/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/


#endif



