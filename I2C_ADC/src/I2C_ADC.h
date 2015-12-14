/*
 * I2C_ADC.h
 *
 * Created: 11/11/2015 14:08:12
 * Author: Olly McBride
 *
 * This work is licensed under the Creative Commons Attribution 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
 *
 * AD7995 4 channel I2C ADC
 *
 */ 

#ifndef I2C_ADC_H_
#define I2C_ADC_H_

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

//#define ADC2_DEBUG

#define ADC2_ADDR  0x28     // I2C ADC address 0x28 010 1000 40

#define ENABLE    1
#define DISABLE   0

#define NUM_CHANNELS    4   // number of channels available
#define ADC2_CH0    0
#define ADC2_CH1    1
#define ADC2_CH2    2
#define ADC2_CH3    3

#define LENGTH        16    // length of data read for AD7995-0, 2 bytes

//#define I2C_ADC_NC  959     // I2C disconnected value

class I2C_ADC
{
	public:
		I2C_ADC();
		void begin(void);
		uint16_t read(uint8_t channel);

		uint8_t enableADC[NUM_CHANNELS] = {0};
		
	private:
		void printBin(uint8_t inVal);
};

extern I2C_ADC ADC2;

#endif /* I2C_ADC_H_ */