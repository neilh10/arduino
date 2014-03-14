/* SchedulerBasic.c
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
 *  Created on: Mar 1, 2014
 *      Author: Neil Hancock
 */

#define TASK_PRIORITY_LIST
#include "WProgram.h"
#include "HAL_Timer.h"
#include "TaskSched.h"
#include "taskLists.h"

/* module SchedulerBasicP  {
 * TEP 106 - http://www.tinyos.net/tinyos-2.x/doc/
 * + simple priority task
 *
 *Possible alternatives for round robbin
 * Contiki Process.c/.h http://contiki.sourceforge.net/docs/2.6/a00464.html
 *  provides interface Scheduler;
  provides interface TaskBasic[uint8_t id];
*/
#ifdef	__cplusplus
//extern "C"
//{
#endif // __cplusplus

  uint16_t TaskSched::prio_tasks; //Priority Tasks - strictly hierachical from B0 Highest to B15 Last

  uint8_t TaskSched::m_head;
  uint8_t TaskSched::m_tail;
  uint8_t TaskSched::m_next[NUM_TASKS];

  // Helper functions (internal functions) intentionally do not have atomic
  // sections.  It is left as the duty of the exported interface functions to
  // manage atomicity to minimize chances for binary code bloat.

  // move the head forward
  // if the head is at the end, mark the tail at the end, too
  // mark the task as not in the queue
   uint8_t TaskSched::popTask()
  {
	if (prio_tasks)
	{ //Any priority tasks to execute
		uint16_t pt_mask= 0x01;
		uint16_t pt_num=0;
		uint8_t taskId=NO_TASK;
		do
		{
			if (prio_tasks & pt_mask)
			{
				//found a task - look it up
				taskId = taskPriorityConvert[pt_num];
				prio_tasks &= ~pt_mask;//Clear this task and return with
				return taskId;
			}
			pt_mask <<= 1;
			pt_num++;
		}
		while ((0!=pt_mask) && (0!=prio_tasks));
	}
	else if( m_head != NO_TASK )
    {
      uint8_t id = m_head;
      m_head = m_next[m_head];
      if( m_head == NO_TASK )
      {
    	  m_tail = NO_TASK;
      }
      m_next[id] = NO_TASK;
      return id;
    }

	//TODO - implement audit of m_next[] to verify empty
    return NO_TASK;

  }//popTask

  bool TaskSched::isWaiting( uint8_t id )
  {
    return (m_next[id] != NO_TASK) || (m_tail == id);
  }

  bool TaskSched::pushTask( uint8_t id )
  {
    if( !isWaiting(id) )
    {
      if( m_head == NO_TASK )
      {
    	  m_head = id;
    	  m_tail = id;
      }
      else
      {
    	  m_next[m_tail] = id;
    	  m_tail = id;
      }
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }

  void TaskSched::init()
  {
    //TODO atomic
    {
      prio_tasks =0;
      memset( (void *)m_next, NO_TASK, sizeof(m_next) );
      m_head = NO_TASK;
      m_tail = NO_TASK;
    }
  }
#if 0
bool2 Scheduler_runNextTask() {
    uint8_t nextTask;
    error_t result=TRUE;
    // atomic
    cli();
    {
      nextTask = popTask();
      if (nextTask >= TASK_LAST) {result=FALSE; /*error++;*/}
      if( nextTask == NO_TASK) { result= FALSE;}
    }
    sei();
    if (TRUE==result){
    //TODO-done signal TaskBasic.runTask[nextTask]();
        (*(taskBasicFn[nextTask].someFunc))();
    }
    return result;
}

  void Scheduler_taskLoop()
  {
    for (;;)
    {
      uint8_t nextTask;


      cli();{ //atomic
    	  while ((nextTask = popTask()) == NO_TASK)
    	  {
    		  //call McuSleep.sleep();
    		  McuSleep_sleep();
    	  }
      }sei();
     //signal TaskBasic.runTask[nextTask]();
      (*(taskBasicFn[nextTask].someFunc))();

    }
  }
#endif //0
  /*
   * Return SUCCESS if the post succeeded, EBUSY if it was already posted.
   */

//  async command error_t TaskBasic.postTask[uint8_t id]()
error_t TaskSched::postTask(uint8_t id)
  { bool2 result_t;
    //TODO atomic
	cli();
	result_t = pushTask(id);
	sei();
    return result_t ? SUCCESS : EBUSY;
  }

void TaskSched::prioPostTask(uint16_t prio_bitmap_id) {
	prio_tasks |= prio_bitmap_id;
}

#ifdef	__cplusplus
//}
#endif // __cplusplus
//End




