/*
 * CFile1.c
 *
 * Created: 02/11/2015 14:16:19
 *  Author: Olly
 */ 

#include <FingerLib.h>

// Architecture specific include to select serial
#if defined(ARDUINO_ARCH_AVR)
#define MYSERIAL Serial
#elif defined(ARDUINO_ARCH_SAMD)
#define MYSERIAL SerialUSB
#else
#error "Could not set up Serial. This library only supports boards with an AVR or SAMD processor."
#endif

// uncomment one of the following to select the board
//#define ADULT_BOARD
//#define FOUR_MOTOR_BOARD
//#define CHILD_BOARD
#define WEBSITE_BOARD

// number of controllable fingers (number of motors)
#define NUM_FINGERS  5

// uncomment one of the following to select which hand is used
int handFlag = LEFT;
//int handFlag = RIGHT;

// initialise Finger class to array
Finger finger[NUM_FINGERS];

void setup()
{
  // MYSERIAL.print is used to allow compatability between both the Mega (Serial.print) and the Zero Native Port (SerialUSB.print)
  MYSERIAL.begin(38400);
  MYSERIAL.println("Started");

  // configure all of the finger pins 
  pinAssignment();
  MYSERIAL.println("Pins configured");

}

void loop()
{
  // set all of the fingers to OPEN
  openHand();
  MYSERIAL.println("Opening");
  // give them time to OPEN
  delay(2000);
  // set all of the fingers to CLOSE
  closeHand();
  MYSERIAL.println("Closing");
  // give them time to CLOSE
  delay(2000);
}

// count through all the fingers and set them to open
void openHand(void)
{
  int i;
  for(i=0;i<NUM_FINGERS;i++)
  {
    finger[i].open();
  }
}

// count through all the fingers and set them to close
void closeHand(void)
{
  int i;
  for(i=0;i<NUM_FINGERS;i++)
  {
    finger[i].close();
  }
}

