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
RESTReader restReader;
JpModbus modbus;
Prices prices;

void setup()
{
    debugger.startSerial();
    Serial.print("### SETUP START in:");
    for (int i = 0; i < 10; i++)
    {
        Serial.print(" ");
        Serial.print(i);
        Serial.print(" ");

            delay(1000);
    }

    modbus.Begin();

    restReader.test();
    restReader.checkHTTPstatus();
    restReader.SkipHTTPheaders();
    restReader.json();
    prices = restReader.getPrices();
    restReader.printPrizesSerial();

    Serial.println(prices.min);

    // modbus.updateHoldingRegister(prices);

    // Prices prices = restReader.getPrices();
    // for (float price : prices.prices)
    // {
    //     Serial.println(price);
    // }
    // Serial.println(prices.average);
    // Serial.println(prices.max);
    // Serial.println(prices.min);
    // Serial.println(prices.off_peak_1);
    // Serial.println(prices.off_peak_2);
    // Serial.println(prices.off_peak_2);
    // Serial.println(prices.peak);

}

void loop()
{
    // delay(1000);
    // // modbus.updateHoldingRegister(prices);
    modbus.pollDataOnce();
    restReader.DEBUG_printOneLineFromHTTP();
    // //  Serial.print("\n");delay(1000);
    // delay(1000);
}
