/**************************************************************************/
/*!
    @file     FRAM_MB85RC_I2C_simple_write_read.ino
    @author   SOSAndroid (E. Ha.)
    @license  BSD (see license.txt)

    Example sketch that stores and retreive any kind of data.

    @section  HISTORY

    v1.0 - First release
	v1.0.1 - fix constructor call error
	
*/
/**************************************************************************/

#include <Wire.h>
#include <math.h>

#include <FRAM_MB85RC_I2C.h>

uint8_t writevalue = 0xCC;
uint8_t readvalue = 0xFF;

//random address to write from
uint16_t writeaddress = 0x025;

//Creating object for FRAM chip
FRAM_MB85RC_I2C mymemory;

void setup() {

	Serial.begin(9600);
	while (!Serial) ; //wait until Serial ready
	Wire.begin();
	
    Serial.println("Starting...");

	
	mymemory.begin();


	//write to FRAM chip
	byte resultw, resultr;
    resultw = mymemory.writeByte(writeaddress, writevalue);
	//read from FRAM chip
	resultr = mymemory.readByte(writeaddress, &readvalue);
	
	Serial.print("Written value 0x");
	Serial.println(writevalue, HEX);
	
	Serial.print("Read value 0x");
	Serial.println(readvalue, HEX);	

	//get one device ID
	uint16_t densitycode;
	resultr = mymemory.getOneDeviceID(3, &densitycode);

	Serial.print("DensityCode 0x");
	Serial.println(densitycode, HEX);
	
}

void loop() {
	// nothing to do
}