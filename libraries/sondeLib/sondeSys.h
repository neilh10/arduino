/* sondeSys.h
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
 *  Created on: Mar 1, 2014
 *      Author: Neil Hancock
 */
#ifndef SYS_H_
#define SYS_H_

//_BEGIN_STD_C
#ifdef	__cplusplus
extern "C"
{
#endif


//Issue between !std-define C bool and C++ defined bool
typedef int bool2;
enum { FALSE = 0, TRUE = 1 };

typedef uint32_t result_t;
typedef uint32_t error_t;

//TinyError.h
enum {
  SUCCESS        =  0,
  FAIL           =  1,           // Generic condition: backwards compatible
  ESIZE          =  2,           // Parameter passed in was too big.
  ECANCEL        =  3,           // Operation cancelled by a call.
  EOFF           =  4,           // Subsystem is not active
  EBUSY          =  5,           // The underlying system is busy; retry later
  EINVAL         =  6,           // An invalid parameter was passed
  ERETRY         =  7,           // A rare and transient failure: can retry
  ERESERVE       =  8,           // Reservation required before usage
  EALREADY       =  9,           // The device state you are requesting is already set
  ENOMEM         = 10,           // Memory required not available
  ENOACK         = 11,           // A packet was not acknowledged
  ELAST          = 11            // Last enum value
} ;//error_t_enum;

//typedef uint8_t error_t NESC_COMBINE("ecombine");

//error_t ecombine(error_t r1, error_t r2) @safe()
/* Returns: r1 if r1 == r2, FAIL otherwise. This is the standard error
     combination function: two successes, or two identical errors are
     preserved, while conflicting errors are represented by FAIL.
* /
{
  return r1 == r2 ? r1 : FAIL;
} */

//void Scheduler_init();
//void Scheduler_taskLoop(void);
void mainSched_list();
void McuSleep_sleep(void); //define per chip see tos/chips



//Set of unions to access uint8_t in uint32_t - was avr-gcc/nesC hopefully same standard gcc
typedef struct {
    uint8_t l; //LSB
    uint8_t m; //MSB
    } tTwoByte;
typedef struct {
    uint8_t b1; //LSB
    uint8_t b2; //
    uint8_t b3; //
    uint8_t b4; //MSB
    } tFourUint8;
typedef struct {
    uint8_t b1; //LSB
    uint8_t b2; //
    uint8_t b3; //
    uint8_t b4; //
    uint8_t b5; //
    uint8_t b6; //
    uint8_t b7; //
    uint8_t b8; //MSB
    } tEightUint8;
typedef union {
    tTwoByte b; //byte
    uint16_t w; //Word
  } uByteWrd;
typedef struct {
    uByteWrd l; //LSB
    uByteWrd m; //MSB
    } tTwoUint16;

typedef union {
//tFourByte b; //byte
    tTwoUint16 w; //word
    uint32_t l;   //long
    } uByteWrdLng;
typedef struct {
    uByteWrdLng l; //LSB
    uByteWrdLng m; //MSB
    }__attribute__ ((packed)) tTwoUint32;

//Signed unions
typedef struct {
    int8_t l; //LSB
    int8_t m; //MSB
} __attribute__ ((packed)) tTwoInt8;

typedef union {
    tTwoInt8 b; //byte
    int16_t w; //Word
  } __attribute__ ((packed)) uInt8Int16;

typedef struct {
    uInt8Int16 l; //LSB
    uInt8Int16 m; //MSB
} __attribute__ ((packed)) tTwoInt16;
typedef union {
//tFourByte b; //byte
    tTwoInt16 w; //word
    int32_t l;   //long
    } __attribute__ ((packed)) uInt32Int16;

//Set of unions to access uint16_t in uint64_t/tos_time
typedef struct {
  uint16_t l; //LSB
  uint16_t m; //MSB
} __attribute__ ((packed)) w32_t;

typedef union {
  tFourUint8 u8; //
  uint32_t w32;   //long
} __attribute__ ((packed)) w32_4u8_u;
typedef union {
  w32_t two_u16; //word
  uint32_t w32;   //long
} __attribute__ ((packed)) w32Two_u16_u;
typedef struct {
  w32Two_u16_u  l; //u32 LSB
  w32Two_u16_u  m; //u32 MSB
} __attribute__ ((packed)) w64_t;

typedef union {
  uint8_t     b[8];
  tEightUint8 u8; //
  uint64_t w64;   //long
} __attribute__ ((packed)) w64_8u8_u;
typedef union {
  w64_t   two_u32;   //two u32
  uint64_t w64;  //long
} __attribute__ ((packed)) w64Two_u32_u;

//_END_STD_C
#ifdef	__cplusplus
}
#endif

#endif /* SYS_H_ */
