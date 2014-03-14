/* debugMsg.c
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

#include "WProgram.h"
#include "debugMsg.h"
#include "Utils.h"

//#define DBG_DBGCONSOLE_TEST

#ifdef	__cplusplus
extern "C"
{
#endif

  static const prog_uchar strCrLf[] = "\n\r";
  static const prog_uchar strSpace[] = " ";
  static const prog_uchar strDash[] = "-";
  static const prog_uchar strSqBL[] = "[";
  static const prog_uchar strSqBR[] = "]";
  static const prog_uchar strColon[] = ":";
#if 0
  static const prog_uchar strTOSmsg[] = " TOSmsg:";
  static const prog_uchar strMHop[] = " MHop:";
  static const prog_uchar strSr12BitAll[] = " StrSr12BitAll:";
  static const prog_uchar strAM_DelugeAdvMsg[] = " AM_DelugeAdvMsg:";
 static const prog_uchar sPortHeader[] = "\n\r     A  B  C  D  E  F  G";
 static const prog_uchar sPin[] = "\n\r PIN ";
 static const prog_uchar sPort[] = "\n\rPort ";
 static const prog_uchar sDdr[] = "\n\r DDR ";
#endif //0

#if 0
/*ce***************************************************/
command void sendDbg.AdcBuf(const prog_uchar * pStr,const tSensorsLogAdc *pLogAdc) {
  int8_t cnt;
  int8_t length;

 //call PrintUtils.printP(strCrLf);
 //call PrintUtils.printP(pStr);

 //call PrintUtils.printP(strColon);
 //call PrintUtils.printHex(pLogAdc->status);
 //call PrintUtils.printP(strDash);
 //call PrintUtils.printHex(pLogAdc->iaCntrl);
 //call PrintUtils.printP(strDash);
 //call PrintUtils.printHex(pLogAdc->iaGain);
 //call PrintUtils.printP(strDash);

  length = pLogAdc->numAdcReadings;
 //call PrintUtils.printHex(length);

  for (cnt = 0; cnt < SENSORS_LOG_ADC_SIZE ; cnt++){
    if (0==(cnt & 0x3)) {
     //call PrintUtils.printP(strColon);
    } else {
     //call PrintUtils.printP(strSpace);
    }
   //call PrintUtils.printHexword(pLogAdc->data[cnt]);
  }
 //call PrintUtils.printP(strCrLf);

} //.AdcBufEvt() */
#endif

/*cf***************************************************/
void dbgMsgHdr(const prog_uchar * pStr,uint8_t uniqueId)
{
	Serial.print("\n\r");
	//prnt_timeSecs();
	prnt_timeYYYMMDDHHMMSS();
	Serial.print(pStr);
	Serial.write(strColon);
	Serial.write(uniqueId);
	Serial.write(strColon);
}//dbgMsgHdr

//*****************************************************
#if 0 // TODO
void  dbgMsgSensorReadings(const tSensorReadings *pRd) {
int8_t cnt;
int8_t length;

 //call PrintUtils.printHex(pRd->msgType);
 //call PrintUtils.printP(strSpace);
  length = pRd->length;
 //call PrintUtils.printHex(length);
 //call PrintUtils.printP(strSpace);
 //call PrintUtils.printHexlong(pRd->time);
 //call PrintUtils.printP(strDash);
 //call PrintUtils.printHex(pRd->StatusSonde);
 //call PrintUtils.printP(strDash);
 //call PrintUtils.printHexword(pRd->StatusSensors);

  //Adjust length to number data points
  length = ((length -MT_Sr12bitAllMt02OvrhdLen)>>1);
  if (length >SENSOR_READINGS_ACT_NUM) {
    length = SENSOR_READINGS_ACT_NUM;
  }
  for (cnt = 0; cnt < length ; cnt++){
    if (0==(cnt & 0x3)) {
     //call PrintUtils.printP(strColon);
    } else {
     //call PrintUtils.printP(strSpace);
    }
   //call PrintUtils.printHexword(pRd->data[cnt]);
  }
//  call PrintUtils.printP(strCrLf);

}
#endif //0
#if 0
/*ce***************************************************/
 void sendDbg.sensorReadings(const prog_uchar * pStr,uint8_t uniqueId,const tSensorReadings *pRd) {

  //** Merge with MtSr12bitAll if possible
  dbgMsgHdr(pStr,uniqueId);
  dbgMsgSensorReadings(pRd);
} //.sensorReadings() */

