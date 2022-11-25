/**
 * @file HttpClient.cpp
 * @author Jonas Pettersen (Jonas.st.pettersen@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-25
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "HttpClient.h"

/**
 * @brief Construct a new HttpClient object
 *
 */
HttpClient::HttpClient(String url, String method)
{
    this->host = "nordpoolveas.jonaspettersen.no";
    this->uri = "/api/nordpool/hourly";
    this->method = method;
}

/**
 * @brief Destroy the HttpClient::HttpClient object
 *
 */
HttpClient::~HttpClient()
{
}

/**
 * @brief Fetching data from Nordpool API
 *
 */
void NordPool::fetchData()
{
    this->connect();
    this->checkHTTPstatus();
    this->skipHTTPheaders();
    this->setData();
}

void NordPool::setData()
{
    Serial.println("Start Json...");
    Stream &input = client;

    StaticJsonDocument<1024> doc;
    // DynamicJsonDocument doc(1024);

    DeserializationError error = deserializeJson(doc, input);

    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        Serial.print("doc.capacity: ");
        Serial.println(doc.capacity());
        return;
    }
    this->data = doc;
}

/**
 * @brief Method for testing vital functions
 *
 */
void HttpClient::connect()
{
    debug("running connect() from HttpClient...");

    // this->server  = "nordpoolveas.jonaspettersen.no";
    sprintf(server, this->url); // Write con.string to array

    debug("call function: startEthernet()");
    // Enter a MAC address for your controller below.
    // Newer Ethernet shields have a MAC address printed on a sticker on the shield
    byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

    // Set the static IP address to use if the DHCP fails to assign
    IPAddress ip(192, 168, 100, 105);
    IPAddress myDns(192, 168, 0, 1);

    // start the Ethernet connection:
    debug("Initialize Ethernet with DHCP:");
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
        client.print(this->method);
        client.print(" ");
        client.print(this->uri);
        client.println(" HTTP/1.0");
        // client.println("GET /api/nordpool/daily HTTP/1.1");
        client.print("Host: ");
        client.println(this->host);
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
bool NordPool::checkHTTPstatus()
{
    // resurving an prices buffer for the incomming prices
    char status[32] = {0};
    // read prices from pricesstream (HTTP)
    client.readBytesUntil(0x0D, status, sizeof(status));
    // check if prices contain the wanted information.

    if (strcmp(status, "HTTP/1.1 200 OK") != 0)
    {
        Serial.println(F("Unexpected response: "));
        for (int i = 0; i < sizeof(status); i++)
        {
            Serial.print(status[i], HEX);
            Serial.print(" ");
        }
        return false;
    }

    this->debug("HTTP status OK!\n");
    return true;
}

/**
 * @brief Skip HTTP headers
 *
 * @return true if HTTP headers skipped OK
 * @return false if Invalid response
 */
bool NordPool::skipHTTPheaders()
{
    Serial.println("call function: SkipHTTPheaders()");

    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders))
    {
        Serial.println(F("Invalid response"));
        return false;
    }

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
 * @brief
 *
 * @param url
 * @return HttpClient
 */
static HttpClient HttpClient::get(String url)
{
    HttpClient instance = new HttpClient(url, "GET");

    instance.fetchData();

    return instance;
}

/**
 * @brief Get the Data object
 * 
 * @return StaticJsonDocument<1024> 
 */
StaticJsonDocument<1024> HttpClient::getJsonDocument()
{
    return this->data;
}

/**
 * @brief 
 * 
 * @param msg 
 */
void HttpClient::debug(char *msg)
{
    if (!this->shouldDebug)
        return;

    char buff[180];
    sprintf(buff, "DEBUG:\t%s\n", msg);
    Serial.print(buff);

    // Serial.print("DEBUG: ");
    // Serial.println(msg);
}