/* debug.h
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
#ifndef DEBUG_H_
#define DEBUG_H_

// Used elsewhere but seems to be accessblie from C++ here
//#ifdef __cplusplus
// This ugly hack allows us to define C++ overloaded functions, when included
// from within an extern "C", as newlib's sys/stat.h does.
//extern "C++" {


enum eMiModuleId {//Unique identifier for modules
  eMiBegin=0,
  eMiSensorsDataMngM = 0x01,
  eMiDataStoreMngM=0x02,
  eMiSensorsMngM1 =0x03,
  eMiSensorsMngM2 =0x04,
  eMiSensorsMngM3 =0x05,
  eMiSensorsMngM4 =0x06,
  eMiSensorsAdcHwM=0x07,
  eMiSensorsMngPrsrM = 0x08,
  eMiSensorsMngCondM = 0x09,
  eMiDataMngM1 = 0x0A,
  eMiDataMngM2 = 0x0B,
  eMiDataMngM3 = 0x0C,
  eMiSystemMngStartupM = 0x10,
  eMiSystemMngM = 0x11,
  eMiSystemMngM2= 0x12,
  eMiRadioSendM=0x13,
  eMiRadioSendM2=0x14,
  eMiRadioSendMsleep=eMiRadioSendM2,
  eMiRadioXBeeM1=0x15,
  eMiRadioXTendM1=0x16,
  eMiRadioXbDmM1=0x18, //XB DigiMesh
  eMiPwrMngM1=0x18,
  eMiPwrMngM2=0x19,
  eMiRmApiMngM1=0x1A,
  eMiRmApiMngM2=0x1B,
  eMiFramerM1=0x1C,
  eMiFramerM2=0x1D,
  eMiRadioMtCellM1=0x1E, //MT Cell1
  eMiRadioMtCellM2=0x1F, //MT Cell2 in case used

  eMiHplAdcMcp3421M1=0x020,

  eMiSensorsSelfTestM= 0x30,
  eMiSensorsCalibrateM=0x31,
  eMiHALHwStateSondeM=0x38,
  eMiDebug,
  eMiDebug2,
  eMiDebugAdcM,
  eMiDebugI2cM,

  eMiHibernateM1=0x40,
  eMiTimerM1=0x41,
  eMiTimerM2=0x42,
  eMiTimerM3=0x43,
  eMiHPLClockM1=0x44,
  eMiHPLClockM2=0x45,
  eMiHPLClockM3=0x46,
  eMiLoggerDataM1=0x47,

  eMiDebugMyIdAvailable=0xff,
  eMiEnd
} ;

enum eLocalMyId {
  eLmi_NotInUse=0xff
};
enum eLocalLedState {
 // On Ui board this is the amber led
  eLlsRedNoChange=0x00,
  eLlsRedOff     =0x01,
  eLlsRedFlshFast=0x02,
  eLlsRedFlshSlow=0x03,
  eLlsRedOnSolid =0x04,
  eLlsRedFlshOnce=0x05,
  eLlsRedFlsh2F  =0x06,
  eLlsRedFlsh3F  =0x07,
  eLisRedMask    =0x0F,

  /* On the UI board this is the red led
   */
  eLlsGreenNoChange=0x00,
  eLlsGreenOff     =0x10,
  eLlsGreenFlshFast=0x20,
  eLlsGreenFlshSlow=0x30,
  eLlsGreenOnSolid =0x40,
  eLlsGreenFlshOnce=0x50,
  eLlsGreenFlsh2F  =0x60,
  eLlsGreenFlsh3F  =0x70,
  //
  eLisGreenShift = 0x04,
  eLisGreenMask    =0xF0,
  eLlsEnd
};

enum eMaskIndx {
  MASK_INDX_SystemMngXxx, //0 SystemMng SystemMngStartup
  MASK_INDX_DataMngM,     //1
  MASK_INDX_SensorsMngM,  //2 SensorsMngM
  MASK_INDX_SensorsMngXxx,//3 SensorsMngPrsrM SensorsMngCondM
  MASK_INDX_SensorsAdcHwM,//4 RadioSendM
  MASK_INDX_RadioLinkLayer,//5 RadioLinkLayer
  MASK_INDX_RadioSend,     //6 RadioSendM
  MASK_INDX_RouteInM,      //7 routineInM
  MASK_INDX_HALHwStateSondeM,//8
  MASK_INDX_FlashSerial,  //9
//  MASK_INDX_HALTwiPerphlM,
  MASK_INDX_SIZE,
  //The size of this is also used in sonde_gen.h
  MASK_INDX_NOP=0xff,
  MASK_INDX_DEF_VALUE=0xff //usually all off 0x00 or all on 0xff
};

