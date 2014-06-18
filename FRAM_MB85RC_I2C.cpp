/**************************************************************************/
/*!
    @file     FRAM_MB85RC_I2C.cpp
    @author   SOSAndroid (E. Ha.)
    @license  BSD (see license.txt)

    Driver for the MB85RC I2C FRAM from Fujitsu.

    @section  HISTORY

    v1.0 - First release
	v1.0.1 - Robustness enhancement
	v1.0.2 - fix constructor, introducing byte move in memory
*/
/**************************************************************************/

#include <stdlib.h>
#include <Wire.h>
#include "FRAM_MB85RC_I2C.h"

/*========================================================================*/
/*                            CONSTRUCTORS                                */
/*========================================================================*/

/**************************************************************************/
/*!
    Constructor
*/
/**************************************************************************/
FRAM_MB85RC_I2C::FRAM_MB85RC_I2C(void) 
{
		FRAM_MB85RC_I2C::FRAM_MB85RC_I2C(MB85RC_DEFAULT_ADDRESS, MB85RC_DEFAULT_WP_STATUS, MB85RC_DEFAULT_WP_PIN);
}

FRAM_MB85RC_I2C::FRAM_MB85RC_I2C(uint8_t address, boolean wp) 
{
		FRAM_MB85RC_I2C::FRAM_MB85RC_I2C(address, wp, MB85RC_DEFAULT_WP_PIN);
}

FRAM_MB85RC_I2C::FRAM_MB85RC_I2C(uint8_t address, boolean wp, int pin) 
{
		_framInitialised = false;
		i2c_addr = address;
		wpPin = pin;
		
		byte deviceFound = FRAM_MB85RC_I2C::checkDevice();
		byte result = FRAM_MB85RC_I2C::initWP(wp);

    #ifdef SERIAL_DEBUG
		if (Serial){
			Serial.println("FRAM_MB85RC_I2C object created");
			Serial.print("I2C device address 0x");
			Serial.println(i2c_addr, HEX);
			Serial.print("WP pin number ");
			Serial.println(wpPin, DEC);
			Serial.print("Write protect management: ");
			if(MB85RC_MANAGE_WP) {
				Serial.println("true");
			}
			else {
				Serial.println("false");
			}
			if(deviceFound == 0) {
				Serial.println("Memory Chip connected");
				FRAM_MB85RC_I2C::deviceIDs2Serial();
			}
			else {
				Serial.println("Memory Chip NOT FOUND");
			}
			Serial.println("...... ...... ......");
		}
    #endif
}

/*========================================================================*/
/*                           PUBLIC FUNCTIONS                             */
/*========================================================================*/

/**************************************************************************/
/*!
    Check if device is connected at address @i2c_addr
	returns 0 = device found
			7 = device not found
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::checkDevice(void) 
{
  
  /* Make sure we're actually connected */
  byte result = getDeviceIDs();
  
  if ((result == 0) && (manufacturer = 0x00A) && (densitycode > 0x02) && (densitycode < 0x08) && (maxaddress != 0)) {
		//densitycode 0x03 = 64K chip, densitycode 0x07 = 1M chip -- check datasheets & readme
		_framInitialised = true;
  }
  else {
		_framInitialised = false;
  }
  return result;
}


/**************************************************************************/
/*!
    @brief  Writes an array of bytes from a specific address
    
    @params[in] i2cAddr
                The I2C address of the FRAM memory chip (1010+A2+A1+A0)
    @params[in] framAddr
                The 16-bit address to write to in FRAM memory
    @params[in] items
                The number of items to write from the array
	@params[in] values[]
                The array of bytes to write
	@returns
				return code of Wire.endTransmission()
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::writeArray (uint16_t framAddr, byte items, uint8_t values[])
{
  Wire.beginTransmission(i2c_addr);
  Wire.write(framAddr >> 8);
  Wire.write(framAddr & 0xFF);
	for (byte i=0; i < items ; i++) {
		Wire.write(values[i]);
	}
  return Wire.endTransmission();
}

/**************************************************************************/
/*!
    @brief  Writes a single byte to a specific address
    
    @params[in] i2cAddr
                The I2C address of the FRAM memory chip (1010+A2+A1+A0)
    @params[in] framAddr
                The 16-bit address to write to in FRAM memory
	@params[in] value
                One byte to write
	@returns
				return code of Wire.endTransmission()
*/
/**************************************************************************/

