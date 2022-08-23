#include <Arduino.h>
#include <func.h>

///////////////////////////////////////////
// Start serialcomunication for debugging
///////////////////////////////////////////
void startSerial()
{
    // Open serial communications and wait for port to open:
    Serial.begin(115200);
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
void startEthernet()
{

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
}

///////////////////////////////////////////
// Check HTTP status
///////////////////////////////////////////

///////////////////////////////////////////
// Skip HTTP headers
///////////////////////////////////////////