#define debugMaskAccess(MASK_INDX) dbg.debugMsgMasks[MASK_INDX]

enum  eDbgEnums { //mask
//Guideline: module outputs should be 0x01, startup 0x80
// see eMaskIndx/DebugSubSysMasksM for offsets
  eDbgAll_errEvt =0x01,
  eDbgAll_AllEvts =0xffff,
  //0 SystemMngXxx HPLClock TimerM MASK_INDX_SystemMngXxx
  eDbgSysMngXxx_Startup=0x02,
  eDbgSysMngXxx_startUpEvt=0x02,
  eDbgSysMngXxx_Evt=0x04,
  eDbgHalClock_Evt  =0x08,

  eDbgSysMngXxx_PwrMin=0x10,
  eDbgHplClock_Evt  =0x20,
  eDbgHplWatchdog   =0x20,
  eDbgPerStorXxx_Evt=0x40,
  eDbgConsoleAccess =0x80,

  //1 SensorsDataMngM MASK_INDX_DataMngM
  eDbgDataMng_Startup=0x02,
  eDbgDataMng_sdmsAction =0x04,
  eDbgDataMng_Fifo=0x08,

  eDbgDataStoreMng_FlyTime = 0x10,
  eDbgDataStoreMng_Time = 0x20,
  eDbgDataStoreMng_Msg = 0x40,
  //DataStoreM
  //eDbgSnsrDataStore_Output=0x01,
  //2 SensorsMngM
  eDbgSnsrMng_Io=0x02,
  eDbgSnsrMng_DebugEvt =0x04,
  eDbgSnsrMng_StateChng=0x08,
  eDbgSnsrMng_Output   =0x10,
  eDbgSnsrMng_Results  =0x20,
  eDbgSnsrMng_FilterOut = 0x40,
  eDbgSnsrMng_FilterDis = 0x080, //Turn ON filtering
  //3 SensorsMngPrsrM SensorsMngCondM
  eDbgSnrsMng_Prsr = 0x02,
  eDbgSnrsMng_PrsrX10 = 0x04, //Only use X1 data - changes processing
  eDbgSnrsMng_PrsrCalbr = 0x08,// Calibration data

  eDbgSnrsMng_CondOut   =0x10,
  eDbgSnrsMng_CondIo    =0x20,
  eDbgSnrsMng_CondDetail=0x40,
  eDbgSnrsMng_CondCalbr =0x80,
  //4 'a'SensorsAdcHwM
  eDbgSnsrAdcHw_cmd = 0x02,
  eDbgSnsrAdcHw_Evt = 0x04,
  eDbgSnsrAdcHw_Output=0x08,

  eDbgSnsrAdcHw_Twi = 0x10,
  eDbgSnsrAdcHw_Mcp = 0x20,
  eDbgSnsrTwi_input =0x40,
  eDbgSnsrTwi_start =0x80,
  //5 RadioLL RadioLink Layer
  eDbgRadio_FramerMng=0x02, //very dangerous - from int
  eDbgRadio_FramerMngTx=0x04, //very dangerous - from int
  eDbgRadio_FramerTxOctet=0x04, //very dangerous - from int
  eDbgRadio_FramerRxOctet=0x08, //very dangerous - from int

  eDbgRadio_RmApiMng=0x10, //Packet info from background
  eDbgRadio_RmApiTxOctet=0x20, //very dangerous - from int
  eDbgRadio_RmApiRxOctet=0x40, //very dangerous - from int
  eDbgRadio_RmApiDiscard=0x80, // unprocessed messages
  //6 'r'RadioSendM & RadioXBeeM
  eDbgRadio_Cntl=0x02,
  eDbgRadio_Send=0x04,
  eDbgRadio_ATrsp=0x08,
  //eDbgRadio_ATrspND=0x10, ??

