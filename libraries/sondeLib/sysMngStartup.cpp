/* systemMngStartup.cpp
 *
 * Manage the startup of the system - basically ensuring has valid time and a few other things.
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
#include "HAL_Timer.h"
#include "TaskSched.h"
#include "debugMsg.h"
#include "sondeExtern.h"

#define IDX MASK_INDX_SystemMngXxx
#define debugMask_rd() debugMaskAccess(IDX)

/*df***************************************************/

	static uint8_t localStartCnt;
void task_sys_Mng_Startup(void)
//Start up the environment, validate data stores, self test as needed
{
	localStartCnt++;

	dbgUint8_1(eDbgSysMngXxx_Startup, 'A',"SystemMngStartup",localStartCnt);
	//Get update on time - probably from RTC, or server

	//TODO - check system, and then start TASK_SYSTEM_MANAGE_N
	swTimer.start(TID_SYSTEM_MANAGE,TIMER_REPEAT,TIMER_SW_1SEC_TICK,TASK_SYS_MANAGE_N);

}//task_startFn


//End




