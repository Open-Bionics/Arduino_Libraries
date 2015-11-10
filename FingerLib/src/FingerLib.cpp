/*
 * FingerLib.cpp
 *
 * Created: 11/2/2015 11:10:43 AM
 * Author: Olly McBride
 *
 * This work is licensed under the Creative Commons Attribution 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
 *
 */ 

#include "FingerLib.h"


uint8_t TotalFingerCount = 0;					// the total number of attached fingers
static finger_t fingers[MAX_FINGERS];			// static array of finger structures
static char* dirString[2] = {"OPEN","CLOSE"};	// direction string for printDir()
	
static bool timerSetupFlag = false;	
	
/*Finger::Finger(int board, int left_right)
{
	brd = board;
	handSide = left_right;

	attachAll();
	
	if (fingerIndex > MAX_FINGERS) 
	{
		MYSERIAL.println("ERROR - Too many fingers attached");
		while(1);
	}
}*/

Finger::Finger()
{
	if (fingerIndex < MAX_FINGERS)
	{
		fingerIndex = TotalFingerCount++;                    // assign a servo index to this instance
	}
	else
	{
		fingerIndex = -1;  // too many servos
		MYSERIAL.println("ERROR - Too many fingers attached");
		while(1);
	}
}

uint8_t Finger::attach(int dir0, int dir1, int sense)
{
	//static bool timerSetupFlag = false;
	
	if (fingerIndex < MAX_FINGERS) {
		// configure pins
		pinMode(dir0, OUTPUT);                           // set direction1 pin to output
		pinMode(dir1, OUTPUT);                           // set direction2 pin to output
		pinMode(sense, INPUT);                           // set sense pin to input
		// attach all finger pins
		fingers[fingerIndex].Pin.dir[0] = dir0;
		fingers[fingerIndex].Pin.dir[1] = dir1;
		fingers[fingerIndex].Pin.sns = sense;
		// set limits and initial values
		setPosLimits(MIN_FINGER_POS,MAX_FINGER_POS);
		setSpeedLimits(MIN_FINGER_SPEED,MAX_FINGER_SPEED);
		writeSpeed(MAX_FINGER_SPEED);
		// initialise the timer
		if(timerSetupFlag == false)
		{
			passPtr(&fingerPosCtrl);
			timerSetup();			
			timerSetupFlag = true;
		}
		fingers[fingerIndex].Pin.isActive = true;		// this must be set after the check
	}
	
	return fingerIndex;
}

/*
void Finger::attachAll(void)
{	
	// Architecture specific include
	#if defined(ARDUINO_ARCH_SAMD)
	if(brd == ADULT_BOARD)
	{
		TotalFingerCount = 5;
		if(handSide == RIGHT)
		{
			attach(8,9,A4);
			attach(7,6,A3);
			attach(5,12,A2);
			attach(2,10,A1);
			attach(4,3,A0);
		}
		else if(handSide == LEFT)
		{
			attach(8,9,A4);
			attach(4,3,A0);
			attach(2,10,A1);
			attach(5,12,A2);
			attach(7,6,A3);
		}
	}
	else if(brd == FOUR_MOTOR_BOARD)
	{
		TotalFingerCount = 4;
		if(handSide == RIGHT)
		{
			attach(8,9,A4);
			attach(7,6,A3);
			attach(5,12,A2);
			attach(2,10,A1);
		}
		else if(handSide == LEFT)
		{
			attach(8,9,A4);
			attach(7,6,A3);
			attach(5,12,A2);
			attach(2,10,A1);
		}
	}
	else if(brd == CHILD_BOARD)
	{
		TotalFingerCount = 3;
		if(handSide == RIGHT)
		{
			attach(2,10,A1);
			attach(8,9,A4);
			attach(4,3,A0);
		}
		else if(handSide == LEFT)
		{
			attach(2,10,A1);
			attach(4,3,A0);
			attach(8,9,A4);
		}
	}
	#elif defined(ARDUINO_ARCH_AVR)
	if(brd == WEBSITE_BOARD)
	{
		//TotalFingerCount = 5;
		if(handSide == RIGHT)
		{
			fingerIndex = 0;
			attach(5,2,A0);
			fingerIndex++;
			attach(3,6,A1);
			fingerIndex++;
			attach(7,8,A2);
			fingerIndex++;
			attach(10,9,A3);
			fingerIndex++;
			attach(11,12,A4);
		}
		else if(handSide == LEFT)
		{
			attach(5,2,A0);
			attach(11,12,A4);
			attach(10,9,A3);
			attach(7,8,A2);
			attach(3,6,A1);
		}
	}
	#endif
}*/

