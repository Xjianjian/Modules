

#define TIMER_HANDLE_C_
/*------------------------------------------------------------------------------
 					Include files
------------------------------------------------------------------------------*/
#include "OS_Types.h"
#include "platformTypes.h"
#include "EvtHdl.h"
#include "TmrHdl.h"
#include "OpeHdl_Level1.h"
#include "OpeHdl_Level2.h"
#include "OpeHdl_Level3.h"
#include "OpeHdl_Level4.h"
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
                                Global data at RAM
------------------------------------------------------------------------------*/

/*Feed DOG and DI & AI SAMPLE*/
static OsTmrTbl_t OsLv1TmrTbl[OS_LEVEL1_TMR_MAX] = 
{
	{OS_TMR_LV1_TASK1,		OS_TASK_ON,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_ALL,	10,		1},/*ALL*/
	{OS_TMR_LV1_TASK2,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_PRESAMPLE|STA_TYPE_STATE_NORMAL,	5,		1},//|STA_TYPE_STATE_LOWVOLTAGE
	{OS_TMR_LV1_TASK3,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_PRESAMPLE|STA_TYPE_STATE_NORMAL|STA_TYPE_STATE_PRESLEEP,	5,		0},/*ALL*/
	{OS_TMR_LV1_TASK4,		OS_TASK_ON,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_ALL,	1,		2},/*ALL*/
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0}	
};


/*Cyclic Task Handle*/
static OsTmrTbl_t OsLv2TmrTbl[OS_LEVEL2_TMR_MAX] = 
{
	{OS_TMR_LV2_TASK1,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,							5,		0},
	{OS_TMR_LV2_TASK2,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,							10,		1},
	{OS_TMR_LV2_TASK3,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,							10,		2},
	{OS_TMR_LV2_TASK4,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,							10,		1},
	{OS_TMR_LV2_TASK5,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,							10,		4},
	{OS_TMR_LV2_TASK6,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,							10,		5},
	{OS_TMR_LV2_TASK7,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,							10,		1},
	{OS_TMR_LV2_TASK8,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_SLEEP,							10,		1},
	{OS_TMR_LV2_TASK9,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,							10,		1},
	{OS_TMR_LV2_TASK10,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,							10,		1},
	{OS_TMR_LV2_TASK11,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,							5,		1},
	{OS_TMR_LV2_TASK12,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_PRESLEEP,						10,		1},
	{OS_TMR_LV2_TASK13,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,							10,		1},
	{OS_TMR_LV2_TASK14,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,							10,		1},
	{OS_TMR_LV2_TASK15,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,							10,		1},
	{OS_TMR_LV2_TASK16,		OS_TASK_OFF,		OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,							10,		1}
};


/*Diag */
static OsTmrTbl_t OsLv3TmrTbl[OS_LEVEL3_TMR_MAX] = 
{
	{OS_TMR_LV3_TASK1,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,	10,		0},
	{OS_TMR_LV3_TASK2,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,	10,		0},
	{OS_TMR_LV3_TASK3,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,	100,	0},
	{OS_TMR_LV3_TASK4,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,	10,		0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,10,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0}	
};

/*OUTPUT: DO and CAN TX ,33664*/
static OsTmrTbl_t OsLv4TmrTbl[OS_LEVEL4_TMR_MAX] = 
{
	{OS_TMR_LV4_TASK1,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_PRESAMPLE|STA_TYPE_STATE_NORMAL,	10,		0},
	{OS_TMR_LV4_TASK2,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_PRESAMPLE|STA_TYPE_STATE_NORMAL,	10,		0},
	{OS_TMR_LV4_TASK3,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_NORMAL,	5,	4},/*ALL*/
	{OS_TMR_LV4_TASK4,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_STATE_PRESAMPLE|STA_TYPE_STATE_NORMAL|STA_TYPE_STATE_PRESLEEP, 10,	0},/* 1*/
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0},
	{OS_TMR_TASK_NONE,		OS_TASK_OFF,	OS_TMR_TYPE_CYCLIC,		STA_TYPE_NONE,0,	0}	
};


os_bool_t task_group1_ready[OS_LEVEL1_TMR_MAX] =
{
	FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,
	FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE
};

os_bool_t task_group2_ready[OS_LEVEL2_TMR_MAX] =
{
	FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,
	FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE
};

os_bool_t task_group3_ready[OS_LEVEL3_TMR_MAX] =
{
	FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,
	FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE
};

os_bool_t task_group4_ready[OS_LEVEL4_TMR_MAX] =
{
	FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,
	FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE,FALSE
};


/*------------------------------------------------------------------------------
                                Local data at ROM
------------------------------------------------------------------------------*/
os_bool_t low_voltage_state = FALSE;

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
static void StartTmr(const os_word_t EvtNo, const os_byte_t TmrTblLen, OsTmrTbl_t * TmrTbl);
static void StopTmr(const os_word_t EvtNo, const os_byte_t TmrTblLen, OsTmrTbl_t * TmrTbl);
static void TmrCheckTimeout(const os_byte_t EvtType, const os_byte_t TmrTblLen, OsTmrTbl_t * TmrTbl);


