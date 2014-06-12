Arduino library for I2C FRAM MB85RC series from Fujitsu
==============
V 1.0 - 2014-06


MB85RC from Fujitsu is a I2C Ferroelectric Random Access Memory (FRAM). Read/write endurance for each memory slot : 10^12 cycles.
16 bit adresses, 8 bits data slots.

Supports 64K, 128K, 256K & 512K devices. Works for 1M devices when considering each device as 2 differents 512K devices (see datasheet: 17 bits addresses of which MSB is replacing A0 device address bit).

[MB85RCxxxx](http://www.fujitsu.com/us/semiconductors/memory/fram/#serial2) Fujitsu's page

## Features ##
- Manage single bit (read, set, clear, toggle) from a byte
- Write one 8-bits, 16-bits or 32-bits value
- Write one array of bytes 
- Read one 8-bits, 16-bits or 32-bits value
- Read one array of bytes (up to 256 per call - maximum supported by Arduino's Wire lib)
- Get device information
	- 1: Manufacturer ID
	- 2: Product ID
	- 3: Density code
	- 4: Density human readable
- Manage write protect pin
- Erase memory (set all chip to 0x00)
- Debug mode manageable from header file


## Devices ##
| Devices | Information |
| ------- | ----------- |
| MB85RC64A | [datasheet](http://www.fujitsu.com/downloads/MICRO/fsa/pdf/products/memory/fram/MB85RC64A-DS501-00019-2v0-E.pdf) |
| | Manufacturer: 0x00A, ProductID: 0x???, DensityCode: 0x03, Maxaddress: 8192, Density: 64, R/W cycles: 10^12 |
| MB85RC128A | [datasheet](http://www.fujitsu.com/downloads/MICRO/fsa/pdf/products/memory/fram/e513110.pdf)
| | Manufacturer: 0x00A, ProductID: 0x???, DensityCode: 0x04, Maxaddress: 16384, Density: 128, R/W cycles: 10^12 |
| MB85RC256V | [datasheet](http://www.fujitsu.com/downloads/MICRO/fsa/pdf/products/memory/fram/MB85RC256V-DS501-00017-3v0-E.pdf) |
| | Manufacturer: 0x00A, ProductID: 0x510, DensityCode: 0x05, Maxaddress: 32768, Density: 256, R/W cycles: 10^12 |
| MB85RC512T | [datasheet](http://www.fujitsu.com/downloads/MICRO/fsa/pdf/products/memory/fram/MB85RC512T-DS501-00028-0v01-E.pdf) |
| | Manufacturer: 0x00A, ProductID: 0x658, DensityCode: 0x06, Maxaddress: 65536, Density: 512, R/W cycles: 10^13 |
| MB85RC1MT | [datasheet](http://www.fujitsu.com/downloads/MICRO/fsa/pdf/products/memory/fram/MB85RC1MT-DS501-00027-0v01-E.pdf) |
| | Manufacturer: 0x00A, ProductID: 0x758, DensityCode: 0x07, Maxaddress: 131072, Density: 1024, R/W cycles: 10^13 |

## Adresses ##
Devices address : b1010 + A2 + A1 + A0.

All devices are pulling down internaly A2, A1 & A0. Default address is b1010000 (0x50) - exception 1M chips which seems to be a double 512K devices in a single package

## Errors ##
The error management is eased by returning a byte value for almost each method. Most of the time, this is the status code from Wire.endTransmission() function.
- 0: success
- 1: data too long to fit in transmit buffer or other error
- 2: received NACK on transmit of address
- 3: received NACK on transmit of data
- 4: other error
- 9: bit position out of range

## Testing ##
- Tested only against MB85RC256V - breakout board from Adafruit http://www.adafruit.com/product/1895
- Tested on Arduino Mega 
- Please comment about other devices (Memory & Arduino Boards)


