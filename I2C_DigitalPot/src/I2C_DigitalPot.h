D/*
 * I2C_DigitalPot.h
 *
 * Created: 27/11/15 14:24
 * Author: Olly McBride
 *
 * This work is licensed under the Creative Commons Attribution 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
 *
 * AD5171 64 position I2C digital potentiometer
 *
 */ 

#ifndef I2C_DIGITALPOT_H_
#define I2C_DIGITALPOT_H_

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

#define DPOT_DEBUG

#define DPOT_ADDR   0x2C    // I2C ADC address configurable	010 110 AD0	 
							// AD0 = 0 0x2C 010 1100	OR		AD0 = 1 0x2D 010 1101

class D_POT
{
	public:
		D_POT();
		D_POT(bool AD0, uint16_t Rtotal);
		D_POT(bool AD0, uint16_t Rtotal, uint8_t Vin);
		void writeD(uint8_t D);
		void writeRwb(uint16_t R);
		void writeRwa(uint16_t R);
		void writeR(uint16_t R);
		void writeV(float Vw);

		void enableOTP(void);
		
	private:
		void printBin(uint8_t inVal);
		
		uint8_t _potAddr;
		bool _OTP_EN;
		uint16_t _Rab;
		uint8_t _Vin;
};

#endif /* I2C_DIGITALPOT_H_ */