byte FRAM_MB85RC_I2C::writeByte (uint16_t framAddr, uint8_t value)
{
  uint8_t buffer[] = {value}; 
  return FRAM_MB85RC_I2C::writeArray(framAddr, 1, buffer);

}



/**************************************************************************/
/*!
    @brief  Reads an array of bytes from the specified FRAM address

    @params[in] i2cAddr
                The I2C address of the FRAM memory chip (1010+A2+A1+A0)
    @params[in] framAddr
                The 16-bit address to read from in FRAM memory
	@params[in] items
				number of items to read from memory chip
	@params[out] values[]
				array to be filled in by the memory read
    @returns    
				return code of Wire.endTransmission()
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::readArray (uint16_t framAddr, byte items, uint8_t values[])
{
	byte result;
	if (items == 0) {
		result = 8; //number of bytes asked to read null
	}
	else {
		Wire.beginTransmission(i2c_addr);
		Wire.write(framAddr >> 8);
		Wire.write(framAddr & 0xFF);
		result = Wire.endTransmission();
		
		Wire.requestFrom(i2c_addr, (uint8_t)items);
		for (byte i=0; i < items; i++) {
			values[i] = Wire.read();
		}
	}
	return result;
}

/**************************************************************************/
/*!
    @brief  Reads one byte from the specified FRAM address

    @params[in] i2cAddr
                The I2C address of the FRAM memory chip (1010+A2+A1+A0)
    @params[in] framAddr
                The 16-bit address to read from in FRAM memory
	@params[out] *values
				data read from memory
    @returns    
				return code of Wire.endTransmission()
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::readByte (uint16_t framAddr, uint8_t *value) 
{
	uint8_t buffer[1];
	byte result = FRAM_MB85RC_I2C::readArray(framAddr, 1, buffer);
	*value = buffer[0];
	return result;
}
/**************************************************************************/
/*!
    @brief  Copy a byte from one address to another in the memory scope

    @params[in] i2cAddr
                The I2C address of the FRAM memory chip (1010+A2+A1+A0)
    @params[in] origAddr
                The 16-bit address to read from in FRAM memory
	@params[in] destAddr
				The 16-bit address to write in FRAM memory
    @returns    
				return code of Wire.endTransmission()
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::copyByte (uint16_t origAddr, uint16_t destAddr) 
{
	uint8_t buffer[1];
	byte result = FRAM_MB85RC_I2C::readByte(origAddr, buffer);
	result = FRAM_MB85RC_I2C::writeByte(destAddr, buffer[0]);
	return result;
}


/**************************************************************************/
/*!
    @brief  Reads one bit from the specified FRAM address

    @params[in] framAddr
                The 16-bit address to read from in FRAM memory
    @params[in] bitNb
                The bit position to read
	@params[out] *bit
				value of the bit: 0 | 1
    @returns    
				return code of Wire.endTransmission()
				return code 9 if bit position is larger than 7
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::readBit(uint16_t framAddr, uint8_t bitNb, byte *bit)
{
	byte result;
	if (bitNb > 7) {
		result = 9;
	}
	else {
		uint8_t buffer[1];
		result = FRAM_MB85RC_I2C::readArray(framAddr, 1, buffer);
		*bit = bitRead(buffer[0], bitNb);
	}
	return result;
}

/**************************************************************************/
/*!
    @brief  Set one bit to the specified FRAM address

    @params[in] framAddr
                The 16-bit address to read from in FRAM memory
    @params[in] bitNb
                The bit position to set
    @returns    
				return code of Wire.endTransmission()
				return code 9 if bit position is larger than 7
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::setOneBit(uint16_t framAddr, uint8_t bitNb)
{
	byte result;
	if (bitNb > 7)  {
		result = 9;
	}
	else {
		uint8_t buffer[1];
		result = FRAM_MB85RC_I2C::readArray(framAddr, 1, buffer);
		bitSet(buffer[0], bitNb);
		result = FRAM_MB85RC_I2C::writeArray(framAddr, 1, buffer);
	}
	return result;
}
/**************************************************************************/
/*!
    @brief  Clear one bit to the specified FRAM address

    @params[in] framAddr
                The 16-bit address to read from in FRAM memory
    @params[in] bitNb
                The bit position to clear
    @returns    
				return code of Wire.endTransmission()
				return code 9 if bit position is larger than 7
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::clearOneBit(uint16_t framAddr, uint8_t bitNb)
{
	byte result;
	if (bitNb > 7) {
		result = 9;
	}
	else {
		uint8_t buffer[1];
		result = FRAM_MB85RC_I2C::readArray(framAddr, 1, buffer);
		bitClear(buffer[0], bitNb);
		result = FRAM_MB85RC_I2C::writeArray(framAddr, 1, buffer);
	}
	return result;
}
/**************************************************************************/
/*!
    @brief  Toggle one bit to the specified FRAM address

    @params[in] framAddr
                The 16-bit address to read from in FRAM memory
    @params[in] bitNb
                The bit position to toggle
    @returns    
				return code of Wire.endTransmission()
				return code 9 if bit position is larger than 7
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::toggleBit(uint16_t framAddr, uint8_t bitNb)
{
	byte result;
	if (bitNb > 7) {
		result = 9;
	}
	else {
		uint8_t buffer[1];
		result = FRAM_MB85RC_I2C::readArray(framAddr, 1, buffer);
		
		if ( (buffer[0] & (1 << bitNb)) == (1 << bitNb) )
		{
			bitClear(buffer[0], bitNb);
		}
		else {
			bitSet(buffer[0], bitNb);
		}
		result = FRAM_MB85RC_I2C::writeArray(framAddr, 1, buffer);
	}
	return result;
}
/**************************************************************************/
/*!
    @brief  Reads a 16bits value from the specified FRAM address

    @params[in] framAddr
                The 16-bit address to read from in FRAM memory
	@params[out] value
				16bits word
    @returns    
				return code of Wire.endTransmission()
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::readWord(uint16_t framAddr, uint16_t *value)
{
	uint8_t buffer[2];
	byte result = FRAM_MB85RC_I2C::readArray(framAddr, 2, buffer);
	*value = ((uint16_t) buffer[0] << 8) + (uint16_t)buffer[1];
	return result;
}

/**************************************************************************/
/*!
    @brief  Write a 16bits value from the specified FRAM address

    @params[in] framAddr
                The 16-bit address to read from in FRAM memory
	@params[in] value
				16bits word
    @returns    
				return code of Wire.endTransmission()
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::writeWord(uint16_t framAddr, uint16_t value)
{
	uint8_t buffer[2] = {(value >> 8), (value & 0xFF)};
	return FRAM_MB85RC_I2C::writeArray(framAddr, 2, buffer);
}
/**************************************************************************/
/*!
    @brief  Read a 32bits value from the specified FRAM address

    @params[in] framAddr
                The 16-bit address to read from FRAM memory
	@params[in] value
				32bits word
    @returns    
				return code of Wire.endTransmission()
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::readLong(uint16_t framAddr, uint32_t *value)
{
	uint8_t buffer[4];
	byte result = FRAM_MB85RC_I2C::readArray(framAddr, 4, buffer);
	*value = ((uint32_t) buffer[0] << 24) + ((uint32_t)buffer[1] << 16) + ((uint32_t)buffer[2] << 8) + (uint32_t)buffer[3];
	return result;

}
/**************************************************************************/
/*!
    @brief  Write a 32bits value to the specified FRAM address

    @params[in] framAddr
                The 16-bit address to write to FRAM memory
	@params[in] value
				32bits word
    @returns    
				return code of Wire.endTransmission()
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::writeLong(uint16_t framAddr, uint32_t value)
{
	uint8_t buffer[4];
	buffer[0] = (uint8_t)(value >> 24);
	buffer[1] = (uint8_t)((value & 0xFFFFFF) >> 16);
	buffer[1] = (uint8_t)((value & 0xFFFF) >> 8);
	buffer[1] = (uint8_t)(value & 0xFF);
	return FRAM_MB85RC_I2C::writeArray(framAddr, 4, buffer);
}
/**************************************************************************/
/*!
    @brief  Reads the Manufacturer ID and the Product ID frm the IC

    @params[in]   idtype
				  1: Manufacturer ID, 2: ProductID, 3:density code, 4:density
	@params[out]  *id
                  The 16 bits ID value
    @returns
				  0: success
				  1: error
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::getOneDeviceID(uint8_t idType, uint16_t *id) 
{
	byte result;
	const uint8_t manuf = 1;
	const uint8_t prod = 2;
	const uint8_t densc = 3;
	const uint8_t densi = 4;
	
	switch (idType) {
		case manuf:
			*id = manufacturer;
			result = 0;
			break;
		case prod:
			*id = productid;
			result = 0;
			break;
		case densc:
			*id = densitycode;
			result = 0;
			break;
		case densi:
			*id = density;
			result = 0;
			break;
		default:
			*id = 0;
			result = 5;
			break;
	}
	return result;
}

/**************************************************************************/
/*!
    @brief  Return the readiness of the memory chip

    @params[in]  none
	@returns
				  boolean
				  true : ready
				  false : not ready
*/
/**************************************************************************/
boolean FRAM_MB85RC_I2C::isReady(void) {
	return _framInitialised;
}


