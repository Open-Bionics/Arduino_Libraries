/*
 * SingleFingerBasic
 * Created: 02/11/2015 14:16:19
 * Author: Olly McBride - Open Bionics
 */ 

#include <FingerLib.h>

// uncomment one of the following to select the board
//#define ADULT_BOARD
//#define FOUR_MOTOR_BOARD
//#define CHILD_BOARD
#define WEBSITE_BOARD

// number of controllable fingers (number of motors)
#define NUM_FINGERS  1

// initialise Finger class to array, and assigns pins
Finger finger[NUM_FINGERS];

void setup()
{ 
  // MYSERIAL.print is used to allow compatability between both the Mega (Serial.print) and the Zero Native Port (SerialUSB.print)
  MYSERIAL.begin(38400);
  MYSERIAL.println("Started");

  // configure finger pins
  finger[0].attach(3,6,A1);
  MYSERIAL.println("Pins configured");
}

void loop()
{
  // set the finger to OPEN
  finger[0].open();
  MYSERIAL.println("Opening");
  // give it time to OPEN
  delay(2000);
  // set the finger to CLOSE
  finger[0].close();
  MYSERIAL.println("Closing");
  // give it time to CLOSE
  delay(2000);

}


