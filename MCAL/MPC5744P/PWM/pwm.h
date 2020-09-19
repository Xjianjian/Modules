

/* ---- Source switch on ------------------------------------------------------*/
#ifndef PWM_H_
#define PWM_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_pwm_module0		0
#define m_pwm_module1		1
#define m_pwm_moduleCnt		2
#define m_pwm_moduleNone	m_pwm_moduleCnt

	
#define m_pwm_subChannelA   	0
#define m_pwm_subChannelB		1
#define m_pwm_subChannelX   	2
#define m_pwm_subChannelCnt   	3
#define m_pwm_subChannelNone   	m_pwm_subChannelCnt



#define m_pwm_subModule0		0
#define m_pwm_subModule1		1
#define m_pwm_subModule2		2
#define m_pwm_subModule3		3
#define m_pwm_subModuleCnt		4
#define m_pwm_subModuleNone		m_pwm_subModuleCnt


/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef struct
{
	int16_t val1;
	int16_t initVal;
}pwmOutFreqDt_t;


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
extern void pwm_init(void);
extern void pwm_update(void);

#endif

