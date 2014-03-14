/* HAL_Timer.h
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
 *  Created on: Mar 5, 2014
 *      Author: Neil Hancock
 */

#ifndef __HAL_TIMER_H__
#define __HAL_TIMER_H__

#include "taskLists.h"

enum eTIM_type {
    TIMER_REPEAT = 0,
    TIMER_ONE_SHOT = 1,
} ;
typedef struct halt_timersDb_t {
   uint8_t type;		// one-shot or repeat timer
   uint8_t taskId;		// call back taskId
   //TODO future void (*callBackFunc)(void);
   //These are kept in bmSec for compatibility and convenience
   // This could probably be reduced to int16_t and be more
   // reliably tested
   int32_t ticks;		// clock ticks for a repeat timer - units 'ticks' TODO !bmSecs
   int32_t ticksLeft;	// ticks left before the timer expires - units 'ticks' TODO !bmSecs
} halt_timersDb_t;

#ifdef __cplusplus
extern "C" {
#endif

class HAL_Timer  {
  private:

    int8_t queue_head_last;//debug - last copy
    uint32_t mState;		// each bit represent a timer state
    uint16_t minInterval_bmSec; //minimum Interval of all timers - binay mSec
    volatile uint16_t interval_outstanding_bmSec; //Storage for communication to task - units binary milliSeconds
    volatile bool intervalExpired_sema; //0=no action 1 = systemTick and is ready to be processed
    volatile uint16_t  hwThreshold_bmSec; // Last HwThreshold used

    //Timer queue -FIFO
    static int8_t queue_head;//last idx[queue] to timer, incr & pull timer off here, 0->Top->0
    static int8_t queue_tail;//last place inserted, incr & push timer in here 0->Top->0
    static uint8_t queue_size;//number in queue, incr when inserting timer, decr when removing
    static uint8_t queue[NUM_TIMERS+1];//Slightly larger queue than used as insurance

#define TIMERS_QUE_SZ (NUM_TIMERS - 1)

    static halt_timersDb_t mTimerList[NUM_TIMERS];

    // ----------------------------------------------------------
    void init();
    void end();

    //void enqueue(uint8_t value);
    result_t Timer_stop(uint8_t id);

  public:
	HAL_Timer() { init();}
	~HAL_Timer() { end();}

	bool begin(unsigned int newPeriod);

	result_t start(
			uint8_t timerId,//eTID_TimerId mandatory
			char timerType,//eTIM_type
	      uint32_t interval_bmSec,
	      uint8_t callbackTaskId //!NO_TASK then post eTASK_id
		  //,void (*inTimerCallBackFunc)(uint8_t) //!NULL then call back
	      );

	bool status(uint8_t TimerId) { return((mState&(0x1L<<TimerId))?TRUE:FALSE);}
	result_t stop(uint8_t id);

	void task_signalOneTimer(void);
	void task_timerHandleFire(void);
};

#ifdef __cplusplus
}
#endif

#endif /* __HAL_TIMER_H__ */