/**************************************************************************/
/*!
    @brief  Return tu Write Protect status

    @params[in]  none
	@returns
				  boolean
				  true : write protect enabled
				  false: wirte protect disabled
*/
/**************************************************************************/
boolean FRAM_MB85RC_I2C::getWPStatus(void) {
	return wpStatus;
}

/**************************************************************************/
/*!
    @brief  Enable write protect function of the chip by pulling up WP pin

    @params[in]   MB85RC_MANAGE_WP
                  WP management switch defined in header file
    @params[in]   wpPin
                  pin number for WP pin
	@param[out]	  wpStatus
	@returns
				  0: success
				  1: error, WP not managed
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::enableWP(void) {
	byte result;
	if (MB85RC_MANAGE_WP) {
		digitalWrite(wpPin,HIGH);
		wpStatus = true;
		result = 0;
	}
	else {
		result = 10;
	}
	return result;
}
/**************************************************************************/
/*!
    @brief  Disable write protect function of the chip by pulling up WP pin

    @params[in]   MB85RC_MANAGE_WP
                  WP management switch defined in header file
    @params[in]   wpPin
                  pin number for WP pin
	@param[out]	  wpStatus
	@returns
				  0: success
				  1: error, WP not managed
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::disableWP() {
	byte result;
	if (MB85RC_MANAGE_WP) {
		digitalWrite(wpPin,LOW);
		wpStatus = false;
		result = 0;
	}
	else {
		result = 10;
	}
	return result;
}
/**************************************************************************/
/*!
    @brief  Erase device by overwriting it to 0x00

    @params[in]   SERIAL_DEBUG
                  Outputs erasing results to Serial
	@returns
				  0: success
				  1-4: error writing at a certain position
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::eraseDevice(void) {
		byte result = 0;
		uint16_t i = 0;
		
		#ifdef SERIAL_DEBUG
			if (Serial){
				Serial.println("Start erasing device");
			}
		#endif
		
		while((i < maxaddress) && (result == 0)){
		  result = FRAM_MB85RC_I2C::writeByte(i, 0x00);
		  i++;
		}
		
	
		#ifdef SERIAL_DEBUG
			if (Serial){
				if (result !=0) {
						Serial.print("ERROR: device erasing stopped at position ");
						Serial.println(i, DEC);
						Serial.println("...... ...... ......");
				}
				else {
						Serial.println("device erased");
						Serial.println("...... ...... ......");
				}
			}
		#endif
		return result;
}


/*========================================================================*/
/*                           PRIVATE FUNCTIONS                            */
/*========================================================================*/