/*ce***************************************************/
 void dbgSensorsLogAdc(const tSensorsLogAdc *pRd) {
int8_t cnt;
int8_t length;

 //call PrintUtils.printHex(pRd->status);
 //call PrintUtils.printP(strDash);
 //call PrintUtils.printHex(pRd->iaCntrl);
 //call PrintUtils.printP(strDash);
 //call PrintUtils.printHex(pRd->iaGain);
 //call PrintUtils.printP(strDash);
  length = pRd->numAdcReadings;
 //call PrintUtils.printHex(length);

  if (length >SENSOR_READINGS_ACT_NUM) {
    length = SENSOR_READINGS_ACT_NUM;
  }
  for (cnt = 0; cnt < length ; cnt++){
    if (0==(cnt & 0x3)) {
     //call PrintUtils.printP(strColon);
    } else {
     //call PrintUtils.printP(strSpace);
    }
   //call PrintUtils.printHexword(pRd->data[cnt]);
  }
//  call PrintUtils.printP(strCrLf);

 } //dbgSensorsLogAdc() */
/*ce***************************************************/
 void sendDbg.sensorsLogAdc2(const prog_uchar * pStr,uint8_t uniqueId,uint16_t prm2,
  const tSensorsLogAdc *pRd) {
  //Print with two supplied paramters - one 16bits, and ptr
  dbgMsgHdr(pStr,uniqueId);
 //call PrintUtils.printHexword(prm2);
 //call PrintUtils.printChar(':');
  dbgSensorsLogAdc(pRd);
} //.sensorLogAdc() */

/*ce***************************************************/
  void sendDbg.sensorsLogAdc(const prog_uchar * pStr,uint8_t uniqueId,const tSensorsLogAdc *pRd) {

  dbgMsgHdr(pStr,uniqueId);
  dbgSensorsLogAdc(pRd);
 } //.sensorLogAdc() */

/*ce***************************************************/
  void sendDbg.halLogger(const prog_uchar * pStr,uint8_t uniqueId,const tHalLogger *pRd,uint8_t recLength) {
//int8_t cnt;
//int8_t length;

  dbgMsgHdr(pStr,uniqueId);
 //call PrintUtils.printHex(pRd->destAckFlags);
 //call PrintUtils.printHex(pRd->chksum);
  if  (recLength > 0) {
   //call PrintUtils.printP(strSqBL);
    dbgMsgSensorReadings(&pRd->record);
   //call PrintUtils.printP(strSqBR);
  }
//  call PrintUtils.printP(strCrLf);
 }//sendDbg.halLogger

 /*ce***************************************************/
 void sendDbg.txStatusDm8B(const prog_uchar * pStr,uint8_t uniqueId,  const void *pMsgIn) {
// Modem Status 0x8B processing -
// XBP 900 DigiMesh v182x 90000903_B pg 33
  tMsgRxGeneric *pMsg = (tMsgRxGeneric *)pMsgIn;

  //Print with two supplied paramters - one 16bits, and ptr
  dbgMsgHdr(pStr,uniqueId);
 //call PrintUtils.printHex(pMsg->u.rxTxStatus8B.frameId);
 //call PrintUtils.printChar(':');
 //call PrintUtils.printHex(pMsg->u.rxTxStatus8B.remAddr1);
 //call PrintUtils.printHex(pMsg->u.rxTxStatus8B.remAddr2);
 //call PrintUtils.printP(PSTR(" Retry="));
 //call PrintUtils.printHex(pMsg->u.rxTxStatus8B.txRetryCnt);
 //call PrintUtils.printP(PSTR(" Discovery="));
 //call PrintUtils.printHex(pMsg->u.rxTxStatus8B.statusDiscovery);

} //sendDbg.txStatusDm8B

