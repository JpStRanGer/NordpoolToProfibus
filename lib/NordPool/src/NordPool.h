#ifndef NORDPOOL_H
#define NORDPOOL_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Ethernet.h>

extern EthernetClient client;

class NordPool
{
private:
    /* data */
    float _prices[24];
    float _min;
    float _max;
    float _average;
    float _peak;
    float _off_peak_1;
    float _off_peak_2;
    
    char _server[40];
    void json();
    bool checkHTTPstatus();
    bool SkipHTTPheaders();
    void connect();

public:
    NordPool(String server);
    NordPool();
    ~NordPool();
    void setServer(String server);
    void update(); //Run to get new prices from server.
    void printValues();

    float getMin();
    float getMax();
    float getAverage();
    float getPeak();
    float getOff_peak_1();
    float getOff_peak_2();
    float getPrice(int hour);
    

};

#endif