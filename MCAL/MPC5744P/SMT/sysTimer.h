
/* ---- Source switch on ------------------------------------------------------*/
#ifndef GENTIMER_H_
#define GENTIMER_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_stm_isValidTimChannel(chanNo) ((chanNo >=0) && (chanNo <=3))
#define m_stm_isValiddiv(prediv) (((prediv) >= 1) && ((prediv) <= 256))

#define m_stm_InitFunId				0

#define m_stm_FetchCnt				  (STM_0_CNT)



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
extern void stm_init(bool start,uint8_t PreDiv,bool FrzDebug);
extern void stm_setStamp(uint8_t stampIndx);
extern bool stm_getDiff(uint8_t stampIndx,uint32_t * elapseTime);
extern void stm_delayUs(uint32_t us);
extern void stm_clrStamp(uint8_t stampIndx);
extern uint32_t stm_genRandomNumber(void);

#endif

