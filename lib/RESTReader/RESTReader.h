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
//#include <SPI.h>
#include <Ethernet.h>
#include <structs.h>

class RESTReader
{
private:
    /// PRIVATE PROPERTIES/METHODS
    float _prices[24];
    float _min;
    float _max;
    float _average;
    float _peak;
    float _off_peak_1;
    float _off_peak_2;

public:
    /// PUBLIC PROPERTIES
    Prices prices; 
    bool shouldDebug = true;
    EthernetClient client;
    char server[]; // name address for Google (using DNS)

    /// PUBLIC METHODS
    RESTReader(); /// @brief Construct a new RESTReader object
    ~RESTReader(); /// @brief Destroy the RESTReader object
    void test();
    bool checkHTTPstatus();
    bool SkipHTTPheaders();
    void startSerial();
    void printf(char *fmt, ... ); // Creating a printf() wrapper
    void debug(char *msg);
    void DEBUG_printOneLineFromHTTP();
    void json();
    void printPrizesSerial();
    Prices getPrices();

};

#endif