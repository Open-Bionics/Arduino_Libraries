#include <FingerLib.h>


/*  prints all ADC vals over MYSERIAL.
 *  fingers can be controlled individually by sending finger number
 *  direction toggles at each Serial command 
 *  motors stop at limits defined by  defaultds in setLimits()
 */

// MACROS
#define IS_BETWEEN(x,a,b) ((((x)>=(a))&&((x)<=(b)))?(1):(0))        // check if value x is between values a and b

// uncomment one of the following to select the board
//#define ADULT_BOARD
//#define FOUR_MOTOR_BOARD
#define CHILD_BOARD
//#define WEBSITE_BOARD

// number of controllable fingers (number of motors)
#define NUM_FINGERS  5

// uncomment one of the following to select which hand is used
//int handFlag = LEFT;
int handFlag = RIGHT;

// initialise Finger class to array
Finger finger[NUM_FINGERS];

char* names[MAX_FINGERS] = {" m0"," m1"," m2"," m3"," m4"," m5"};
int fingerNum = 0;

void setup()
{
  MYSERIAL.begin(38400);
  while (!MYSERIAL) {
    ; // wait for serial port to connect
  }
  MYSERIAL.println("Serial Initialised");

  pinAssignment();
  MYSERIAL.println("Pins Initialised");

  // set custom speed & pos limits using setSpeedLimits(min,max) and setPosLimits(min,max)
  //for(int f=0;f<NUM_FINGERS;f++)
  //{
  //  finger[f].setSpeedLimits(150,MAX_FINGER_SPEED);
  //  finger[f].setPosLimits(100,924);
  //}
}

void loop()
{
  int i;
  if(MYSERIAL.available())
  {
    fingerNum = (int) MYSERIAL.read() - 48;
    if(IS_BETWEEN(fingerNum,0,(NUM_FINGERS-1)))
    {
      //MYSERIAL.print("Finger = ");
      //MYSERIAL.print(fingerNum);
      finger[fingerNum].open_close();     // toggle direction
    }
    else if(fingerNum == NUM_FINGERS) 
    {
      for(int i=0;i<NUM_FINGERS;i++)
      {
        finger[i].open_close();     // toggle direction 
      }
    }
    else if(fingerNum > NUM_FINGERS) 
    {
      stopMotors();
      MYSERIAL.println("MOTORS STOPPED");
      delay(500); 
    }
  }

  for(i=0;i<NUM_FINGERS;i++)
  {
    MYSERIAL.print(names[i]);
    MYSERIAL.print(" = ");
    MYSERIAL.print(finger[i].readPos());
    MYSERIAL.print("\t");
    delay(10);
  }
  MYSERIAL.println();

}

void stopMotors(void)
{
  int i;
  for(i=0;i<NUM_FINGERS;i++)
  {
    finger[i].stopMotor();
    //finger[i].writePos(finger[i].readPos());
  }
}
