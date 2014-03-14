/* DebugConsole.cpp
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

    Console Interpreter - provides infrastructure for a simple debug console interpreter.



*/
#include "WProgram.h"
#include "usb_serial.h"
#include "debugMsg.h"
#include "DebugConsole.h"
//#include "sondeTeensy3.h"
#include "TaskSched.h"
#include "HAL_Timer.h"
#include "Utils.h"
#include "sondeExtern.h"

/** Sonde console debug command parsing.
 * <p>Other debug commands may be added as part of a DebugCmd. </p>
 *
 *  @author Neil Hancock
 */
//#define DBG_DBGCONSOLE_TEST
//#define DEBUG_TEST

//TODO expand to include
//#include "loggerData.h"
//#include "HALEepromInternalDef.h"

#define debugMask_rd() debugMaskAccess(MASK_INDX_SystemMngXxx)
#define RXUIBUF_CMD_LEN 40
#define RXUIBUF_CMD_CHAR (RXUIBUF_CMD_LEN-1)

#define HELP_AZ_STRINGS

static cchar strPrompt[] = "\n\raz@";
static cchar strPromptTerm[] = "] ";
static cchar strInitPrompt[] = "\n\rTeensy Sonde Version: 0.1-";
static cchar strReason[] = " ResetReason:0x";
//static cchar strColon[] = ":";
//static cchar strSpace[] = " ";
static cchar strCmdErrorPre[] = "\n\rin: ";
static cchar strCmdErrorPost[] = ": !cmd";
//static cchar strCmdZzzz[] = " Zzz.. ";
static cchar strCrNl[] = "\n\r";
//static cchar strModType[] = " ModemType=0x";
static cchar strNode[] = "\n\rNode=0x";
static cchar strRequiresReset[] = " RequiresReset ";

#ifdef HELP_AZ_STRINGS
static cchar HelpStr1a[] =
  "\n\rHelp listing:"
  //adc
  ;
//static cchar HelpStr1[] =
//  "\n\rF b[5|1|2] r=57K 115K 230K"
//?  "\n\r D[HHHH] Dac" history

//  "\n\r f[i|t|m|w] Frame Id temp marketing read, marketing write" -
//  DS1825 one wire
//  ;

static cchar HelpStr2[] =
  //DebugProvM
  "\n\r"
  ;
#endif //HELP_AZ_STRINGS

  // User input buffer and copy for repeat '!' cmd
  char rxUiBufa[RXUIBUF_CMD_LEN];
  char rxUiBufCpy[RXUIBUF_CMD_LEN];

  char * rxBufpoint;
  //char * rxTestpoint;
  uint8_t countcmd;
  uint16_t countadd;
  uint32_t countmore;
  char *pTxTestAddr1,*pTxTestAddr2;
  uint16_t  TxTestAddr1, TxTestAddr2;
  uint16_t myNode; //This node
  uint8_t processSerialInput; //Sema - true if input should be processed
  uint16_t resetReasonHw; //TODO: move to startUp module to always be available

  enum eSisComms { //Serial Input State
    eSisCmds,//Process Debug Commands
    eSisRadio
        };
  uint8_t siState; //Serial Input State - eSisComms

/*dc***************************************************/
DebugConsole::DebugConsole() {
 rxBufpoint = rxUiBufa;
  *rxBufpoint = 0;
  siState = eSisCmds;

  //TODO  SimpleQue_init();

  countcmd = 0;
#ifdef DBG_DBGCONSOLE_TEST
  countadd = 0x4200;
  countmore = 0x42A04300;
#endif

}//DebugConsole()

#ifdef DEBUG_TEST //TODO delete
/*df***************************************************/
void wait(int period) {
  int i,y;
  for (y=0;y<period;y++) {
    for (i=0;i<1000;i++) {
      TOSH_wait();
    };
  }
}
#endif //wait()

