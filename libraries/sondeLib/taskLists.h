/* sondeTeensy3.h
 * 
 * http://www.biomonitors.com/
 * Copyright (c) 2014 Neil Hancock.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * 1. The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 *  Created on: Mar 12, 2014
 *      Author: Neil Hancock
 */
#ifndef __SONDETEENSY3_H_
#define __SONDETEENSY3_H_
#include "sondeSys.h"

#define HW_TIMER_COUNT_mS  10
#define HW_TIMER_COUNT_uS  (HW_TIMER_COUNT_mS*1000)
#define	TIMER_SW_DIVISOR 10
#define TIMER_SW_100MSEC_TICK 100
#define TIMER_SW_1SEC_TICK 1000/TIMER_SW_DIVISOR
#define TIMER_SW_10SEC_TICK 10000/TIMER_SW_DIVISOR
#define TIMER_SW_5MIN_TICK 300000/TIMER_SW_DIVISOR

enum eTID_TimerId {//Add to List of timers for new timer. Limited total 32
	TID_SYSTEM_MANAGE=0,
	TID_CONSOLE=1,
	TID_EXAMPLE1,
	TID_last,
	NTIMERS=TID_last,
	NUM_TIMERS = NTIMERS
	//TODO: Priority timers limited to 32 timers total
	//#define NTIMERS uniqueCount("Timer") - FUT: can this be used
} ;//List of Timers

#if NTIMERS >30
error; see HAL_Timer:mstate
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum eTASK_id {
	TASK_SYS_MNG_STARTUP_N =0,
	TASK_SYS_MANAGE_N =1,
	TASK_CONSOLE_ANNOUNCE_N, //TODO?
	TASK_CONSOLE_PROMPT_N,//TODO
	TASK_CONSOLE_USER_CMD_N,//TODO
	TASK_CONSOLE_INPUT_N, //TODO
	TASK_CONSOLE_HELP_N, //TODO
	//TASK_HELP_STR2_N,  //TODO
	//TASK_signalOneTimer_N=1,//PRIO capable
	//TASK_timerHandleFire_N=2,//PRIO capable

	TASK_EXAMPLE1_N,
	TASK_dieNow, //Let me at end

	//
	TASK_LAST,
	NUM_TASKS=TASK_LAST,
    NO_TASK = 0xFF,
	TASKPRIO_MASK=0xFF00, //?? 16 tasks can be special
};

enum eTASK_prioId {//Priority Tasks - Highest B0
	//List that maps to lookup table
	//TASKPRIO_signalOneTimer_B =0x0001,
	//TASKPRIO_timerHandleFire_B=0x0002,
	//TASKPRIO_ =,
	TASK_PRIO_TOTAL=16,
	TASKPRIO_none=0
};


#ifdef __cplusplus
}
#endif
void task_sys_Mng_Startup(void);
void task_sys_Manage(void);
void task_example1(void);

#ifdef TASK_PRIORITY_LIST
const uint8_t taskPriorityConvert [TASK_PRIO_TOTAL] = {
	//TASK_signalOneTimer_N,  //0 TASKPRIO_signalOneTimer_B
	//TASK_timerHandleFire_N, //1
	NO_TASK, //2
	NO_TASK, //3

	NO_TASK, //4
	NO_TASK, //5
	NO_TASK, //6
	NO_TASK, //7

	NO_TASK,
	NO_TASK,
	NO_TASK,
	NO_TASK,

	NO_TASK,
	NO_TASK,
	NO_TASK,
	NO_TASK
};
#else
extern const uint8_t taskPriorityConvert[];
#endif //TASK_PRIORITY_LIST

#endif /* __SONDETEENSY3_H_ */
