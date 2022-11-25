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

bool getNewData = true;
const unsigned long eventInterval = 60000;
unsigned long previousTime = 0;

void getData()
{
    prices = RESTReader::fetchPrices();

}

void sendData()
{
    modbus.updateHoldingRegister(RESTReader::fetchPrices());
    modbus.pollDataOnce();
}

void setup()
{

    debugger.startSerial();
    modbus.Begin();
}

void loop()
{
    /* Updates frequently to check for new time*/
    unsigned long currentTime = millis();

    /* Check if time differace is grater than eventinterval */
    if (currentTime - previousTime >= eventInterval)
    {
        /* Tell code to get new data */
        getNewData = true;
        /* Update the timing for the next time around */
        previousTime = currentTime;
    }

    if (getNewData)
    {
        getData();
        getNewData = false;
    }

    sendData();
}
