/*
 * I2C_PortExpander.cpp
 *
 * Created: 11/11/2015 14:08:12
 * Author: Olly McBride
 *
 * This work is licensed under the Creative Commons Attribution 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
 *
 */ 

#include <Wire.h>
#include "I2C_PortExpander.h"

// Constructors ////////////////////////////////////////////////////////////////
I2C_PORTEXPANDER::I2C_PORTEXPANDER()
{
}

// Public Methods //////////////////////////////////////////////////////////////
void I2C_PORTEXPANDER::begin(void)
{
	#ifdef PULL_ADC_PINS_HIGH
		pinMode(A6,OUTPUT);
		pinMode(A7,OUTPUT);
		digitalWrite(A6,HIGH);
		digitalWrite(A7,HIGH);
	#endif
	
	Wire.begin();
	portMode(PORT0,OUTPUT);
	portMode(PORT1,INPUT);

	portWrite(PORT0,LOW);     // initialise PORT0 to 0
}

void I2C_PORTEXPANDER::portMode(uint8_t port, uint8_t state)
{
	uint8_t configByte;
	uint8_t txBuff;

	state = !state;

	configByte = 0x06 | port;   // select port for config (B000000110 or B000000111)
	txBuff = 0xFF * state;

	#ifdef PORTE_DEBUG
	MYSERIAL.print("portMode port");
	MYSERIAL.print(port);
	MYSERIAL.print(" state ");
	MYSERIAL.print(state);
	MYSERIAL.print("  configByte ");
	leadingZeros(configByte);
	MYSERIAL.print(configByte,BIN);
	MYSERIAL.print("  txBuff ");
	leadingZeros(txBuff);
	MYSERIAL.println(txBuff,BIN);
	#endif

	Wire.beginTransmission(PORT_EXPANDER_ADDR);
	Wire.write(configByte);
	Wire.write(txBuff);
	Wire.endTransmission();
}

void I2C_PORTEXPANDER::portWrite(uint8_t port, uint8_t state)
{
	uint8_t configByte;

	configByte = 0x02 | (port<<1);
	portMask[port] = 0xFF * port;

	#ifdef PORTE_DEBUG
	MYSERIAL.print("portWrite port");
	MYSERIAL.print(port);
	MYSERIAL.print(" state ");
	MYSERIAL.print(state);
	MYSERIAL.print("  configByte ");
	leadingZeros(configByte);
	MYSERIAL.print(configByte,BIN);
	MYSERIAL.print("  portMask");
	MYSERIAL.print(port);
	MYSERIAL.print(" ");
	leadingZeros(portMask[port]);
	MYSERIAL.println(portMask[port],BIN);
	#endif

	Wire.beginTransmission(PORT_EXPANDER_ADDR);
	Wire.write(configByte);
	Wire.write(portMask[port]);
	Wire.endTransmission();
}

void I2C_PORTEXPANDER::write(uint8_t pin, uint8_t val, uint8_t duration)
{
	uint8_t port, configByte;
	
	#ifdef PIN_LATCH
	if(duration>0)  // if latch duration entered
	{
		latchIndex++; // increase stack index
		latchStack[latchIndex][PIN] = pin;
		latchStack[latchIndex][COUNT] = duration;
		latchStack[latchIndex][STATE] = val;
	}
	#endif

	port = getPort(pin);

	configByte = 0x02 | (port<<1);    // select OUTPUT register (B00000010 or B00000011)
	
	if(val==HIGH)
	{
		portMask[port] |= 1<<(pin-(10*port));
	}
	else if (val==LOW)
	{
		portMask[port] &= ~(1<<(pin-(10*port)));
	}

	#ifdef PORTE_DEBUG
	MYSERIAL.print("write pin ");
	MYSERIAL.print(pin);
	MYSERIAL.print(" writeVal ");
	MYSERIAL.print(val);
	MYSERIAL.print(" port");
	MYSERIAL.print(port);
	MYSERIAL.print("  configByte ");
	leadingZeros(configByte);
	MYSERIAL.print(configByte,BIN);
	MYSERIAL.print("  portMask");
	MYSERIAL.print(port);
	MYSERIAL.print(" ");
	leadingZeros(portMask[port]);
	MYSERIAL.println(portMask[port],BIN);
	#endif
	
	Wire.beginTransmission(PORT_EXPANDER_ADDR);
	Wire.write(configByte);           // select output register
	Wire.write(portMask[port]);
	Wire.endTransmission();
}

void I2C_PORTEXPANDER::write(uint8_t pin, uint8_t val)
{
	write(pin,val,0);
}

uint8_t I2C_PORTEXPANDER::read(uint8_t pin)
{
	uint8_t configByte;
	uint8_t port;
	uint8_t val;

	port = getPort(pin);
	
	configByte = port<<0;               // select INPUT register (B00000000 or B00000001)
	
	Wire.beginTransmission(PORT_EXPANDER_ADDR);
	Wire.write(configByte);            // send Input register (B00000000 or B00000001)
	Wire.endTransmission();
	
	Wire.requestFrom(PORT_EXPANDER_ADDR,1);
	portMask[port] = Wire.read();

	val = (portMask[port]>>(pin-(10*port))) & 0x01;

	#ifdef PORTE_DEBUG
	MYSERIAL.print("read pin ");
	MYSERIAL.print(pin);
	MYSERIAL.print(" port");
	MYSERIAL.print(port);
	MYSERIAL.print("  configByte ");
	leadingZeros(configByte);
	MYSERIAL.print(configByte,BIN);
	MYSERIAL.print("  portMask");
	MYSERIAL.print(port);
	MYSERIAL.print(" ");
	leadingZeros(portMask[port]);
	MYSERIAL.print(portMask[port],BIN);
	MYSERIAL.print(" rxVal ");
	MYSERIAL.println(val);
	#endif

	return val;

}

// Private Methods //////////////////////////////////////////////////////////////
uint8_t I2C_PORTEXPANDER::getPort(uint8_t pin)
{
	if(pin>=10)
	{
		return PORT1;
	}
	else
	{
		return PORT0;
	}
}

void I2C_PORTEXPANDER::leadingZeros(uint8_t inVal)
{
	for (uint8_t i = 1; i < 8; i++)
	{
		if (inVal < pow(2, i)) MYSERIAL.print(B0);
	}
}

// Preinstantiate Objects //////////////////////////////////////////////////////
I2C_PORTEXPANDER portExpander = I2C_PORTEXPANDER();