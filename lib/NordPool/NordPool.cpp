/**
 * @file NordPool.cpp
 * @author Jonas Pettersen (Jonas.st.pettersen@gmail.com)
 * @brief
 * @version 0.1
 * @date 2022-11-01
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "NordPool.h"

/**
 * @brief Construct a new NordPool object
 *
 */
NordPool::NordPool()
{
    // this->startSerial();
    // this->test();
    // this->checkHTTPstatus();
}

/**
 * @brief Destroy the NordPool::NordPool object
 *
 */
NordPool::~NordPool()
{
}

/**
 * @brief Creating a printf() wrapper
 * @ref https://playground.arduino.cc/Main/Printf/
 *
 * @param fmt is input STRING ex. printf("this is a %s","text") prints: this is a text
 * @param ... is as many following arguments as needed in the formated string fmt
 */
void NordPool::printf(char *fmt, ...)
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
void NordPool::DEBUG_printOneLineFromHTTP()
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
Prices NordPool::getPrices()
{
    // if (this->prices)
    // {
    //     return this->prices;
    // }
    // HttpClient httpClient = HttpClient::get("https://nordpoolveas.jonaspettersen.no/api/nordpool/hourly");
    // StaticJsonDocument<1024> doc = httpClient.getJsonDocument();
    StaticJsonDocument<1024> doc = HttpClient::get("https://nordpoolveas.jonaspettersen.no/api/nordpool/hourly").getJsonDocument();

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

    return this->prices;
}

/**
 * @brief Prints all Class/price values on to serial monitor.
 *
 */
void NordPool::printPrizesSerial()
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


void NordPool::convertPriceUnit(float unit)
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
void NordPool::debug(char *msg)
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
 * @brief
 *
 * @return Prices
 */
static Prices NordPool::fetchPrices()
{
    Prices prices;

    NordPool nordPool;

    prices = nordPool.getPrices();

    nordPool.printPrizesSerial();

    nordPool.~NordPool();

    return prices;
}