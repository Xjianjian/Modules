

/* ---- Source switch on ------------------------------------------------------*/

#define EVENT_HANDLE_C_
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "OS_Types.h"
#include "platformTypes.h"
#include "EvtHdl.h"
#include "TmrHdl.h"

/*-----------------------------------------------------------------------------
                   		Module local defines and constants
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                                 Module local types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Local data at RAM
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
                                Global data at RA
------------------------------------------------------------------------------*/
static OSEvtBuf_t OSEvtBufLv1;	/*Level1 Event Table*/
static OSEvtBuf_t OSEvtBufLv2;	/*Level2 Event Table*/
static OSEvtBuf_t OSEvtBufLv3;	/*Level3 Event Table*/
static OSEvtBuf_t OSEvtBufLv4;	/*Level4 Event Table*/






/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Local data at EEPROM
------------------------------------------------------------------------------*/



/*------------------------------------------------------------------------------
                                Global data at ROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                             Global data at EEPROM
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
                         Declaration of local functions
------------------------------------------------------------------------------*/
static void InitEvtTbl(OSEvtBuf_t * evtTbl);
static os_bool_t EvtExist(OSEvtBuf_t * evtTbl);
static os_byte_t GetEvt(OSEvtBuf_t * evtTbl);
static os_bool_t PutEvt(OSEvtBuf_t * evtTbl,const os_byte_t no);
/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/
void EvtHdl_Init(void)
{
	InitEvtTbl(&OSEvtBufLv1);
	InitEvtTbl(&OSEvtBufLv2);
	InitEvtTbl(&OSEvtBufLv3);
	InitEvtTbl(&OSEvtBufLv4);
}

os_byte_t EvtHdl_CalStdTaskPrio(void)
{
	os_byte_t prio;
	prio = (TRUE == EvtExist(&OSEvtBufLv1))?(1 << 7):(0);
	prio += (TRUE == EvtExist(&OSEvtBufLv2))?(1 << 6):(0);
	prio += (TRUE == EvtExist(&OSEvtBufLv3))?(1 << 5):(0);
	return prio;
}

os_byte_t EvtHdl_CalIsrTaskPrio(void)
{
	os_byte_t prio;
	prio = (TRUE == EvtExist(&OSEvtBufLv4))?(1 << 4):(0);
	return prio;
}

os_byte_t EvtHdl_GetEvt(os_byte_t EventType)
{
	os_byte_t Evt = OS_EVT_NONE;
	os_bool_t * p_ready_flag = NULL_PTR;
	switch( EventType)
	{
		case OS_EVT_TYPE_LEVEL1:
			p_ready_flag = task_group1_ready;
			Evt = GetEvt( &OSEvtBufLv1);
			break;
		case OS_EVT_TYPE_LEVEL2:
			p_ready_flag = task_group2_ready;
			Evt = GetEvt( &OSEvtBufLv2);
			break;
		case OS_EVT_TYPE_LEVEL3:
			p_ready_flag = task_group3_ready;
			Evt = GetEvt( &OSEvtBufLv3);
			break;
		case OS_EVT_TYPE_LEVEL4:
			p_ready_flag = task_group4_ready;
			Evt = GetEvt( &OSEvtBufLv4);
			break;
		default:
			break;
	}
	if(OS_EVT_NONE != Evt && NULL_PTR != p_ready_flag
		&& Evt <= 16 && Evt >=1
		&& TRUE == p_ready_flag[Evt - 1])
	{
		p_ready_flag[Evt - 1] = FALSE;
	}
	OS_ENABLE_INTERRUPT;
	return Evt;
}


os_bool_t EvtHdl_PutEvt(os_byte_t EventType,const os_byte_t no)
{
	os_bool_t Ret = TRUE;
	switch(EventType)
	{
		case OS_EVT_TYPE_LEVEL1:
			Ret = PutEvt( &OSEvtBufLv1,no);
			break;
		case OS_EVT_TYPE_LEVEL2:
			Ret = PutEvt( &OSEvtBufLv2,no);
			break;
		case OS_EVT_TYPE_LEVEL3:
			Ret = PutEvt( &OSEvtBufLv3,no);
			break;
		case OS_EVT_TYPE_LEVEL4:
			Ret = PutEvt( &OSEvtBufLv4,no);
			break;
		default:
			break; 
	}
	return Ret;
}


/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/

static void InitEvtTbl(OSEvtBuf_t * evtTbl)
{
	os_word_t Pos;
	evtTbl->head = 0;
	evtTbl->tail = 0;
	for(Pos = 0; Pos <OS_EVENT_MAX; Pos ++)
	{
		evtTbl->No[Pos] = 0;
	}
}

static os_bool_t EvtExist(OSEvtBuf_t * evtTbl)
{
	os_bool_t exist = FALSE;
	if(evtTbl->head != evtTbl->tail)
	{
		exist = TRUE;
	}
	return exist;
}



static os_byte_t GetEvt(OSEvtBuf_t * evtTbl)
{
	
	os_byte_t Evt = OS_EVT_NONE;
	OS_DISABLE_INTERRUPT;
	if(evtTbl->head != evtTbl->tail)
	{
		
		Evt = evtTbl->No[evtTbl->head];
		evtTbl->head = OS_NEXT_EVENT(evtTbl->head);
	}
	OS_ENABLE_INTERRUPT;
	return Evt;
}


static os_bool_t PutEvt(OSEvtBuf_t * evtTbl,const os_byte_t no)
{
	
	os_bool_t Ret = FALSE;
	OS_DISABLE_INTERRUPT;
	if(OS_NEXT_EVENT(evtTbl->tail) != evtTbl->head)
	{
		evtTbl->No[evtTbl->tail] = no;
		evtTbl->tail = OS_NEXT_EVENT(evtTbl->tail);
		Ret = TRUE;
	}
	OS_ENABLE_INTERRUPT;
	return Ret;
}




/* ---- Source switch off ---------------------------------------------------- */
#undef EVENT_HANDLE_C_

/*--- End of file ------------------------------------------------------------*/
