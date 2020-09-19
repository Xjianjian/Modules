
/* ---- Source switch on ------------------------------------------------------*/
#ifndef EVT_HDL__H_
#define EVT_HDL__H_

/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "OS_Types.h"

/*------------------------------------------------------------------------------
                             Defines and constants
------------------------------------------------------------------------------*/
#define OS_EVENT_MAX				(32)				/*Caution:must be (1 << x)*/

#define OS_NEXT_EVENT(x)			((x+1) & (OS_EVENT_MAX - 1))
#define OS_BACK_EVENT(x)			((x-1) & (OS_EVENT_MAX - 1))

#define OS_EVT_PRIO_LEVEL1			0x80
#define OS_EVT_PRIO_LEVEL2			0x40
#define OS_EVT_PRIO_LEVEL3			0x20
#define OS_EVT_PRIO_LEVEL4			0X10

#define	OS_EVT_NONE				0x00	/* event none */


#define OS_DISABLE_INTERRUPT	 __asm__(" wrteei 0")
#define OS_ENABLE_INTERRUPT	 	 __asm__(" wrteei 1")


#define	OS_EVT_TYPE_LEVEL1		(os_byte_t)1	/* Event Level1 */
#define	OS_EVT_TYPE_LEVEL2		(os_byte_t)2	/* Event Level2 */
#define	OS_EVT_TYPE_LEVEL3		(os_byte_t)3	/* Event Level3 */
#define	OS_EVT_TYPE_LEVEL4		(os_byte_t)4	/* Event Level4 */

/*------------------------------------------------------------------------------
                                   Data types
------------------------------------------------------------------------------*/
typedef struct
{
	os_byte_t head;
	os_byte_t tail;
	os_byte_t No[OS_EVENT_MAX];
}OSEvtBuf_t;







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
extern void EvtHdl_Init(void);
extern os_byte_t EvtHdl_CalStdTaskPrio(void);
extern os_byte_t EvtHdl_CalIsrTaskPrio(void);
extern os_byte_t EvtHdl_GetEvt(os_byte_t EventType);
extern os_bool_t EvtHdl_PutEvt(os_byte_t EventType,const os_byte_t no);


#endif


