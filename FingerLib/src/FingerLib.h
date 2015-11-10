/*
 * FingerLib.h
 *
 * Created: 02/11/2015 11:12:48
 * Author: Olly McBride
 *
 * This work is licensed under the Creative Commons Attribution 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
 *
 */ 

#ifndef FINGERLIB_H_
#define FINGERLIB_H_

#include <inttypes.h>
#include <Arduino.h>

// Architecture specific include
#if defined(ARDUINO_ARCH_AVR)
#define MYSERIAL Serial
#include "timers/avr_FingerTimer.h"
#elif defined(ARDUINO_ARCH_SAMD)
#define MYSERIAL SerialUSB
#include "timers/samd_FingerTimer.h"
#else
#error "This library only supports boards with an AVR or SAMD processor."
#endif

#define OPEN	0
#define	CLOSE	1
#define RIGHT	1	// handFlag
#define LEFT	2	// handFlag

// #define ADULT_BOARD			5
// #define FOUR_MOTOR_BOARD	4
// #define CHILD_BOARD			3
// #define WEBSITE_BOARD		2

#define MAX_FINGERS			6			// maximum number of fingers
#define MAX_FINGER_SPEED	255			// maximum motor speed
#define MIN_FINGER_SPEED	150			// minimum motor speed
#define MAX_FINGER_POS		975			// maximum motor position
#define MIN_FINGER_POS		50			// minimum motor position
#define POS_REACHED_TOLERANCE	50		// tolerance for posReached()

typedef struct  {
	uint8_t dir[2];
	uint8_t sns;
	uint8_t isActive	:1 ;             // true if this channel is enabled
} FingerPin_t;

typedef struct {
	FingerPin_t Pin;
	uint8_t CurrDir;
	uint16_t TargSpeed;
	int16_t CurrSpeed;
	uint16_t MinSpeed;
	uint16_t MaxSpeed;
	uint16_t TargPos;
	uint16_t CurrPos;
	int16_t CurrErr;
	uint16_t MinPos;
	uint16_t MaxPos;
} finger_t;

class Finger
{
	public:
		Finger();
		Finger(int board, int left_right);
		uint8_t attach(int d0, int d1, int sense);				// attach direction & sense pins of a finger    
		//void attachAll(void);
		void detach(void);
		bool attached(void);                   
		void writePos(int value);   
		void writeDir(int value);   
		void writeSpeed(int value);   
		uint16_t readPos(void);
		int16_t readPosError(void);
		uint8_t readDir(void);
		uint8_t readSpeed(void);
		void setPosLimits(int min, int max);
		void setSpeedLimits(int min, int max); 
		
		void stopMotor(void);				// stop single motor
		bool reachedPos(void);				// returns if position reached
		bool reachedPos(uint16_t posErr);	// returns if position reached
		void open(void);					// open finger to min position
		void close(void);					// close finger to max position
		
		void printSpeed(void);
		void printSpeed(int newL);
		void printPos(void);
		void printPos(int newL);
		void printPosError(void);
		void printPosError(int newL);
		void printDir(void);
		void printDir(int newL);
		void printReached(void);
		void printReached(int newL);
		void printDetails(void);
		
	private:    
 		uint8_t fingerIndex;			// index into the channel data for this finger
		uint8_t brd;					// board
		uint8_t handSide;				// Left/Right
};

void motorControl(int fNum, signed int motorSpeed);
void fingerPosCtrl(void);

#endif /* FINGERLIB_H_ */