/*dt***************************************************/
void DebugConsole::task_console_prompt() {
//Executed whenever current cmd is finished - can sometimes be output too soon...
#ifdef DEBUG_TEST
  uint8_t array[4];

  array[0] = 0xa0;
  array[1] = 0x0a;
  array[2] = 42;
  array[3] = 0x42;
  Serial.dumpHex(array, 4, "-.-");

#ifdef NOT_USED
  // This is a test section on how code access registers in hardware
  wait(20); //each wait 1000 TOSH_wait ~1char Tx
  TxTestAddr1 = (int)UDR0; //0x2C 44decimal
  pTxTestAddr1 = (char *)TxTestAddr1; //UART0 addr 44 decimal
  *pTxTestAddr1 = '!';  //33/ 0x21 - doesn't work, generates funn y code
  wait(3);
  TxTestAddr2 = (int)0x2c;
  pTxTestAddr2 = (char *)TxTestAddr2; //UART0 addr 44 decimal
  *pTxTestAddr2 = '*';  //35 0x23 - works - was hash but had to remove
  wait(4);
  outp('%',UDR0); //37 0x25 - works
  wait(5);
  outp('&',TxTestAddr1); //38 0x26 - invalid wrong outp addr
  wait(6);
  outp('(',TxTestAddr2); //40 0x28 - invalid wrong outp addr

  //    wait(5);
  //    *((char *)UDR0)='$'; //36 0x24
  //    outp('$',0x002c);
#endif //NOT_USED

#endif

  Serial.print(strPrompt);
  Serial.print(myNode);
  Serial.print('-');
  Serial.print(countcmd++);
  Serial.print(strPromptTerm);
} //task_console_prompt

#if 0
/*df***************************************************/
void DebugConsole::helpModemType() {
uint8_t modemType;
    Serial.print(F(strModType));
    Serial.print(modemType=PersStor.thisRfmdmId(),HEX);
    switch (modemType) {
    case eMT_MX_XbpS1: Serial.print(F(" XBP2.4G"));break;
    case eMT_MX_XTend: Serial.print(F(" XT900MHz"));break;
    case eMT_MX_XbpDm900: Serial.printP(PSTR(" XBP900MHz"));break;
    case eMT_MtCellular: Serial.printP(PSTR(" Cell-"));
         switch ( call PersStor.thisRfmdmIminorCp()) {
         case eMtm_MtGprs1: Serial.printP(PSTR("GPRS")); break;
         case eMtm_MtCdma1: Serial.printP(PSTR("CDMA")); break;
         default: Serial.printP(PSTR("UNDEF"));break;
         }
         break;
    //case eMT_MtCdma2000: Serial.printP(PSTR(" Cdma2000"));break;
    default:  Serial.printP(PSTR(" Unknown"));
  }
}//helpModemType
#endif //0

/*dt***************************************************/
//TODO temp
String hex(int a){
  String temp=String(a,HEX);
  if(temp.length()==1){
    temp="0"+temp;
  }
  return temp;
}
/*dt***************************************************/
void resetReason() {
	uint16_t mask=1;
	Serial.print(strReason);
	Serial.print(resetReasonHw,HEX);
	do {
		switch (mask & resetReasonHw){
		//RCM_SRS0
		case 0x0001: Serial.print(F(" wakeup")); break;
		case 0x0002: Serial.print(F(" LowVoltage"));  break;
		case 0x0004: Serial.print(F(" LossOfClock")); break;
		case 0x0008: Serial.print(F(" LossOfLock")); break;
		//case 0x0010 reserved
		case 0x0020: Serial.print(F(" wdog")); break;
		case 0x0040: Serial.print(F(" ExtResetPin")); break;
		case 0x0080: Serial.print(F(" PwrOn")); break;

		//RCM_SRS1
		case 0x0100: Serial.print(F(" JTAG")); break;
		case 0x0200: Serial.print(F(" CoreLockup")); break;
		case 0x0400: Serial.print(F(" SoftWare")); break;
		case 0x0800: Serial.print(F(" MDM_AP")); break;

		case 0x1000: Serial.print(F(" EZPT")); break;
		case 0x2000: Serial.print(F(" SACKERR")); break;
		//default:  break;
		}
	} while (mask <<= 1);
}
/*dt***************************************************/
void DebugConsole::task_console_announce() {//TASK_CONSOLE_ANNOUNCE_N
//Easy way of announcing system

	  // Announce system
	Serial.print(strInitPrompt);
	//TODO Serial.dateTimeUtc(&G_Ident.unix_time);
#define ANNOUNC_SYS
#ifdef ANNOUNC_SYS
/* should use something like this to rate-limit the printing
 *   if (QUE_PRINT_TRIGGER > call SimpleQueCntl.size()) {
 */
  // Announce system
  Serial.print("\n\r LoggerSet:");
  //Serial.printDecword(cFfpRecordsTotal);
  //TODO Serial.print(cFfpRecordsTotal);

    //K20
  resetReasonHw= RCM_SRS0;
  resetReasonHw |= (RCM_SRS1<<8);

  ifDebugHlp() {
    systemConfigDetails();
    resetReason();
  }

  Serial.print(strNode);
  //TODO myNode = call PersStor.myNode();
  myNode =0x1a;
  Serial.print(hex(myNode));
  //TODO helpModemType();
  Serial.print("\n\rTime : ");

  //dbg.timeTos(&sTime_mS,ePtfcDateTimeDecIntlStd); TODO?
  prnt_timeYYYMMDDHHMMSS();

  scheduler.postTask(TASK_CONSOLE_PROMPT_N);//TODO task_console_prompt();
#endif

} //task_console_announce

