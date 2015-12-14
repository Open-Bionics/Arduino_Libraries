/*
 * I2C_ADC.cpp
 *
 * Created: 11/11/2015 14:08:12
 * Author: Olly McBride
 *
 * This work is licensed under the Creative Commons Attribution 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
 *
 */ 

#include <Wire.h>
#include "I2C_ADC.h"

// Constructors ////////////////////////////////////////////////////////////////
I2C_ADC::I2C_ADC()
{
}

// Public Methods //////////////////////////////////////////////////////////////
void I2C_ADC::begin()
{
	uint8_t configMask = 0xF0;       //enable CH3, CH2, CH1, CH0      B11110000
	
	#ifdef PULL_ADC_PINS_HIGH
		pinMode(A6,OUTPUT);
		pinMode(A7,OUTPUT);
		digitalWrite(A6,HIGH);
		digitalWrite(A7,HIGH);
	#endif

	enableADC[ADC2_CH0] = 1;
	enableADC[ADC2_CH1] = 1;
	enableADC[ADC2_CH2] = 1;
	enableADC[ADC2_CH3] = 1;
	
	Wire.begin();
	
	Wire.beginTransmission(ADC2_ADDR);
	Wire.write(configMask);
	Wire.endTransmission();

	#ifdef ADC2_DEBUG
	MYSERIAL.print("ADC2.begin() addr ");
	printBin(ADC2_ADDR);
	MYSERIAL.print(" configMask ");
	printBin(configMask);
	MYSERIAL.print("\n");
	#endif
}

uint16_t I2C_ADC::read(uint8_t channel)
{
	uint8_t i;
	uint8_t rxByte[2];
	uint16_t ADCval[NUM_CHANNELS];
	#ifdef ADC2_DEBUG
	MYSERIAL.print("ADC2.read() addr ");
	printBin(ADC2_ADDR);
	MYSERIAL.print(" ");
	#endif
	Wire.requestFrom(ADC2_ADDR,(NUM_CHANNELS*2));
	for(i=0;i<NUM_CHANNELS;i++)
	{
		if(enableADC[i])
		{
			rxByte[0] = Wire.read();
			rxByte[1] = Wire.read();
			ADCval[i] = ((rxByte[0]<< 6) & 0x3C0) | ((rxByte[1] >> 2) & B00111111);
			#ifdef ADC2_DEBUG
			MYSERIAL.print("rxByte[");
			MYSERIAL.print((2*i));
			MYSERIAL.print("] ");
			printBin(rxByte[0]);
			MYSERIAL.print("  rxByte[");
			MYSERIAL.print((2*i)+1);
			MYSERIAL.print("] ");
			printBin(rxByte[1]);
			MYSERIAL.print("  ");
			#endif
		}
		else
		{
			ADCval[i] = 0;
		}
	}

	#ifdef ADC2_DEBUG
	MYSERIAL.print("  ADC2.read() channel ");
	MYSERIAL.print(channel);
	MYSERIAL.print(" ADCval[] ");
	MYSERIAL.println(ADCval[channel]);
	#endif
	
	return ADCval[channel];
}

// Private Methods //////////////////////////////////////////////////////////////

void I2C_ADC::printBin(uint8_t inVal)
{
	for (uint8_t i = 1; i < 8; i++)
	{
		if (inVal < pow(2, i)) MYSERIAL.print(B0);
	}
	MYSERIAL.print(inVal,BIN);
}

// Preinstantiate Objects //////////////////////////////////////////////////////
I2C_ADC ADC2 = I2C_ADC();