  //7 RouteIn (Rx&Tx)
  eDbgRouteIn_RxAll=0x02, //RxAll messages
  eDbgRouteIn_RxRadio=0x04,
  eDbgRouteIn_TxAll=0x08, //TxAll messages
  eDbgRouteIn_DefaultEvent=0x10,
  //8 HALHwStateSonde
  eDbgHalPwr=0x02,
  eDbgHalPwrRadio=0x04,
  eDbgHalPwrAnlgMaster=0x08,
  eDbgHalPwrAnlg=0x10,
  eDbgHalPwrRecBat=0x20,
  eDbgHalMemPort=0x40,
  eDbgPwrMng=0x80,
  //9 FLASH EEPROMPage
  eDbgFlsh_Mng   =0x02, //Manage
  eDbgFlsh_StorIo=0x04, //Input/Output
  eDbgFlsh_IoInit=0x08, //Entry points
  eDbgFlsh_req=0x10,
  eDbgFlsh_execCmd=0x20,
  eDbgFlsh_done   =0x40,
  eDbgFlsh_bytes  =0x80, //!! careful
  //***
  eDbgConsole,
  eDbgEnumsEnd,
  //eDbg_DEBUG_CONSOLE - in files as doesn't work here
};

enum { //result_t continued from tos.h
    SUCCESS_DBG_NO_PROMPT=0x10
    };
//stubs
#define prog_uchar char
#define cchar char

int debugMask_rd(void);

void sendDbg_uint8_2(const char *strIn,uint8_t uniqueId, uByteWrd w16);
void sendDbg_uint8_3(const char *strIn,uint8_t uniqueId, w32_4u8_u w16);
void sendDbg_uint8_4(const char *strIn,uint8_t uniqueId, w32_4u8_u w16);
void MaintErr_evt(uint8_t uniqueId, uint8_t modId, uint32_t clientData);

#define eDbg_DEBUG_CONSOLE
/*
#define dbgSensorsReadings(mask,uniqueId,strIn, ptr) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.sensorReadings(strIn,uniqueId,ptr)
#define dbgSensorsLogAdc(mask,uniqueId,strIn, ptr) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.sensorsLogAdc(strIn,uniqueId,ptr)
* /
#define sendDbgWord32(mask,uniqueId,strIn, outbits) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.word32(strIn,uniqueId,(uint32_t)outbits)
*/
#define ifDebugHlp() if (eDbgConsoleAccess & debugMask_rd())
#define dbgEol(mask) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.oEol();
#define dbgBySp(mask,data) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.oByteSpace(data)
#define dbgChr(mask,data1) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.oChar(data1)
#define dbgChrByt(mask,data1,data2) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.oChrByt(data1,data2)
#define dbgChrWord(mask,data1,data2) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.oChrWord(data1,data2)

#define dbgStr(mask,uniqueId,strIn)\
    if (mask & debugMask_rd()) \
      //signal sendDbg.str1(strIn,uniqueId)
#define dbgStrD(mask,uniqueId,strInP,strInD)\
    if (mask & debugMask_rd())\
      //signal sendDbg.strD(strInP,uniqueId,(const char *)strInD)

#define dbgWord_2(mask,uniqueId,strIn, out16bitsB,out16bitsA) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.word_2(strIn,uniqueId,(uint16_t)out16bitsB,(uint16_t)out16bitsA)
#define dbgWord_3(mask,uniqueId,strIn, out16bitsC,out16bitsB,out16bitsA) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.word_3(strIn,uniqueId,(uint16_t)out16bitsC,(uint16_t)out16bitsB,(uint16_t)out16bitsA)
#define dbgWord(mask,uniqueId,strIn, out16bits) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.word(strIn,uniqueId,(uint16_t)out16bits)
#define dbgWordG(dbgGate,mask,uniqueId,strIn, out16bits) \
    if (dbgGate && (mask & debugMask_rd())) \
      //signal sendDbg.word(strIn,uniqueId,(uint16_t)out16bits)
#define dbgWord32(mask,uniqueId,strIn, out32bits) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.word32(strIn,uniqueId,(uint32_t)out32bits)
#define dbgWord64(mask,uniqueId,strIn, out64bits) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.word64(strIn,uniqueId,(uint64_t)out64bits)
#define dbg_u8u16(mask,uniqueId,strIn, prm2,prm1) \
    if (mask & debugMask_rd())\
      //signal sendDbg.u8u16(strIn,uniqueId,prm2,prm1);

#define dbg_u8u16u32(mask,uniqueId,strIn, prm3,prm2,prm1) \
    if (mask & debugMask_rd())\
      //signal sendDbg.u8u16u32(strIn,uniqueId,prm3,prm2,prm1);

#define dbg_u8u32(mask,uniqueId,strIn, prm2,prm1) \
    if (mask & debugMask_rd())\
      //signal sendDbg.u8u32(strIn,uniqueId,prm2,prm1);

#if 1 // TODO needs redoing to use uint8
#define dbgUint8_1(mask,uniqueId,strIn, prm1) \
    if (mask & debugMask_rd()) \
    dbg.uint8(uniqueId,(char *)strIn, prm1);