#if 0 //TODO delete or mv other subsystem inits
/*dc***************************************************/
//command result_t StdControl.start() {
DebugConsole::DebugConsole() {
  dbg(DBG_USR1, "Starting DebugConsole\n");
  call DebugCmdProv.init();
  call DebugCmdAdc.init();
  call DebugCmdErrors.init();
  call DebugCmdEepromInternal.init();
  call DebugCmdI2c.init();
  call DebugCmdMem.init();
  call DebugCmdSubSysMasks.init();
  call DebugCmdTestState.init();
#ifdef DEFDS1825
  call DebugCmdFrame.init();
#endif //DEFDS1825
  call DebugCmdFlash.init();
#ifdef ANNOUNC_SYS


#endif
  // post startupTask(); TODO - del auto
  return SUCCESS;
}
/*dc***************************************************/
command result_t StdControl.stop() {
  dbg(DBG_USR1, "Stopping\n");
  return SUCCESS;
}
#endif //0
/*dc***************************************************/
//TODO find in lib
int strcmp(const unsigned char * a, const unsigned char * b) {
  while (*a && *b && *a == *b) { ++a; ++b; };
  return *a - *b;
}

#ifdef DBG_DBGCONSOLE_TEST
  static cchar strCheckSonde[] = "[check@sonde / ";
  static cchar strFwdSlash[] = "/";

/*dt***************************************************/
  //TODO del task void check() {
void DebugConsole::task_check() {
  Serial.printP(strCheckSonde);
  Serial.printHex(countcmd++);
  Serial.printP(strFwdSlash);
  Serial.printHexword(countadd++);
  Serial.printP(strFwdSlash);
  Serial.printHexword(0x1122);
  Serial.printP(strFwdSlash);
  Serial.printHexword(0x3344);
  Serial.printP(strFwdSlash);
  Serial.printHexword(0xFFEE);
  Serial.printP(strFwdSlash);
  Serial.printHexlong(0xFFEEDDCC);
  Serial.printP(strFwdSlash);
  Serial.printHexlong(0x11223344);
  Serial.printP(strFwdSlash);
  Serial.printHexlong(countmore++);
  Serial.printP(strPromptTerm);
}

/*dt***************************************************/
void DebugConsole::task_controlInfo() {
	call DebugQueCntl.control();
}
  static cchar strFloodStrt[] = "\n\r **FLOOD**\n\r";
  static cchar strFloodEnd[] = "\n\r **FLOOD ENDED**\n\r";

/*dt***************************************************/
void DebugConsole::task_testFlood() {
  /** Test the output buffering.
  Still has a bug, as it only does up to about 5
  **/
  int i;
  Serial.printP(strFloodStrt);
  for (i = 0; i < 256; i++) {
    Serial.printHex(i);
  }
  Serial.printP(strFloodEnd);
}
#endif
/*dc***************************************************/
void DebugConsole::systemConfigDetails() {

    Serial.print(" TxBuf ");
    //TODO

    //size of ram - see Release/remMonPrj.map
    extern int _ebss;
    extern int _sbss;
    int ramUsed = (&_ebss-&_sbss);
    Serial.print(" RamUsed:");
    Serial.print(ramUsed);
    Serial.print('/');
    //For MK20DX256 0x64K or 65536, starting at 0x1FFF8000
#define RAM_SIZE 65536
    Serial.print(RAM_SIZE-ramUsed);

    Serial.print(" Eeprom:");
    //TODO Serial.print(cPeiEepromInternalSize);

    Serial.print("\n\r NumTimers =");
    Serial.print(NUM_TIMERS);


    Serial.print(" NumTasks=");
    Serial.print(TASK_LAST);

}//systemConfigDetails()

