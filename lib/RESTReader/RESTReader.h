/**
 * @file RESTReader.h
 * @author  Jonas Pettersen (Jonas.st.pettersen@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-10-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef RESTReader_h
#define RESTReader_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <Ethernet.h>

class RESTReader
{
private:
    /// @brief PRIVATE PROPERTIES/METHODS
public:
    /// @brief PUBLIC PROPERTIES
    EthernetClient client;
    char server[100]; // name address for Google (using DNS)

    /// @brief PUBLIC METHODS
    RESTReader(); /// @brief Construct a new RESTReader object
    // ~RESTReader(); /// @brief Destroy the RESTReader object
    void test();
    bool checkHTTPstatus();
    void startSerial();
    // void Debug(char msg);
};

#endif