#define dbg_Uint8_G(dbgGate,mask,uniqueId,strIn, prm1) \
    if (dbgGate && (mask & debugMask_rd())) \
    dbg.uint8(uniqueId,strIn, prm1);

#define dbgUint8_2(mask,uniqueId,strIn, prm2,prm1) \
    if (mask & debugMask_rd()){\
      dbg.uint8(uniqueId,(char *)strIn,prm2,prm1);\
    }

#define dbgUint8_3(mask,uniqueId,strIn, prm3,prm2,prm1) \
    if (mask & debugMask_rd()){\
      dbg.uint8(uniqueId,(char *)strIn,prm3,prm2,prm1);\
    }
//sendDbg_uint8_3(strIn,uniqueId,w32);
#define dbgUint8_4(mask,uniqueId,strIn, prm4,prm3,prm2,prm1) \
    if (mask & debugMask_rd()){\
      dbg._uint8(strIn,uniqueId,prm4,prm3,prm2,prm1);\
    }
#endif
#define dbg_Uint16_2(mask,uniqueId,strIn, prm2,prm1) \
    if (mask & debugMask_rd()){\
      w32Two_u16_u w32; \
      w32.two_u16.l = prm1;\
      w32.two_u16.m = prm2;\
      sendDbg.word32(strIn,uniqueId,w32.w32);\
    }

#define dbg_u16u32(mask,uniqueId,strIn, prm2,prm1) \
    if (mask & debugMask_rd())\
      //signal sendDbg.u16u32(strIn,uniqueId,prm2,prm1);


#define dbgBuffer(mask,uniqueId,strIn,ptr,num)\
    if (mask & debugMask_rd()) \
      //signal sendDbg.bufGen(strIn,uniqueId,ptr,num)
#define dbgSnsrReadings(mask,uniqueId,strIn, ptr) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.sensorReadings(strIn,uniqueId,ptr)
#define dbgLogAdc(mask,uniqueId,strIn, ptr) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.sensorsLogAdc(strIn,uniqueId,ptr)
#define dbgHalLogger(mask,uniqueId,strIn, ptr,recLen) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.halLogger(strIn,uniqueId,ptr,recLen)
#define dbgTxStatusDm8B(mask,uniqueId,strIn, pMsg) \
    if (mask & debugMask_rd()) \
      //signal sendDbg.txStatusDm8B(strIn,uniqueId,pMsg)
#define dbgTosMhopMsg(mask,strIn,uniqueId, ptr) \
    if (mask & debugMask_rd()) \
    //signal sendDbg.tosMhopMsg(strIn,uniqueId,ptr)
#define dbgTosMsg(mask,strIn,uniqueId, ptr) \
    if (mask & debugMask_rd()) \
    //signal sendDbg.tosSMsg(strIn,uniqueId,ptr)


#define dbgMemPort(mask,uniqueId,strIn)\
    if (mask & debugMask_rd()) \
      //signal sendDbg.memPort(strIn,uniqueId)
#define errEvt(uniqueId,strId) \
    if (eDbgAll_errEvt & debugMask_rd()) \
      //signal sendDbg.str1(strId,uniqueId);
#define errEvtStr(uniqueId,strId,str2) \
    if (eDbgAll_errEvt & debugMask_rd()) \
      //signal sendDbg.str(strId,uniqueId,str2);
#define errEvtUint8(uniqueId,strId,uint8) \
    if (eDbgAll_errEvt & debugMask_rd()) \
      //signal sendDbg._uint8_(strId,uniqueId,uint8);
#define errEvt2Uint8(strId,uniqueId,callerId,clientId) \
    if (eDbgAll_errEvt & debugMask_rd()) \
      //signal sendDbg._2uint8_(strId,uniqueId,callerId,clientId);
#define errEvtWord16(uniqueId,strId,wrd16) \
    if (eDbgAll_errEvt & debugMask_rd()) \
      //signal sendDbg.word(strId,uniqueId,wrd16);

typedef struct {
  uint16_t cntOverFlow;
  uint16_t cntOutCompare;
  int16_t hibmCntDwnT_sec; //Master 'reload' counter - Uints: "1 seconds"
  int16_t hibCntDwnT_sec; //Count down - Units: "1 seconds" 1 - 65,536, and cSleepForever
} myHPLClock_t;

#ifndef __DEBUG_MSG_CPP__

//TODO extern uint16_t debugMsgMasks[MASK_INDX_SIZE];

#endif //__DEBUG_MSG_CPP__
//}  //extern "C++"
#endif /* DEBUG_H_ */
