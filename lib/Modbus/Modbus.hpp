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
    bool shouldDebug;
    Prices prices;
    // const int _numCoils = 10;
    // const int _numDiscreteInputs = 10;
    const int _numHoldingRegisters = 40;
    // const int _numInputRegisters = 10;

public:
    JpModbus();
    void Begin();
    void updateHoldingRegister(const Prices& prices);
    void pollDataOnce();
    // void TestToWriteData();
    // void debug(char *msg);
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
    Serial.println("Modbus RTU Server Kitchen Sink");
    delay(1000);

    // start the Modbus RTU server, with (slave) id 4
    if (!ModbusRTUServer.begin(4, 9600))
    {
        Serial.println("Failed to start Modbus RTU Server!");
        while (1)
            ;
    }

    // // configure coils at address 0x00
    // ModbusRTUServer.configureCoils(0x00, this->_numCoils);

    // // configure discrete inputs at address 0x00
    // ModbusRTUServer.configureDiscreteInputs(0x00, this->_numDiscreteInputs);

    // configure holding registers at address 0x00
    ModbusRTUServer.configureHoldingRegisters(0x00, this->_numHoldingRegisters);

    // // configure input registers at address 0x00
    // ModbusRTUServer.configureInputRegisters(0x00, this->_numInputRegisters);
    // Serial.println("Modbus RTU configureInputRegisters ");
}

/**
 * @brief Update Holding Register
 *
 * @param prices (struct)
 */
void JpModbus::updateHoldingRegister(const Prices& data)
{
    /// @brief write META-data to holdingRegisterWrite
    /// @param data is of type Prices struct
    ModbusRTUServer.holdingRegisterWrite(0x00, int(data.min));
    ModbusRTUServer.holdingRegisterWrite(0x01, int(data.max));
    ModbusRTUServer.holdingRegisterWrite(0x02, int(data.average));
    ModbusRTUServer.holdingRegisterWrite(0x03, int(data.peak));
    ModbusRTUServer.holdingRegisterWrite(0x04, int(data.off_peak_1));
    ModbusRTUServer.holdingRegisterWrite(0x05, int(data.off_peak_2));

    /// @brief loop throw all prices in array and write them to register
    /// @param data
    int i = 0;
    int firstHoureRegister = 6;
    for (float price : data.prices)
    {
        ModbusRTUServer.holdingRegisterWrite(i + firstHoureRegister, int(price));
        i++;
    }
}

void JpModbus::pollDataOnce()
{

    ModbusRTUServer.poll();
}

// void JpModbus::TestToWriteData()
// {
//     ModbusRTUServer.holdingRegisterWrite(0x00, float(10.56));
//     ModbusRTUServer.holdingRegisterWrite(0x01, float(20));
//     ModbusRTUServer.holdingRegisterWrite(0x02, float(30));
//     ModbusRTUServer.holdingRegisterWrite(0x03, float(40));
//     ModbusRTUServer.holdingRegisterWrite(0x04, float(50.87));
//     ModbusRTUServer.holdingRegisterWrite(0x05, float(660.929));
//     ModbusRTUServer.holdingRegisterWrite(0x06, float(70));
//     ModbusRTUServer.holdingRegisterWrite(0x07, float(80));
//     ModbusRTUServer.holdingRegisterWrite(0x08, float(90));
// }

/**
 * @brief Debug method for enabling internal serial printing.
 * NEEDS serialBegin to be activatet outside this class!
 *
 * @param msg
 */
// void JpModbus::debug(char *msg)
// {
//     if (!this->shouldDebug)
//         return;

//     char buff[180];
//     sprintf(buff, "DEBUG:\t%s\n", msg);
//     Serial.print(buff);

//     // Serial.print("DEBUG: ");
//     // Serial.println(msg);
// }
#endif