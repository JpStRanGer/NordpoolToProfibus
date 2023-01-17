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
#include "Prices.h"

class RESTReader
{
private:
    /// PRIVATE PROPERTIES/METHODS
    EthernetClient client_;
    char server_hostname_[50]; // name address for Google (using DNS)
    Prices *prices_; 
    uint16_t server_port_number_ = 80; 

public:
    /// PUBLIC METHODS
    RESTReader(Prices *p); /// @brief Construct a new RESTReader object
    ~RESTReader(); /// @brief Destroy the RESTReader object
    int connect();
    bool checkHTTPstatus();
    bool SkipHTTPheaders();
    void printf(char *fmt, ... ); // Creating a printf() wrapper
    void DEBUG_printOneLineFromHTTP();
    bool parse_payload();

};

#endif