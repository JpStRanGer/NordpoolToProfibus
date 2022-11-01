#ifndef JPMODBUS_H
#define JPMODBUS_H

/*
  Modbus RTU Server Kitchen Sink

  This sketch creates a Modbus RTU Server and demonstrates
  how to use various Modbus Server APIs.

  Circuit:
   - MKR board
   - MKR 485 shield
     - ISO GND connected to GND of the Modbus RTU server
     - Y connected to A/Y of the Modbus RTU client
     - Z connected to B/Z of the Modbus RTU client
     - Jumper positions
       - FULL set to OFF
       - Z \/\/ Y set to OFF

  created 18 July 2018
  by Sandeep Mistry
*/

#include <SPI.h>
#include <Arduino.h>
#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>

class JpModbus
{
private:
    const int _numCoils = 10;
    const int _numDiscreteInputs = 10;
    const int _numHoldingRegisters = 10;
    const int _numInputRegisters = 10;

public:
    JpModbus();
    void Begin();
    void Loop();
};

/**
 * @brief Construct a new Modbus:: Modbus object
 *
 */
JpModbus::JpModbus()
{
}

/**
 * @brief
 *
 */
void JpModbus::Begin()
{
    Serial.begin(9600);
    while (!Serial)
        delay(1000);

    Serial.println("Modbus RTU Server Kitchen Sink");
    delay(1000);

    // start the Modbus RTU server, with (slave) id 4
    if (!ModbusRTUServer.begin(4, 9600))
    {
        Serial.println("Failed to start Modbus RTU Server!");
        while (1)
            ;
    }

    // configure coils at address 0x00
    ModbusRTUServer.configureCoils(0x00, this->_numCoils);

    // configure discrete inputs at address 0x00
    ModbusRTUServer.configureDiscreteInputs(0x00, this->_numDiscreteInputs);

    // configure holding registers at address 0x00
    ModbusRTUServer.configureHoldingRegisters(0x00, this->_numHoldingRegisters);

    // configure input registers at address 0x00
    ModbusRTUServer.configureInputRegisters(0x00, this->_numInputRegisters);
    ModbusRTUServer.holdingRegisterWrite(0x01, 1234);
    Serial.println("Modbus RTU configureInputRegisters ");
}

void JpModbus::Loop()
{
;
}
#endif