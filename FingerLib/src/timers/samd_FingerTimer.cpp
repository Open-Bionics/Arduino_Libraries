/*
 * sam_FingerTimer.cpp
 *
 * Created: 02/11/2015 11:12:48
 * Author: Olly McBride
 *
 * This work is licensed under the Creative Commons Attribution 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
 *
 */ 

#if defined(ARDUINO_ARCH_SAMD)

#include "samd_FingerTimer.h"

#define mS(val) ((val)*5)

#define SERVO_CTRL_TIME   mS(0.4)
#define PIN_LATCH_TIME    mS(1)
#define HAPTIC_TIME       mS(1)
#define MILLI_TIME        mS(1)
#define SECOND_TIME       1000

// create global pointer to function
void (*ptr2Func)(void) = NULL;

// function to receive pointer and assign to global pointer
void passPtr(void (*f)(void))
{
	ptr2Func = f;
}

void timerSetup(void)      
{
  // Enable clock for TC4 
  REG_GCLK_CLKCTRL = (uint16_t) (GCLK_CLKCTRL_CLKEN | GCLK_CLKCTRL_GEN_GCLK0 | GCLK_CLKCTRL_ID_TC4_TC5) ;
  while ( GCLK->STATUS.bit.SYNCBUSY == 1 ); // wait for sync 

  // The type cast must fit with the selected timer mode 
  TcCount16* TC = (TcCount16*) TC4;		// get timer struct

  TC->CTRLA.reg &= ~TC_CTRLA_ENABLE;	// Disable TC
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync 

  TC->CTRLA.reg |= TC_CTRLA_MODE_COUNT16;	// Set Timer counter Mode to 16 bits
  while (TC->STATUS.bit.SYNCBUSY == 1);		// wait for sync 
  TC->CTRLA.reg |= TC_CTRLA_WAVEGEN_MFRQ;	// Set TC Freq
  while (TC->STATUS.bit.SYNCBUSY == 1);		// wait for sync 

  TC->CTRLA.reg |= TC_CTRLA_PRESCALER_DIV2;		// Set prescaler
  while (TC->STATUS.bit.SYNCBUSY == 1);			// wait for sync 

  TC->CC[0].reg = 2397;					// 5kHz
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync 
  
  // Interrupts 
  TC->INTENSET.reg = 0;              // disable all interrupts
  TC->INTENSET.bit.OVF = 1;          // enable overflow
  TC->INTENSET.bit.MC0 = 1;          // enable compare match to CC0

  // Enable InterruptVector
  NVIC_EnableIRQ(TC4_IRQn);

  // Enable TC
  TC->CTRLA.reg |= TC_CTRLA_ENABLE;
  while (TC->STATUS.bit.SYNCBUSY == 1); // wait for sync
  
}

void TC4_Handler()
{
  TcCount16* TC = (TcCount16*) TC4; // get timer struct
  if (TC->INTFLAG.bit.OVF == 1)     // An overflow caused the interrupt
  {  
    TC->INTFLAG.bit.OVF = 1;		// writing a one clears the flag ovf flag

    static long timer5cnt = 0;        // main timer counter increments every call of the interrupt
    static long servoCount = 0;       // position control for a single motor
    
    timer5cnt++;    // increment timer counter every 200uS
 
    // position control for a single motor
    if((timer5cnt - servoCount) >= SERVO_CTRL_TIME)
    {
      servoCount = timer5cnt;
	  ptr2Func();
	}
  }
  
  if (TC->INTFLAG.bit.MC0 == 1) {  // A compare to cc0 caused the interrupt
    TC->INTFLAG.bit.MC0 = 1;    // writing a one clears the flag ovf flag
  }
}

#endif /* defined(ARDUINO_ARCH_SAMD) */