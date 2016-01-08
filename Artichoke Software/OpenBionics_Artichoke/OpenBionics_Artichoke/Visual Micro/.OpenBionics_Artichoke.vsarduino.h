/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino/Genuino Mega w/ ATmega2560 (Mega 2560), Platform=avr, Package=arduino
*/

#define __AVR_ATmega2560__
#define ARDUINO 165
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define F_CPU 16000000L
#define ARDUINO 165
#define ARDUINO_AVR_MEGA2560
#define ARDUINO_ARCH_AVR
extern "C" void __cxa_pure_virtual() {;}

//
//
void gripMovement(int gripNum, int inPos, int dir, int speed);
void gripMovement(int gripNum, int inPos);
void demoMode(void);
void fingerRoll(int del,int dir);
void printGrip(int grip, int dir);
void fingerControl(int fingerNumber, int stopPos, int fingerDirection, int fingerSpeed);
void stopMotor(int fingerNumber);
void stopMotors(void);
void muscleControl(void);
void runNoiseFloorCalc(void);
int readMuscle(int muscleNum);
int readMuscleSensor(int muscleNum);
void checkActivation(int* signal, bool* activation);
int calcNoiseFloor(int muscleNum, int reading);
int calcThresh(int muscleNum, int noise);
void determineArmDirection(int* signal);
void checkGripChange(void);
int gripChange(void);
int handIsCurrentlyOpen(void);
int intervalEllapsed(long interval);
void standardMuscleControl(int muscleDir);
void positionalMuscleControl(int *signal);
void printADCValues(int* signal, bool* activation, int dir);
void muscleGraph(void);
void IOconfig(void);
void pinAssignment(void);
void serialEvent();
bool codeSeen(char code);
float codeVal();
float parsenumber(char code);
void getCmd(void);
void processCommand(void);
void constrainReceivedValues(void);
void manageSerialSettings(void);
void setDefaults(void);
void clearAll(void);
void initialEEPROMconfig(void);
void startUpMessages(void);
void printInstructions(void);
void timerSetup(void);
void milliSecInterrupt(void);
double customSeconds(void);
int arrayMean(int *inArray, int size);
int rollingMean(int *inArray, int size, int val);
int rollingMode(int *inArray, int size, int val);
void rollingBuff(int *inArray, int size, int val);
void printArray(int* inArray, int arraySize);
void printArray(char* inArray, int arraySize);
void overwriteArray(int* inArray, int arraySize, int val);
long map(long x, long in_min, long in_max, long out_min, long out_max);

#include "C:\Program Files (x86)\Arduino\hardware\arduino\avr\variants\mega\pins_arduino.h" 
#include "C:\Program Files (x86)\Arduino\hardware\arduino\avr\cores\arduino\arduino.h"
#include <OpenBionics_Artichoke.ino>
#include <Animation.ino>
#include <AnimationPositions.h>
#include <CustomGrip.ino>
#include <Demo.ino>
#include <Globals.h>
#include <Globals.ino>
#include <MotorControl.ino>
#include <MuscleSense.ino>
#include <PinManagement.ino>
#include <SerialControl.ino>
#include <Timer.ino>
#include <Utils.h>
#include <Utils.ino>
