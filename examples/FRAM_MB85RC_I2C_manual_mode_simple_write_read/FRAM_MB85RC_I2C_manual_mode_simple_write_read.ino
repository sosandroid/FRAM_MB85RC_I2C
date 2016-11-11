/**************************************************************************/
/*!
    @file     FRAM_MB85RC_I2C_manual_mode_simple_write_read.ino
    @author   SOSAndroid (E. Ha.)
    @license  BSD (see license.txt)

    Example sketch that write & read on a 16K chip using the object "manual setting".

    @section  HISTORY

    v1.0.0 - First release
	V1.1.0 - Reworked code to test wider range of the memory

	
*/
/**************************************************************************/

#include <Wire.h>
#include <FRAM_MB85RC_I2C.h>

uint8_t writevalue = 0xCC;
uint16_t writevalue2 = 0xBEEF;
uint8_t readvalue = 0xFF; // at the end, readvalue should be equal to writevalue
uint16_t readvalue2 = 0xDEAD;

//random addresses to write from
uint16_t writeaddress = 0x025; // Beginning of the memory map
uint16_t writeaddress2 = 0x750; // End of the memory map

//Example for mamual mode and a 16K chip --- 16K devices does not have A2, A1 nor A0 support. Device address `i2c_addr = 0b1010000` where 3LSB stands for 3 MSB of the memory address
uint8_t chipaddress = 0x50;
boolean wp = false; //Allows memory write
int pin = 13;
uint16_t chipDensity = 16;

//Creating object for FRAM chip
FRAM_MB85RC_I2C mymemory(chipaddress, wp, pin, chipDensity);
byte resultw, resultr;


void setup() {

	Serial.begin(115200);
	while (!Serial) ; //wait until Serial ready
	Wire.begin();
	
    Serial.println("Starting...");

	
	mymemory.begin();

//--------------------------- First run, beginning of memory map ---------------------
	Serial.print("Writing at location 0x");
	Serial.println(writeaddress, HEX);
	
    resultw = mymemory.writeByte(writeaddress, writevalue);
	resultr = mymemory.readByte(writeaddress, &readvalue);
	
	Serial.print("Written value 0x");
	Serial.println(writevalue, HEX);
	
	Serial.print("Read value 0x");
	Serial.println(readvalue, HEX);
	
	if (writevalue == readvalue) {
		Serial.println("Write Byte test : OK");
	}
	else {
		Serial.println("Write Byte test : NOT OK");
	}
	Serial.println(".... ....");
	
//-------------------------- Second run, end of the memory map -----------------------
    
	Serial.print("Writing at location 0x");
	Serial.println(writeaddress2, HEX);
	
	resultw = mymemory.writeWord(writeaddress2, writevalue2);
	resultr = mymemory.readWord(writeaddress2, &readvalue2);
	
	Serial.print("Written value 0x");
	Serial.println(writevalue2, HEX);
	
	Serial.print("Read value 0x");
	Serial.println(readvalue2, HEX);
	
	if (writevalue2 == readvalue2) {
		Serial.println("Write word test : OK");
	}
	else {
		Serial.println("Write word test : NOT OK");
	}
	Serial.println(".... ....");
	//------------------------- Device IDs -----------------------------------------------

	//get one device ID
	uint16_t id;
	resultr = mymemory.getOneDeviceID(1, &id);
	Serial.print("Manufacturer 0x");
	Serial.println(id, HEX);
	
	resultr = mymemory.getOneDeviceID(3, &id);
	Serial.print("DensityCode 0x");
	Serial.println(id, HEX);
	
	resultr = mymemory.getOneDeviceID(4, &id);
	Serial.print("Density ");
	Serial.println(id, DEC);

	
}

void loop() {
	// nothing to do
}