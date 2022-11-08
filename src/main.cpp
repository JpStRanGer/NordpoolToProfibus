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
#include <RESTReader.h>
#include <structs.h>

/// @brief
Debugger debugger;
RESTReader restReader;


void setup()
{
    debugger.startSerial();

    restReader.test();
    restReader.checkHTTPstatus();
    restReader.SkipHTTPheaders();
    restReader.json();
    restReader.printPrizes();
}

void loop()
{
    // restReader.DEBUG_printOneLineFromHTTP();
    // Serial.print("\n");delay(1000);
}
