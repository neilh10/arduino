//-------------------
#include "WProgram.h"
#include "HAL_Timer.h"
#include "Time.h"
#include "debugMsg.h"
#include "TaskSched.h"
#include "sondeExtern.h"

//#define ARDUINO


//This is per file debug mapping
#define debugMask_rd() debugMaskAccess(MASK_INDX_SystemMngXxx)
//Example:  see dbgUint8_1

//Local variables
volatile bool timerList_semaphore=FALSE;
uint32_t localIsrCount=0;
extern volatile uint32_t systick_millis_count;


/*df***************************************************/
void setup(void)
{

	// To use Teensy 3.0 without Arduino, simply put your code here.

	// setTime(int hr,int min,int sec,int day, int month, int yr);
	setTime(       0,       0,      0,      1,   1,   2014); //default product born
	Serial.begin(115200);


	// TODO - setup RTC, needs external Xtal
	// time_t t = processSyncMessage(); //See basic parse capability time.zip TimeTeensy3.ino
	//Teensy3Clock.set(t); // set the RTC
	//setTime(t);


	pinMode(13, OUTPUT); //Leds TODO move to PIO init

	//Clear USB serial input buffers
	{int count=0;
		while (Serial.available()){
			if (count++ > 10000) {
				dbgUint8_1(eDbgAll_errEvt, 'A',"main Serial Err ",count);
				break;
			}
			Serial.read();
		}
	}

	swTimer.start(TID_SYSTEM_MANAGE,TIMER_REPEAT,TIMER_SW_1SEC_TICK,TASK_SYS_MNG_STARTUP_N);
	scheduler.postTask(TASK_CONSOLE_ANNOUNCE_N);//Entry point to console apps
	swTimer.start(TID_CONSOLE, TIMER_REPEAT,TIMER_SW_100MSEC_TICK,TASK_CONSOLE_INPUT_N);
	swTimer.start(TID_EXAMPLE1, TIMER_REPEAT,TIMER_SW_1SEC_TICK,TASK_EXAMPLE1_N);
}


/* For a new task, copy this module and make the following entries
 * libraries\sondeLib\TaskLists.h -
 *    enum  TASK_EXAMPLE1_N   - keep the name same as lower case functipn
 *    and also protoype
 *    void task_example1(void);
 *
 * libraries\sondeLib\sysScheduler.cpp - enter the new task
 *   case TASK_EXAMPLE1_N:        task_example1(); break;
 *
 * Define a new timer enum in  eTID_TimerId
 * eg TID__PERIOD2 and then put the following entry somewhere else to start it
 *  defining either TIMER_REPEAT or TIMER_ONE_SHOT
 *  the time "unit" is in number of TIMER_TICKS which comes from SYS_TICKS
 *  the TASK_ must already be defined.
 *
    swTimer.start(TID_PERIOD2,TIMER_REPEAT,TIMER_SW_1SEC_TICK,TASK_EXAMPLE1_N);
* or
     swTimer.start(TID_PERIOD2,TIMER_ONE_SHOT,TIMER_SW_1SEC_TICK,TASK_EXAMPLE1_N);
 *
 */
int example1_cnt;
void task_example1(void)
/* To be repeatedly called
 */
{
	// Can use a protothread for breaking up tasks - which is a limited Finite State Machine

	//Simple update mechanism
	if (++example1_cnt > 10) {example1_cnt=1;}

	swTimer.start(TID_EXAMPLE1, TIMER_REPEAT,example1_cnt*TIMER_SW_1SEC_TICK,TASK_EXAMPLE1_N);
	dbgUint8_1(eDbgAll_AllEvts, 'A'," Example1 ",example1_cnt);

// swTimer.stop(TID_DESC1FN); - stop calling when complete
} //end task_example1Fn

#if !defined(ARDUINO)
int main() {
	// Arduino's main() function just calls setup() and loop()....
	setup();
	while (1) {
		loop();
		yield();
	}
}
#endif

/*df***************************************************/
void loop(void)
{
	//This is background task level
	if (timerList_semaphore)
	{
		timerList_semaphore=FALSE;
		mainSched_list(); //Make all tasks happen
	}else {
		McuSleep_sleep();	//power down anylysis
	}
}//End loop



/*df***************************************************/
//The following should overide the weak  mk20dx128.c:systick_default_isr
void systick_isr(void)
{
	systick_millis_count++;

	if (++localIsrCount>HW_TIMER_COUNT_mS){
		localIsrCount=0;
		swTimer.task_timerHandleFire();
		timerList_semaphore =TRUE;
	}

}//systick_isr
//End main.cpp


