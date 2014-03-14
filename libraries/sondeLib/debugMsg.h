/* debugMsg.h
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


#ifndef DEBUGMSG_H_
#define DEBUGMSG_H_

//#include <stdint.h>
#include "mk20dx128.h"
#include "taskLists.h"
#include "debug.h"

#ifdef __cplusplus
extern "C" {
#endif

class sendDbg  {

	public:
	void _hex1(uint8_t n);
	void _hex2(uint8_t n);
	void _hex4(uint16_t n);
	void _hex8(uint32_t n);
	void _hexA(uint8_t n);
	void _hexA(uint16_t n);
	void _hexA(uint32_t n);
	void _u8u16(const prog_uchar * pStr,uint8_t uniqueId,const uint8_t outByte,const uint16_t outWord);
	void _u8u16u32(const prog_uchar * pStr,uint8_t uniqueId,const uint8_t outByte,const uint16_t outWord,const uint32_t outLong);
	void _u8u32(const prog_uchar * pStr,uint8_t uniqueId,const uint8_t outByte,const uint32_t outLong);

	void uint8( uint8_t uniqueId,cchar * pStr, uint8_t outByte);//replaces dbg_Uint8_ dbg._uint8_
	void uint8( uint8_t uniqueId,cchar * pStr, uint8_t outByte2, uint8_t outByte1);//replaces dbg_Uint8_2
	void uint8( uint8_t uniqueId,cchar * pStr, uint8_t outByte3,uint8_t outByte2, uint8_t outByte1);//replaces dbg_Uint8_3
	void uint8( uint8_t uniqueId,cchar * pStr, uint8_t outByte4,uint8_t outByte3,uint8_t outByte2, uint8_t outByte1);//replaces dbg_Uint8_4

//	void uint8(uint8_t offset, uint16_t mask, char * pStr,uint8_t uniqueId, uint8_t outByte,uint8_t outByte2);
//	void uint8(uint8_t offset, uint16_t mask, char * pStr,uint8_t uniqueId, uint8_t outByte,uint8_t outByte2,uint8_t outByte3);
	void _uint8_(const prog_uchar * pStr,uint8_t uniqueId,const uint8_t outByte);
	void _2uint8_(const char * pStr,uint8_t uniqueId,uint8_t param1,uint8_t param2);
	void _uint8_2(const char*, uint8_t, uByteWrd);
	void _uint8_3(char const*, unsigned char, w32_4u8_u);
	void _uint8_4(char const*, unsigned char, w32_4u8_u);
	//void sendDbg_uint8_4(char const*, unsigned char, w32_4u8_u);
	//void timeSecs();
	//void timeYYYMMDDHHMMSS();

	uint16_t debugMsgMasks[MASK_INDX_SIZE] = {
		//0xffff,0xffff,0xffff,0xffff,  0xffff,0xffff,0xffff,0xffff,
		  0x0081,0x0001,0x0001,0x0001,  0x0001,0x0001,0x0001,0x0001,//Error masks only eDbgAll_errEvt
	};

	private:
	//void dbgMsgHdr(const prog_uchar * pStr,uint8_t uniqueId);


};

class maintError  {
	public:
		result_t _init(uint8_t cmdAction);
		void _evt(unsigned char, unsigned char, unsigned long);
		void task_DieNow(void);

	private:
		void saveTimeEe(void);
};


#ifdef __cplusplus
}
#endif

#endif /* DEBUGMSG_H_ */
