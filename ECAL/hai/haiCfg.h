
#ifndef HAI_CFG_H_
#define HAI_CFG_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/

#define m_hai_mem_fullCnt	3

#define m_hai_adc_module_num	2

typedef enum
{
//adc0
	e_ai_aps1 = 0,//chl 0
	e_ai_aps2,//chl 8
	e_ai_liquid_lvl_sensor,//chl 12
	e_ai_nchg_cc,//chl 7
	e_ai_qchg_cc2,//chl 1
	e_ai_mux_out,//chl 2
	e_ai_highsignal2_outputFB,//chl 13
	e_ai_highsignal1_outputFB,//chl 14
	e_ai_04,//chl 4
	e_ai_05,//chl 3
	e_ai_08,//chl 6
	e_ai_09,//chl 5
	e_ai_10,//chl 11

//adc1
	e_ai_11,//chl 2
	e_ai_12,//chl 7
	e_ai_13,//chl 1
	e_ai_14,//chl 8

	e_ai_max
}haiCfg_ai_obj;

#define m_hai_mod0SigCnt			13
#define m_hai_mod1SigCnt			4

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef void (*pAdcDtUpdtHook)(void);
	
typedef struct
{
	uint8_t 		module;
	uint16_t 		chan;
	bool 			inj;
	bool 			nor;
	bool 			isrEn; 
	uint8_t			isrMask;
	bool 			filter;
}haiCfg_t;

typedef struct haiSigList
{
	uint8_t sig;
	struct haiSigList *next;
	struct haiSigList *prv;
}haiSigList_t;

typedef struct
{
	uint8_t cnt;
	struct haiSigList *haiSigList;
}haiSigList_conf_t;
/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
extern const haiCfg_t haiCfgGrp[e_ai_max];
extern float hal_ai_deb_buf[e_ai_max][m_hai_mem_fullCnt];
extern const float hal_ai_lpf_factor[e_ai_max][m_hai_mem_fullCnt];
extern const haiSigList_conf_t haiSigList_conf[m_hai_adc_module_num];
/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/

#endif



