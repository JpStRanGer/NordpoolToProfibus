/**
 * @file HttpClient.h
 * @author Jonas Pettersen (Jonas.st.pettersen@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-25
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef HttpClient_h
#define HttpClient_h

#include <Arduino.h>
#include <ArduinoJson.h>
//#include <SPI.h>
#include <Ethernet.h>
#include <structs.h>

class HttpClient
{
private:
    char url[];
    char method[];

public:
    EthernetClient client;
    HttpClient();
    HttpClient(String url, String method);
    ~HttpClient();
    void connect();
    static HttpClient get(String url);
    StaticJsonDocument<1024> getJsonDocument();
}

#endif