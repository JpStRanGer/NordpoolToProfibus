/**
 * @file RESTReader.h
 * @author your name (you@domain.com)
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
/// @brief PUBLIC PROPERTIES/METHODS
    RESTReader(); /// @brief Construct a new RESTReader object
    ~RESTReader(); /// @brief Destroy the RESTReader object
    void test();
};

#endif