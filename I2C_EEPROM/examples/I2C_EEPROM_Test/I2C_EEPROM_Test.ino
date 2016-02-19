#include <Wire.h>

// uncomment the following if using the Website Board
//#define PULL_ADC_PINS_HIGH

#include <I2C_EEPROM.h>

/*  I2C_EEPROM Test
 *  If the letter k is entered over serial, a read, write, read cycle is performed
 *  as the EEPROM location denoted by MEM_POS
 */

#define AMOUNT_TO_WRITE 1000

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

  MYSERIAL.println("Press 'k' to test the EEPROM, 'r' to read the contents (0-255) or press 'c' to clear the EEPROM (set all values to 255)");
}

void loop(void)
{
  if(MYSERIAL.available())
  {
    char in = MYSERIAL.read();
    if(in == 'k')
    {
      in = 0;
      MYSERIAL.print("About to read from pos ");
      MYSERIAL.println(MEM_POS);
      rxByte[0] = EEPROM2.read(MEM_POS);
      MYSERIAL.print("Value read = ");
      MYSERIAL.println(rxByte[0]);
      MYSERIAL.print("About to write the value ");
      MYSERIAL.print(val);
      MYSERIAL.print(" to pos ");
      MYSERIAL.println(MEM_POS);
      EEPROM2.write(MEM_POS, val);
      MYSERIAL.print("About to read again from pos ");
      MYSERIAL.println(MEM_POS);
      rxByte[1] = EEPROM2.read(MEM_POS);
      MYSERIAL.print("Value read = ");
      MYSERIAL.println(rxByte[1]);

      if(rxByte[1] == val)
        MYSERIAL.println("Test Passed ");
      else
        MYSERIAL.println("Test Failed :( ");
      
      MYSERIAL.println("Test Complete \n");

      delay(33);
    }
    else if(in == 'c')
    {
      in = 0;
      MYSERIAL.println("Clearing EEPROM");
      EEPROM2.clearAll(0,AMOUNT_TO_WRITE);
      MYSERIAL.println("Clear Complete \n");
    }
    else if(in == 'r')
    {
      in = 0;
      int i;
      for(i=0;i<AMOUNT_TO_WRITE;i++)
      {
        MYSERIAL.print(i);
        MYSERIAL.print(" ");
        MYSERIAL.println(EEPROM2.read(i));
      }
    }
  }
}

