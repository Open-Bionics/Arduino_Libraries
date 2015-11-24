#include <Wire.h>

// uncomment the following if using the Website Board
//#define PULL_ADC_PINS_HIGH

#include <I2C_EEPROM.h>

/*  I2C_EEPROM Test
 *  If the letter k is entered over serial, a read, write, read cycle is performed
 *  as the EEPROM location denoted by MEM_POS
 */

#define MEM_POS 12    // EEPROM location to store value
int val = 70;         // value to write to EEPROM

int rxByte[3];        // buffer to store received values


void setup()
{
  MYSERIAL.begin(38400);
  while (!MYSERIAL);        // wait for Serial port to connect
  MYSERIAL.println("Started");
  
  EEPROM2.begin();
  MYSERIAL.println("I2C Initialised");
}

void loop(void)
{
  if(MYSERIAL.available())
  {
    char in = MYSERIAL.read();
    if(in == 'k')
    {
      MYSERIAL.println("About to read");
      rxByte[0] = EEPROM2.read(MEM_POS);
      MYSERIAL.println("About to write");
      EEPROM2.write(MEM_POS, val);
      MYSERIAL.println("About to read again");
      rxByte[1] = EEPROM2.read(MEM_POS);
      MYSERIAL.println("Over \n");

      //EEPROM2.clearAll(0,128);

      delay(33);
    }
  }
}