/**************************************************************************/
/*!
    @brief  Reads the Manufacturer ID and the Product ID from the IC and populate calss' variables

    @params[in]   none
	@params[out]  manufacturerID
                  The 12-bit manufacturer ID (Fujitsu = 0x00A)
    @params[out]  productID
                  The memory density (bytes 11..8) and proprietary
                  Product ID fields (bytes 7..0). Should be 0x510 for
                  the MB85RC256V for instance.
	@param[out]	  The memory densitycode (bytes 11..8)
				  from 0x03 (64K chip) to 0x07 (1M chip)
	@param[out]	  The memory density got from density code
				  from 64 to 1024K			  
	@param[out]	  The memory max address of storage slot
    @returns
				  return code of Wire.endTransmission()
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::getDeviceIDs(void)
{
	uint8_t localbuffer[3] = { 0, 0, 0 };
	uint8_t result;
	const uint16_t dens64 = 0x03;
	const uint16_t dens128 = 0x04;
	const uint16_t dens256 = 0x05;
	const uint16_t dens512 = 0x06;
	const uint16_t dens1024 = 0x07;
	
	/* Get device IDs sequence                                                                                                  */
	/* Send 0xF8 to 12C bus. Bit shift to right as beginTransmission() requires a 7bit. beginTransmission() 0 for write => 0xF8 */
	/* Send device address as 8 bits. Bit shift to left as we are using a simple write()                                        */
	/* Send 0xF9 to I2C bus. By requesting 3 bytes to read, requestFrom() add a 1 bit at the end of a 7 bits address => 0xF9    */
	/* See p.10 of http://www.fujitsu.com/downloads/MICRO/fsa/pdf/products/memory/fram/MB85RC-DS501-00017-3v0-E.pdf             */
	/*                                                                                                                          */
	Wire.beginTransmission(MB85RC_SLAVE_ID >> 1);
	Wire.write((byte)(i2c_addr << 1));
	result = Wire.endTransmission(false);
 
	Wire.requestFrom(MB85RC_SLAVE_ID >> 1, 3);
	localbuffer[0] = (uint8_t) Wire.read();
	localbuffer[1] = (uint8_t) Wire.read();
	localbuffer[2] = (uint8_t) Wire.read();
	
	/* Shift values to separate IDs */
	manufacturer = (localbuffer[0] << 4) + (localbuffer[1] >> 4);
	densitycode = (uint16_t)(localbuffer[1] & 0x0F);
	productid = ((localbuffer[1] & 0x0F) << 8) + localbuffer[2];
	
	switch (densitycode) {
		case dens64:
			density = 64;
			maxaddress = 8192;
			break;
		case dens128:
			density = 128;
			maxaddress = 16384;
			break;
		case dens256:
			density = 256;
			maxaddress = 32768;
			break;
		case dens512:
			density = 512;
			maxaddress = 65536;
			break;
		case dens1024:
			density = 1024;
			maxaddress = 65536; /* should be 2 times greater but we are considering the 1M device as 2 physical 512K devices. */
			break;
		default:
			density = 0; /* means error */
			maxaddress = 0; /* means error */
			if (result == 0) result = 7; /*device unidentified, comminication ok*/
			break;
	}
  return result;
}