void Finger::detach(void)
{
	fingers[fingerIndex].Pin.isActive = false;
}

bool Finger::attached(void)
{
	return fingers[fingerIndex].Pin.isActive;
}

void Finger::writePos(int value)
{
	// constrain position value to limits
	fingers[fingerIndex].TargPos = constrain(value,fingers[fingerIndex].MinPos,fingers[fingerIndex].MaxPos);
	// calcuate new position error (to remove false positives in reachedPos() )
	fingers[fingerIndex].CurrErr = (signed int) (fingers[fingerIndex].TargPos - fingers[fingerIndex].CurrPos);
	//determine direction of travel
	if(value > fingers[fingerIndex].CurrPos)
		fingers[fingerIndex].CurrDir = CLOSE;
	else
		fingers[fingerIndex].CurrDir = OPEN;	
	
}

void Finger::writeDir(int value)
{
	/*// toggle direction
	fingers[fingerIndex].CurrDir = !fingers[fingerIndex].CurrDir;
	// set target position using limits
	if(fingers[fingerIndex].CurrDir == OPEN)
		fingers[fingerIndex].TargPos = fingers[fingerIndex].MinPos;
	else if(fingers[fingerIndex].CurrDir == CLOSE)
		fingers[fingerIndex].TargPos = fingers[fingerIndex].MaxPos;*/
	
	// toggle direction
	fingers[fingerIndex].CurrDir = !fingers[fingerIndex].CurrDir;
	// set target position using limits
	if(fingers[fingerIndex].CurrDir == OPEN)
	writePos(fingers[fingerIndex].MinPos);
	else if(fingers[fingerIndex].CurrDir == CLOSE)
	writePos(fingers[fingerIndex].MaxPos);
}

void Finger::writeSpeed(int value)
{
	fingers[fingerIndex].TargSpeed = constrain(value,fingers[fingerIndex].MinSpeed,fingers[fingerIndex].MaxSpeed);
}

uint8_t Finger::readDir(void)
{
	return fingers[fingerIndex].CurrDir;
}

uint16_t Finger::readPos(void)
{
	return fingers[fingerIndex].CurrPos;
}

int16_t Finger::readPosError(void)
{
	return fingers[fingerIndex].CurrErr;
}

uint8_t Finger::readSpeed(void)
{
	return fingers[fingerIndex].CurrSpeed;
}

void Finger::setPosLimits(int min, int max)
{
	// set limits
	fingers[fingerIndex].MinPos = min;
	fingers[fingerIndex].MaxPos = max;
}

void Finger::setSpeedLimits(int min, int max)
{
	// set limits
	fingers[fingerIndex].MinSpeed = min;
	fingers[fingerIndex].MaxSpeed = max;
}

void Finger::stopMotor(void)
{
	// set motor speed to 0
	fingers[fingerIndex].TargSpeed = 0;
}

bool Finger::reachedPos(void)
{
	// return 1 if motor reaches target position
	if(abs(readPosError()) < POS_REACHED_TOLERANCE)
	return 1;
	else
	return 0;
}

bool Finger::reachedPos(uint16_t posErr)
{
	// return 1 if motor reaches custom target position
	if(abs(readPosError()) < posErr)
	return 1;
	else
	return 0;
}

void Finger::open(void)
{
	writePos(fingers[fingerIndex].MinPos);
}

void Finger::close(void)
{
	writePos(fingers[fingerIndex].MaxPos);
}


void Finger::printSpeed(void)
{
	printSpeed(0);
}

void Finger::printSpeed(int newL)
{
	MYSERIAL.print("Speed ");
	MYSERIAL.print(readSpeed());
	MYSERIAL.print("  ");
	if(newL)
		MYSERIAL.print("\n");
}

void Finger::printPos(void)
{
	printPos(0);
}

void Finger::printPos(int newL)
{
	MYSERIAL.print("Pos ");
	MYSERIAL.print(readPos());
	MYSERIAL.print("  ");
	if(newL)
	MYSERIAL.print("\n");
}

void Finger::printPosError(void)
{
	printPosError(0);
}

void Finger::printPosError(int newL)
{
	MYSERIAL.print("Err ");
	MYSERIAL.print(readPosError());
	MYSERIAL.print("  ");
	if(newL)
	MYSERIAL.print("\n");
}

