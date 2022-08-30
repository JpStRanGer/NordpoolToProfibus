#include <Arduino.h>
#include <Ethernet.h>
#include <NordPool.h>

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
// IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "nordpoolveas.jonaspettersen.no"; // name address for Google (using DNS)
// char server[] = "www.google.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
// EthernetClient client;

EthernetClient client;
// NordPool NP(server);
NordPool NP;

void setup()
{


    // Open serial communications and wait for port to open:
    Serial.println("#START (Main): NP.startSerial()");
    NP.startSerial();
    Serial.println("#END (Main): NP.startSerial()\n");

    Serial.println("#START (Main): NP.setServer()");
    NP.setServer(server);
    Serial.println("#END (Main): NP.setServer()\n");

    Serial.println("#START (Main): NP.update()");
    NP.update();
    Serial.println("#END (Main): NP.update()\n");
    
    Serial.println("#START (Main): NP.printValues()");
    NP.printValues();
    Serial.println("#END (Main): NP.printValues()\n");
}

void loop()
{



    // static int i = 0;
    // Serial.println(i++);

    // NP.update();
    // NP.print();
    // delay(15000);
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
