/* TaskBasic.h
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
 *  Created on: Mar 3, 2014
 *      Author: Neil Hancock
 */
//Build list of tasks to be run - only invoked once
//#include "HAL_Timer.h"

#ifndef TASKBASIC_H_
#define TASKBASIC_H_

#ifdef __cplusplus
extern "C" {
#endif

class TaskSched  {
public:
	TaskSched(){init();};
	~TaskSched(){};
	void init(void ); //only invoke to re-init()
	error_t postTask(uint8_t id);
	void    prioPostTask(uint16_t id);
	bool    isWaiting( uint8_t id );
	uint8_t popTask();
private:
	 //declare static which means only once making it a singleton
	 bool pushTask( uint8_t id );
	 static uint16_t prio_tasks; //Priority Tasks - strictly hierachical from B0 Highest to B15 Last

	 static uint8_t m_head;
	 static uint8_t m_tail;
	 //static uint8_t m_next[NUM_TASKS]; reiterated on .cpp
	 static uint8_t m_next[];

	 //static const uint8_t taskPriorityConvert [TASK_PRIO_TOTAL];
};


#ifdef	__cplusplus
}
#endif //	__cplusplus
#endif /* TASKBASIC_H_ */
