/*
 * avr_FingerTimer.cpp
 *
 * Created: 02/11/2015 11:12:48
 * Author: Olly McBride
 *
 * This work is licensed under the Creative Commons Attribution 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
 *
 */ 

#if defined(ARDUINO_ARCH_AVR)

#include "avr_FingerTimer.h"

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
  changePWMFreq();  // change PWM to 31KHz
                  
  cli();//stop interrupts

  // Timer5 5Khz timer 200uS
  TCCR5A = 0;// set entire TCCR5A register to 0
  TCCR5B = 0;// same for TCCR5B
  TCNT5  = 0;//initialize counter value to 0
  // set compare match register for 5khz increments
  OCR5A = 1598;// = (16*10^6) / (1*5000) - 1 (must be <65536) [seems to be needed to be /2 again to acheive correct freq]
  // turn on CTC mode
  TCCR5B |= (1 << WGM52);
  // Set CS50 bit for no prescaler
  TCCR5B |= (1 << CS50);  
  // enable timer compare interrupt
  TIMSK5 |= (1 << OCIE5A);

  sei();//allow interrupts
}

// Timer5 at 2Hz every 200uS 
ISR(TIMER5_COMPA_vect)    
{
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

void changePWMFreq(void)
{
	TCCR1B = (TCCR1B & 0b11111000) | 0x01;
	TCCR2B = (TCCR2B & 0b11111000) | 0x01;
	TCCR3B = (TCCR3B & 0b11111000) | 0x01;
	TCCR4B = (TCCR4B & 0b11111000) | 0x01;
}

#endif /* defined(ARDUINO_ARCH_AVR) */