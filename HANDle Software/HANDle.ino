/*	HANDle - Open Bionics
*	Author - Olly McBride
*	Date - January 2016
*
*	This work is licensed under the Creative Commons Attribution-ShareAlike 4.0 International License.
*	To view a copy of this license, visit http://creativecommons.org/licenses/by-sa/4.0/.
*
*	HANDle
*
*/

// slightly modified version of WiiChuck
#include <Wiichuck.h>     // can be downloaded from https://github.com/nicolaskruchten/arduino/tree/master/libraries/WiiChuck


// Comment out the following line to prevent 


Wiichuck wii;

void HANDleMain(void)
{
	static bool initialised = false;		// initialised flag
	static float HANDlePos = 0;				// starting pos for the HANDle
	
	float yPos, increment;

	
	// configuration
	if(!initialised)
	{
    pinMode(A6,OUTPUT);
    pinMode(A7,OUTPUT);
    digitalWrite(A6,HIGH);
    digitalWrite(A7,HIGH);

  
		Wire.begin();
		wii.init();
		MYSERIAL.println("Calibrating mid position of joystick");
		wii.poll();
		delay(100);
		wii.poll();
		wii.calibrate();  // calibration (needs a poll beforehand to work)
		MYSERIAL.println("Complete");
		initialised = true;
	}
	
	wii.poll();
	
	if(wii.buttonC()&&wii.buttonZ())
	{
		MYSERIAL.println("Calibrating mid position of joystick");
		wii.poll();
		delay(100);
		wii.poll();
		wii.calibrate();  // calibration (needs a poll before hand to work)
	}
	// change grip on a button press
	if(wii.buttonC())
	{
		currentGrip ++;
		if(currentGrip >= NUM_GRIPS)
			currentGrip = 0;
		delay(200);
	}
	if(wii.buttonZ())
	{
		currentGrip --;
		if(currentGrip < 0)
			currentGrip = NUM_GRIPS-1;
		delay(200);
	}
	
	yPos = wii.joyY();
	
	MYSERIAL.print("Grip ");
  MYSERIAL.print(currentGrip);
	MYSERIAL.print("\tyPos ");
	MYSERIAL.print(yPos);
	
	yPos = constrain(yPos,-90,90);
	yPos = (float) yPos/(180/PI);			// deg to rad

	increment = 2*sin(yPos);
	HANDlePos += increment;
	HANDlePos = constrain(HANDlePos,0,100);
		
	gripMovement(currentGrip,HANDlePos);
	

	MYSERIAL.print("  \tHand pos ");
	MYSERIAL.print((int) HANDlePos);
  MYSERIAL.print("\t");
  MYSERIAL.println(textString.grips[currentGrip]);

}

