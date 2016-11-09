/**************************************************************************/
/*!
    @file     FRAM_MB85RC_I2C_manual_mode_simple_write_read.ino
    @author   SOSAndroid (E. Ha.)
    @license  BSD (see license.txt)

    Example sketch that write & read on a 16K chip using the object "manual setting".

    @section  HISTORY

    v1.0.0 - First release

	
*/
/**************************************************************************/

#include <Wire.h>
#include <FRAM_MB85RC_I2C.h>

uint8_t writevalue = 0xCC;
uint8_t readvalue = 0xFF; // at the end, readvalue should be equal to writevalue

//random address to write from
uint16_t writeaddress = 0x025;

//Example for mamual mode and a 16K chip --- 16K devices does not have A2, A1 nor A0 support. This is used for memory addressing `i2c_addr = 0b1010000`
uint8_t chipaddress = 0x50;
boolean wp = true;
int pin = 13;
uint16_t chipDensity = 16;

//Creating object for FRAM chip
FRAM_MB85RC_I2C mymemory(chipaddress, wp, pin, chipDensity);

void setup() {

	Serial.begin(115200);
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