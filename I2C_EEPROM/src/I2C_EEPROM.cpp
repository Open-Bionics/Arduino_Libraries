/*
 * I2C_EEPROM.cpp
 *
 * Created: 11/11/2015 14:08:12
 * Author: Olly McBride
 *
 * This work is licensed under the Creative Commons Attribution 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
 *
 */ 

#include <Wire.h>
#include "I2C_EEPROM.h"

// Constructors ////////////////////////////////////////////////////////////////
I2C_EEPROM::I2C_EEPROM()
{
}

// Public Methods //////////////////////////////////////////////////////////////
void I2C_EEPROM::begin(void)
{
	#ifdef PULL_ADC_PINS_HIGH
		pinMode(A6,OUTPUT);
		pinMode(A7,OUTPUT);
		digitalWrite(A6,HIGH);
		digitalWrite(A7,HIGH);
	#endif
	
	Wire.begin();
}

uint8_t I2C_EEPROM::read(uint16_t addr)
{
	uint8_t rxByte;

	Wire.beginTransmission(EEPROM_ADDR);
	writeAddr(addr);
	Wire.endTransmission(RESTART);
	rxByte = readLast();
	
	#ifdef EEPROM2_DEBUG
	MYSERIAL.print("read chipAddr ");
	printBin(EEPROM_ADDR);
	MYSERIAL.print(" loc ");
	MYSERIAL.print(addr);
	MYSERIAL.print(" received ");
	MYSERIAL.println(rxByte);
	#endif
	
	return rxByte;
}

uint8_t I2C_EEPROM::readLast(void)
{
	uint8_t rxByte;
	Wire.requestFrom(EEPROM_ADDR,1);
	rxByte = Wire.read();
	return rxByte;
}

void I2C_EEPROM::readPage(uint16_t addr, uint8_t* val, uint8_t numToRead)
{
	numToRead = constrain(numToRead,0,127);
	Wire.beginTransmission(EEPROM_ADDR);
	writeAddr(addr);
	Wire.endTransmission();
	Wire.requestFrom(EEPROM_ADDR,(int)numToRead);
	for(int i=0;i<numToRead;i++)
	{
		val[i] = Wire.read();
	}
	Wire.endTransmission();
}

void I2C_EEPROM::write(uint16_t addr, uint8_t val)
{
	Wire.beginTransmission(EEPROM_ADDR);
	writeAddr(addr);
	Wire.write(val);
	Wire.endTransmission();
	#ifdef EEPROM2_DEBUG
	MYSERIAL.print(val);
	MYSERIAL.println(" written");
	#endif
	delay(TWR);
}

void I2C_EEPROM::writePage(uint16_t addr, uint8_t* val, uint8_t numToWrite)
{
	Wire.beginTransmission(EEPROM_ADDR);
	writeAddr(addr);
	for(int i=0;i<numToWrite;i++)
	{
		Wire.write(val[i]);
	}
	Wire.endTransmission();
	delay(TWR);
}

void I2C_EEPROM::writeAll(uint8_t val, uint16_t start, uint16_t stop)
{
	int i;
	for(i=start;i<=stop;i++)
	{
		write(i,val);
	}
}

void I2C_EEPROM::clearAll(uint16_t start, uint16_t stop)
{
	writeAll(255,start,stop);
}

// Private Methods //////////////////////////////////////////////////////////////
void I2C_EEPROM::pollACK(void)
{
	// sending the Start condition followed by the addr word + R/nW.
	// if internal wrist cycle has completed, device responds with 0
	
}

void I2C_EEPROM::writeAddr(uint16_t addr)
{
	Wire.write((uint8_t)((addr>>8)&0xff));   // MSB
	Wire.write((uint8_t)(addr&0xff));        // LSB
}

void I2C_EEPROM::printBin(uint8_t inVal)
{
	for (uint8_t i = 1; i < 8; i++)
	{
		if (inVal < pow(2, i)) MYSERIAL.print(B0);
	}
	MYSERIAL.print(inVal,BIN);
}

// Preinstantiate Objects //////////////////////////////////////////////////////
I2C_EEPROM EEPROM2 = I2C_EEPROM();