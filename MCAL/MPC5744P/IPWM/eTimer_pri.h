

/* ---- Source switch on ------------------------------------------------------*/
#ifndef ETIMER_PRI_H_
#define ETIMER_PRI_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define mc_etim_ipDiv1  	0
#define mc_etim_ipDiv2  	1
#define mc_etim_ipDiv4  	2
#define mc_etim_ipDiv8  	3
#define mc_etim_ipDiv16  	4
#define mc_etim_ipDiv32  	5
#define mc_etim_ipDiv64 	6
#define mc_etim_ipDiv128  	7


#define mc_etim_isValidModule(module) 	(((module)>=0) && ((module) <= 3))
#define mc_etim_isValidFunType(funType) (((funType)= = mc_eTim_funNone) || (mc_eTim_funInCapt == (funType)) || (mc_eTim_funOutCmp == (funType)))
#define mc_etim_isValidChan(chan) 		(((chan)>=0) && ((chan) <= 5))
#define mc_etim_isValidDiv(div)	  		(((div) >= mc_etim_ipDiv1) && ((div) <= mc_etim_ipDiv128))
	
#define mc_etim_singlePinInitFunId		0
#define mc_etim_casPinInitFunId			1


/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
	



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