/*------------------------------------------------------------------------------
                      Implementation of global functions
------------------------------------------------------------------------------*/

void TmrHdl_init(void)
{
	os_byte_t os_i = 0;
	low_voltage_state = FALSE;

#if 1
	for(os_i = 0; os_i < OS_LEVEL1_TMR_MAX; os_i ++)
	{
		task_group1_ready[os_i] = FALSE;

		task_group2_ready[os_i] = FALSE;

		task_group3_ready[os_i] = FALSE;

		task_group4_ready[os_i] = FALSE;
	}
#endif
}


void TmrHdl_switchSession(os_byte_t session)
{
	os_byte_t task_i = 0;
	for(task_i = 0; task_i < OS_LEVEL1_TMR_MAX; task_i ++)
	{
		if(0 == (OsLv1TmrTbl[task_i].Sess & session))
		{
			StopTmr(OsLv1TmrTbl[task_i].EvtNo , OS_LEVEL1_TMR_MAX, OsLv1TmrTbl);
		}
		else
		{
			StartTmr(OsLv1TmrTbl[task_i].EvtNo , OS_LEVEL1_TMR_MAX, OsLv1TmrTbl);
		}
	}

	for(task_i = 0; task_i < OS_LEVEL2_TMR_MAX; task_i ++)
	{
		if(0 == (OsLv2TmrTbl[task_i].Sess & session))
		{
			StopTmr(OsLv2TmrTbl[task_i].EvtNo , OS_LEVEL2_TMR_MAX, OsLv2TmrTbl);
		}
		else
		{
			StartTmr(OsLv2TmrTbl[task_i].EvtNo , OS_LEVEL2_TMR_MAX, OsLv2TmrTbl);
		}
	}

	for(task_i = 0; task_i < OS_LEVEL3_TMR_MAX; task_i ++)
	{
		if(0 == (OsLv3TmrTbl[task_i].Sess & session))
		{
			StopTmr(OsLv3TmrTbl[task_i].EvtNo , OS_LEVEL3_TMR_MAX, OsLv3TmrTbl);
		}
		else
		{
			StartTmr(OsLv3TmrTbl[task_i].EvtNo , OS_LEVEL3_TMR_MAX, OsLv3TmrTbl);
		}
	}


	for(task_i = 0; task_i < OS_LEVEL4_TMR_MAX; task_i ++)
	{
		if(0 == (OsLv4TmrTbl[task_i].Sess & session))
		{
			StopTmr(OsLv4TmrTbl[task_i].EvtNo , OS_LEVEL4_TMR_MAX, OsLv4TmrTbl);
		}
		else
		{
			StartTmr(OsLv4TmrTbl[task_i].EvtNo , OS_LEVEL4_TMR_MAX, OsLv4TmrTbl);
		}
	}
}

void TmrHdl_StartTmr(os_byte_t EventType, const os_word_t EventNo)
{
	switch (EventType)
	{
		case OS_EVT_TYPE_LEVEL1:
			StartTmr(EventNo,OS_LEVEL1_TMR_MAX,OsLv1TmrTbl);
			break;
		case OS_EVT_TYPE_LEVEL2:
			StartTmr(EventNo,OS_LEVEL2_TMR_MAX,OsLv2TmrTbl);
			break;
		case OS_EVT_TYPE_LEVEL3:
			StartTmr(EventNo,OS_LEVEL3_TMR_MAX,OsLv3TmrTbl);
			break;
		case OS_EVT_TYPE_LEVEL4:
			StartTmr(EventNo,OS_LEVEL4_TMR_MAX,OsLv4TmrTbl);
			break;
		default:
			break;
	}
}

void TmrHdl_StopTmr(os_byte_t EventType, const os_word_t EventNo)
{
	switch (EventType)
	{
		case OS_EVT_TYPE_LEVEL1:
			StopTmr(EventNo,OS_LEVEL1_TMR_MAX,OsLv1TmrTbl);
			break;
		case OS_EVT_TYPE_LEVEL2:
			StopTmr(EventNo,OS_LEVEL2_TMR_MAX,OsLv2TmrTbl);
			break;
		case OS_EVT_TYPE_LEVEL3:
			StopTmr(EventNo,OS_LEVEL3_TMR_MAX,OsLv3TmrTbl);
			break;
		case OS_EVT_TYPE_LEVEL4:
			StopTmr(EventNo,OS_LEVEL4_TMR_MAX,OsLv4TmrTbl);
			break;
		default:
			break;
	}
}


