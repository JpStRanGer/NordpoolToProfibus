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
RESTReader::RESTReader(Prices *p) : prices_(p)
{
    // TODO: add setup from SD-card

    // this->server  = "nordpoolveas.jonaspettersen.no";
    sprintf(server_hostname_, "nordpoolveas.jonaspettersen.no"); // Write con.string to array

    Serial.println("call function: startEthernet()");
    // Enter a MAC address for your controller below.
    // Newer Ethernet shields have a MAC address printed on a sticker on the shield
    // byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; Debug MAC-adresse, kun for debug
    byte mac[] = {0x42, 0x5F, 0xB5, 0xDF, 0xEF, 0x22};

    // Set the static IP address to use if the DHCP fails to assign
    // IPAddress ip(192, 168, 100, 105); Admin-nettadresse for DEBUG
    IPAddress ip(192, 168, 20, 11);
    IPAddress myDns(192, 168, 20, 1); //

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
    // print your local IP address:
    Serial.print("The Local IP is (DHCP): ");
    Serial.println(Ethernet.localIP());
    Serial.print("The IP (hardcoded) is: ");
    Serial.println(ip);
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
int RESTReader::connect()
{
    Serial.println("running connect() from RESTReader...");

    // give the Ethernet shield a second to initialize:
    delay(1000);
    Serial.print("connecting to ");
    Serial.print(server_hostname_);
    Serial.println("...");

    // if you get a connection, report back via serial:
    int n = client_.connect(server_hostname_, server_port_number_);
    if (n)
    {
        Serial.print("connected to ");
        Serial.println(client_.remoteIP());
        // Make a HTTP request:
        client_.println("GET /api/nordpool/hourly HTTP/1.0");
        // client.println("GET /api/nordpool/daily HTTP/1.1");
        client_.print("Host: ");
        client_.println(server_hostname_);
        // client.println("Connection: close");
        client_.println();
    }
    else
    {
        // if you didn't get a connection to the server:
        Serial.println("connection failed");
    }
    return n;
}

/**
 * @brief Check HTTP status
 *
 * @return true if status is okey
 * @return false if status is bad
 */
bool RESTReader::checkHTTPstatus()
{
    // resurving an prices buffer for the incomming prices
    char status[32] = {0};
    // read prices from pricesstream (HTTP)
    client_.readBytesUntil(0x0D, status, sizeof(status));
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

    Serial.println("HTTP status OK!\n");
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
    Serial.println("call function: SkipHTTPheaders()");

    char endOfHeaders[] = "\r\n\r\n";
    if (!client_.find(endOfHeaders))
    {
        Serial.println(F("Invalid response"));
        return false;
    }

    while (client_.available() && client_.peek() != '{')
    {
        char c = 0;
        client_.readBytes(&c, 1);
        Serial.print("BAD CARACTER: ");
        Serial.println(c);
    }
    return true;
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

/**
 * @brief DEBUG DUNCTIONS
 * print one Line of the inncomming prices i client
 *
 */
void RESTReader::DEBUG_printOneLineFromHTTP()
{

    // Check HTTP status
    char status[250] = {0};
    client_.readBytesUntil('\r', status, sizeof(status));
    // Serial.print(F("Serial response: "));
    for (unsigned int i = 0; i < sizeof(status); i++)
    {
        switch (status[i])
        {
        case 0x0A:
            Serial.print("0A");
            break;
        case 0x0D:
            Serial.print("0D");
            break;
        default:
            Serial.print(status[i], HEX);
            break;
        }
        // Serial.print(status[i],HEX);
        // Serial.print(status[i],BIN);
        Serial.print(" ");
    }
    Serial.println();
    for (int i = 0; i < sizeof(status); i++)
    {
        switch (status[i])
        {
        case 0x0A:
            Serial.print("0A");
            break;
        case 0x0D:
            Serial.print("0D");
            break;
        default:
            Serial.print(status[i]);
            break;
        }
        // Serial.print(status[i]);
    }
    Serial.println();
}

/**
 * @brief Converting Json prices to arduino C types.
 *
 */
bool RESTReader::parse_payload()
{
    Serial.println("Start parse_payload()...");
    Stream &input = client_;

    StaticJsonDocument<1024> doc;
    // DynamicJsonDocument doc(1024);

    DeserializationError error = deserializeJson(doc, input);

    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        Serial.print("doc.capacity: ");
        Serial.println(doc.capacity());

        return false;
    }

    int i = 0;
    Serial.println("Start FOR-loop...");
    for (JsonObject prices_item : doc["data"].as<JsonArray>())
    {
        // const char *prices_item_name = prices_item["name"]; // "00 - 01", "01 - 02", "02 - 03", "03 - 04", ...
        float prices_item_value = prices_item["value"]; // 2053.76, 2036.25, 2030.67, 2031.85, 2042.02, 2176.52, ...

        if (i <= sizeof(prices_->hour_prices) - 1)
        {
            prices_->hour_prices[i] = prices_item_value;
        }
        i++;
    }

    Serial.print("Number of values:");
    Serial.println(i);
    Serial.println("Start META-data...");
    JsonObject meta = doc["meta"];

    prices_->min = meta["min"];               // 1977.85
    prices_->max = meta["max"];               // 6533.37
    prices_->average = meta["average"];       // 2577.98
    prices_->peak = meta["peak"];             // 2678.39
    prices_->off_peak_1 = meta["off_peak_1"]; // 2652.87
    prices_->off_peak_2 = meta["off_peak_2"]; // 2126.95

    return true;
}
