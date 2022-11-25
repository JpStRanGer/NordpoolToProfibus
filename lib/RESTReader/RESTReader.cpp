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
void RESTReader::connect()
{
    debug("running test() from RESTReader...");

    // this->server  = "nordpoolveas.jonaspettersen.no";
    sprintf(server, "nordpoolveas.jonaspettersen.no"); // Write con.string to array

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
bool RESTReader::SkipHTTPheaders()
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
 * @brief Creating a printf() wrapper
 * @ref https://playground.arduino.cc/Main/Printf/
 *
 * @param fmt is input STRING ex. printf("this is a %s","text") prints: this is a text
 * @param ... is as many following arguments as needed in the formated string fmt
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
    client.readBytesUntil('\r\n', status, sizeof(status));
    // Serial.print(F("Serial response: "));
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
void RESTReader::json()
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
    int i = 0;
    this->debug("Start FOR-loop...");
    for (JsonObject prices_item : doc["data"].as<JsonArray>())
    {
        const char *prices_item_name = prices_item["name"]; // "00 - 01", "01 - 02", "02 - 03", "03 - 04", ...
        float prices_item_value = prices_item["value"];     // 2053.76, 2036.25, 2030.67, 2031.85, 2042.02, 2176.52, ...

        this->prices.prices[i] = prices_item_value;
        i++;
    }
    this->debug("Start META-data...");
    JsonObject meta = doc["meta"];

    this->prices.min = meta["min"];               // 1977.85
    this->prices.max = meta["max"];               // 6533.37
    this->prices.average = meta["average"];       // 2577.98
    this->prices.peak = meta["peak"];             // 2678.39
    this->prices.off_peak_1 = meta["off_peak_1"]; // 2652.87
    this->prices.off_peak_2 = meta["off_peak_2"]; // 2126.95
}

/**
 * @brief Prints all Class/price values on to serial monitor.
 *
 */
void RESTReader::printPrizesSerial()
{
    for (float prize : this->prices.prices)
    {
        Serial.print("time of day: ");
        // Serial.print(prize);
        Serial.print(", value: ");
        Serial.println(prize);
    }

    Serial.print("meta_min: ");
    Serial.println(this->prices.min);
    Serial.print("meta_max: ");
    Serial.println(this->prices.max);
    Serial.print("meta_average: ");
    Serial.println(this->prices.average);
    Serial.print("meta_peak: ");
    Serial.println(this->prices.peak);
    Serial.print("meta_off_peak_1: ");
    Serial.println(this->prices.off_peak_1);
    Serial.print("meta_off_peak_2: ");
    Serial.println(this->prices.off_peak_2);
}

/**
 * @brief Get Prices
 *
 * @return a structure (struct) of prices, containing all the prices.
 */
Prices RESTReader::getPrices()
{
    return this->prices;
}

void RESTReader::convertPriceUnit(float unit)
{
    int arrLength = sizeof(this->prices.prices) / 4;
    for (int i = 0; i < arrLength; i++)
    {
        this->prices.prices[i] = this->prices.prices[i] * unit;
    }
    this->prices.min *= unit;
    this->prices.max *= unit;
    this->prices.average *= unit;
    this->prices.peak *= unit;
    this->prices.off_peak_1 *= unit;
    this->prices.off_peak_2 *= unit;
}

/**
 * @brief Debug method for enabling internal serial printing.
 * NEEDS serialBegin to be activatet outside this class!
 *
 * @param msg
 */
void RESTReader::debug(char *msg)
{
    if (!this->shouldDebug)
        return;

    char buff[180];
    sprintf(buff, "DEBUG:\t%s\n", msg);
    Serial.print(buff);

    // Serial.print("DEBUG: ");
    // Serial.println(msg);
}

/**
 * @brief Fetching data from Nordpool API
 *
 */
void RESTReader::fetchData()
{
    this->connect();
    this->checkHTTPstatus();
    this->SkipHTTPheaders();
    this->json();
    this->printPrizesSerial();
    // this->convertPriceUnit(1);
    // this->printPrizesSerial();
}

/**
 * @brief 
 * 
 * @return Prices 
 */
static Prices RESTReader::fetchPrices()
{
    Prices prices;

    RESTReader restReader;

    restReader.fetchData();

    prices = restReader.getPrices();

    restReader.~RESTReader();

    return prices;
}