

#ifndef __SYSCLOCK_H__
#define __SYSCLOCK_H__

#include "typedefs.h"

#define m_sysClk_cfg_modeUseRUN0  TRUE
#define m_sysClk_cfg_OSC_20MHz    FALSE

#define m_sysClk_cfg    200000000 //defines system clock frequency in 200MHz

/* Function Export */
extern void sysClk_init(void);
extern void sysClk_uninit(void);
extern void sysSchClk_init(void);
extern void periClk_init(void);
extern uint32_t sys_random(bool seedInit);

/* Macros for task timer */
#define sysClk_CurCnt             STM_0_CNT   /* 32bit counter */
#define sysClk_MS(ms)             ((ms) * 10000ul) /* [0.1 us] */
#define sysClk_1ms                (10000ul)
#define sysClk_10ms               (100000ul)
#define sysClk_TimePass(lastTime) (sysClk_CurCnt - (lastTime))

#endif
/*--------EOF--------*/

