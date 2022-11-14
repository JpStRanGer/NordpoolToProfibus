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
const unsigned long eventInterval = 10000;
unsigned long previousTime = 0;

void getData()
{

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
}

void sendData()
{
    modbus.updateHoldingRegister(prices);
    modbus.pollDataOnce();
}

void setup()
{

    debugger.startSerial();
    modbus.Begin();
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
    }

    
}