void TmrHdl_CheckAllTimeout(os_byte_t EventType)
{
	switch (EventType)
	{
		case OS_EVT_TYPE_LEVEL1:
			TmrCheckTimeout(OS_EVT_TYPE_LEVEL1,OS_LEVEL1_TMR_MAX,OsLv1TmrTbl);
			break;
		case OS_EVT_TYPE_LEVEL2:
			TmrCheckTimeout(OS_EVT_TYPE_LEVEL2,OS_LEVEL2_TMR_MAX,OsLv2TmrTbl);
			break;
		case OS_EVT_TYPE_LEVEL3:
			TmrCheckTimeout(OS_EVT_TYPE_LEVEL3,OS_LEVEL3_TMR_MAX,OsLv3TmrTbl);
			break;
		case OS_EVT_TYPE_LEVEL4:
			TmrCheckTimeout(OS_EVT_TYPE_LEVEL4,OS_LEVEL4_TMR_MAX,OsLv4TmrTbl);
			break;
		default:
			break;
	}
}


void TmrHdl_IsrTask(void)
{
	os_byte_t Prio;
	os_byte_t Ret;

	TmrHdl_CheckAllTimeout(OS_EVT_TYPE_LEVEL1);
	TmrHdl_CheckAllTimeout(OS_EVT_TYPE_LEVEL2);
	TmrHdl_CheckAllTimeout(OS_EVT_TYPE_LEVEL3);
	TmrHdl_CheckAllTimeout(OS_EVT_TYPE_LEVEL4);
	
	for(;;)
	{
		Prio = EvtHdl_CalIsrTaskPrio();
		if(Prio >= OS_EVT_PRIO_LEVEL4)
		{
			Ret = OpeHdl_level4();
			if(OS_RET_OK != Ret)
			{
				break;
			}
		}
		else
		{
			break;
		}
		
	}
}


/*------------------------------------------------------------------------------
                      Implementation of local functions
------------------------------------------------------------------------------*/
static void StartTmr(const os_word_t EvtNo, const os_byte_t TmrTblLen, OsTmrTbl_t * TmrTbl)
{
	os_byte_t task_i = 0;
	OS_DISABLE_INTERRUPT;
	for(;task_i < TmrTblLen; task_i ++)
	{
		if(EvtNo == TmrTbl[task_i].EvtNo)
		{
			if(OS_TASK_OFF == TmrTbl[task_i].Sw)
			{
				TmrTbl[task_i].Sw = OS_TASK_ON;
				TmrTbl[task_i].Count = TmrTbl[task_i].Init;
			}
		}
	}
	OS_ENABLE_INTERRUPT;

}

static void StopTmr(const os_word_t EvtNo, const os_byte_t TmrTblLen, OsTmrTbl_t * TmrTbl)
{
	os_byte_t task_i = 0;
	OS_DISABLE_INTERRUPT;
	for(;task_i < TmrTblLen; task_i ++)
	{
		if(EvtNo == TmrTbl[task_i].EvtNo)
		{
			if(OS_TASK_ON == TmrTbl[task_i].Sw)
			{
				TmrTbl[task_i].Sw = OS_TASK_OFF;
				TmrTbl[task_i].Count = 0;
			}
		}
	}
	OS_ENABLE_INTERRUPT;

}



static void TmrCheckTimeout(const os_byte_t EvtType, const os_byte_t TmrTblLen, OsTmrTbl_t * TmrTbl)
{
	os_byte_t task_i = 0;
	os_bool_t * p_ready_flag = NULL_PTR;
	OS_DISABLE_INTERRUPT;
	for(;task_i < TmrTblLen; task_i ++)
	{
		if(OS_TASK_ON == TmrTbl[task_i].Sw)
		{
			/*here ignore the return value*/
			switch(EvtType)
			{
				case OS_EVT_TYPE_LEVEL1:
					p_ready_flag = task_group1_ready;
					break;
				case OS_EVT_TYPE_LEVEL2:
					p_ready_flag = task_group2_ready;
					break;
				case OS_EVT_TYPE_LEVEL3:
					p_ready_flag = task_group3_ready;
					break;
				case OS_EVT_TYPE_LEVEL4:
					p_ready_flag = task_group4_ready;
					break;
				default:
					break;
			}
			if(NULL_PTR != p_ready_flag && FALSE == p_ready_flag[task_i])
			{
				if(TmrTbl[task_i].Count > 0)
				{
					TmrTbl[task_i].Count --;
				}
				if(0 == TmrTbl[task_i].Count)
				{
					p_ready_flag[task_i] = TRUE;
					EvtHdl_PutEvt(EvtType, TmrTbl[task_i].EvtNo);
					if(OS_TMR_TYPE_CYCLIC == TmrTbl[task_i].Type)
					{
						TmrTbl[task_i].Count = TmrTbl[task_i].Init;
					}
					else
					{
						TmrTbl[task_i].Count = 0;
						TmrTbl[task_i].Sw = OS_TASK_OFF;
					}
				}
			}
		}
	}
	OS_ENABLE_INTERRUPT;
}






/* ---- Source switch off ---------------------------------------------------- */
#undef TIMER_HANDLE_C_

/*--- End of file ------------------------------------------------------------*/
