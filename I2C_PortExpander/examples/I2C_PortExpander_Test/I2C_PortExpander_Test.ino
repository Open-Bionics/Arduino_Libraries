#include <Wire.h>

// uncomment the following if using the Website Board
//#define PULL_ADC_PINS_HIGH

#include <I2C_PortExpander.h>

/*  I2C_PortExpander Test
 *  Blink all intialised pins, or toggle LED on button press
 */


void setup(void)
{
  MYSERIAL.begin(38400);
  while (!MYSERIAL);        // wait for Serial port to connect
  MYSERIAL.println("Started");
  
  portExpander.begin();
  MYSERIAL.println("I2C Initialised");

  // turn on LED to show initialisation complete
  portExpander.write(ARM_LED,HIGH);
}

void loop(void)
{
  int toggle = 0;
  int prev = 0;
  int cur = 0;
  while(1)
  {
    // blink all initialised pins
    MYSERIAL.print("Turning them all on...");
    portExpander.write(ARM_LED,HIGH);
    portExpander.write(HAP_M0,HIGH);
    portExpander.write(HAP_M1,HIGH);
    portExpander.write(HAP_M2,HIGH);
    portExpander.write(HAP_M3,HIGH);
    portExpander.write(HAP_M4,HIGH);
    MYSERIAL.println(" Complete");
    delay(1000);
    MYSERIAL.print("Turning them all off...");
    portExpander.write(ARM_LED,LOW);
    portExpander.write(HAP_M0,LOW);
    portExpander.write(HAP_M1,LOW);
    portExpander.write(HAP_M2,LOW);
    portExpander.write(HAP_M3,LOW);
    portExpander.write(HAP_M4,LOW);
    MYSERIAL.println(" Complete");
    delay(1000);

    
    /* // toggle the LED on the button press
    prev = cur;
    cur = portExpander.read(ARM_BTN);
    if((!prev)&&cur)toggle = !toggle;
    portExpander.write(ARM_LED,toggle);
    */

    
  }
}
  
  