/**************************************************************************/
/*!
    @brief  Utility function to print out memory chip IDs to serial if Debug enabled 

    @params[in]   SERIAL_DEBUG
	@param[out]	  none
	@returns
				  0: success
				  4: error, debug not activated or Serial not available
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::deviceIDs2Serial(void) {
    byte result = 4;
	#ifdef SERIAL_DEBUG
		if (Serial){
			Serial.print("Manufacturer 0x"); Serial.println(manufacturer, HEX);
			Serial.print("ProductID 0x"); Serial.println(productid, HEX);
			Serial.print("Density code 0x"); Serial.println(densitycode, HEX);
			Serial.print("Density "); Serial.print(density, DEC); Serial.println("K");
			if ((manufacturer == 0x00A) && (density != 0))  Serial.println("Device identfied");
			if ((manufacturer == 0x00A) && (density = 0))  Serial.println("Device NOT identfied");
			Serial.println("...... ...... ......");
			result = 0;
		}
    #endif
	return result;
}

/**************************************************************************/
/*!
    @brief  Init write protect function for class constructor

    @params[in]   MB85RC_MANAGE_WP
                  WP management switch defined in header file
    @params[in]   wpPin
                  pin number for WP pin
    @params[in]   wp
                  Boolean for startup WP
	@param[out]	  wpStatus
	@returns
				  0: success, init done
				  1: error - should never happen
*/
/**************************************************************************/
byte FRAM_MB85RC_I2C::initWP(boolean wp) {
	byte result;
	if (MB85RC_MANAGE_WP) {
		pinMode(wpPin,OUTPUT);
		if (wp) {
			result = FRAM_MB85RC_I2C::enableWP();
		}
		else {
			result = FRAM_MB85RC_I2C::disableWP();
		}
	}
	else {
		wpStatus = false;
		result = 0;
	}
	return result;
}