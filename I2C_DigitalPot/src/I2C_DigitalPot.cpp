/*
 * I2C_DigitalPot.cpp
 *
 * Created: 27/11/15 14:24
 * Author: Olly McBride
 *
 * This work is licensed under the Creative Commons Attribution 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by/4.0/.
 *
 * AD5171 64 position I2C digital potentiometer
 *
 */

#include <Wire.h>
#include "I2C_DigitalPot.h"

// Constructors ////////////////////////////////////////////////////////////////
D_POT::D_POT()
{
	D_POT(0,10000,0);			// set Rtotal to 10k, disable potential divider mode 
}

D_POT::D_POT(bool AD0, uint16_t Rtotal)
{
	D_POT(AD0,Rtotal,0);			// disable potential divider mode 			
}

D_POT::D_POT(bool AD0, uint16_t Rtotal, uint8_t Vin)
{
	_potAddr = DPOT_ADDR | AD0;		// configurable address
	_OTP_EN = false;				// disable one-time programming mode
	_Rab = Rtotal;					// store total resistance
	_Vin = Vin;						// store Vin
	
	#ifdef DPOT_DEBUG
		MYSERIAL.print("D_POT  AD0 ");
		MYSERIAL.print(AD0);
		MYSERIAL.print(" addr ");
		printBin(_potAddr);
		MYSERIAL.print(" Rtotal ");
		MYSERIAL.print(Rtotal);
		MYSERIAL.print(" Vin ");
		MYSERIAL.print(Vin);
		if(_OTP_EN)
			MYSERIAL.print(" OTP ENABLED ");
		MYSERIAL.print("\n");
	#endif
	
	Wire.begin();
}

// Public Methods //////////////////////////////////////////////////////////////

// write date to pot
void D_POT::writeD(uint8_t D)
{
	uint8_t OTP_instr;
	
	// uncomment the next line to enable one-time programming mode
	//OTP_instr = (uint8_t) _OTP_EN;
	OTP_instr = 0; 
	
	D = constrain(D,0,63);					// 0 < D < 63

	#ifdef DPOT_DEBUG
		MYSERIAL.print("D_POT.writeD(");
		MYSERIAL.print(D);
		MYSERIAL.print(") to ");
		printBin(_potAddr);
		if(OTP_instr)
			MYSERIAL.print(" WARNING OTP ENABLED ");
		MYSERIAL.print("\n");
	#endif
	
	Wire.beginTransmission(_potAddr);
	Wire.write(OTP_instr);					// activate OTP if flag enabled
	Wire.write(D);							// write data 
	Wire.endTransmission();
	
	#ifdef DPOT_DEBUG
		MYSERIAL.println("Complete");
	#endif
}

void D_POT::writeRwb(uint16_t Rwb)
{
	const uint8_t Rw = 60;					// wiper resistance
	uint8_t D;
	
	D = (uint8_t)((Rwb - Rw) * 63)/_Rab;	// convert target resistance Rwb to data value D (0 < D < Rw)
	
	#ifdef DPOT_DEBUG
		MYSERIAL.print("D_POT.writeRwb(");
		MYSERIAL.print(Rwb);
		MYSERIAL.print(") D = ");
		MYSERIAL.print(D);
		MYSERIAL.print(" addr ");
		printBin(_potAddr);
		MYSERIAL.print("\n");
	#endif
	
	writeD(D);
}

void D_POT::writeRwa(uint16_t Rwa)
{
	const uint8_t Rw = 60;						// wiper resistance
	uint8_t D;
	
	D = (uint8_t)(63*(_Rab + Rw - Rwa))/_Rab;	// convert target resistance Rwa to data value D (0 < D < Rw)
	
	#ifdef DPOT_DEBUG
		MYSERIAL.print("D_POT.writeRwa(");
		MYSERIAL.print(Rwa);
		MYSERIAL.print(") D = ");
		MYSERIAL.print(D);
		MYSERIAL.print(" addr ");
		printBin(_potAddr);
		MYSERIAL.print("\n");
	#endif
	
	writeD(D);
}

void D_POT::writeR(uint16_t R)
{
	writeRwb(R);
}

void D_POT::writeV(float Vw)
{
	uint8_t D;
	
	D = (uint8_t) ((float)(Vw/_Vin)*63);
		
	#ifdef DPOT_DEBUG
		MYSERIAL.print("D_POT.writeV(");
		MYSERIAL.print(Vw);
		MYSERIAL.print(") D = ");
		MYSERIAL.print(D);
		MYSERIAL.print(" addr ");
		printBin(_potAddr);
		MYSERIAL.print("\n");
	#endif
	
	writeD(D);
}

void D_POT::enableOTP(void)
{
	// uncomment the following line to enable one-time programming mode
	//_OTP_EN = true;
	// you also need to uncomment the stated line within writeD()
}

// Private Methods //////////////////////////////////////////////////////////////

void D_POT::printBin(uint8_t inVal)
{
	for (uint8_t i = 1; i < 8; i++)
	{
		if (inVal < pow(2, i)) MYSERIAL.print(B0);
	}
	MYSERIAL.print(inVal,BIN);
}
