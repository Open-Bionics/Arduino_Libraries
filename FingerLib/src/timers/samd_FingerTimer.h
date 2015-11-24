/*
 * sam_FingerTimer.h
 *
 * Created: 02/11/2015 11:12:48
 * Author: Olly McBride
 *
 * This work is licensed under the Creative Commons Attribution 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
 *
 */ 

#if defined(ARDUINO_ARCH_SAMD)

#ifndef SAM_FINGERTIMER_H_
#define SAM_FINGERTIMER_H_

#include <Arduino.h>

#define mS(val) ((val)*5)

#define SERVO_CTRL_TIME   mS(0.4)
#define PIN_LATCH_TIME    mS(1)
#define HAPTIC_TIME       mS(1)
#define MILLI_TIME        mS(1)
#define SECOND_TIME       1000

// function prototypes
void _timerSetup(void);
void TC4_Handler();
long customMillis(void);

void _passMotorPtr(void (*f)(void));
void _attachFuncToTimer(void (*f)(void));

#endif /* SAM_FINGERTIMER_H_ */
#endif /* defined(ARDUINO_ARCH_SAMD) */