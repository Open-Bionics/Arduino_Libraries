/*
 * I2C_EEPROM.h
 *
 * Created: 11/11/2015 14:08:12
 * Author: Olly McBride
 *
 * This work is licensed under the Creative Commons Attribution 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
 *
 */ 

#ifndef I2C_EEPROM_H_
#define I2C_EEPROM_H_

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

//#define EEPROM2_DEBUG

#define EEPROM_ADDR 0x50  // B01010000
#define TWR 5             // num mS after EEPROM write
#define RESTART 0         // send RESTART instead of STOP

class I2C_EEPROM
{	
	public:
		I2C_EEPROM();
		void begin(void);
		uint8_t read(uint16_t addr);
		uint8_t readLast(void);
		void readPage(uint16_t addr, uint8_t* val, uint8_t numToRead);
		void write(uint16_t addr, uint8_t val);
		void writePage(uint16_t addr, uint8_t* val, uint8_t numToWrite);
		void writeAll(uint8_t val, uint16_t start, uint16_t stop);
		void clearAll(uint16_t start, uint16_t stop);
		//void update(uint16_t idx, uint8_t val);
	
	private:
		void pollACK(void);
		void writeAddr(uint16_t addr);
		void printBin(uint8_t inVal);
};

extern I2C_EEPROM EEPROM2;

#endif /* I2C_EEPROM_H_ */