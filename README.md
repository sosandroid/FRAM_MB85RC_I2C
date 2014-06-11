Arduino library for I2C FRAM MB85RC series from Fujitsu
==============
V 1.0 - 2014-06


MB85RC from Fujitsu is a I2C Ferroelectric Random Access Memory (FRAM). Read/write endurance for each memory slot : 10^12 cycles.
16 bit adresses, 8 bits data slots
Page for MB85RC chips http://www.fujitsu.com/us/semiconductors/memory/fram/#serial2

## Features ##
- Write 1 byte
- Write n bytes (up to 256 per call - maximum supported by Arduino's Wire lib)
- Read n bytes (up to 256 per call - maximum supported by Arduino's Wire lib)
- Get device information
- Manage write protect pin
- Erase memory (set all chip to 0x00)
- Debug mode manageable from .h file


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
Devices address : b1010 + A2 + A1 + A0. Support 64K, 128K, 256K & 512K devices

Works for 1M devices when considering each device as 2 differents 512K devices (see datasheet: 17 bits addresses of which MSB is replacing A0 device address bit).

All devices are pulling down internaly A2, A1 & A0. Default address is b1010000 (0x50) - exception 1M chips which seems to be a double 512K devices in a single package

## Testing ##
- Tested only against MB85RC256V - breakout board from Adafruit http://www.adafruit.com/product/1895
- Tested on Arduino Mega 
- Please comment about other devices (Memory & Arduino Boards)


