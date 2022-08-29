#include "NordPool.h"
/**
 * @brief Construct a new Nord Pool:: Nord Pool object
 *
 */
NordPool::NordPool()
{
}

/**
 * @brief Construct a new Nord Pool:: Nord Pool object
 *
 * @param server - URL for where the program should look for Json-data.
 */

NordPool::NordPool(String server)
{
    Serial.println("#START (Construct): NP.NordPool()");
    // int len = server.length() + 1;
    // server.toCharArray(_server, len);
    this->setServer(server);
    Serial.println("#END (Construct): NP.NordPool()");
}

/**
 * @brief Destroy the Nord Pool:: Nord Pool object
 *
 */
NordPool::~NordPool()
{
}

/**
 * @brief Method sets servername on private propertie _server.
 *
 * @param server - URL for where the program should look for Json-data.
 */
void NordPool::setServer(String server)
{
    Serial.println("#START (setServer): NP.setServer()");
    int len = server.length() + 1;
    server.toCharArray(_server, len);
    Serial.println("#END (setServer): NP.setServer()");
}

/**
 * @brief Starting serial interface
 *
 */
void NordPool::startSerial()
{
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    Serial.println("call function: startSerial()");
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Serial communication startet!");
}

/**
 * @brief
 *
 */
void NordPool::connect()
{
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
    Serial.print(_server);
    Serial.println("...");

    // if you get a connection, report back via serial:

    if (client.connect(_server, 80))
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
        Serial.println("NordPool::connect()  ->  connection failed");
    }
};

bool NordPool::checkHTTPstatus()
{
    // resurving an data buffer for the incomming data
    char status[32] = {0};
    // read data from datastream (HTTP)
    client.readBytesUntil(0x0D, status, sizeof(status));
    // check if data contain the wanted information.
    if (strcmp(status, "HTTP/1.1 200 OK") != 0)
    {
        Serial.println(F("Unexpected response: "));
        Serial.println("##########status START##########");
        Serial.println(status);
        for (int i = 0; i < int(sizeof(status)); i++)
        {
            Serial.print(status[i], HEX);
            Serial.print(" ");
        }
        Serial.println();
        Serial.println("#########Status END#########");
        return false;
    }
    return true;
}

bool NordPool::SkipHTTPheaders()
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
        //    Serial.print("BAD CARACTER: ");
        //    Serial.println(c);
    }
    return true;
}

void NordPool::json()
{

    Stream &input = client;

    //  StaticJsonDocument<1024> doc;
    DynamicJsonDocument doc(1024);

    DeserializationError error = deserializeJson(doc, input);

    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        Serial.print("doc.capacity: ");
        Serial.println(doc.capacity());
        return;
    }

    for (JsonObject data_item : doc["data"].as<JsonArray>())
    {

        const char *data_item_name = data_item["name"]; // "00 - 01", "01 - 02", "02 - 03", "03 - 04", ...
        // Serial.print("time of day: ");
        // Serial.print(data_item_name);
        float data_item_value = data_item["value"]; // 2053.76, 2036.25, 2030.67, 2031.85, 2042.02, 2176.52, ...
        // Serial.print(", value: ");
        // Serial.println(data_item_value);

        /* Set values to the class variable*/
        String hour_s = "";
        hour_s.concat(data_item_name[0]);
        hour_s.concat(data_item_name[1]);
        byte hour = hour_s.toInt();
        _prices[hour] = data_item_value;
    }

    JsonObject meta = doc["meta"];
    _min = meta["min"]; // 1977.85
    Serial.print(", meta_min: ");
    Serial.println(_min);

    _max = meta["max"]; // 6533.37
    Serial.print(", meta_max: ");
    Serial.println(_max);

    _average = meta["average"]; // 2577.98
    Serial.print(", meta_average: ");
    Serial.println(_average);

    _peak = meta["peak"]; // 2678.39
    Serial.print(", meta_peak: ");
    Serial.println(_peak);

    _off_peak_1 = meta["off_peak_1"]; // 2652.87
    Serial.print(", meta_off_peak_1: ");
    Serial.println(_off_peak_1);

    _off_peak_2 = meta["off_peak_2"]; // 2126.95
    Serial.print(", meta_off_peak_2: ");
    Serial.println(_off_peak_2);
}

// Logon the internet, and get values from server.
void NordPool::update()
{
    connect();
    // checkHTTPstatus()?Serial.println("##HTTP status OK!##"):Serial.println("##HTTP status FAILD!##");
    if (SkipHTTPheaders())
    {
        Serial.println("## Skip HTTP headers OK! ##");
        Serial.println("## Start json() ##");
        json(); //?Serial.println("##HTTP status OK!##"):Serial.println("##HTTP status FAILD!##");;
        Serial.println("## End json() ##");
    }
    else
    {
        Serial.println("## Skip HTTP headers FAILD! ##");
    }
}

// Print stored values
void NordPool::printValues()
{
    Serial.print("00 - 01:\t");
    Serial.println(_prices[0]);
    Serial.print("01 - 02:\t");
    Serial.println(_prices[1]);
    Serial.print("02 - 02:\t");
    Serial.println(_prices[2]);
    Serial.print("03 - 04:\t");
    Serial.println(_prices[3]);
    Serial.print("04 - 05:\t");
    Serial.println(_prices[4]);
    Serial.print("05 - 06:\t");
    Serial.println(_prices[5]);
    Serial.print("06 - 07:\t");
    Serial.println(_prices[6]);
    Serial.print("07 - 08:\t");
    Serial.println(_prices[7]);
    Serial.print("08 - 09:\t");
    Serial.println(_prices[8]);
    Serial.print("09 - 10:\t");
    Serial.println(_prices[9]);
    Serial.print("10 - 11:\t");
    Serial.println(_prices[10]);
    Serial.print("11 - 12:\t");
    Serial.println(_prices[11]);
    Serial.print("12 - 13:\t");
    Serial.println(_prices[12]);
    Serial.print("13 - 14:\t");
    Serial.println(_prices[13]);
    Serial.print("14 - 15:\t");
    Serial.println(_prices[14]);
    Serial.print("15 - 16:\t");
    Serial.println(_prices[15]);
    Serial.print("16 - 17:\t");
    Serial.println(_prices[16]);
    Serial.print("17 - 18:\t");
    Serial.println(_prices[17]);
    Serial.print("18 - 19:\t");
    Serial.println(_prices[18]);
    Serial.print("19 - 20:\t");
    Serial.println(_prices[19]);
    Serial.print("20 - 21:\t");
    Serial.println(_prices[20]);
    Serial.print("21 - 22:\t");
    Serial.println(_prices[21]);
    Serial.print("22 - 23:\t");
    Serial.println(_prices[22]);
    Serial.print("23 - 24:\t");
    Serial.println(_prices[23]);
    Serial.print("min:\t");
    Serial.println(_min);
    Serial.print("max:\t");
    Serial.println(_max);
    Serial.print("average:\t");
    Serial.println(_average);
    Serial.print("peak:\t");
    Serial.println(_peak);
    Serial.print("off_peak1:\t");
    Serial.println(_off_peak_1);
    Serial.print("off_peak2:\t");
    Serial.println(_off_peak_2);
}
float NordPool::getMin() { return _min; };
float NordPool::getMax() { return _max; };
float NordPool::getAverage() { return _average; };
float NordPool::getPeak() { return _peak; };
float NordPool::getOff_peak_1() { return _off_peak_1; };
float NordPool::getOff_peak_2() { return _off_peak_2; };
float NordPool::getPrice(int h) { return _prices[h]; };