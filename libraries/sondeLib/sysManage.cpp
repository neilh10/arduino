/* systemManage.c
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
 *  Created on: Mar 8, 2014
 *      Author: Neil Hancock
 */

#include "WProgram.h"
#include <util/atomic.h>
#include "HAL_Timer.h"
#include "debugMsg.h"
#include "TaskSched.h"
#include "debugConsole.h"
//#include "sondeTeensy3.h"

//#define __swTimer__
//#define __dbgCon__
//#define __maintError__
HAL_Timer swTimer;
TaskSched scheduler; //TODO rem Duplicate either here or sondeTeensy3.h
DebugConsole dbgCnsl;
debugCmd_d  dbg_d;
sendDbg dbg;
maintError MaintErr;
//#include "sondeTeensy3.h"

#define IDX MASK_INDX_SystemMngXxx
#define debugMask_rd() debugMaskAccess(IDX)

/*df***************************************************/
static uint8_t sysMng_Fsm=0;
static uint8_t sysMng_Cnt=0;
void task_sys_Manage(void)
/* Manage the continuing system using a FSM
 * Each stage of the system is a finite state. The stages are
 * power availability
 * Wall time verification
 * data collection
 * data storage
 * data transmission
 * hibernating
 *
 *
 *
 * */
{
	sysMng_Cnt++;
	switch (sysMng_Fsm) {
	case 0:
	default:
		//Enters this from
		swTimer.start(TID_SYSTEM_MANAGE,TIMER_REPEAT,TIMER_SW_1SEC_TICK,TASK_SYS_MANAGE_N);
		digitalWriteFast(13, HIGH);
		dbgUint8_1(eDbgSysMngXxx_Evt,'A',"sMng", sysMng_Cnt);
		//dbg.uint8(IDX,eDbgSysMngXxx_Evt, 'A',"sMng",localStartCnt);
		sysMng_Fsm=1;
		break;
	case 1:
		dbgUint8_1(eDbgSysMngXxx_Evt, 'B',"sMng",sysMng_Cnt);
		if (0x01 & sysMng_Cnt ) {
			digitalWriteFast(13, HIGH);
		} else {
			digitalWriteFast(13, LOW);
		}
		//if (CellCallComplete())
		{
		//	sysMng_Fsm=2;
		}
		break;


	case 2:
		swTimer.start(TID_SYSTEM_MANAGE,TIMER_REPEAT,TIMER_SW_5MIN_TICK,TASK_SYS_MANAGE_N);
		digitalWriteFast(13, LOW);
		dbgUint8_1(eDbgSysMngXxx_Evt, 'C',"sMng Sleep",sysMng_Cnt);
		sysMng_Fsm=0;
		sysMng_Cnt=0;
		break;

	}
}//task_systemManage


//End




