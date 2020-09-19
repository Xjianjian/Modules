

/* ---- Source switch on ------------------------------------------------------*/
#ifndef TMR_HDL__H_
#define TMR_HDL__H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "OS_Types.h"

/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define OS_TMR_TYPE_ONESHOT					0
#define OS_TMR_TYPE_CYCLIC					1

#define OS_TASK_ON							1
#define OS_TASK_OFF							0


#define OS_LEVEL1_TMR_MAX					16
#define OS_LEVEL2_TMR_MAX					16
#define OS_LEVEL3_TMR_MAX					16
#define OS_LEVEL4_TMR_MAX					16

#define OS_TMR_TASK_NONE					0


#define OS_TMR_LV1_TASK1					1
#define OS_TMR_LV1_TASK2					2
#define OS_TMR_LV1_TASK3					3
#define OS_TMR_LV1_TASK4					4

#define OS_TMR_LV2_TASK1					1
#define OS_TMR_LV2_TASK2					2
#define OS_TMR_LV2_TASK3					3
#define OS_TMR_LV2_TASK4					4
#define OS_TMR_LV2_TASK5					5
#define OS_TMR_LV2_TASK6					6
#define OS_TMR_LV2_TASK7					7
#define OS_TMR_LV2_TASK8					8
#define OS_TMR_LV2_TASK9					9
#define OS_TMR_LV2_TASK10					10
#define OS_TMR_LV2_TASK11					11
#define OS_TMR_LV2_TASK12					12
#define OS_TMR_LV2_TASK13					13
#define OS_TMR_LV2_TASK14					14
#define OS_TMR_LV2_TASK15					15
#define OS_TMR_LV2_TASK16					16



#define OS_TMR_LV3_TASK1					1
#define OS_TMR_LV3_TASK2					2
#define OS_TMR_LV3_TASK3					3
#define OS_TMR_LV3_TASK4					4

#define OS_TMR_LV4_TASK1					1
#define OS_TMR_LV4_TASK2					2
#define OS_TMR_LV4_TASK3					3
#define OS_TMR_LV4_TASK4					4




#define	STA_TYPE_NONE					0x00
#define	STA_TYPE_ALL					0xFF
#define	STA_TYPE_STATE_PRESAMPLE		0x01
#define	STA_TYPE_STATE_NORMAL			0x02
#define	STA_TYPE_STATE_PRESLEEP			0x04
#define	STA_TYPE_STATE_SLEEP			0x08
#define	STA_TYPE_STATE_FAIL				0x10
//#define STA_TYPE_STATE_LOWVOLTAGE		0x20






/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/

typedef struct
{
	os_word_t EvtNo;
	os_byte_t Sw;
	os_byte_t Type;
	os_byte_t Sess;
	os_word_t Init;
	os_word_t Count;
}OsTmrTbl_t;







/*------------------------------------------------------------------------------
                      External declaration of global RAM-Variables
------------------------------------------------------------------------------*/
extern os_bool_t low_voltage_state;
extern os_bool_t task_group1_ready[];
extern os_bool_t task_group2_ready[];
extern os_bool_t task_group3_ready[];
extern os_bool_t task_group4_ready[];

/*------------------------------------------------------------------------------
                      External declaration of global ROM-Variables
------------------------------------------------------------------------------*/




/*------------------------------------------------------------------------------
                    External declaration of global EEPROM-Variables
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                              Declaration of global functions
------------------------------------------------------------------------------*/
extern void TmrHdl_init(void);
extern void TmrHdl_switchSession(os_byte_t session);

#endif