void Finger::printDir(void)
{
	printDir(0);
}

void Finger::printDir(int newL)
{
	MYSERIAL.print("Dir ");
	MYSERIAL.print(dirString[readDir()]);
	MYSERIAL.print("  ");
	if(newL)
	MYSERIAL.print("\n");
}

void Finger::printReached(void)
{
	printReached(0);
}

void Finger::printReached(int newL)
{
	MYSERIAL.print("Reached ");
	MYSERIAL.print(reachedPos());
	MYSERIAL.print("  ");
	if(newL)
	MYSERIAL.print("\n");
}

void Finger::printDetails(void)
{
	MYSERIAL.print("Finger ");
	MYSERIAL.print(fingerIndex);
	MYSERIAL.print("  ");
	printSpeed();
	printPos();
	printDir();
	printReached(1);

}

// controls motor PWM values based on current and target position using a proportional controller (triggered by interrupt)
// To Do: Update motor controller
void fingerPosCtrl(void)  
{
	static int fingerCounter = 0;	// counts through attached fingers
	
	signed int motorSpeed = 0;			// used to calculate the motor speed as a vector (±255)
	float m;							// the proportional gradient 
	signed int vectorise = 1;			// changes the sign '±' of the value
	
	int proportionalOffset  = 150;         
	signed int motorStopOffset = 20;      
	
	
	if(TotalFingerCount > 0)			// if fingers are attached
	{
		// count through each finger at each function call
		if(fingerCounter < (TotalFingerCount - 1))	fingerCounter++;
		else fingerCounter = 0;  
		
		// read position and calc positional error
		fingers[fingerCounter].CurrPos = analogRead(fingers[fingerCounter].Pin.sns); 
		fingers[fingerCounter].CurrErr = (signed int) (fingers[fingerCounter].TargPos - fingers[fingerCounter].CurrPos);

		// speed/position line gradient (see graph below for explanation)
		m = (float) (((float) fingers[fingerCounter].TargSpeed)/((float) proportionalOffset)); 

		// change the ± sign on the motorSpeed depending on required direction
		if(fingers[fingerCounter].CurrErr>=0)   
		{
			vectorise = -1;
		}

		// constrain speed
		if(abs(fingers[fingerCounter].CurrErr) < motorStopOffset) motorSpeed = 0;              // motor dead zone
		else if(fingers[fingerCounter].CurrErr > (proportionalOffset + motorStopOffset)) motorSpeed = fingers[fingerCounter].TargSpeed;        // set to max speed speed depending on direction
		else if(fingers[fingerCounter].CurrErr < -(proportionalOffset + motorStopOffset)) motorSpeed = -fingers[fingerCounter].TargSpeed;      // set to max speed speed depending on direction
		else if(abs(fingers[fingerCounter].CurrErr) <= (proportionalOffset + motorStopOffset))
		{
			motorSpeed = (m * (fingers[fingerCounter].CurrErr + (motorStopOffset * vectorise))) - (fingers[fingerCounter].MinSpeed * vectorise); // proportional control
		}
		
		// constrain speed to limits
		motorSpeed = constrain(motorSpeed,-((signed int)fingers[fingerCounter].MaxSpeed) ,(signed int)fingers[fingerCounter].MaxSpeed);
		
		// send speed to motors
		motorControl(fingerCounter, motorSpeed);
	}
}

void motorControl(int fNum, signed int motorSpeed)
{
	bool direction = 0;
	
	// split vectorised speed into speed and direction elements, and limit the results
	if(motorSpeed < -((signed int)fingers[fNum].MinSpeed))    
	{
		(motorSpeed < -fingers[fNum].MaxSpeed) ? motorSpeed = fingers[fNum].MaxSpeed : motorSpeed = -motorSpeed;
		direction = OPEN;
	}
	else if(motorSpeed > ((signed int)fingers[fNum].MinSpeed))
	{
		(motorSpeed > fingers[fNum].MaxSpeed) ? motorSpeed = fingers[fNum].MaxSpeed : motorSpeed;
		direction = CLOSE;
	}
	else motorSpeed = 0;
	
	// store current speed
	fingers[fNum].CurrSpeed = motorSpeed;
	
	// write the speed to the motors
	analogWrite(fingers[fNum].Pin.dir[direction],motorSpeed);   //write fingerSpeed to one direction pin
	analogWrite(fingers[fNum].Pin.dir[!direction],0);			//write 0 to other direction pin
}