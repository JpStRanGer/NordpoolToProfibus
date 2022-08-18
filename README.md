# NordpoolToProfibus
## _Simple application for getting electricity prices to any SCADA system_

## Introduction
This application utilizes Arduino Mega with rs485 shield and ethernet shield to webscrape the REST-API of [Nordpool](https://www.nordpoolgroup.com/) to catch hourely and dayly electricity prices and send it to any SCADA- or PLS- system using Profibus.

## Technologies
the NordpoolToProfibus application uses different hardware and open source projects to work properly:
### Hardware
- [Arduino Mega 2560](https://store.arduino.cc/products/arduino-mega-2560-rev3)
- [DFRobot RS485 Shield V1.0](https://wiki.dfrobot.com/Arduino_RS485_Shield_SKU__DFR0259)
- [Arduino Ethernet Shield](https://docs.arduino.cc/retired/shields/arduino-ethernet-shield-without-poe-module)
### Software
- [Arduino C](https://www.arduino.cc/reference/en/libraries/) - Spessilized version of C++ made for programming arduino boards and other microcontrollers.
- [ArduinoJson.h](https://arduinojson.org/) - library for easy handling of Json data.
- [SPI.h](https://www.arduino.cc/reference/en/language/functions/communication/spi/) - Serial library.
- [Ethernet.h](https://www.arduino.cc/reference/en/libraries/ethernet/) - library for utilizing ethernet communication and sending/receive HTTP querys.
