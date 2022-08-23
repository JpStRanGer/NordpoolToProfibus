#include <Arduino.h>
#include <Ethernet.h>
#include "func.h"
#include "NordPool.h"
<<<<<<< HEAD

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
// IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
String server = "nordpoolveas.jonaspettersen.no"; // name address for Google (using DNS)
// char server[] = "www.google.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
// EthernetClient client;

EthernetClient client;
NordPool NP;

void setup()
{
    // Open serial communications and wait for port to open:
    startSerial();
    startEthernet();
    NP.setServer(server);
    NP.update();
    NP.print();
=======

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
// IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
String server = "nordpoolveas.jonaspettersen.no"; // name address for Google (using DNS)
// char server[] = "www.google.com";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
// EthernetClient client;

EthernetClient client;
NordPool NP;

void setup()
{
    // Open serial communications and wait for port to open:
    startSerial();
    startEthernet();
    NP.setServer(server);

>>>>>>> 409abbdeddd0d220577de3528d00271d85968c2b
}

void loop()
{
<<<<<<< HEAD



    // static int i = 0;
    // Serial.println(i++);

    // NP.update();
    // NP.print();
    // delay(15000);
=======
    static int i =0;
    Serial.println(i++);

    NP.update();
    NP.print();
    delay(15000);
>>>>>>> 409abbdeddd0d220577de3528d00271d85968c2b
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
