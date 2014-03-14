/* HAL_Timer.cpp
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

#include "WProgram.h"
#include "HAL_Timer.h"
#include "TaskSched.h"
#include "debugMsg.h"
#include "sondeExtern.h"

/* ------------------------------------------------------------
* static class variables need to be reiterated here before use
* ------------------------------------------------------------*/
int8_t  HAL_Timer::queue_head;//last idx[queue] to timer, incr & pull timer off here, 0->Top->0
int8_t  HAL_Timer::queue_tail;//last place inserted, incr & push timer in here 0->Top->0
uint8_t HAL_Timer::queue_size;//number in queue, incr when inserting timer, decr when removing
uint8_t HAL_Timer::queue[NUM_TIMERS+1];//Slightly larger queue than used as insurance

halt_timersDb_t  HAL_Timer::mTimerList[NUM_TIMERS];

#define IDX MASK_INDX_SystemMngXxx
#define debugMask_rd() debugMaskAccess(IDX)


   cchar strTimerErr[] = "Timer **Err";
//    static char strTimerErrSignal[] = "TimerM **ErrSignal";
//    static char strTimerErrEnque[] = "TimerM **ErrEnque";

/*dc***************************************************/
void HAL_Timer::init()
{
	mState=0;
    queue_head_last = queue_head = queue_tail = -1;
    queue_size = 0;
}


/*dc***************************************************/
//bool HAL_Timer::Timer_begin(unsigned int newPeriod)
bool HAL_Timer::begin(unsigned int newPeriod)
{
return TRUE;
}//HAL_Timer::Timer_begin

/*dc***************************************************/
result_t HAL_Timer::stop(uint8_t id)
{	//Stop the specified time
  if (id>=NUM_TIMERS) {
    dbgUint8_1(eDbgAll_errEvt,'D',strTimerErr,id);
    return FAIL; ///Exit
  }
#ifdef DBG_TIMERM1
  dbg_u8u32(eDbgHalClock_Evt,'E',PSTR("TimerM-Stop"),id,mState);
#endif
  if (mState&(0x1L<<id))
  { // if the timer is running
	  //TODO atomic
	  {mState &= ~(0x1L<<id);}

	  if (!mState)
	  {
      //There are no active timers
        //setIntervalFlag = 1;
	   }
	  return SUCCESS;
  }
  return FAIL; //timer not running
  }// HAL_Timer::Timer_stop

/*dc***************************************************/
result_t HAL_Timer::start(
		uint8_t timerId,//eTID_TimerId mandatory
		char timerType,//eTIM_type
      uint32_t interval_ticks,
      uint8_t callbackTaskId //!NO_TASK then post eTASK_id
	  //,void (*inTimerCallBackFunc)(uint8_t) //!NULL then call back
      )
{

    if (timerId >= NUM_TIMERS) {

    	dbgUint8_1(eDbgAll_errEvt, 'A',strTimerErr,timerId);
    	return FAIL;
    }

    if (timerType > TIMER_ONE_SHOT)  {
    	dbgUint8_1(eDbgAll_errEvt, 'B',strTimerErr,timerType);
    	return FAIL;
    }
#define cTimerMinimium_ticks 2
    if (interval_ticks < cTimerMinimium_ticks)  { //cTimerMax_bmSec int32 and should never be negative
      //dbgWord(eDbgAll_errEvt,'C',strTimerErr,interval_bmSec);
      interval_ticks = cTimerMinimium_ticks;
    }

	dbgUint8_3(eDbgHalClock_Evt, 'A',"TimerEvIn ",
			timerId, (uint8_t)timerType,callbackTaskId);

    mTimerList[timerId].ticks = interval_ticks;
    mTimerList[timerId].type = timerType;
    mTimerList[timerId].taskId = callbackTaskId;
    //TODO mTimerList[timerId].callBackFn = inTimerCallBackFunc;
    if (eDbgHalClock_Evt & debugMask_rd()) {
      //call prn.printChar('{');
      //call prn.printChar('A'+id);
    }
    //TODO: atomic
    {
      //diff_bmSec = readHwTmr_bmSec(); //Get current time in bmSec
      //interval_bmSec += diff_bmSec;
      mTimerList[timerId].ticksLeft = interval_ticks;
      mState|=(0x1L<<timerId);
    }
#ifdef DBG_TIMERM1
    dbg_u8u16u32(eDbgHalClock_Evt,'B',PSTR("TimerM-StartIntvl"),timerId,minInterval_bmSec,mState);
#endif

    return SUCCESS;
    }//Timer_start

/*dt***************************************************
 * Called once per active tick.
 * Decrements all active timers, and if one is found
 * posts to a task
 * ***************************************************/
void HAL_Timer::task_timerHandleFire() {
  uint8_t i;
  uint16_t int_out;
  uint32_t mStateMsk;
	//setIntervalFlag = 1;
  /* Handled at interrupt or high priority. */

  if (eDbgHalClock_Evt & debugMask_rd()) {
    //call prn.printChar('*');
  }

  //TODO del atomic
  {
    //Clear the sema and snapshot the time period being processed
    int_out = 1;//TODO interval_outstanding_bmSec;
  }
  if (mState) {
    //There is at least one active timer
    mStateMsk =0x1L; //TODO - speedup using this
    for (i=0;i<NUM_TIMERS;i++,mStateMsk<<=1)  {
      //*TODO FUTuse a mStatesMsk to speed up,
      //*if (mState & mStateMsk) {
      if (mState&(0x1L<<i)) {
        //Found the active timer - check if expired
        mTimerList[i].ticksLeft -= int_out;
        if (0 == mTimerList[i].ticksLeft) {
            if (mTimerList[i].type==TIMER_REPEAT) {
              mTimerList[i].ticksLeft = mTimerList[i].ticks;
            } else {// one shot timer
              //mState &= ~mStateMsk;
              mState &=~(0x1L<<i);
            }
            //enqueue(i); //to be pulled off by signalOneTimer()
            //TaskPriority_postTask(TASKPRIO_signalOneTimer_B);
        	//dbg.uint8(IDX,eDbgHalClock_Evt, 'B',(char *)"TimerEvOut ",i,mTimerList[i].taskId);
            scheduler.postTask(mTimerList[i].taskId);
        }
      }
    }
  }

 }// timerHandleFire()

// ------------------------------------------------------------
// stop the timers if running
// ------------------------------------------------------------
void HAL_Timer::end() {
	//TODO if needed
  //if (status == TIMER_PIT) stop_PIT();
  //status = TIMER_OFF;
}

//End HAL_Timer.cpp