#ifdef HELP_AZ_STRINGS
/*df***************************************************/
void helpStrS() {
  Serial.print(
      "\n\r s[tart|top|ys|pwr[upd]] sys actions "
      );
} //helpStrS
/*dt***************************************************/
//TODO del task void helpTask() {
void DebugConsole::task_console_help() {//TASK_CONSOLE_HELP

  Serial.print(strInitPrompt);
  //TODO Serial.dateTimeUtc(&G_Ident.unix_time);
  //TODO helpModemType();
  Serial.print("\n\r");//nlcr();
  systemConfigDetails();
  resetReason();
  Serial.print("\n\rTime : ");

  Serial.print('[');
  //TODO  prnt_timeTos(&sTime_mS,ePtfcDateTimeDecIntlStd);
  prnt_timeSecs();
  Serial.print("] ");
  prnt_timeYYYMMDDHHMMSS();


  Serial.print(HelpStr1a);
  ifDebugHlp() { //The following not seen by customer
    //TODO call DebugCmdAdc.help(); //'a'
    Serial.print(" "
      //"\n\r br battery options provisioned"
      );

    // dbg_Calibration.help();//'c'
    dbg_d.help(); //'d'
    // dbg_Errors.help(); //'e'
    // dbg_EepromInternal.help();//'E'
    // dbg_Flash.help(); //'F'
    // dbg_I2c.help(); //'i'
    // dbg_Frame.help();

    Serial.print(
      "\n\r TODO hXXXX HibTime 0000-0xFFFFsec  =Listing .ExpireTimer"
      //"\n\r HIBER - hibernate, reset brings out, H[0|1] HibState"
      );
  }
//  Serial.print(
//    "\n\r logg [ahswdN] a-all,h-help,s-status, w-waiting to be uploaded,'n'-last  n*10, all"
//    );
  ifDebugHlp() {
#if 0 //TODO
    Serial.print(
      "\n\r lpbk[debug|radio|off] -status|debug<->radio|loops radioUart|frees loopback"
      "\n\r L[1|0] LEDs On/Off"
      );
#endif //TODO
  }

#if 0 //TODO
  ifDebugHlp() {
    dbg_Mem.help();
    dbg_Radio.help();
    dbg_Prov.help();
    //dbg_Pr.help();
    helpStrS();
    dbg_TestState.help();
  }
  dbg_Time.help(); //Split command - does its own ifDebugHlp
#endif //0

//#define OUTPUT_QUEUE_SPACE 400

//If the output queue is flooded with too much help output, figure out rate limit.
  //if (OUTPUT_QUEUE_SPACE > SimpleQueCntl_available()) {
  //  scheduler.postTask(TASK_HELP_STR2_N );//task_helpDebugStr2());
  //} else
  {
    Serial.print(HelpStr2);
#ifdef DBG_DBGCONSOLE_TEST
    Serial.print("check, cntl, flood");
#endif //DBG_DBGCONSOLE_TEST
    Serial.print(strCrNl);
  }

}//task_help_
#endif //HELP_AZ_STRINGS
#if 0 //TODO
/*df***************************************************/
result_t DebugConsole::dacInterface(const char *pCmdBuf) { //-recycle this interfaces
  /**
    input
      01234567
      DHHHH
      HHHH is 16 bit value to be written to Dac.
      **/
  uint16_t dacValue;
  uint8_t retResult;

  pCmdBuf++;
  if (SUCCESS == (retResult=prnt_scanHex4Char(pCmdBuf,&dacValue))) {
    //Serial.print(dacValue,HEX);
    //SensorsDacCond.set(0,dacValue);
  }
  return retResult;

} //dacInterface
/*df***************************************************/
result_t DebugConsole::batteryMngFn(const char *pCmdBuf) { //LiIon Battery
  /** Manage functions for the LiIon (FUT and External battery)
  input
  brp - rechargeable battery provision (to attempt recharge)
  brc - recharge r-bat from any external source when available on Scap
  **/
  uint8_t retResult=SUCCESS;

  pCmdBuf++;
  if (*pCmdBuf == 'r') {
    uint8_t *pRbatProvisioning = call DbgRbatProvisioning.rd();
    uint8_t rbatProvisioning = *pRbatProvisioning;
    Serial.print("\n\rRbatProv(Ee:0x");
    Serial.print(getAddrEeInt(batteryProvisioning_eBp),HEX);
    Serial.print((")=");
    Serial.print(rbatProvisioning,HEX);
    if (eBp_unprovisioned!=rbatProvisioning) {
      Serial.print(" Craft set");
      if (TRUE==call PersStor.isRbatProvisioned()) {
        Serial.print(" Provisioned");
      }else if (TRUE==call PersStor.isRbatUnProvisioned()) {
        Serial.print(" UnProvisioned");
      }
      if (call PersStor.isRbatCharging()) {
        Serial.print(" Charging");
      }
    }//rbatProvisioning

  } else {
    Serial.printP("\n\rDon't understand ");
    Serial.print((char)(*pCmdBuf));
  }
  return retResult;
}//batteryMngM
#endif //0
/*df***************************************************/
result_t DebugConsole::hibernateDebugFn(const char *pCmdBuf) { //-recycle this interfaces
  /**
    input
      01234567
      Hxxx
      xxxx is 16 bit value to be written to hibernate fn.
      **/
  uint8_t retResult=SUCCESS;
#if 0
  uint16_t hibValue;

  pCmdBuf++;
  if (*pCmdBuf == '=') {
    myHPLClock_t *pHPLClock;
    hibValue = HibernCtl.getTime();
    {tPeriodicCycle periodicCycle = call PersStor.periodicCycleData();
      if (periodicCycle.isTransferMultGateway_bool) {
        Serial.print(F("GatN:smpl1="));
      } else {
        Serial.print(F("Gat1:smplN="));
      }
      Serial.print(hex(periodicCycle.Max_Cnt));
      //Need to get DataMng:periodicCycle_Cntr
      Serial.print(F(" curCnt=?"));
    }
    Serial.print(F(" hiberTime="));
    Serial.print(call PersStor.hibernationTimeS(),HEX);
    Serial.print(F(" hiberVal="));
    Serial.print(hibValue,HEX);
    pHPLClock = (myHPLClock_t *) call HPLClockStatus.state(0);
    Serial.print(' ');
    Serial.print(pHPLClock->hibmCntDwnT_sec,HEX);
    Serial.print('-');
    Serial.print(pHPLClock->hibCntDwnT_sec,HEX);
  } else if (*pCmdBuf == '.') {
    call ClockAlternateExpire.doit(0);
  } else if (SUCCESS == (retResult=prnt_scanHex4Char(pCmdBuf,&hibValue))) {
    call HibernCtl.setTime(hibValue,eMiDebug2);
    call ClockAlternateExpire.doit(0);
  }
#endif //0
  return retResult;

} //hiberanteDebugFn


/*dt***************************************************/
void DebugConsole::task_console_user_cmd()
{
  uint16_t cnt;
  //decode input except for ?
  result_t result;
  result = SUCCESS;

  //Expect cmd string in rxUiBuf[0] -> (rxBufpoint-1)

  //TODO is atomic needed?
  { //looooong atomic
    if ('!' == rxUiBufa[0])
    {
		// Repeat character, copy buffer and re-execute
		for (cnt = 0; cnt <RXUIBUF_CMD_LEN; cnt++) {
			rxUiBufa[cnt] = rxUiBufCpy[cnt];
			//note rxBufPoint is invalid now, but it is reset later.
		}
    } else {
        for (cnt = 0; cnt <RXUIBUF_CMD_LEN; cnt++) {
          rxUiBufCpy[cnt] = rxUiBufa[cnt];
        }
    }

#ifdef DBG_DBGCONSOLE_TEST
    if (0 == strcmp("flood", rxUiBufa)) {
      post testFlood();
    } else if (0 == strcmp("cntl", rxUiBufa)) {
      post controlInfo();
    } else if (0 == strcmp("check", rxUiBufa)) {
      post check();
    }
#endif

    switch (rxUiBufa[0])
    {
#ifdef HELP_AZ_STRINGS
    case '?':
      scheduler.postTask(TASK_CONSOLE_HELP_N); //TODO del: helpTask();
      break;
#endif //HELP_AZ_STRINGS
    case 'a':
      //result = call DebugCmdAdc.debugFn((const char *)rxUiBuf);
      break;

    case 'b':
      //result = batteryMngFn((const char *)rxUiBuf);
      break;
    case 'c':
      //result = call DebugCmdCalibration.debugFn((const char *)rxUiBuf);
      break;
    case 'd':
      //result = call DebugCmdSubSysMasks.debugFn((const char *)rxUiBuf);
    	result = dbg_d.cmd((const char *)rxUiBufa);
      break;

//    case 'D':
//      result = dacInterface((const char *)rxUiBuf);
//      break;
    case 'e':
      //result = call DebugCmdErrors.debugFn((const char *)rxUiBuf);
      break;

    case 'E':
      //result = call DebugCmdEepromInternal.debugFn((const char *)rxUiBuf);
      break;

#ifdef DEFDS1825
          case 'f':
      result = call DebugCmdFrame.debugFn((const char *)rxUiBufa);
      break;
#endif //DEFDS1825
    case 'F':
      //result = call DebugCmdFlash.debugFn((const char *)rxUiBuf);
      break;
    case 'i':  //i cmd
      //result = call DebugCmdI2c.debugFn((const char *)rxUiBuf);
      break;

    case 'h':
      //result = hibernateDebugFn((const char *)rxUiBuf);
      break;
    case 'H': //HibState
    {
		  result = SUCCESS;
		  switch (rxUiBufa[1])
		  {
		  case '0'://Hibernation - turn off console interface
			//call HibernCtl.HibernConsoleOff_srcConsole();
			break;
		  case '1': //Hibernateion - enable console
			//call HibernCtl.HibernConsoleEnable_srcConsole();
			break;
		  case 'I': //HIBERnate
			if ('B' == rxUiBufa[2] && 'E'==rxUiBufa[3]&& 'R'==rxUiBufa[4]) {
			  //call HibernCtl.HibernForever(0);
			}
			break;
		  default:
			Serial.print('=');
			//Serial.print(hex(TOSH_hibState()));
			Serial.print(':');
			//Serial.printHexword(((tHibernateReport *)call HibernateStatus.state(0))->hibState);
			break;
		  }
    }
    break;
    case 'l': //logger or lpbk
		  if ('o' == rxUiBufa[1] && 'g'==rxUiBufa[2] && 'g'==rxUiBufa[3]) {
			//result = call UserCmdLogger.debugFn((const char *)rxUiBuf);
			break;
		  }
		  ifDebugHlp()
		  {
			  if ('p' == rxUiBufa[1] && 'b'==rxUiBufa[2] && 'k'==rxUiBufa[3])
			  {
				  //Parsed lpbk - now check one of three options
				  switch (rxUiBufa[4])
				  {
				  case 'd'://lpbkdbg-Radio- reset pulls out of this
					  //call SystemMngDebug.doit(eMssNoAction);//put in benign state
					   //turn on radio - assumes a radio that will allow +++ for AT cmd mode
					  //call HALPwrRadio.enable((eHprBoost|eHprRadio),eMiDebug);
					  //result = call LoopBackRadio.enable(1,eMiDebug);
					  //lpbkdbg-Radio
					  break;
				  case 'r'://lpbkradio
					  //call SystemMngDebug.doit(eMssNoAction);//put in benign state
					  //call HALPwrRadio.enable((eHprBoost|eHprRadio),eMiDebug); //turn on radio
					  //call HILRechrgBatMng.rbatPwrOut(TRUE,eMiDebug);
					  //result = call LoopBackRadio.enable(0,eMiDebug);
					  break;
				  case 'o'://lpbkoff
					  //result = call LoopBackRadio.disable(0,eMiDebug);
					  Serial.print(strRequiresReset);
					  break;
				  default: //lpbk status
					  Serial.print('=');
					  //Serial.print(hex(call LoopBackRadio.status(0,eMiDebug));)
					  break;
				  }//switch
			  }
		  }//ifDebugHlp()
		  break;
    case 'L': //TODO Leds manage command '1'= display, !1 = disable Display
		  ifDebugHlp()
		  {
			switch (rxUiBufa[1]) {
			//case '1': result = call LedsMngLocal.enable(0); break;
			//case '0': result = call LedsMngLocal.disable(0); break;
			default:
			  Serial.print(" UI=");
			  //Serial.print(call LedsMngUi.status(0));
			  Serial.print(" BRD=");
			  //Serial.printHex(call LedsMngLocal.status(0));
			  break;
			}
		  }
		  break;
    case 'M':
      //result = call DebugCmdMem.debugFn((const char *)rxUiBuf);
      break;
    case 'p':
      //result = call DebugCmdProv.debugFn((const char *)rxUiBuf);
      break;
    case 'R':
      //result = call DebugCmdRadio.debugFn((const char *)rxUiBuf);
      break;
    case 'r':
		  if ('e' == rxUiBufa[1] && 's'==rxUiBufa[2]&& 'e'==rxUiBufa[3]&& 't'==rxUiBufa[4])
		  {
			// **** DO RESET ***
			  MaintErr._evt('A',eDbgConsole,0);

		  }
		  break;
    case 's':
		  if ('t' == rxUiBufa[1] && 'a'==rxUiBufa[2]&& 'r'==rxUiBufa[3]&& 't'==rxUiBufa[4])
		  {
			//call PersStor.wtrStartProv();
		  } else if ('t' == rxUiBufa[1] && 'o'==rxUiBufa[2]&& 'p'==rxUiBufa[3])
		  {
			//call PersStor.wtrStopProv();
		  } else if ('p' == rxUiBufa[1] && 'w'==rxUiBufa[2]&& 'r'==rxUiBufa[3])
		  {
			if ('u' == rxUiBufa[4] && 'p'==rxUiBufa[5] && 'd'==rxUiBufa[6])
			{
			  //call PwrMngModDbg.update(); //update the status
			} else {
			  //call PwrMngModDbg.prnt(); //Get status
			}
		  } else if ('c' == rxUiBufa[1] && 'l'==rxUiBufa[2]&& 'r'==rxUiBufa[3])
		  {
			  //call RadioStatsDbg.clrStats();
		  } else if ('y' == rxUiBufa[1] && 's'==rxUiBufa[2])
		  {
	#if 0
			  Dsm_msgTxStats *pMsgTxState;
			  //call PwrMngModDbg.prnt(); //Get status
			  pMsgTxState = (Dsm_msgTxStats *) call RadioStatsDbg.rd();
			  Serial.print("\n\r GtwStats Attempts="));
			  Serial.print(hex(pMsgTxState->GatewayFindReq));
			  Serial.print(" noGsm=");
			  Serial.print(hex(pMsgTxState->GatewayNoGsmFound));
			  Serial.print(" noIp="));
			  Serial.print(hex(pMsgTxState->GatewayNoIpFound));
			  Serial.print(" errTimeout="));
			  Serial.print(hex(pMsgTxState->GatewayErrTimeout));
			  Serial.print(" con=");
			  Serial.print(hex(pMsgTxState->GatewayConnection));
			  Serial.print("\n\r TxMsgStats 1st=");
			  Serial.print(pMsgTxState->Window1st);
			  Serial.print(" 2nd=");
			  Serial.print(hex(pMsgTxState->Window2nd));
	#endif //0
			  Serial.print("\n\r timer1 fin/start \n\r");
			  //TODOSerial.timeTos(&pMsgTxState->timer1Finish,ePtfcDateTimeDecIntlStd);
			  Serial.print(" / ");
			  //TODO Serial.timeTos(&pMsgTxState->timer1Start,ePtfcDateTimeDecIntlStd);
	 //old - del?
	//          Serial.nlcr();
	//          Serial.timeTos(&pMsgTxState->timer1Finish,ePtfcSecondsDecF);
	//          Serial.printChar('/');
	//          Serial.timeTos(&pMsgTxState->timer1Start,ePtfcSecondsDecF);

		  } else {
			helpStrS();
		  }
		  break;
    case 'T': //current time
      //result = call DebugCmdUserTime.debugFn((const char *)rxUiBuf);
      break;
    case 't':
		  ifDebugHlp() {
			//result = call DebugCmdTestState.debugFn((const char *)rxUiBuf);
		  }
		  break;
    case 'u':
      //result = call DebugCmdUser.debugFn((const char *)rxUiBuf);
      break;
    case '#': break;//Comment - ignore
    default:
        /* If enter was pressed, "handle" command */
        result = FAIL;
        break;
    }//switch (rxUiBufa[0])

    if (FAIL == result)
    {
      Serial.print(strCmdErrorPre);
      Serial.print(rxUiBufa);
      Serial.print(strCmdErrorPost);
    }

    rxBufpoint = rxUiBufa;
    *rxBufpoint = 0;

    if (SUCCESS_DBG_NO_PROMPT != result) {
      scheduler.postTask(TASK_CONSOLE_PROMPT_N);//TODO del: was prompt();
    }
  }

}//processUserCmd()

