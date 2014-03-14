/* Utils.h
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
 */

#ifndef __PRINTUTILS_H_
#define __PRINTUTILS_H_

#include <inttypes.h>
#include "Time.h"

// C language implementation
#ifdef __cplusplus
extern "C" {
#endif
//int usb_serial_getchar(void);
//extern uint32_t usb_cdc_line_coding[2];
result_t prnt_scanHex1Char(const char * inputPtr, uint8_t * outPtr);
result_t prnt_scanHex2Char(const char * input, uint8_t * out) ;
result_t prnt_scanHex3Char(const char * input, uint16_t * out);
result_t prnt_scanHex4Char(const char * input, uint16_t * out) ;


void prnt_timeYYYMMDDHHMMSS(void);
void prnt_timeSecs();

#ifdef __cplusplus
}
#endif



#endif /* PRINTUTILS_H_ */
