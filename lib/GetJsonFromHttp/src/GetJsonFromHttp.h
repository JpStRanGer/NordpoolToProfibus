#ifndef GETJSONFROMHTTP_H
#define GETJSONFROMHTTP_H

#include <Arduino.h>
#include <Ethernet.h>

class GetJsonFromHttp
{
public:
    /**
     * @brief starts serial communication interface
     *
     */
    void startSerial();
    void startEthernet();
    bool checkHTTPstatus();
    bool SkipHTTPheaders();
    bool DEBUG_ReadSerial();
    bool DEBUG_ReadKeyFromSerial();
    void printPortStatus();
    void DEBUG_printOneLineFromHTTP_ASCII();

private:
    // if you don't want to use DNS (and reduce your sketch size)
    // use the numeric IP instead of the name for the server:
    // IPAddress _server(74,125,232,128);  // numeric IP for Google (no DNS)
    char _server[] = "nordpoolveas.jonaspettersen.no"; // name address for Json-file from Nordpool
    // char server[] = "www.google.com";    // name address for Google (using DNS)

    // Initialize the Ethernet client library
    // with the IP address and port of the server
    // that you want to connect to (port 80 is default for HTTP):
    EthernetClient _client;
}

#endif