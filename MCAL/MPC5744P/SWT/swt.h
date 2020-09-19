
#ifndef SWT_H_
#define SWT_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_swt_fixedService		(0b00)		
#define m_swt_keyedService		(0b01)	/*pseudo random number*/

#define m_swt_reguMode			0
#define m_swt_winMode		    1

#define m_swt_invalidAccessBusErr		0
#define m_swt_invalidAccessReset		1

//#define m_



#define m_swt_en				TRUE
#define m_swt_serviceWay		m_swt_fixedService
#define m_swt_mode				m_swt_reguMode
#define m_swt_IAR				m_swt_invalidAccessReset


#define m_swt_ITR				FALSE		/*interrupt then reset*/
#define m_swt_stopInstopMode	TRUE
#define m_swt_stopInDebugMode	TRUE

#define m_swt_cntFreq			(16000000)		/*unit:hz*/
#define m_swt_period			(200)			/*uint:ms*/
#define m_swt_timeoutCnt		((m_swt_cntFreq /1000) * m_swt_period)

#if m_swt_mode == m_swt_winMode
	#define m_swt_windowStartVal			0
#endif



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
extern void swt_init(void);
extern void swt_uninit(void);
extern void swt_feedDog(void);


#endif

