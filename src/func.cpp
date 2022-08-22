#include "func.h"
EthernetClient client;

///////////////////////////////////////////
// Start serialcomunication for debugging
///////////////////////////////////////////
void startSerial()
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

////////////////////////////////
// Start Ethernet connection
////////////////////////////////
void startEthernet(char server[])
{
    int abc = 66;

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

///////////////////////////////////////////
// Check HTTP status
///////////////////////////////////////////
bool checkHTTPstatus()
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

///////////////////////////////////////////
// Skip HTTP headers
///////////////////////////////////////////
bool SkipHTTPheaders()
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

void json()
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
        Serial.print("time of day: ");
        Serial.print(data_item_name);
        float data_item_value = data_item["value"]; // 2053.76, 2036.25, 2030.67, 2031.85, 2042.02, 2176.52, ...
        Serial.print(", value: ");
        Serial.println(data_item_value);
    }

    JsonObject meta = doc["meta"];
    float meta_min = meta["min"]; // 1977.85
    Serial.print(", meta_min: ");
    Serial.println(meta_min);
    float meta_max = meta["max"]; // 6533.37
    Serial.print(", meta_max: ");
    Serial.println(meta_max);
    float meta_average = meta["average"]; // 2577.98
    Serial.print(", meta_average: ");
    Serial.println(meta_average);
    float meta_peak = meta["peak"]; // 2678.39
    Serial.print(", meta_peak: ");
    Serial.println(meta_peak);
    float meta_off_peak_1 = meta["off_peak_1"]; // 2652.87
    Serial.print(", meta_off_peak_1: ");
    Serial.println(meta_off_peak_1);
    float meta_off_peak_2 = meta["off_peak_2"]; // 2126.95
    Serial.print(", meta_off_peak_2: ");
    Serial.println(meta_off_peak_2);
}
