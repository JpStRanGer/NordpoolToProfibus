/**
 * @file main.cpp
 * @author  Jonas Pettersen (Jonas.st.pettersen@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <Arduino.h>
#include <Debugger.hpp>
#include <Modbus.hpp>
#include <RESTReader.h>
#include <structs.h>

/// @brief
Debugger debugger;
JpModbus modbus;
Prices prices;

void getData()
{
}

void sendData()
{
}

void setup()
{
    debugger.startSerial();

    RESTReader restReader;
    restReader.test();
    restReader.checkHTTPstatus();
    restReader.SkipHTTPheaders();
    restReader.json();
    restReader.printPrizesSerial();
    restReader.convertPriceUnit(1);
    restReader.printPrizesSerial();

    prices = restReader.getPrices();
    restReader.~RESTReader();

    modbus.Begin();
}

void loop()
{

    // delay(1000);
    modbus.updateHoldingRegister(prices);
    modbus.pollDataOnce();
    // restReader.DEBUG_printOneLineFromHTTP();
    // //  Serial.print("\n");delay(1000);
    // delay(1000);
}
