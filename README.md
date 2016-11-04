Arduino library for I2C FRAM MB85RC series from Fujitsu & some of the Cypress FM24, CY15B
==============

    v1.0 - First release
	v1.0.1 - Robustness enhancement
	v1.0.2 - fix constructor, introducing byte move in memory
	v1.0.3 - fix writeLong() function
	v1.0.4 - fix constructor call error
	v1.0.4.1 - Add example to help @porcao
	v1.0.5 - Enlarge density chip support by making check more flexible, Error codes not anymore hardcoded, add connect example, add Cypress FM24 & CY15B series comment.


MB85RC from Fujitsu is a I2C Ferroelectric Random Access Memory (FRAM). Read/write endurance for each memory slot : 10^12 cycles.
16 bit adresses, 8 bits data slots.

Supports 64K, 128K, 256K & 512K devices. Works for 1M devices when considering each device as 2 differents 512K devices (see datasheet: 17 bits addresses of which MSB is replacing A0 device address bit).

[MB85RCxxxx](http://www.fujitsu.com/us/products/devices/semiconductor/memory/fram/lineup/index.html) Fujitsu's page

## Features ##
- Manage single bit (read, set, clear, toggle) from a byte
- Write one 8-bits, 16-bits or 32-bits value
- Write one array of bytes 
- Read one 8-bits, 16-bits or 32-bits value
- Read one array of bytes (up to 256 per call - maximum supported by Arduino's Wire lib)
- Move a byte from an address to another
- Get device information
	- 1: Manufacturer ID
	- 2: Product ID
	- 3: Density code
	- 4: Density human readable
- Manage write protect pin
- Erase memory (set all chip to 0x00)
- Debug mode manageable from header file


## Devices ##

It seems that *Cypress FM24* and *CY15B* series are supported by this library. However, his has not been tested. Please leave feedback if you perform such a test. [More info](http://www.cypress.com/products/f-ram-serial).

| Devices | Information | Supported |
| ------- | ----------- | --------- |
| MB85RC04V | [datasheet](http://edevice.fujitsu.com/fj/DATASHEET/e-ds/MB85RC04V-DS501-00016-4v0-E.pdf) |
| | Manufacturer: 0x00A, ProductID: 0x010, DensityCode: 0x00, Maxaddress: 512, Address Length: 9bits, Density: 4, R/W cycles: 10^12 | Not yet (1) |
| MB85RC16V | [datasheet](http://edevice.fujitsu.com/fj/DATASHEET/e-ds/MB85RC16V-DS501-00010-8v0-E.pdf) |
| | Manufacturer: 0x00A, ProductID: 0x???, DensityCode: 0x??, Maxaddress: 2048, Address Length: 11bits, Density: 16, R/W cycles: 10^12 | Not yet (2) |
| MB85RC64V | [datasheet](http://edevice.fujitsu.com/fj/DATASHEET/e-ds/MB85RC64V-DS501-00013-7v0-E.pdf) |
| | Manufacturer: 0x00A, ProductID: 0x???, DensityCode: 0x03, Maxaddress: 8192, Address Length: 13bits, Density: 64, R/W cycles: 10^12 | Yes |
| MB85RC128A | [datasheet](http://www.fujitsu.com/downloads/MICRO/fsa/pdf/products/memory/fram/e513110.pdf) |
| | Manufacturer: 0x00A, ProductID: 0x???, DensityCode: 0x04, Maxaddress: 16384, Address Length: 14bits, Density: 128, R/W cycles: 10^12 | Yes |
| MB85RC256V | [datasheet](http://www.fujitsu.com/downloads/MICRO/fsa/pdf/products/memory/fram/MB85RC256V-DS501-00017-3v0-E.pdf) |
| | Manufacturer: 0x00A, ProductID: 0x510, DensityCode: 0x05, Maxaddress: 32768, Address Length: 15bits, Density: 256, R/W cycles: 10^12 | Yes |
| MB85RC512T | [datasheet](http://www.fujitsu.com/downloads/MICRO/fsa/pdf/products/memory/fram/MB85RC512T-DS501-00028-0v01-E.pdf) |
| | Manufacturer: 0x00A, ProductID: 0x658, DensityCode: 0x06, Maxaddress: 65536, Address Length: 16bits, Density: 512, R/W cycles: 10^13 | Yes |
| MB85RC1MT | [datasheet](http://www.fujitsu.com/downloads/MICRO/fsa/pdf/products/memory/fram/MB85RC1MT-DS501-00027-0v01-E.pdf) |
| | Manufacturer: 0x00A, ProductID: 0x758, DensityCode: 0x07, Maxaddress: 131072, Density: 1024, R/W cycles: 10^13 | Yes |

Note 1 : MB85RC04V has a 9 bits adressing memory map. The 9th bit is set in the device address bytes
Note 2 : MB85RC16V has a 11 bits adressing memory map. The 3 MSB are set in the device address bytes in place of A2~A0


## Adresses ##
Devices address : b1010 + A2 + A1 + A0.

All devices are pulling down internaly A2, A1 & A0. Default address is b1010000 (0x50) - exception 1M chips which seems to be a double 512K devices in a single package

MB85RC16V devices does not have A2, A1 nor A0 support. This is used for memory addressing
MB85RC04V devices have only A2 & A1 support. A0 is used for memory addressing.

## Errors ##
The error management is eased by returning a byte value for almost each method. Most of the time, this is the status code from Wire.endTransmission() function.
- 0: success
- 1: data too long to fit in transmit buffer
- 2: received NACK on transmit of address
- 3: received NACK on transmit of data
- 4: Serial not accessible
- 5: Not referenced device ID
- 7: Fram chip unidentified
- 8: number of bytes asked to read null
- 9: bit position out of range
- 10: not permitted operation

## Testing ##
- Tested only against MB85RC256V - breakout board from Adafruit http://www.adafruit.com/product/1895
- Tested on Arduino Mega with Arduino IDE 1.0.5
- Please comment about other devices (Memory & Arduino Boards)
- When trying to get the device IDs of MB85RC16V, @porcao got 0xFFF as manufacturer ID, OxFFF as product ID and 0xF as Density code. Issue seems comming from the mix of chip address & memory address when sending commande according to Wire lib.

## To do ##
- Create a more robust error management (function to handle that with higher layer)
- Rework the debug mode

## Credits ##
- [Kevin Townsend](https://github.com/microbuilder) who wrote the [Adafruit Lib](https://github.com/adafruit/Adafruit_FRAM_I2C)