/*cf***************************************************/
void tosMsgHdr(const TOS_Msg *pMsg) {

 //call PrintUtils.printP(strTOSmsg);
 //call PrintUtils.printHexword(pMsg->addr);
 //call PrintUtils.printChar('-');
 //call PrintUtils.printHex(pMsg->type);
 //call PrintUtils.printChar('-');
 //call PrintUtils.printHex(pMsg->group);
 //call PrintUtils.printChar('@');
 //call PrintUtils.printHex(pMsg->length);
}
/*cf***************************************************/
void tosMHopHdr(const TOS_MHopMsg *pMsg) {

 //call PrintUtils.printP(strMHop);
 //call PrintUtils.printHexword(pMsg->sourceaddr);
 //call PrintUtils.printChar('-');
 //call PrintUtils.printHexword(pMsg->originaddr);
 //call PrintUtils.printChar('+');
 //call PrintUtils.printHexword(pMsg->seqno);
 //call PrintUtils.printChar('+');
 //call PrintUtils.printHex(pMsg->hopcount);
}
/*ce***************************************************/
void MtSr12bitAll(tSr12bitAllMt02 *pMsg) {
  int8_t cnt;
  int8_t lenInWords;
  uint8_t msgTypeLength = pMsg->msgTypeLen;

 //call PrintUtils.printP(strSr12BitAll);
 //call PrintUtils.printHex(msgTypeLength);
 //call PrintUtils.printChar('-');
 //call PrintUtils.printHexlong(pMsg->time);
 //call PrintUtils.printChar('-');
 //call PrintUtils.printHex(pMsg->StatusSonde);
 //call PrintUtils.printChar('+');
 //call PrintUtils.printHexword(pMsg->StatusSensors);
 //call PrintUtils.printChar('=');
  lenInWords = ((msgTypeLength - MT_Sr12bitAllMt02OvrhdLen )>>1);
  /* Debug if needed
  call PrintUtils.printHex(pMsg->msgTypeLen);
  Serial.print(' ');
  call PrintUtils.printHex(MT_Sr12bitAllMt02HdrLen);
  Serial.print(' ');
  call PrintUtils.printHex(lenInWords);
  call PrintUtils.printChar('='); */

  for (cnt = 0; cnt < lenInWords ; cnt++){
   //call PrintUtils.printHexword(pMsg->SensorReading[cnt]);

    if (0x0F==(cnt&0x0F)) {
         //call PrintUtils.printP(strCrLf);
        } else if (0x3==(cnt & 0x3)) {
         //call PrintUtils.printChar(':');
        } else {
         //Serial.print(' ');
        }
      }
} //MtSr12bitAll

/*ce***************************************************/
void Am_DelugeAdvMsgHndl(const TOS_Msg *pTosMsg) {
  DelugeAdvMsg *pMsg = (DelugeAdvMsg *)pTosMsg->data;
//  uint8_t msgTypeLength = pTosMsg->length;

 //call PrintUtils.printP(strAM_DelugeAdvMsg);
 //call PrintUtils.printHexword(pMsg->sourceAddr);
 //call PrintUtils.printChar('-');
 //call PrintUtils.printHex(pMsg->version);
 //call PrintUtils.printChar('-');
 //call PrintUtils.printHex(pMsg->type);
 //call PrintUtils.printChar(':');


 //call PrintUtils.printHexword(pMsg->nodeDesc.vNum);
 //Serial.print(' ');
 //call PrintUtils.printHexlong(pMsg->nodeDesc.uid);
 //Serial.print(' ');
 //call PrintUtils.printHexword(pMsg->nodeDesc.imgNum);
 //call PrintUtils.printChar(':');

 //call PrintUtils.printHexlong(pMsg->imgDesc.uid);
 //Serial.print(' ');
 //call PrintUtils.printHexword(pMsg->imgDesc.vNum);
 //Serial.print(' ');
 //call PrintUtils.printHex(pMsg->imgDesc.imgNum);
 //Serial.print(' ');
 //call PrintUtils.printHex(pMsg->imgDesc.numPgs);
 //call PrintUtils.printChar('=');
 //call PrintUtils.printHex(pMsg->imgDesc.numPgsComplete);

 //call PrintUtils.printChar(':');
 //call PrintUtils.printHex(pMsg->numImages);
}
/*cf***************************************************/
void Am_SourceLabelInd(const TOS_Msg *pTosMsg) {
  tMng_SourceLabelInd_Am09 *pMsg = (tMng_SourceLabelInd_Am09 *)pTosMsg->data;
//  uint8_t msgTypeLength = pTosMsg->length;

 //call PrintUtils.printP(PSTR("LabelInd:"));
 //call PrintUtils.printD(pMsg->label);
}//Am_SourceLabelInd
/*cf***************************************************/
void Am_SourceStateChngeInd(const TOS_Msg *pTosMsg) {
  tMng_SourceStateChngInd_Am0B *pMsg = (tMng_SourceStateChngInd_Am0B *)pTosMsg->data;
//  uint8_t msgTypeLength = pTosMsg->length;

 //call PrintUtils.printP(PSTR("SrcStateChngInd:Old/New="));
 //call PrintUtils.printHex(pMsg->oldState);
 //call PrintUtils.printChar('/');
 //call PrintUtils.printHex(pMsg->newState);
 //call PrintUtils.printChar(':');
 //call PrintUtils.printD(pMsg->label);
}//Am_SourceStateChngeInd