//TODO ?? Serial.read() poll USB
/*de***************************************************/
result_t DebugConsole::SerialInput_charEv(uint8_t data) {
  // This is derived from an interrupt an should be as minimilast
  //   as possible

  if (data == '\r') //Token for completed cmd - don't expect 'n' to be part of terminal <return>
  {
	  scheduler.postTask(TASK_CONSOLE_USER_CMD_N);//TODO del: was processUserCmd();
#define cAsciiCntlC    0x03
  } else if (data == cAsciiCntlC)
  { //Abort
    rxUiBufa[0]=data;
    rxBufpoint = &rxUiBufa[1];
    *rxBufpoint = 0;
    scheduler.postTask(TASK_CONSOLE_USER_CMD_N);// task_process_user_cmd();
  } else
  {
		// Echo data
		 //TODO SimpleQue_putChar(data);//call SimpleQue.putChar(data);
		  Serial.print((char)data); //Send back to user
		*rxBufpoint = data;

		//next position or cap it if too many chars
		if ((++rxBufpoint - rxUiBufa) >= RXUIBUF_CMD_CHAR) {
		  rxBufpoint = rxUiBufa + RXUIBUF_CMD_CHAR-1;
		}
		*rxBufpoint = 0;
  }
  return SUCCESS;
} //charEv

