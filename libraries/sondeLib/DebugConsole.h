/* DebugConsole.h
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
 *  Created on: Mar 10, 2014
 *      Author: Neil Hancock
 */

#ifndef _DEBUGCONSOLE_H_
#define _DEBUGCONSOLE_H_

#ifdef __cplusplus
extern "C" {
#endif

class DebugConsole  { /*dc***************************************************/
public:
	DebugConsole();
	~DebugConsole(){};

	void task_console_announce(); //Announce System - from init
    void task_console_input();   // USB polling function
	void task_console_user_cmd(); //Internal Interface
	void task_console_help();
	void task_console_prompt(); //Internal Output whenever needed
	result_t hibernateDebugFn(const char *pCmdBuf);
private:
	void helpModemType();

	void systemConfigDetails();
	result_t SerialInput_charEv(uint8_t data);
	//void SimpleQue_init();//TODO is elsewhere
	//int SimpleQueCntl_available();
	//void SimpleQue_putChar(uint8_t data);

};

#ifdef __cplusplus
}
#endif

class debugCmd_d { /*dc***************************************************/
public:
	result_t help();
	result_t cmd(const char *dataBufPtr);
private:
};

/*dc***************************************************/
#ifdef __debugConsole__
//DebugConsole dbgCon;
//debugCmd_d  dbg_d;
#else
extern DebugConsole dbgCnsl;
extern debugCmd_d  dbg_d;
#endif //__debugConsole__
#endif /* _DEBUGCONSOLE_H_ */