/*cf***************************************************/
void printTosMhopMsg(const TOS_Msg *pMsg) {
  int8_t cnt;
  int8_t length;
  TOS_MHopMsg *pMHop = (TOS_MHopMsg *)pMsg->data;

  tosMsgHdr(pMsg);
  tosMHopHdr(pMHop);
 //call PrintUtils.printP(strCrLf);
  length = pMsg->length - (/*eTOS_MSG_Overhead +*/ eTOS_MHopMsg_Overhead);
/* //Debug if needed
  call PrintUtils.printHex(eTOS_MSG_Overhead);
  call PrintUtils.printChar('+');
  call PrintUtils.printHex(eTOS_MHopMsg_Overhead);
  call PrintUtils.printChar('+');
  call PrintUtils.printHex(length);
  call PrintUtils.printChar('=');
  call PrintUtils.printHex(TOSH_DATA_LENGTH);
  call PrintUtils.printP(strCrLf);

  if (length >TOSH_DATA_LENGTH) {
    length = TOSH_DATA_LENGTH;
  }

  switch (pMHop->data[0]) {
    case MT_Sr12bitAllMt02: MtSr12bitAll((tSr12bitAllMt02 *)pMHop->data); break;
    default:
      for (cnt = 0; cnt < length ; cnt++){
       //call PrintUtils.printHex(pMHop->data[cnt]);

        if (0x0F==(cnt&0x0F)) {
         //call PrintUtils.printP(strCrLf);
        } else if (0x3==(cnt & 0x3)) {
         //call PrintUtils.printChar(':');
        } else {
         //Serial.print(' ');
        }
      }
  }
//  call PrintUtils.printP(strDash);
//  call PrintUtils.printHex(pMsg->crc);
} //.printTosMhopMsg() */
/*cf***************************************************/
void printTosSMsg(const TOS_Msg *pMsg) {
  int8_t cnt;
  int8_t length;
 // TOS_MHopMsg *pMHop = (TOS_MHopMsg *)pMsg->data;

  tosMsgHdr(pMsg);
//  tosMHopHdr(pMHop);
  call PrintUtils.printP(strCrLf);
  length = pMsg->length; //- eTOS_MSG_Overhead;

  if (length >TOSH_DATA_LENGTH) {
    length = TOSH_DATA_LENGTH;
  }

  switch (pMsg->type) {
//    case MT_Sr12bitAllMt02: MtSr12bitAll((tSr12bitAllMt02
    //    *)pMHop->data); break;
    //case AM_DELUGEREQMSG AM_DELUGEDATAMSG AM_NETPROGMSG
  case AM_DELUGEADVMSG:
    Am_DelugeAdvMsgHndl(pMsg);
    call PrintUtils.printP(strCrLf);
    //break; - fall through
  case AM_SOURCE_LABEL_IND:
    Am_SourceLabelInd(pMsg);
    break;
  case AM_SOURCE_STATE_CHANGE_IND:
    Am_SourceStateChngeInd(pMsg);
    break;
  default:
      for (cnt = 0; cnt < length ; cnt++){
        call PrintUtils.printHex(pMsg->data[cnt]);

        if (0x0F==(cnt&0x0F)) {
          call PrintUtils.printP(strCrLf);
        } else if (0x3==(cnt & 0x3)) {
          call PrintUtils.printChar(':');
        } else {
          Serial.print(' ');
        }
      }
  }
//  call PrintUtils.printP(strDash);
//  call PrintUtils.printHex(pMsg->crc);
} //.printTosSMsg() */
/*ce***************************************************/
 void sendDbg.tosSMsg(const prog_uchar * pStr,const uint8_t uniqueId,const TOS_Msg *pMsg) {

  dbgMsgHdr(pStr,uniqueId);
  printTosSMsg(pMsg);
}
/*ce***************************************************/
 void sendDbg.tosMhopMsg(const prog_uchar * pStr,const uint8_t uniqueId,const TOS_Msg *pMsg) {

  dbgMsgHdr(pStr,uniqueId);
  printTosMhopMsg(pMsg);
}
/*  typedef struct _MsgRcvEntry {
    uint8_t Proto;
    uint8_t Token;	// Used for sending acknowledgements
    uint16_t Length;	// Does not include 'Proto' or 'Token' fields
    TOS_MsgPtr pMsg;
  } MsgRcvEntry_t ; */

#define NoCompile
#ifdef NoCompile

/*ce***************************************************/
 void sendDbg.Rfc1622Msg(const prog_uchar * pStr,uint8_t uniqueId,const char *pMsg) {
  //int8_t cnt;
  //int8_t length;
//TOS_MHopMsg *pMHop = (TOS_MHopMsg *)pMsg->data;
    MsgRcvEntry_t *pRfc1622 = (MsgRcvEntry_t *)pMsg;

    dbgMsgHdr(pStr,uniqueId);

    call PrintUtils.printHex(pRfc1622->Proto);
    call PrintUtils.printHex(pRfc1622->Token);
    call PrintUtils.printHex(pRfc1622->Length);

    //printTosMhopMsg( (const TOS_Msg *) pMsg); - causes compiler error
    printTosMhopMsg((const TOS_MsgPtr) pRfc1622->pMsg);
    /* */
}
#endif
/*ce***************************************************/
  void sendDbg.bufGen(const prog_uchar * pStr,const uint8_t uniqueId,const void *pInBuf,uint8_t length) {
   int8_t cnt;
   uint8_t *pRd = (uint8_t *) pInBuf;

  dbgMsgHdr(pStr,uniqueId);
  call PrintUtils.printP(strColon);
  call PrintUtils.printHex(length);
  call PrintUtils.printP(strColon);

#define MAX_LENGTH 0x20
  if (length >MAX_LENGTH) {
    length = MAX_LENGTH;
    call PrintUtils.printHex(length);
    call PrintUtils.printP(strColon);
  }
  for (cnt = 0; cnt < length ; cnt++,pRd++){
    if (0==(cnt & 0x3)) {
      call PrintUtils.printP(strColon);
    } else {
      call PrintUtils.printP(strSpace);
    }
    call PrintUtils.printHex(*pRd);
  }
//  call PrintUtils.bufGen()

} //.sensorLogAdc() */
#endif //0
/*ce***************************************************/
inline  void sendDbg_str1(const prog_uchar * pStr,uint8_t uniqueId) {
  dbgMsgHdr(pStr,uniqueId);
} //.word() */
/*ce***************************************************/
void sendDbg_oChar(const uint8_t oChar) {
  //TODO call PrintUtils.printChar(oChar);
	Serial.print(oChar);

} //.oChar() */
/*ce***************************************************/
void sendDbg_oByte(const uint8_t outByte) {
  //TODO call PrintUtils.printHex(outByte);
	Serial.print(outByte);

} //.oByte() */
/*ce***************************************************/
void sendDbg_oWord(const uint16_t outWord) {
  //TODO call PrintUtils.printHexword(outWord);
	Serial.write(outWord);

} //.oWord() */
/*ce***************************************************/
inline void sendDbg_oStrD(const uint8_t * pStr) {
  //TODO call PrintUtils.printD(pStr);
	Serial.write((char *)pStr);
} //.oStr() */
/*ce***************************************************/
inline void sendDbg_oStrP(const prog_uchar * pStr) {
  //TODO call PrintUtils.printP(pStr);
	Serial.write(pStr);
} //.oStr() */
/*ce***************************************************/
 void sendDbg_oChrNbl(const uint8_t uniqueId,const uint8_t otByte) {
  //TODO call PrintUtils.printChar(uniqueId);
  //TODO call PrintUtils.printHexNibble(otByte);
} //.oChrNbl() */
/*ce***************************************************/
void sendDbg_oChrByt(const uint8_t uniqueId,const uint8_t otByte) {
  //call PrintUtils.printChar(uniqueId);
  //call PrintUtils.printHex(otByte);
} //.oChrByt() */
/*ce***************************************************/
 void sendDbg_oChrWord(const uint8_t uniqueId,const uint16_t otWord) {
  //call PrintUtils.printChar(uniqueId);
  //call PrintUtils.printHexword(otWord);
} //.oChrByt() */


/*ce***************************************************/
inline void sendDbg_oByteSpace(const uint8_t otByte) {
    //call PrintUtils.printHex(otByte);
    //Serial.print(' ');
} //.oByteSpace() */
/*ce***************************************************/
inline void sendDbg_oWordSpace(const uint16_t otWord) {
    //call PrintUtils.printHexword(otWord);
    //Serial.print(' ');
} //.oWordpace() */
/*ce***************************************************/
inline void sendDbg_oWord32Space(const uint32_t otWord) {
    //call PrintUtils.printHexlong(otWord);
    //Serial.print(' ');
} //.oWordSpace() */
/*ce***************************************************/
inline void sendDbg_oEol() { //End Of Line
    //call PrintUtils.printChar('\n');
    //call PrintUtils.printChar('\r');
} //.oEol() */

/*ce***************************************************/
 void sendDbg::_hex1(uint8_t n)
{
	n &= 15;
	if (n < 10) {
		Serial.write('0' + n);
	} else {
		Serial.write('A' - 10 + n);
	}
}


/*ce***************************************************/
inline void sendDbg::_hex2(uint8_t n)
{
	_hex1(n >> 4);
	_hex1(n);
}
/*ce***************************************************/
inline void sendDbg::_hexA(uint8_t n)
{
	_hex2(n);
}

/*ce***************************************************/
inline void sendDbg::_hex4(uint16_t n)
{
	_hex2((uint8_t)n >> 8);
	_hex2((uint8_t)n);
}
/*ce***************************************************/
inline void sendDbg::_hexA(uint16_t n)
{
	_hex2(n >> 8);
	_hex2(n);
}

/*ce***************************************************/
void  sendDbg::_hex8(uint32_t n)
{
	_hex2((uint8_t)n >> 24);
	_hex2((uint8_t)n >> 16);
	_hex2((uint8_t)n >> 8);
	_hex2((uint8_t)n);
}
/*ce***************************************************/
inline void sendDbg::_hexA(uint32_t n)
{
 _hex8(n);
}
/*ce***************************************************/
 void sendDbg::_u8u16(const prog_uchar * pStr,uint8_t uniqueId,const uint8_t outByte,const uint16_t outWord) {

  dbgMsgHdr(pStr,uniqueId);
  //call PrintUtils.printHex(outByte);
  //Serial.print(' ');
  //call PrintUtils.printHexword(outWord);
} //.u8u16 */
/*ce***************************************************/
void sendDbg::_u8u16u32(const prog_uchar * pStr,uint8_t uniqueId,const uint8_t outByte,const uint16_t outWord,const uint32_t outLong) {

  dbgMsgHdr(pStr,uniqueId);
  //call PrintUtils.printHex(outByte);
  //Serial.print(' ');
  //call PrintUtils.printHexword(outWord);
  //Serial.print(' ');
  //call PrintUtils.printHexlong(outLong);
} //.u8u16u32 */

/*ce***************************************************/
void sendDbg::_u8u32(const prog_uchar * pStr,uint8_t uniqueId,const uint8_t outByte,const uint32_t outLong) {

  dbgMsgHdr(pStr,uniqueId);
  //call PrintUtils.printHex(outByte);
  //Serial.print(' ');
  //call PrintUtils.printHexlong(outLong);
} //.u8u32 */

/*ce***************************************************/
void sendDbg::_2uint8_(const char * pStr,uint8_t uniqueId,uint8_t param1,uint8_t param2) {

  dbgMsgHdr(pStr,uniqueId);

  Serial.print('=');
  _hex2(param1);
  Serial.print(' ');
  _hexA(param2);
} //.word() */

/*ce***************************************************/
/*TODO del void sendDbg::uint8b(uint8_t uniqueId,const char * pStr, uint8_t outByte1) {
 //if (mask & debugMsgMasks[offset])
 {
	  dbgMsgHdr(pStr,uniqueId);
	  Serial.print(' ');
	  _hexA(outByte1);
 }
}*/
void sendDbg::uint8(uint8_t uniqueId,cchar * pStr, uint8_t outByte1) {
 //if (mask & debugMsgMasks[offset])
 {
	  dbgMsgHdr(pStr,uniqueId);
	  Serial.print(' ');
	  _hexA(outByte1);
 }

} //uint8

 /*ce***************************************************/
 void sendDbg::uint8(uint8_t uniqueId,cchar * pStr, uint8_t outByte2, uint8_t outByte1) {
  //if (mask & debugMsgMasks[offset])
  {
	 dbgMsgHdr(pStr,uniqueId);
 	 Serial.print(' ');
 	 _hexA(outByte2);
 	 Serial.print(' ');
 	 _hexA(outByte1);
  }
 } //uint8

 /*ce***************************************************/
 void sendDbg::uint8(uint8_t uniqueId,cchar * pStr, uint8_t outByte3, uint8_t outByte2, uint8_t outByte1) {
  //if (mask & debugMsgMasks[offset])
  {
	 dbgMsgHdr(pStr,uniqueId);
 	 Serial.print(' ');
 	 _hexA(outByte3);
 	 Serial.print(' ');
 	 _hexA(outByte2);
 	 Serial.print(' ');
 	 _hexA(outByte1);
  }
 } //uint8

 /*ce***************************************************/
 void sendDbg::uint8(uint8_t uniqueId,cchar * pStr,uint8_t outByte4, uint8_t outByte3, uint8_t outByte2, uint8_t outByte1) {
  //if (mask & debugMsgMasks[offset])
  {
	 dbgMsgHdr(pStr,uniqueId);
 	 Serial.print(' ');
 	 _hexA(outByte4);
 	 Serial.print(' ');
 	 _hexA(outByte3);
 	 Serial.print(' ');
 	 _hexA(outByte2);
 	 Serial.print(' ');
 	 _hexA(outByte1);
  }
 } //uint8
#if 0 //TODO - delete replace by uint8(variable)
/*ce***************************************************/
void sendDbg::_uint8_(const prog_uchar * pStr,uint8_t uniqueId,const uint8_t outByte) {

  dbgMsgHdr(pStr,uniqueId);
//del  call PrintUtils.printChar('=');
  //call PrintUtils.printHex(outByte);
  //delcall PrintUtils.printChar(':');
} //.word() */

/*ce***************************************************/
 void sendDbg::_uint8_2(const prog_uchar * pStr,uint8_t uniqueId,uByteWrd w16) {

  dbgMsgHdr(pStr,uniqueId);
 _hex2(w16.b.m);
 Serial.print(' ');
 _hex2(w16.b.l);
} //.word() */
/*ce***************************************************/
 void sendDbg::_uint8_3(const prog_uchar * pStr,uint8_t uniqueId,w32_4u8_u w32) {

  dbgMsgHdr(pStr,uniqueId);
 //call PrintUtils.printHex(w32.u8.b3);
  _hex2(w32.u8.b3);
  Serial.print(' ');
 //call PrintUtils.printHex(w32.u8.b2);
  _hex2(w32.u8.b2);
  Serial.print(' ');
 //call PrintUtils.printHex(w32.u8.b1);
  _hex2(w32.u8.b1);
} //.word() */
/*ce***************************************************/
 void sendDbg::_uint8_4(const prog_uchar * pStr,uint8_t uniqueId,w32_4u8_u w32) {

  dbgMsgHdr(pStr,uniqueId);
 //call PrintUtils.printHex(w32.u8.b4);
  _hex2(w32.u8.b4);
  Serial.print(' ');
 //call PrintUtils.printHex(w32.u8.b3);
  _hex2(w32.u8.b3);
  Serial.print(' ');
 //call PrintUtils.printHex(w32.u8.b2);
  _hex2(w32.u8.b2);
  Serial.print(' ');
 //call PrintUtils.printHex(w32.u8.b1);
  _hex2(w32.u8.b1);
} //.word() */
#endif//
/*ce***************************************************/
 void sendDbgu16u32(const prog_uchar * pStr,uint8_t uniqueId,const uint16_t outWord,const uint32_t outLong) {

  dbgMsgHdr(pStr,uniqueId);
 //call PrintUtils.printHexword(outWord);
 //Serial.print(' ');
  Serial.print(' ');
 //call PrintUtils.printHexlong(outLong);
} //.u16u32 */
/*ce***************************************************/
 void sendDbgword(const prog_uchar * pStr,uint8_t uniqueId,const uint16_t outWord) {

  dbgMsgHdr(pStr,uniqueId);
 //call PrintUtils.printHexword(outWord);

} //.word() */
/*ce***************************************************/
 void sendDbg_word_2(const prog_uchar * pStr,uint8_t uniqueId,
  const uint16_t outWordB,const uint16_t outWordA) {

  dbgMsgHdr(pStr,uniqueId);
 //call PrintUtils.printHexword(outWordB);
 //Serial.print(' ');
  Serial.print(' ');
 //call PrintUtils.printHexword(outWordA);

} //.word3() */
/*ce***************************************************/
 void sendDbg_word_3(const prog_uchar * pStr,uint8_t uniqueId,
  const uint16_t outWordC,const uint16_t outWordB,const uint16_t outWordA) {

  dbgMsgHdr(pStr,uniqueId);
 //call PrintUtils.printHexword(outWordC);
 //Serial.print(' ');
  Serial.print(' ');
 //call PrintUtils.printHexword(outWordB);
 //Serial.print(' ');
  Serial.print(' ');
 //call PrintUtils.printHexword(outWordA);

} //.word3() */
/*ce***************************************************/
 void sendDbg_word32(const prog_uchar * pStr,uint8_t uniqueId,const uint32_t outWord) {

  dbgMsgHdr(pStr,uniqueId);
 //call PrintUtils.printHexlong(outWord);

} //.word() */
/*ce***************************************************/
// void sendDbg_word64(const prog_uchar * pStr,uint8_t uniqueId,const uint32_t upperWord,const uint32_t lowerWord) {
 void sendDbg_word64(const prog_uchar * pStr,uint8_t uniqueId,const uint64_t outWord) {

  dbgMsgHdr(pStr,uniqueId);
 //call PrintUtils.printHexword64(outWord);

} //.word() */

  /*df***************************************************/
  void printPortValue(uint8_t portValue) {
   //call PrintUtils.printHex(portValue);
   //Serial.print(' ');
	  Serial.print(' ');
  }
  /*df***************************************************/
  void printSeqPin() {
#if 0
    uint8_t portValue;

   //call PrintUtils.printP(sPin);
    portValue = inp(PINA); printPortValue(portValue);
    portValue = inp(PINB); printPortValue(portValue);
    portValue = inp(PINC); printPortValue(portValue);
    portValue = inp(PIND); printPortValue(portValue);
    portValue = inp(PINE); printPortValue(portValue);
    portValue = inp(PINF); printPortValue(portValue);
    portValue = inp(PING); printPortValue(portValue);
#endif
  } //printSeqPin
  /*df***************************************************/
  void printSeqDdr() {
#if 0
	  uint8_t portValue;

   //call PrintUtils.printP(sDdr);
    portValue = inp(DDRA); printPortValue(portValue);
    portValue = inp(DDRB); printPortValue(portValue);
    portValue = inp(DDRC); printPortValue(portValue);
    portValue = inp(DDRD); printPortValue(portValue);
    portValue = inp(DDRE); printPortValue(portValue);
    portValue = inp(DDRF); printPortValue(portValue);
    portValue = inp(DDRG); printPortValue(portValue);
#endif
 } //printSeqDdr
  /*df***************************************************/
  void printSeqPort() {
#if 0
    uint8_t portValue;

   //call PrintUtils.printP(sPort);

    portValue = inp(PORTA); printPortValue(portValue);
    portValue = inp(PORTB); printPortValue(portValue);
    portValue = inp(PORTC); printPortValue(portValue);
    portValue = inp(PORTD); printPortValue(portValue);
    portValue = inp(PORTE); printPortValue(portValue);
    portValue = inp(PORTF); printPortValue(portValue);
    portValue = inp(PORTG); printPortValue(portValue);
#endif
  } //printSeqPort

/*ce***************************************************/
 void sendDbg_memPort(const prog_uchar * pStr,uint8_t uniqueId) {

  dbgMsgHdr(pStr,uniqueId);
   //call PrintUtils.printP(sPortHeader);
    printSeqPin();
    printSeqPort();
    printSeqDdr();

} //.memPort() */
/*ce***************************************************/
//       void str(const prog_uchar * pStr,const uint8_t uniqueId,const prog_uchar * pStr2);
 void sendDbg_str(const prog_uchar * pStr,const uint8_t uniqueId,const prog_uchar * pStr2) {

  dbgMsgHdr(pStr,uniqueId);
 //call PrintUtils.printP(pStr2);

} //.str() */
/*ce***************************************************/
 void sendDbg_strD(const prog_uchar * pStr,const uint8_t uniqueId,const char * pStrD) {

  dbgMsgHdr(pStr,uniqueId);
 //call PrintUtils.printD(pStrD);

} //.strD() */
/*ce***************************************************/
//      event void str(const prog_uchar * pStr,const uint8_t uniqueId,const prog_uchar * pStr2);
 void sendDbg_timeAddrData(const prog_uchar * pStr,uint16_t addr, uint16_t data) {

  prnt_timeSecs();
 //call PrintUtils.printP(pStr);
 //call PrintUtils.printChar(':');
 //call PrintUtils.printHexword(addr);
 //call PrintUtils.printChar('-');
 //call PrintUtils.printHexword(data);

} //.str() */
/*de***************************************************/
//inline    async event result_t Clock.fire() { return SUCCESS;    }
//} //end implementation

//}//namespace
#ifdef	__cplusplus
}
#endif
//End




