/**
 * @file RESTReader.cpp
 * @author Jonas Pettersen (Jonas.st.pettersen@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "RESTReader.h"

/**
 * @brief Construct a new RESTReader object
 *
 */
RESTReader::RESTReader()
{
    // this->startSerial();
    // this->test();
    // this->checkHTTPstatus();
}

/**
 * @brief Destroy the RESTReader::RESTReader object
 *
 */
RESTReader::~RESTReader()
{
}

/**
 * @brief Method for testing vital functions
 *
 */
void RESTReader::test()
{
    Serial.println("running test() from RESTReader...");

    // this->server  = "nordpoolveas.jonaspettersen.no";
    sprintf(server, "nordpoolveas.jonaspettersen.no"); // Write con.string to array

    Serial.println("call function: startEthernet()");
    // Enter a MAC address for your controller below.
    // Newer Ethernet shields have a MAC address printed on a sticker on the shield
    byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

    // Set the static IP address to use if the DHCP fails to assign
    IPAddress ip(192, 168, 0, 177);
    IPAddress myDns(192, 168, 0, 1);

    // start the Ethernet connection:
    Serial.println("Initialize Ethernet with DHCP:");
    if (Ethernet.begin(mac) == 0)
    {
        Serial.println("Failed to configure Ethernet using DHCP");
        // Check for Ethernet hardware present
        if (Ethernet.hardwareStatus() == EthernetNoHardware)
        {
            Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
            while (true)
            {
                delay(1); // do nothing, no point running without Ethernet hardware
            }
        }
        if (Ethernet.linkStatus() == LinkOFF)
        {
            Serial.println("Ethernet cable is not connected.");
        }
        // try to congifure using IP address instead of DHCP:
        Ethernet.begin(mac, ip, myDns);
    }
    else
    {
        Serial.print("  DHCP assigned IP ");
        Serial.println(Ethernet.localIP());
    }
    // give the Ethernet shield a second to initialize:
    delay(1000);
    Serial.print("connecting to ");
    Serial.print(server);
    Serial.println("...");

    // if you get a connection, report back via serial:
    if (client.connect(server, 80))
    {
        Serial.print("connected to ");
        Serial.println(client.remoteIP());
        // Make a HTTP request:
        client.println("GET /api/nordpool/hourly HTTP/1.0");
        // client.println("GET /api/nordpool/daily HTTP/1.1");
        client.println("Host: nordpoolveas.jonaspettersen.no");
        // client.println("Connection: close");
        client.println();
    }
    else
    {
        // if you didn't get a connection to the server:
        Serial.println("connection failed");
    }
}
/**
 * @brief Check HTTP status
 *
 * @return true if status is okey
 * @return false if status is bad
 */
bool RESTReader::checkHTTPstatus()
{
    // resurving an data buffer for the incomming data
    char status[32] = {0};
    // read data from datastream (HTTP)
    client.readBytesUntil(0x0D, status, sizeof(status));
    // check if data contain the wanted information.
    Serial.print("DEBUG: print status -> ");
    Serial.println(status);
    Serial.println(strcmp(status, "HTTP/1.1 200 OK"));

    if (strcmp(status, "HTTP/1.1 200 OK") != 0)
    {
        Serial.println(F("Unexpected response: "));
        Serial.println("##########status START##########");
        Serial.println(status);
        for (int i = 0; i < sizeof(status); i++)
        {
            Serial.print(status[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
        Serial.println("#########Status END#########");
        return false;
    }

    Serial.println("DEBUG HTTP status OK!\n");
    return true;
}

/**
 * @brief Skip HTTP headers
 *
 * @return true if HTTP headers skipped OK
 * @return false if Invalid response
 */
bool RESTReader::SkipHTTPheaders()
{
    debug("before");
    Serial.println("call function: SkipHTTPheaders()");
    debug("after");
    //
    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders))
    {
        Serial.println(F("Invalid response"));
        return false;
    }
    this->debug("kasdlknjasdfkl");
    while (client.available() && client.peek() != '{')
    {
        char c = 0;
        client.readBytes(&c, 1);
           Serial.print("BAD CARACTER: ");
           Serial.println(c);
    }
    return true;
}

/**
 * @brief Start Serialcommunication
 *
 */
void RESTReader::startSerial()
{
    // Open serial communications and wait for port to open:
    // if (!Serial)
    Serial.begin(9600);

    Serial.println("call function: startSerial()");
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Serial communication startet!");
}

/**
 * @brief Creating a printf() wrapper
 * @ref https://playground.arduino.cc/Main/Printf/
 * 
 * @param fmt is input STRING ex. printf("this is a %s","text") prints: this is a text
 * @param ... 
 */
void RESTReader::printf(char *fmt, ...)
{
    char buf[128]; // resulting string limited to 128 chars
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, 128, fmt, args);
    va_end(args);
    Serial.print(buf);
}

void RESTReader::debug(char *msg)
{
    if (!this->shouldDebug)
        return;

    Serial.print("DEBUG: ");
    Serial.println(msg);
}
