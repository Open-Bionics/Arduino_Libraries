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


uint8_t _TotalFingerCount = 0;					// the total number of attached _fingers
static finger_t _fingers[MAX_FINGERS];			// static array of finger structures
static char* _dirString[2] = {"OPEN","CLOSE"};	// direction string for printDir()
	
bool _timerSetupFlag = false;					// flag to prevent multiple timer intialisations

// Constructors ////////////////////////////////////////////////////////////////
Finger::Finger()
{
	if (fingerIndex < MAX_FINGERS)
	{
		fingerIndex = _TotalFingerCount++;                    // assign a servo index to this instance
	}
	else
	{
		fingerIndex = -1;  // too many servos
		MYSERIAL.println("ERROR - Too many _fingers attached");
		while(1);
	}
}

// Public Methods //////////////////////////////////////////////////////////////
uint8_t Finger::attach(int dir0, int dir1, int sense)
{
	if (fingerIndex < MAX_FINGERS) 
	{
		// configure pins
		pinMode(dir0, OUTPUT);                           // set direction1 pin to output
		pinMode(dir1, OUTPUT);                           // set direction2 pin to output
		pinMode(sense, INPUT);                           // set sense pin to input
		// attach all finger pins
		_fingers[fingerIndex].Pin.dir[0] = dir0;
		_fingers[fingerIndex].Pin.dir[1] = dir1;
		_fingers[fingerIndex].Pin.sns = sense;
		// set limits and initial values
		setPosLimits(MIN_FINGER_POS,MAX_FINGER_POS);
		setSpeedLimits(MIN_FINGER_SPEED,MAX_FINGER_SPEED);
		writeSpeed(MAX_FINGER_SPEED);
		// enable the motor
		_fingers[fingerIndex].motorEn = true;
		// initialise the timer
		if(_timerSetupFlag == false)
		{
			_passMotorPtr(&fingerPosCtrl);
			_timerSetup();			
			_timerSetupFlag = true;
		}
		_fingers[fingerIndex].Pin.isActive = true;		// this must be set after the check
	}
	
	return fingerIndex;
}

void Finger::detach(void)
{
	_fingers[fingerIndex].Pin.isActive = false;
}

bool Finger::attached(void)
{
	return _fingers[fingerIndex].Pin.isActive;
}

void Finger::writePos(int value)
{
	// constrain position value to limits
	_fingers[fingerIndex].TargPos = constrain(value,_fingers[fingerIndex].MinPos,_fingers[fingerIndex].MaxPos);
	// calcuate new position error (to remove false positives in reachedPos() )
	_fingers[fingerIndex].CurrErr = (signed int) (_fingers[fingerIndex].TargPos - _fingers[fingerIndex].CurrPos);
	//determine direction of travel
	if(value > _fingers[fingerIndex].CurrPos)
		_fingers[fingerIndex].CurrDir = CLOSE;
	else
		_fingers[fingerIndex].CurrDir = OPEN;	
	
}

void Finger::writeDir(int value)
{
	// toggle direction
	_fingers[fingerIndex].CurrDir = !_fingers[fingerIndex].CurrDir;
	// set target position using limits
	if(_fingers[fingerIndex].CurrDir == OPEN)
		writePos(_fingers[fingerIndex].MinPos);
	else if(_fingers[fingerIndex].CurrDir == CLOSE)
		writePos(_fingers[fingerIndex].MaxPos);
}

void Finger::writeSpeed(int value)
{
	_fingers[fingerIndex].TargSpeed = constrain(value,_fingers[fingerIndex].MinSpeed,_fingers[fingerIndex].MaxSpeed);
}

uint8_t Finger::readDir(void)
{
	return _fingers[fingerIndex].CurrDir;
}

uint16_t Finger::readPos(void)
{
	return _fingers[fingerIndex].CurrPos;
}

int16_t Finger::readPosError(void)
{
	return _fingers[fingerIndex].CurrErr;
}

uint8_t Finger::readSpeed(void)
{
	return _fingers[fingerIndex].CurrSpeed;
}

void Finger::setPosLimits(int min, int max)
{
	// set limits
	_fingers[fingerIndex].MinPos = min;
	_fingers[fingerIndex].MaxPos = max;
}

void Finger::setSpeedLimits(int min, int max)
{
	// set limits
	_fingers[fingerIndex].MinSpeed = min;
	_fingers[fingerIndex].MaxSpeed = max;
}

void Finger::stopMotor(void)
{
	// set motor speed to 0
	//_fingers[fingerIndex].TargSpeed = 0;
	writePos(readPos());			// set target position to current pos
}

