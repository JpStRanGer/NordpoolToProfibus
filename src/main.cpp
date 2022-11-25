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
#include <NordPool.h>
#include <structs.h>

/// @brief
Debugger debugger;
JpModbus modbus;

const unsigned long eventInterval = 1000;
unsigned long previousTime = 0;

void setup()
{

    debugger.startSerial();
    modbus.Begin();
}

void loop()
{
    /* Updates frequently to check for new time*/
    unsigned long currentTime = millis();
    float timeDiff = currentTime - previousTime;

    /* Check if time differace is grater than eventinterval */
    if (timeDiff >= eventInterval)
    {
        modbus.updateHoldingRegister(NordPool::fetchPrices());
        previousTime = currentTime;
    }

    modbus.pollDataOnce();
}
