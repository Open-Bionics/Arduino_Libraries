#include <Wire.h>
#include <I2C_ADC.h>

/*  I2C_ADC Test
 *  Constantly read all 4 channels and print results to Serial
 */

// uncomment the following if using the Website Board
#define PULL_ADC_PINS_HIGH

void setup()
{
  MYSERIAL.begin(38400);
  while (!MYSERIAL);        // wait for Serial port to connect
  MYSERIAL.println("Started");

  #ifdef PULL_ADC_PINS_HIGH
    pinMode(A6,OUTPUT);
    pinMode(A7,OUTPUT);
    digitalWrite(A6,HIGH);
    digitalWrite(A7,HIGH);
  #endif
  
  ADC2.begin();
  MYSERIAL.println("I2C Initialised");
}

void loop()
{
  int i;

  while(1)
  {
    for(i=0;i<NUM_CHANNELS;i++)
    {
      #ifdef ADC2_DEBUG
        ADC2.read(i);
      #else
        MYSERIAL.print(ADC2.read(i));
        MYSERIAL.print("\t ");
      #endif
    }
    MYSERIAL.print("\n");
    delay(60);
  }
}

