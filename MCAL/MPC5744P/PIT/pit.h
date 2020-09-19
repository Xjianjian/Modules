

/* ---- Source switch on ------------------------------------------------------*/
#ifndef PIT_H_
#define PIT_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_pit_chanNum				4

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
extern void pit_start(void);

extern void pit_stop(void);

extern void pit_frz(void);

extern void pit_free(void);

extern void pit_enChan(uint8_t chan);

extern void pit_disChan(uint8_t chan);

extern void pit_setLoadVal(uint8_t chan,uint32_t periodVal);

extern uint32_t pit_readCnt(uint8_t chan);

extern void pit_setChanMode(uint8_t chan,bool chain,bool isrEn,bool start);

void pit_clrIsrFlag(uint8_t chan);

bool pit_getIsrFlag(uint8_t chan);


#endif

