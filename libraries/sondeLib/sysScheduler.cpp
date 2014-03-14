/* sysScheduler.cpp
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
 *  Created on: Mar 11, 2014
 *      Author: Neil Hancock
 */

#include "WProgram.h"
#include <util/atomic.h>
#include "HAL_Timer.h"
#include "debug.h"
#include "debugMsg.h"
#include "TaskSched.h"
#include "debugConsole.h"
#include "sondeExtern.h"

#define IDX MASK_INDX_SystemMngXxx
#define debugMask_rd() debugMaskAccess(IDX)

/*df***************************************************/
void mainSched_list(void)
{
	uint8_t task_num;
	do {
		ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {//TODO check atomic
			task_num = scheduler.popTask();
			//dbgUint8_1(IDX,eDbgSysMngXxx_Evt, 'C',"sMng Task",task_num);
		}
		switch (task_num)
		{
			//Follow eTASK_id keep case in numerical order for switch to gen table and be fast
			case TASK_SYS_MNG_STARTUP_N: task_sys_Mng_Startup(); break;
			case TASK_SYS_MANAGE_N: task_sys_Manage();break;
			case TASK_CONSOLE_ANNOUNCE_N:dbgCnsl.task_console_announce();break;
			case TASK_CONSOLE_PROMPT_N:  dbgCnsl.task_console_prompt();  break;
			case TASK_CONSOLE_USER_CMD_N:dbgCnsl.task_console_user_cmd();break;
			case TASK_CONSOLE_INPUT_N:   dbgCnsl.task_console_input();   break;
			case TASK_CONSOLE_HELP_N:    dbgCnsl.task_console_help();    break;
			case TASK_EXAMPLE1_N:        task_example1(); break;  //TODO

			//case TASK_timerHandleFire_N: swTimer.task_timerHandleFire();break;
			//Can be last as not called often
			case TASK_dieNow: MaintErr.task_DieNow();break;
			//case : break;
			default:
				//error
			case NO_TASK:
				break;
		}
	} while (NO_TASK != task_num);

} //mainSched_list
//End sysScheduler




