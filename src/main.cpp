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
#include "Debugger.hpp"
#include "Modbus.hpp"
#include "RESTReader.h"
#include "Prices.h"

/// @brief
Debugger debugger;
JpModbus modbus;
Prices prices;
RESTReader *restReader;

bool getNewData = true;
const unsigned long eventInterval = 3600000;
unsigned long previousTime = 0;

void getData()
{
    if (restReader->connect() == 0)
    {
        Serial.print("Error: did not connect!");
        return;
    }

    if (!restReader->checkHTTPstatus())
    {
        Serial.print("Error: check HTTP status");
        return;
    }

    if (!restReader->SkipHTTPheaders())
    {
        Serial.print("Error: Skip HTTP headers");
        return;
    }

    if (!restReader->parse_payload())
    {
        Serial.print("Error: parse payload");
        return;
    }

    prices.print_prices();
}

void sendData()
{
    modbus.updateHoldingRegister(prices);
}

void setup()
{
    debugger.startSerial();
    restReader = new RESTReader(&prices);
    modbus.Begin();
    // modbus.TestToWriteData();
}

void loop()
{

    if (getNewData)
    {
        getData();
        getNewData = false;
    }

    /* Updates frequently */
    unsigned long currentTime = millis();

    /* This is the event */
    if (currentTime - previousTime >= eventInterval)
    {
        /* Event code */
        sendData();

        /* Update the timing for the next time around */
        previousTime = currentTime;

        getNewData = true;
    }

    // delay(1000);
    modbus.pollDataOnce();
    // Serial.println("poll");
}
