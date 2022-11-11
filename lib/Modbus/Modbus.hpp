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

#ifndef JPMODBUS_H
#define JPMODBUS_H

#include <SPI.h>
#include <Arduino.h>
#include <ArduinoRS485.h> // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>
#include <structs.h>

class JpModbus
{
private:
    Prices prices;
    const int _numCoils = 10;
    const int _numDiscreteInputs = 10;
    const int _numHoldingRegisters = 10;
    const int _numInputRegisters = 10;

public:
    JpModbus();
    void Begin();
    void updateHoldingRegister(Prices prices);
    void pollDataOnce();
    void TestToWriteData();
};

/**
 * @brief Construct a new Modbus:: Modbus object
 *
 */
JpModbus::JpModbus()
{
}

/**
 * @brief Setting up modbus variables and start Server/Slave.
 *
 */
void JpModbus::Begin()
{
    // Serial.begin(9600);
    // while (!Serial)
    //     delay(1000);

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
    Serial.println("Modbus RTU configureInputRegisters ");

    //TestToWriteData();
}

/**
 * @brief Update Holding Register
 *
 * @param prices (struct)
 */
void JpModbus::updateHoldingRegister(Prices data)
{
    int i = 0;
    for (float price : data.prices)
    {
        ModbusRTUServer.holdingRegisterWrite(i, data.prices);
    }
}

void JpModbus::pollDataOnce()
{

    ModbusRTUServer.poll();

}

void JpModbus::TestToWriteData()
{
    ModbusRTUServer.holdingRegisterWrite(0x00, float(10.56));
    ModbusRTUServer.holdingRegisterWrite(0x01, float(20));
    ModbusRTUServer.holdingRegisterWrite(0x02, float(30));
    ModbusRTUServer.holdingRegisterWrite(0x03, float(40));
    ModbusRTUServer.holdingRegisterWrite(0x04, float(50.87));
    ModbusRTUServer.holdingRegisterWrite(0x05, float(660.929));
    ModbusRTUServer.holdingRegisterWrite(0x06, float(70));
    ModbusRTUServer.holdingRegisterWrite(0x07, float(80));
    ModbusRTUServer.holdingRegisterWrite(0x08, float(90));
}
#endif