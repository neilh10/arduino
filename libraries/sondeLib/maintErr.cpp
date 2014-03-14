/* maintErr.c
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
 *  Created on: Mar 4, 2014
 *      Author: Neil Hancock
 *

    Maintenance module

 <p>This component is ongoing and takes two types of errors - a
 positive error, and a negative error - or lack of an event</p>
 <br>positive errors -  unexpected events as errors, and cleansup after them -
 most likley performing a warm reboot.
 <br> negative errors - the lack of an event within a period of time

 <p>Future: negative errors</b>
   </p>
 **/

#include "WProgram.h"
#include "HAL_Timer.h"
#include "debugMsg.h"
#include "TaskSched.h"
#include "sondeExtern.h"

#define debugMask_rd() debugMaskAccess(MASK_INDX_SystemMngXxx)
 static const prog_uchar strmaintErrReboot[]= "maintErr-Reboot";

  bool     errorSema;
  uint16_t lpTillReboot;
#define LP_REBOOT 30000
/*dc***************************************************/
  result_t maintError::_init(uint8_t cmdAction) {
errorSema =FALSE;
  return SUCCESS;
}

//*dc*D9C9T9U9**************************************************
void maintError::_evt(uint8_t uniqueId, uint8_t modId, uint32_t clientData) {

   //can only take one type of error at a time
   if (FALSE == errorSema) {
     errorSema = TRUE;
     lpTillReboot = 0;
     dbg_u8u16(eDbgAll_errEvt,uniqueId,strmaintErrReboot, modId,clientData);
     dbgEol(eDbgAll_errEvt);
     //post taskDie();
     scheduler.postTask(TASK_dieNow);
   }
}
/*df***************************************************/
void maintError::saveTimeEe(void) {
// Save the current time to the EE for later use
	//TODO SaveTimeEe - is this needed with RTC
#if 0
  tos_time_t timeSys=call Time.get();
  w64Two_u32_u time;
  time.two_u32.l.w32 = timeSys.low32;
  time.two_u32.m.w32 = timeSys.high32;
#endif

} //saveTimeEe
/*dt***************************************************/
  void maintError::task_DieNow(void) {

    if (++lpTillReboot > LP_REBOOT) {

      saveTimeEe();

      // 0000101111110100000000000000100
      // Assert [2]SYSRESETREQ
      /* 6.2.2.8 Software reset (SW)
      The SYSRESETREQ bit in the NVIC application interrupt and reset control register can
      be set to force a software reset on the device.
      Setting SYSRESETREQ generates a software reset request. This reset forces a system
      reset of all major components except for the debug module.
      A software reset causes the RCM's SRS1[SW] bit to set. */
#define RESTART_ADDR       0xE000ED0C
#define READ_RESTART()     (*(volatile uint32_t *)RESTART_ADDR)
#define WRITE_RESTART(val) ((*(volatile uint32_t *)RESTART_ADDR) = (val))

      WRITE_RESTART(0x5FA0004);
#if 0
      //TODO
      //Cause Watchdog byte - ATmega256 (doc2549J) Pg65
      //System reset
      WDTCSR = ((1<<WDCE)|(1<<WDE) |eWDT_2sec);
      WDTCSR =  (1<<WDE | eWDT_500mS); //Timeout in 16mS
       while (1) {
         //__asm__ __volatile__("nop" "\n\t" ::);
         //{"nop"};
       }
#endif //0
     } else {
       //post taskDie();
       scheduler.postTask(TASK_dieNow);
    }
  }


//End




