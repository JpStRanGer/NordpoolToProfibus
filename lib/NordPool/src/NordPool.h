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
    char _server[40] = "nordpoolveas.jonaspettersen.no";    // name address for Google (using DNS)
    float _prices[24];
    float _min;
    float _max;
    float _average;
    float _peak;
    float _off_peak_1;
    float _off_peak_2;
    
    //char _server[40];
    void json();
    bool checkHTTPstatus();
    bool SkipHTTPheaders();

public:
    void connect();
    NordPool(String server);
    NordPool();
    ~NordPool();
    void setServer(String server);
    void update(); //Run to get new prices from server.
    void printValues();
    void startSerial();

    float getMin();
    float getMax();
    float getAverage();
    float getPeak();
    float getOff_peak_1();
    float getOff_peak_2();
    float getPrice(int hour);
    

};

#endif