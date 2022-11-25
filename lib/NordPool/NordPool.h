/**
 * @file NordPool.h
 * @author  Jonas Pettersen (Jonas.st.pettersen@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-10-28
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef nordpool_h
#define nordpool_h

#include <Arduino.h>
#include <ArduinoJson.h>
//#include <SPI.h>
#include <Ethernet.h>
#include <structs.h>

class NordPool
{
private:
    /// PRIVATE PROPERTIES/METHODS

public:
    /// PUBLIC PROPERTIES
    Prices prices; 
    bool shouldDebug = true;
    EthernetClient client;
    char server[]; // name address for Google (using DNS)

    /// PUBLIC METHODS
    NordPool(); /// @brief Construct a new NordPool object
    ~NordPool(); /// @brief Destroy the NordPool object
    void connect();
    bool checkHTTPstatus();
    bool SkipHTTPheaders();
    void printf(char *fmt, ... ); // Creating a printf() wrapper
    void DEBUG_printOneLineFromHTTP();
    void json();
    void printPrizesSerial();
    Prices getPrices();
    void convertPriceUnit(float unit);
    void debug(char *msg);
    void fetchData();
    static Prices fetchPrices();
    
};

#endif