#include <Wire.h>
#include <I2C_DigitalPot.h>

/*  I2C_Pot Test
 *  Potential divider test mode (v)
 *    Sweep the pot from 0V to VIN in 10 steps
 *    
 *  Variable resistor test mode (r)
 *    Sweep the pot from 0R to TOTAL_R in 10 steps
 *  
 *  
 *  Note:
 *  MYSERIAL.print is used instead of Serial.Print or SerialUSB.print to allow
 *  this example to be used on both the Mega and the Zero without any modification
 */

// uncomment the following if using the Website Board
//#define PULL_ADC_PINS_HIGH

// uncomment one of the following that relates to the digital pot
//#define TOTAL_R   1000         // 1k
//#define TOTAL_R   5000         // 5k
#define TOTAL_R   10000        // 10k
//#define TOTAL_R   50000        // 50k
//#define TOTAL_R   100000       // 100k

// input voltage
#define VIN       3.3

// number of steps for test mode
#define NUM_STEPS  10

D_POT DPOT = D_POT(0,TOTAL_R,VIN);

void setup()
{
  MYSERIAL.begin(38400);
  while (!MYSERIAL);                // wait for Serial port to connect
  MYSERIAL.println("Started");

  #ifdef PULL_ADC_PINS_HIGH
    pinMode(A6,OUTPUT);
    pinMode(A7,OUTPUT);
    digitalWrite(A6,HIGH);
    digitalWrite(A7,HIGH);
  #endif
  
  MYSERIAL.println("Press 'v' to run the potential divider test, or press 'r' to run the variable resistor test");
}

void loop(void)
{
  if(MYSERIAL.available())
  {
    char in = MYSERIAL.read();
    if(in == 'v')
    {
      in = 0;
      dividerTest();
    }
    else if(in == 'c')
    {
      in = 0;
      resistorTest();
    }
  }
}

void dividerTest(void)
{
  int i;
  float Vstep;

  Vstep = (float) VIN/NUM_STEPS;

  MYSERIAL.print("Voltage divider test mode");
  MYSERIAL.print(NUM_STEPS);
  MYSERIAL.print(" from 0V to ");
  MYSERIAL.print(VIN);
  MYSERIAL.print("V, with a step size of");
  MYSERIAL.println(Vstep);

  for(i=0;i<NUM_STEPS;i++)
  {
    MYSERIAL.print("Step ");
    MYSERIAL.print(i);
    MYSERIAL.print(" \t");
    MYSERIAL.print((float)(Vstep * i));
    MYSERIAL.println("V");

    
    DPOT.writeV((float)(i*Vstep));
  }

  MYSERIAL.print("Finished, setting Vout to 0V");
  DPOT.writeV(0);
}

void resistorTest(void)
{
  int i;
  float Rstep;

  Rstep = (float) TOTAL_R/NUM_STEPS;

  MYSERIAL.print("Variable resistor test mode");
  MYSERIAL.print(NUM_STEPS);
  MYSERIAL.print(" from 0R to ");
  MYSERIAL.print(TOTAL_R);
  MYSERIAL.print("R, with a step size of");
  MYSERIAL.println(Rstep);

  for(i=0;i<NUM_STEPS;i++)
  {
    MYSERIAL.print("Step ");
    MYSERIAL.print(i);
    MYSERIAL.print(" \t");
    MYSERIAL.print((float)(Rstep * i));
    MYSERIAL.println("R");

    
    DPOT.writeR((float)(i*Rstep));
  }

  MYSERIAL.print("Finished");
  DPOT.writeR(0);
}




