/*
 * I2C_PortExpander.h
 *
 * Created: 11/11/2015 14:08:12
 * Author: Olly McBride
 *
 * This work is licensed under the Creative Commons Attribution 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
 *
 */ 

#ifndef I2C_PORTEXPANDER_H_
#define I2C_PORTEXPANDER_H_

#include <inttypes.h>
#include <Arduino.h>

// Architecture specific include
#if defined(ARDUINO_ARCH_AVR)
#ifndef MYSERIAL
#define MYSERIAL Serial
#endif
#elif defined(ARDUINO_ARCH_SAMD)
#ifndef MYSERIAL
#define MYSERIAL SerialUSB
#endif
#endif

//#define PORTE_DEBUG

#define PORT_EXPANDER_ADDR  38  // I2C port expander address 0x30 0110000 38    // 39 old, 38 new

/* PINS */
#define ARM_LED		0     // LED pin
#define ARM_BTN		15    // button pin
#define HAP_M0		1     // motor 0
#define HAP_M1		2     // motor 1
#define HAP_M2		3     // motor 2
#define HAP_M3		4     // motor 3
#define HAP_M4		5     // motor 4
/* PORT NAMES */
#define PORT0		0
#define PORT1		1
/* VIBRATION NAMES */
#define CLOCKWISE		0
#define ANTICLOCKWISE	1
#define DOT_DOT_DOT		2
#define DOT_DASH		3
#define DASH_DASH		4
#define LONG_DASH		5
/* PULSE DURATION */
#define HAP_DOT			1		// default vibration duration for a dot
#define HAP_DASH		5		// default vibration duration for a dash
/* HAPTIC SETTINGS */
#define NUM_VIB				6	// number of animations
#define NUM_VIB_STEPS		6	// number of steps per animation
#define NUM_HAPTIC_MOTORS	5	// number of vibration motors

class I2C_PORTEXPANDER
{
	public:
		I2C_PORTEXPANDER();
		void begin(void);
		void write(uint8_t pin, uint8_t val);
		void write(uint8_t pin, uint8_t val, uint8_t duration);
		uint8_t read(uint8_t pin);
		void portMode(uint8_t port, uint8_t state);
		void portWrite(uint8_t port, uint8_t state);
	
		uint8_t runVibFlag = 0;			// flag to determine whether to run animation (-1 = no animation)
		uint8_t stepNum = 0;			// counts through the steps in the haptic animation sequence
		uint16_t vibrCounter = 0;		// counter for haptic animations/vibration patterns
		
	private:
		uint8_t getPort(uint8_t pin);
		void leadingZeros(uint8_t inVal);
	
		uint8_t portMask[2] = {0};
};

extern I2C_PORTEXPANDER portExpander;

#endif /* I2C_PORTEXPANDER_H_ */