

/* ---- Source switch on ------------------------------------------------------*/
#ifndef ETIMER_CFG_H_
#define ETIMER_CFG_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "eTimer_pri.h"

/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define mc_eTim_funNone		 0	
#define mc_eTim_funOutCmp    1
#define mc_eTim_funInCapt	 2


#define mc_eTim_inFreq				(10000000u)
#define mc_eTim_captClkFreq			(1000u)
#define mc_eTim_captPeriodCnt		(mc_eTim_inFreq/mc_eTim_captClkFreq)

#define mc_eTim_cntUp				 TRUE	


typedef enum
{
	e_pi_fan_st_sig = 0,
	e_pi_pupm1_st_sig,
	e_pi_pupm2_st_sig,
	e_pi_pupm3_st_sig,
	e_pi_pupm4_st_sig,
	e_pi_max
}mcal_pi_obj;

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
/*the primary source is used for triggering the Etimer start the counter ,and the second source's edge
	will be captured and note down the primary counter at that moment */
typedef struct
{
	unsigned char module;
	unsigned char chanL;
	unsigned char chanH;	/*only be useful when it is in cascde mode*/
	unsigned char secondSoc;
	unsigned char div;
	unsigned char isrEn;
}etimInCaptCfg_t;



/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
extern const etimInCaptCfg_t etimCasInCaptCfgGrp[e_pi_max];
/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/




#endif