void Finger::disableMotor(void)
{
	// set motor speed to 0
	//_fingers[fingerIndex].TargSpeed = 0;
	_fingers[fingerIndex].motorEn = false;
}

void Finger::enableMotor(void)
{
	// reset motor speed to max limit
	//_fingers[fingerIndex].TargSpeed = MAX_FINGER_SPEED;
	_fingers[fingerIndex].motorEn = true;
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
	writePos(_fingers[fingerIndex].MinPos);
}

void Finger::close(void)
{
	writePos(_fingers[fingerIndex].MaxPos);
}


void Finger::open_close(boolean dir)
{
	if(dir == OPEN)
		open();
	else if(dir == CLOSE)
		close();
}

void Finger::open_close(void)
{
	open_close(!readDir());
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
	MYSERIAL.print(_dirString[readDir()]);
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
	static int fingerCounter = 0;	// counts through attached _fingers
	
	signed int motorSpeed = 0;			// used to calculate the motor speed as a vector (±255)
	float m;							// the proportional gradient 
	signed int vectorise = 1;			// changes the sign '±' of the value
	
	int proportionalOffset  = 150;         
	signed int motorStopOffset = 20;      
	
	
	if(_TotalFingerCount > 0)			// if _fingers are attached
	{
		// count through each finger at each function call
		if(fingerCounter < (_TotalFingerCount - 1))	fingerCounter++;
		else fingerCounter = 0;  
		
		// read position and calc positional error
		_fingers[fingerCounter].CurrPos = analogRead(_fingers[fingerCounter].Pin.sns); 
		_fingers[fingerCounter].CurrErr = (signed int) (_fingers[fingerCounter].TargPos - _fingers[fingerCounter].CurrPos);

		// speed/position line gradient (see graph below for explanation)
		m = (float) (((float) _fingers[fingerCounter].TargSpeed)/((float) proportionalOffset)); 

		// change the ± sign on the motorSpeed depending on required direction
		if(_fingers[fingerCounter].CurrErr>=0)   
		{
			vectorise = -1;
		}

		// constrain speed
		if(abs(_fingers[fingerCounter].CurrErr) < motorStopOffset) motorSpeed = 0;              // motor dead zone
		else if(_fingers[fingerCounter].CurrErr > (proportionalOffset + motorStopOffset)) motorSpeed = _fingers[fingerCounter].TargSpeed;        // set to max speed speed depending on direction
		else if(_fingers[fingerCounter].CurrErr < -(proportionalOffset + motorStopOffset)) motorSpeed = -_fingers[fingerCounter].TargSpeed;      // set to max speed speed depending on direction
		else if(abs(_fingers[fingerCounter].CurrErr) <= (proportionalOffset + motorStopOffset))
		{
			motorSpeed = (m * (_fingers[fingerCounter].CurrErr + (motorStopOffset * vectorise))) - (_fingers[fingerCounter].MinSpeed * vectorise); // proportional control
		}
		
		// constrain speed to limits
		motorSpeed = constrain(motorSpeed,-((signed int)_fingers[fingerCounter].MaxSpeed) ,(signed int)_fingers[fingerCounter].MaxSpeed);
		
		// if motor disabled, set speed to 0
		if(!_fingers[fingerCounter].motorEn)
			motorSpeed = 0;
		
		// send speed to motors
		motorControl(fingerCounter, motorSpeed);
	}
}

void motorControl(int fNum, signed int motorSpeed)
{
	bool direction = 0;
	
	// split vectorised speed into speed and direction elements, and limit the results
	if(motorSpeed < -((signed int)_fingers[fNum].MinSpeed))    
	{
		(motorSpeed < -_fingers[fNum].MaxSpeed) ? motorSpeed = _fingers[fNum].MaxSpeed : motorSpeed = -motorSpeed;
		direction = OPEN;
	}
	else if(motorSpeed > ((signed int)_fingers[fNum].MinSpeed))
	{
		(motorSpeed > _fingers[fNum].MaxSpeed) ? motorSpeed = _fingers[fNum].MaxSpeed : motorSpeed;
		direction = CLOSE;
	}
	else motorSpeed = 0;
	
	// store current speed
	_fingers[fNum].CurrSpeed = motorSpeed;
	
	// write the speed to the motors
	analogWrite(_fingers[fNum].Pin.dir[direction],motorSpeed);   //write fingerSpeed to one direction pin
	analogWrite(_fingers[fNum].Pin.dir[!direction],0);			//write 0 to other direction pin
}