/*df***************************************************/
void DebugConsole::task_console_input() {
//There are potentially a number of sources of input - USB, Bluetooth ..
//Only one supported  USB - and it needs to be polled
	uint8_t rxChar;
	unsigned int localCount=0;
	unsigned int rxCount;


	while (0 !=(rxCount = Serial.available())) //Returns count, loop till
	{
		if (rxCount > 1000)
		{
			dbgUint8_1(eDbgAll_errEvt,'A',"DebugConsole Err rxCount exceeded ",rxCount);
			break;
		}

		rxChar = Serial.read();

		SerialInput_charEv(rxChar);
		if (localCount++ >1000)
		{
			// abort as seems like gone into an infinite loop.
			dbgUint8_1(eDbgAll_errEvt,'B',"DebugConsole Err rxChar exceeded ",localCount);
			break;
		}
	}
}//task_console_input()

//End DebugConsole



//Begin debugCmd_d *******************

#if 0
static const prog_uchar strInternalBufSizeIssue[] = " InternalBufSizeIssue ";
static const prog_uchar strDebugMasks[] = " debugMasks";
// static const prog_uchar strSensorsMng[] = " sensorsMng";
static const prog_uchar strNlCr[] = "\n\r ";
static const prog_uchar strFail[] = " DebugSubSysMasks_EeRd_Fail ";
static const prog_uchar strFailEepromIntrnl[] = "\n\r **FailEepromIntrnl";
#endif //0
/*df***************************************************/
result_t debugCmd_d::cmd(const char *dataBufPtr)//TODO partial implemented
/** 'debug' Cmd processing.
  input
    01234567
    d  - print out state all
    dIAA - set to value
     I = set to MASK_INDX_ value
        'a' - set all to value
        'd' -  DataMngM
        'h' - adc
        'm' - SensorsMngM
        'r' - RouteInM
        'x' - Radio, transmit
    dXAAw - set indx X (0-F) to AA
    'W' - write to EEPROM
    'R' - read from EEPROM
    'S' - startup
Fut  'T[p|e]' setup for pressure and electrical conductivity
    'D' - default
**/
{
	uint8_t  vvalue,mskCntr; // mskIdx;
	const char * pNxtChar;
	uint8_t primSwChar;
	result_t retResult = SUCCESS;
	uint8_t maskIndx = MASK_INDX_NOP;

	primSwChar = *(dataBufPtr+1); //Main Selector character
	pNxtChar = dataBufPtr+2;//&rxUiBuf[2] - the hex numbers
	switch (primSwChar) { //Look for primary selector
	  //case 'A': and a few other used below
	//TODO  case 'd': optMasks(dataBufPtr); break;
	 //TODO case 'R': retResult = parserReadIntEeprom(dataBufPtr); break;
	 //TODO case 'W': retResult = writeIntEeprom(); break;
	  default: //not present so look for two hex characters
		if (SUCCESS == (retResult =  prnt_scanHex2Char(pNxtChar,&vvalue))) {
		  // found two hex numbers, now look for selector
		  // assumed retResult = SUCCESS;
		  switch (primSwChar) { //set all to read values
		  case 'A':
			for (mskCntr = 0; mskCntr<MASK_INDX_SIZE;mskCntr++) {
			  dbg.debugMsgMasks[mskCntr] = vvalue;
			}
			maskIndx = MASK_INDX_NOP;
			break;
		  case 'a': maskIndx = MASK_INDX_SensorsAdcHwM; break;
		  case 'd': maskIndx = MASK_INDX_DataMngM; break;
		  case 'h': maskIndx = MASK_INDX_HALHwStateSondeM; break;
		  case 'm': maskIndx = MASK_INDX_SensorsMngM; break;
		  //case 'R': - already handled above
		  case 'r': maskIndx = MASK_INDX_RadioSend; break;
		  //case 'W': - already handled above
		  case 'x': maskIndx = MASK_INDX_RouteInM; break;
		  default:
			//check for valid offsets 0-F
			retResult = prnt_scanHex1Char((dataBufPtr+1),&maskIndx);

		  }
		}
		if ((MASK_INDX_NOP != maskIndx) && (SUCCESS == retResult)) {
		  dbg.debugMsgMasks[maskIndx] = vvalue;
		}
		break;
	}
	//list all debug variables
	Serial.println(" debugMasks"); //Header
	for (mskCntr = 0; mskCntr<MASK_INDX_SIZE;mskCntr++) {
	  Serial.print(' ');
	  Serial.print(hex(mskCntr));
	}
	Serial.println(' ');
	for (mskCntr = 0; mskCntr<MASK_INDX_SIZE;mskCntr++) {
	  Serial.print(' ');
	  Serial.print(hex(dbg.debugMsgMasks[mskCntr]));
	}
return retResult;
}


/*df***************************************************/
result_t debugCmd_d::help()
{
	Serial.print(
	  "\n\r d[X[00-FF]][d W R T S D] debug mask X=A-all a-adcHw d-DataMng h-hwState m-Mng r-Rad x-Route or indx 0-F"
	  );
	  return SUCCESS;
}
//End other




