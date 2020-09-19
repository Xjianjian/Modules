

/* ---- Source switch on ------------------------------------------------------*/
#ifndef PWM_CFG_H_
#define PWM_CFG_H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define m_pwm_PreDiv1		(0)
#define m_pwm_PreDiv2		(1)
#define m_pwm_PreDiv4		(2)
#define m_pwm_PreDiv8		(3)
#define m_pwm_PreDiv16		(4)
#define m_pwm_PreDiv32		(5)
#define m_pwm_PreDiv64		(6)
#define m_pwm_PreDiv128		(7)

#define m_pwm_ClkFreq		(4000000)
#define m_pwm_PreDiv 		m_pwm_PreDiv4
#define m_pwm_cntFreq		(m_pwm_ClkFreq/4)


#define m_pwm_outChannels   4




#define m_pwm_outFreq100K   (100000)
#define m_pwm_outFreq50K	(50000)
#define m_pwm_outFreq25K	(25000)
#define m_pwm_outFreq20K	(20000)
#define m_pwm_outFreq10K	(10000)
#define m_pwm_outFreq5K		(10000)
#define m_pwm_outFreq2_5K	(2500)
#define m_pwm_outFreq1K		(1000)
#define m_pwm_outFreq500hz	(500)
#define m_pwm_outFreq250hz	(250)
#define m_pwm_outFreq200hz	(200)
#define m_pwm_outFreq100hz	(100)
#define m_pwm_outFreq50hz	(50)
#define m_pwm_outFreq25hz	(25)

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef struct
{
	int16_t  val0;
	bool	 std;
	uint8_t  Mscr;
}PwmCfg_t;




/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/
extern const PwmCfg_t PwmCfgGrp[m_pwm_outChannels];


extern const uint32_t pwmOutChannnelFreq[m_pwm_outChannels];

extern uint8_t hld_po_fan_CONTROL;
extern uint8_t hld_po_pump1_CONTROL;
extern uint8_t hld_po_pump2_CONTROL;
extern uint8_t hld_po_pump3_CONTROL;
extern uint8_t *pwm_duty_grp[m_pwm_outChannels];
/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/


#endif

