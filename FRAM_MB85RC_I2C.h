/**************************************************************************/
/*! 
    @file     FRAM_MB85RC_I2C.h
    @author   SOSAndroid.fr (E. Ha.)

    Driver for the MB85RC I2C FRAM from Fujitsu.
	
    @section LICENSE

    Software License Agreement (BSD License)

    Copyright (c) 2013, SOSAndroid.fr (E. Ha.)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are met:
    1. Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in the
    documentation and/or other materials provided with the distribution.
    3. Neither the name of the copyright holders nor the
    names of its contributors may be used to endorse or promote products
    derived from this software without specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ''AS IS'' AND ANY
    EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
    WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
    DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER BE LIABLE FOR ANY
    DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
    ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
    SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/**************************************************************************/
#ifndef _FRAM_MB85RC_I2C_H_
#define _FRAM_MB85RC_I2C_H_

#if ARDUINO >= 100
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <Wire.h>

// Enabling debug I2C - comment to disable / normal operations
#define SERIAL_DEBUG

// Adresses
#define MB85RC_ADDRESS_A000   0x50
#define MB85RC_ADDRESS_A001   0x51
#define MB85RC_ADDRESS_A010   0x52
#define MB85RC_ADDRESS_A011   0x53
#define MB85RC_ADDRESS_A100   0x54
#define MB85RC_ADDRESS_A101   0x55
#define MB85RC_ADDRESS_A110   0x56
#define MB85RC_ADDRESS_A111   0x57
#define MB85RC_DEFAULT_ADDRESS   MB85RC_ADDRESS_A000
#define MB85RC_SLAVE_ID       0xF8

// Managing Write protect pin
#define MB85RC_MANAGE_WP false //false if WP pin remains not connected
#define MB85RC_DEFAULT_WP_PIN	13 //write protection pin - active high, write enabled when low
#define MB85RC_DEFAULT_WP_STATUS  false //false means protection is off - write is enabled

class FRAM_MB85RC_I2C {
 public:
	FRAM_MB85RC_I2C(void);
	FRAM_MB85RC_I2C(uint8_t address, boolean wp);
	FRAM_MB85RC_I2C(uint8_t address, boolean wp, int pin);
	
	byte	checkDevice(void);
	byte	readBit(uint16_t framAddr, uint8_t bitNb, byte *bit);
	byte	setOneBit(uint16_t framAddr, uint8_t bitNb);
	byte	clearOneBit(uint16_t framAddr, uint8_t bitNb);
	byte	toggleBit(uint16_t framAddr, uint8_t bitNb);
	byte	readArray (uint16_t framAddr, byte items, uint8_t value[]);
	byte	writeArray (uint16_t framAddr, byte items, uint8_t value[]);
	byte	readByte (uint16_t framAddr, uint8_t *value);
	byte	writeByte (uint16_t framAddr, uint8_t value);
	//byte	readWord(uint16_t framAddr, uint16_t *value);
	//byte	writeWord(uint16_t framAddr, uint16_t value);
	//byte	readLong(uint16_t framAddr, uint32_t *value);
	//byte	writeLong(uint16_t framAddr, uint32_t value);
	byte	getOneDeviceID(int idType, uint16_t *id);
	boolean	isReady(void);
	boolean	getWPStatus(void);
	byte	enableWP(void);
	byte	disableWP(void);
	byte	eraseDevice(void);
  
 private:
	uint8_t	i2c_addr;
	boolean	_framInitialised;
	uint16_t	manufacturer;
	uint16_t	productid; 
	uint16_t	densitycode;
	uint16_t	density;
	uint16_t	maxaddress;

	int	wpPin;
	boolean	wpStatus;

	byte	getDeviceIDs(void);	
	byte	initWP(boolean wp);
	byte	deviceIDs2Serial(void);
};

#endif
