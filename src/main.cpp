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
    restReader.printPrizesSerial();

    Prices prices = restReader.getPrices();
    for (float price : prices.prices)
    {
        Serial.println(price);
    }
    Serial.println(prices.average);
    Serial.println(prices.max);
    Serial.println(prices.min);
    Serial.println(prices.off_peak_1);
    Serial.println(prices.off_peak_2);
    Serial.println(prices.off_peak_2);
    Serial.println(prices.peak);
}

void loop()
{
    // restReader.DEBUG_printOneLineFromHTTP();
    // Serial.print("\n");delay(1000);
}
