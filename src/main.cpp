#include <Arduino.h>
#include <Ethernet.h>
#include "func.h"

void startSerial();
void startEthernet(EthernetClient client, char server[]);
bool checkHTTPstatus(EthernetClient client);
bool SkipHTTPheaders(EthernetClient client);
void json(EthernetClient client);

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
// IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "nordpoolveas.jonaspettersen.no"; // name address for Google (using DNS)
// char server[] = "www.google.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
// EthernetClient client;

void setup()
{
    // Open serial communications and wait for port to open:
    startSerial();
    startEthernet(server);

    // checkHTTPstatus()?Serial.println("##HTTP status OK!##"):Serial.println("## HTTP status FAILD! ##");
    SkipHTTPheaders() ? Serial.println("## Skip HTTP headers OK! ##") : Serial.println("## Skip HTTP headers FAILD! ##");

    Serial.println("## Start json() ##");
    json(); //?Serial.println("##HTTP status OK!##"):Serial.println("##HTTP status FAILD!##");;
    Serial.println("## End json() ##");

    EthernetClient client;
    Serial.print("\n\nclient.connected():\n");
    int abc;
    Serial.print(abc);
}

void loop()
{
    ////////////////////
    ////// DEBUG //////
    //////////////////
    // printPortStatus();
    // DEBUG_ReadKeyFromSerial();

    // Check for incomming "enter" from Serial.
    //  if (DEBUG_ReadSerial()){
    //    //read one line (available date) from HTTP stream.
    //      DEBUG_printOneLineFromHTTP();
    //      DEBUG_printOneLineFromHTTP_ASCII();
    //      DEBUG_printOneLineFromHTTP_HEX();
    //  }

    ////////////////////
    ////// production //////
    //////////////////
}
