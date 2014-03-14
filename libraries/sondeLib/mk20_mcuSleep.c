/* McuSleep.c Freescale MK20
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
 *
 *  Influences: tinyos-main/tos McuSleep
 * github.com/tinyos/tinyos-main/../tos/chips/
 *            cortex/m3/sam3/s/McuSleepC.nc
 * forum.pjrc.com/threads/23660-Low-Power-quot-Green-quot-Battery-Operation-Solutions-For-The-Teensy-3?highlight=sleep
 *  Architecture
 *   on sleep setupSLEEP_MODE
 *   on wakeup resumeSLEEP_MODE
 *  TODO supported MK20 sleep mode types
 *    ???
 *

 */

#include <stdint.h>
#include "WProgram.h"
#include "sondeSys.h"

//TODO - place holder for file with a sweep of ideas
/*  provides
    interface McuSleep;
    interface McuPowerState;
    interface CHIP_LowPower;
*/

#ifdef	__cplusplus
extern "C"
{
#endif
//  #define PMC_PIO_CLOCK_MASK 0x3FFFFFFC

  enum {
    S_AWAKE,
    S_SLEEP,
    S_WAIT,
    S_BACKUP,
  };

  static uint32_t ps; //Current power state


//  async command void Sam3LowPower.configure() {
  void MK20LowPower_configure(void) {

	  //ideas - do for MK20 similar azonde example
  // Put the ADC into off mode
   // ADC12B->emr.bits.offmodes = 1;

    // Disable all Peripheral Clocks
    //PMC->pc.pcdr.flat = PMC_PIO_CLOCK_MASK;

    // Stop UTMI
    // PMC->uckr.bits.upllen = 0;

    // Disable brownout detector
   /* {
      supc_mr_t mr = SUPC->mr;
      mr.bits.key  = 0xA5;
      mr.bits.boddis = 1;
      mr.bits.bodrsten = 0;
      SUPC->mr = mr;
    }*/
    // Customize pio settings as appropriate for the platform
    //signal Sam3LowPower.customizePio();
  }
#if 0
static  uint32_t getPowerState() {
    //if (PMC->pcsr.flat & PMC_PIO_CLOCK_MASK)
   // if ((PMC->pc.pcsr.flat > 0) || (PMC->pc1.pcsr.flat > 0))
   //   return S_SLEEP;
   // else
      return S_WAIT;
  }
#endif
  void commonSleep() {
    // Check if any alarms are set for the tc2 alarm hardware.
    // If not, turn off its peripheral clock
    // This is necessary because the TMicro alarm is linked to tc2.
    // More logic will need to be added here later, as more alarms are set up
    // for use.
    //if(!(TC->ch2.imr.bits.cpcs & 0x01))
    //  PMC->pcdr.bits.tc2 = 1;
  }

  void commonResume() {
    // Turn the periperhal clock for tc2 back on so that its alarm can be
    // set and times can be read from it
    //PMC->pcer.bits.tc2 = 1;
  }

  void setupSleepMode() {
    // Nothing special to do here yet
  }
  void resumeFromSleepMode() {
    // Nothing special to do here yet
  }

  void setupWaitMode() {
    // Save the state of the cpu we are about to change
    /*
    wait_restore.mck = call HplSam3Clock.getMainClockSpeed();
    wait_restore.adc_emr = ADC12B->emr;
    wait_restore.pmc_pcsr = PMC->pcsr;
    wait_restore.pmc_uckr = PMC->uckr;
    wait_restore.supc_mr = SUPC->mr;
    wait_restore.pioa_psr = AT91C_BASE_PIOA->PIO_PSR;
    wait_restore.piob_psr = AT91C_BASE_PIOB->PIO_PSR;
    wait_restore.pioc_psr = AT91C_BASE_PIOC->PIO_PSR;
    wait_restore.pioa_osr = AT91C_BASE_PIOA->PIO_OSR;
    wait_restore.piob_osr = AT91C_BASE_PIOB->PIO_OSR;
    wait_restore.pioc_osr = AT91C_BASE_PIOC->PIO_OSR;
    wait_restore.pioa_pusr = AT91C_BASE_PIOA->PIO_PPUSR;
    wait_restore.piob_pusr = AT91C_BASE_PIOB->PIO_PPUSR;
    wait_restore.pioc_pusr = AT91C_BASE_PIOC->PIO_PPUSR;
    */
    // Turn off all clocks to peripheral IO and configure pins
    // appropriately, so that when we go to sleep we are
    // drawing the least amount of current possible
    //TODO call Sam3LowPower.configure();
	MK20LowPower_configure();
    // Force us into 4 MHz with the RC Oscillator
    /*
    call HplSam3Clock.mckInit4RC();
    // Setup for wait mode
    PMC->fsmr.bits.lpm = 1;
    // Only resume from wait mode with an input from the RTT
    PMC->fsmr.bits.rttal = 1;
    // Make sure we DON'T go into deep sleep (i.e. backup mode)
    SCB->scr.bits.sleepdeep = 0;
    */
  }

  void resumeFromWaitMode() {
    // TODO Restore the old clock settings
    /*
    uint32_t oldMck = wait_restore.mck;
    if(oldMck > 13000 && oldMck < 49000){
      call HplSam3Clock.mckInit48();
    }else if(oldMck > 49000 && oldMck < 90000){
      call HplSam3Clock.mckInit84();
    }else if(oldMck > 90000){
      call HplSam3Clock.mckInit96();
    }
    // Restore the ADC to its previous mode
    ADC12B->emr = wait_restore.adc_emr;
    // Reenable peripheral clocks, as appropriate
    PMC->pcer.flat = wait_restore.pmc_pcsr.flat;
    PMC->pcdr.flat = ~wait_restore.pmc_pcsr.flat;
    // Reenable the UTMI clock, as appropriate
    PMC->uckr = wait_restore.pmc_uckr;
    // Reenable brownout detector, as appropriate
    {
      supc_mr_t mr;
      mr = wait_restore.supc_mr;
      mr.bits.key  = 0xA5;
      SUPC->mr = mr;
    }
    */
    // Restore the PIO output/input pin and pullup resistor
    // settings to the values they had before entering wait mode
    /*
    AT91C_BASE_PIOA->PIO_PER = wait_restore.pioa_psr;
    AT91C_BASE_PIOB->PIO_PER = wait_restore.piob_psr;
    AT91C_BASE_PIOC->PIO_PER = wait_restore.pioc_psr;
    AT91C_BASE_PIOA->PIO_PDR = ~wait_restore.pioa_psr;
    AT91C_BASE_PIOB->PIO_PDR = ~wait_restore.piob_psr;
    AT91C_BASE_PIOC->PIO_PDR = ~wait_restore.pioc_psr;

    AT91C_BASE_PIOA->PIO_OER = wait_restore.pioa_osr;
    AT91C_BASE_PIOB->PIO_OER = wait_restore.piob_osr;
    AT91C_BASE_PIOC->PIO_OER = wait_restore.pioc_osr;
    AT91C_BASE_PIOA->PIO_ODR = ~wait_restore.pioa_osr;
    AT91C_BASE_PIOB->PIO_ODR = ~wait_restore.piob_osr;
    AT91C_BASE_PIOC->PIO_ODR = ~wait_restore.pioc_osr;

    // Notice the reverse logic below - its on purpose, check the data sheet
    // 0 means active, 1 means inactive
    AT91C_BASE_PIOA->PIO_PPUER = ~wait_restore.pioa_pusr;
    AT91C_BASE_PIOB->PIO_PPUER = ~wait_restore.piob_pusr;
    AT91C_BASE_PIOC->PIO_PPUER = ~wait_restore.pioc_pusr;
    AT91C_BASE_PIOA->PIO_PPUDR = wait_restore.pioa_pusr;
    AT91C_BASE_PIOB->PIO_PPUDR = wait_restore.piob_pusr;
    AT91C_BASE_PIOC->PIO_PPUDR = wait_restore.pioc_pusr;
    */
  }

  void setupBackupMode() {
    // Not yet supported....
    // Need to think more about how to do this since it is essentially a "hibernate"
    //  mode, meaning we will have to save all register and memory state into
    //  non-volatile memory. Possibly more state will need to be saved as well.
  }
  void resumeFromBackupMode() {
    // Not yet supported....
    // Need to think more about how to do this since it is essentially a "hibernate"
    //  mode and resuming will be from a reboot, not a simple interrupt service routine
    // Consider changing vectors.c to call out to this after checking some state variable
    // stored in the internal EEFC.
  }

//  async command void McuSleep.sleep()
  /*extern "C"*/ void McuSleep_sleep()
  {
#if 0
	  //no action - dummy for time being
    commonSleep();
    switch(ps = getPowerState()) {
      case S_AWAKE:
        //Just stay awake...
        break;
      case S_SLEEP:
        // Setup for sleep mode
        setupSleepMode();
        break;
      case S_WAIT:
        // Setup for wait mode
        setupWaitMode();
        break;
      case S_BACKUP:
        // Setup for backup mode
        setupBackupMode();
        break;
      default:
        // Default setup
        setupSleepMode();
    }

  //  __nesc_enable_interrupt();
    cli();

    /**
     * We need to disable sleep while debugging. Else, the GDB connection gets
     * disconnected.
     */
/*#ifndef DEBUG_CORE
    // Enter appropriate idle mode
    if(ps != S_AWAKE)
      __asm volatile ("wfe");
#endif */

    // Normally, at this point we can only be woken up by an interrupt, so execution continues
    // in the body of the InterruptWrapper.preamble() command before returning to here
    // However, if we never actually went to sleep, we need to force this command to run.
    if(ps == S_AWAKE){
      //call InterruptWrapper.preamble();
        call InterruptWrapper_preamble();
    }
    // all of memory may change at this point, because an IRQ handler
    // may have posted a task!
    //asm volatile("" : : : "memory");

    //__nesc_disable_interrupt();
    sei();
#endif
  }

//  async command void InterruptWrapper.preamble() {
void InterruptWrapper_preamble() {
    //TODO atomic
    {
      switch(ps) {
        case S_AWAKE:
          // Stayed awake the whole time, so do nothing
          break;
        case S_SLEEP:
          // Resume from sleep mode
          resumeFromSleepMode();
          break;
        case S_WAIT:
          // Resume from wait mode
          resumeFromWaitMode();
          break;
        case S_BACKUP:
          // Resume from backup mode
          resumeFromBackupMode();
          break;
        default:
          // Default resume
          resumeFromSleepMode();
      }
      if(ps != S_AWAKE)
        commonResume();
      ps = S_AWAKE;
    }
  }
//  async command void InterruptWrapper.postamble() { /* Do nothing */ }
//  async command void McuPowerState.update(){}
//  async event void HplSam3Clock.mainClockChanged(){}

//  default async event void Sam3LowPower.customizePio() {}
//}

#ifdef	__cplusplus
}
#endif

//End McuSleep.c




