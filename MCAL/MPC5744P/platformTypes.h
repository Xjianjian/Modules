
/* ---- Source switch on ------------------------------------------------------*/
#ifndef PLATFORMTYPES_H_
#define PLATFORMTYPES_H_


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_inputParaValidityCheckEnable			FALSE
#define m_plat_isFunctionState(st)				((em_funSt_off == st) ||(em_funSt_on == st))

#define m_plat_dioModuleId			2
#define m_plat_stmModuleId			3
#define m_plat_canModuleId			4
#define m_plat_sciModuleId			5
#define m_plat_adcModuleId			6
#define m_plat_spiModuleId			7
#define m_plat_hdioModuleId		8



#define m_plat_funStErr 			0
#define m_plat_stmDivErr			1
#define m_plat_stmChannelNoErr		2
#define m_canModuleErr				3
#define m_canBaudRateErr			4
#define m_canClkSocErr				5
#define m_canMbErr					6
#define m_canTxCodeErr				7
#define m_canLenErr				8
#define m_canRxCodeErr				9
#define m_canmcuModuleErr			10
#define m_plat_dioChanIdErr		11
#define m_plat_dioDirErr			12
#define m_plat_dioOutTypeErr		13
#define m_plat_dioPullTypeErr		14
#define m_plat_adcCpuModuErr		15
#define m_plat_adcModuErr			16
#define m_plat_adcfunStERR			17
#define m_plat_adcChanErr			18
#define m_plat_adcIntErr			19
#define m_plat_adcWdErr			20
#define m_plat_spiModuleErr		21
#define m_plat_spiFrameSizeErr		22
#define m_plat_spictasErr			23


#define m_plat_hdioDebIndexErr		24
#define m_plat_hdioWtSigIndexErr   25
#define m_plat_hdioWtValueErr		26

#define m_sci_moduleErr		0
#define m_sci_cpuModuleErr		1
#define m_sci_baudErr			2
#ifndef NULL
#define NULL 	((void *) 0)
#endif
#define TRUE 	(1)
#define FALSE	(0)


/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef enum
{
	em_funSt_off = 0,
	em_funSt_on
}functionSt_t;

typedef unsigned char	bool;



/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/




#endif

