/* Utils.cpp
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
 *
 * PrintUtils that prints to an output stream.
 * <p>This configuration maps to UI output </p>
 */
#include "WProgram.h"
#include "sondeSys.h"
#include "Utils.h"


/*dc****************************************************/
result_t prnt_scanHex1Char(const char * inputPtr, uint8_t * outPtr) {
  /*Expect 0-9,[a-f or A-F]
   *
   *Convert to using strtol
  **/
  char input;
  result_t result = SUCCESS;

  input = *inputPtr;
    if (input >= '0' && input <= '9') {
        *outPtr = input - '0';
    }else if (input >= 'a' && input <= 'f') {
        *outPtr = input - 'a'+0x0a;
    }else if (input >= 'A' && input <= 'F'){
        *outPtr = input - 'A'+0x0a;
    } else {
        result = FAIL;
    }

    return result;
}

/*dc****************************************************/
result_t prnt_scanHex2Char(const char * input, uint8_t * out) {
  /*Expect 0-9,[a-f or A-F]
   *Convert to using strtol
  **/
  uint8_t tempLsb,tempMsb;
  result_t result1,result2;

  result1 = prnt_scanHex1Char(input,&tempMsb);
  input++;
  result2 = prnt_scanHex1Char(input,&tempLsb);
  *out = (tempLsb + (tempMsb<< 4));

  return result1 && result2;
}
/*dc****************************************************/
result_t prnt_scanHex3Char(const char * input, uint16_t * out) {
  /*Expect 0-9,[a-f or A-F]
   *Convert to using strtol
  **/
  uint8_t tempLsb,tempMsb;
  result_t result1,result2;

  result1 = prnt_scanHex1Char(input,&tempMsb);
  input++;
  result2 = prnt_scanHex2Char(input,&tempLsb);
  *out = (tempLsb + (tempMsb<< 8));

  return result1 && result2;
}

/*dc****************************************************/
result_t prnt_scanHex4Char(const char * input, uint16_t * out) {
  /*Expect 0-9,[a-f or A-F]
   *Convert to using strtol
  **/
  uint8_t tempLsb,tempMsb;
  result_t result1,result2;

  result1 = prnt_scanHex2Char(input,&tempMsb);
  input+=2;
  result2 = prnt_scanHex2Char(input,&tempLsb);
  *out = (tempLsb + (tempMsb<< 8));

  return result1 && result2;
}


void prntZeroAdj( uint16_t value) {
	if (value <10)Serial.print('0');
	Serial.print(value);
}

/*cf***************************************************/
void prnt_timeYYYMMDDHHMMSS() {
time_t timeNow_sec = now(); //long int
tmElements_t tm;
	breakTime(timeNow_sec, tm);
#define EPOCH_TIME_START 1970
	Serial.print(EPOCH_TIME_START+tm.Year);
	Serial.print('/');
	prntZeroAdj(tm.Month);
	Serial.print('/');
	prntZeroAdj(tm.Day);
	Serial.print(' ');

	prntZeroAdj(tm.Hour);
	Serial.print(':');
	prntZeroAdj(tm.Minute);
	Serial.print(':');
	prntZeroAdj(tm.Second);
}

/*cf***************************************************/
void prnt_timeSecs() {

// printUtils.time(w64Two_u32_u time)

//See icu\source\il8n\calendar.cpp
//  uint32_t TimeLow32Tbms;
  //uint32_t TimeSecsTsec; // Large range
  time_t TimeSecsTsec = now(); //long int
  //uint16_t Time_mSecTbms;//0-1023 binary mS
  //w64Two_u32_u time;

  //Serial.print("\n\r");
  Serial.print(TimeSecsTsec);
  //Serial.print(' ');
#if 0 //TODO
  tos_time_t tosTime=call Time.get();
#define JAVA_TIME
#ifndef JAVA_TIME
  struct {
    uint16_t days; //large, doesn't matter if overflows
    uint8_t hrs; //0-23 hours
    uint8_t mins; //0-59 minutes
    uint8_t secs; //0-59 seconds
  } Time24hr;
  uint32_t SecsLeftInDayTsec; //0-86,399secs
  uint16_t SecsLeftInHrTsec; //0-3599 secs
#endif

  //Expect that there is no binary seconds - add it in some way
  time.two_u32.l.w32 = (tosTime.low32 & cTimeMaskOff_bmSec ) | (((uint16_t)call Clock.readCounter())<<cTcnt2ShiftTo_bmSec);
  time.two_u32.m.w32 = tosTime.high32;
  Time_mSecTbms = time.two_u32.l.two_u16.l & TOS_TIME_MSEC_MASK;
  TimeSecsTsec  = time.w64 >> TOS_TIME_CONVRT_MSEC_SEC_SHIFT;

//  TimeLow32Tbms = call Time.getLow32();
//  TimeSecsTsec = TimeLow32Tbms >>TOS_TIME_CONVRT_MSEC_SEC_SHIFT; //Cutoff binary Tbms
//  Time_mSecTbms = (uint16_t)TimeLow32Tbms & TOS_TIME_MSEC_MASK;

  call PrintUtils.nlcr();
#ifndef JAVA_TIME
  //9 char hex time as
  //xxxxxxxx:
  //Print in hex as 19 char
  //day hrs min sec bmsecs
  //xx:hh:mm:ss:mmm:
#define cSecsInAMin 60
#define cSecsInAnHr cSecsInAMin*60
#define cSecsInAday cSecsInAnHr*24
  Time24hr.days = TimeSecsTsec/cSecsInAday;
  SecsLeftInDayTsec = (TimeSecsTsec - (uint32_t)Time24hr.days*cSecsInAday);
  Time24hr.hrs = SecsLeftInDayTsec/cSecsInAnHr;
  SecsLeftInHrTsec = SecsLeftInDayTsec - Time24hr.hrs*cSecsInAnHr;
  Time24hr.mins = SecsLeftInHrTsec/cSecsInAMin;
  Time24hr.secs = SecsLeftInHrTsec - Time24hr.mins*cSecsInAMin;
 //call PrintUtils.printHex(Time24hr.days); //print first two digits in Hex
 //call PrintUtils.printP(strColon);
 //call PrintUtils.printDec(Time24hr.hrs);
 //call PrintUtils.printP(strColon);
 //call PrintUtils.printDec(Time24hr.mins);
 //call PrintUtils.printP(strColon);
 //call PrintUtils.printDec(Time24hr.secs);
 //call PrintUtils.printP(strColon);
 //call PrintUtils.printDecword3(Time_mSecTbms);
#else
 //call PrintUtils.printDeclong(TimeSecsTsec);
 //call PrintUtils.printChar('.');
  //Print 3 Hex chars for 10 bits, giving a fraction of 1000mS
// //call PrintUtils.printHexNibble(Time_mSecTbms>>8);
  //  call PrintUtils.printHex(Time_mSecTbms);
  //Print 2 dec char for 8 bits giving 1/256 or 4mS
  //call PrintUtils.printDecword3(Time_mSecTbms);

#endif
 //call PrintUtils.printP(strColon);
#endif//00
}
/*cf***************************************************/
void prnt_TimeSecsF2() {
	Serial.println();
	prnt_timeSecs();
}
//End PrintUtils




