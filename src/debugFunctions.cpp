#include <Arduino.h>
#include <Ethernet.h>

//##########################
// SERIAL FUNCTIONS
//##########################
//////////////////////////////////
//// READ "ENTER" FROM SERIAL MONITOR
//////////////////////////////////

bool DEBUG_ReadSerial()
{
    // Print message to SERIAL!
    //  Serial.println("---RUNNING function: DEBUG_ReadSerial()---");
    //  Serial.println(),Serial.println("Type ENTER to run next function...");
    // reserve variables
    int data;
    // check if serial connection is available: The availble function returns the number of available bytes to be read in the stream.
    if (data = Serial.available() > 0)
    {
        //    Serial.print("Serial is available! data = ");
        //    Serial.println(data);
        // Read the stream and stor all bytes into input variable.
        // char input = Serial.read();   //Store the bytes as ASCII
        byte input = Serial.read(); // Store the bytes as HEXADECIMAL?
        if (input == 0x0A)
        { // 0x0A = Enter
            return true;
        }
        return false;
    }
    return false;
}

//////////////////////////////////
//// READ ALL INPUT FROM SERIAL MONITOR (...One line at a time!)
//// And print to ASCII, HEX and DEC
//////////////////////////////////
bool DEBUG_ReadKeyFromSerial()
{
    // Print message to SERIAL!
    //  Serial.println("---RUNNING function: DEBUG_ReadAllSerial()---");
    //  Serial.println(),Serial.println("Type 'any keys' then 'ENTER' to read next line....");
    while (true)
    {
        // reserve variables
        int data;
        // check if serial connection is available: The availble function returns the number of available bytes to be read in the stream.
        if (data = Serial.available() > 0)
        {
            //      Serial.print("available data (BYTE):" );
            //      Serial.print(data);
            //      Serial.print("\t");
            // Read the stream and store all bytes into input variable.
            char input = Serial.read(); // Store the bytes as ASCII
            // byte input = Serial.read(); //Store the bytes as HEXADECIMAL?
            Serial.print(" You typed:");
            Serial.print(" \t- ASCII: ");
            switch (input)
            {
            case 0x0A:
                Serial.print("NL");
                break;
            case 0x0D:
                Serial.print("CR");
                break;
            default:
                Serial.print(input);
                break;
            }
            //(input==0x0A)?Serial.print("ENTER"):Serial.print(input);
            Serial.print(" \t- DEC: ");
            Serial.print(input, DEC);
            Serial.print(" \t- BIN: ");
            Serial.print(input, BIN);
            Serial.print(" \t- HEX: ");
            switch (input)
            {
            case 0x0A:
                Serial.print("0A");
                break;
            case 0x0D:
                Serial.print("0D");
                break;
            default:
                Serial.print(input, HEX);
                break;
            }
            //(input==0x0A)?Serial.print("0x0A"):Serial.print(input,HEX);
            Serial.println();

            return false;
            //    } else {
            //      Serial.println("Serial.available()");
            //      while (Serial.available() == 0){
            //        delay(100);
            //      }
        }
    }
    // return false;
}

void printPortStatus()
{
    Serial.println(PORTB, BIN);
}

//////////////////////////////////
//// Print All bytes in stream until Carrier Return (new line).
//// Function prints only one line!
//////////////////////////////////
//// PRINT ASCII & HEX
void DEBUG_printOneLineFromHTTP(EthernetClient client)
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
//// PRINT HEX
void DEBUG_printOneLineFromHTTP_HEX(EthernetClient client)
{

    // Check HTTP status
    byte status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));
    // Serial.print(F("Serial response: "));
    for (int i = 0; i < sizeof(status); i++)
    {
        Serial.print(status[i], HEX);
        // Serial.print(status[i],BIN);
        Serial.print(" ");
    }
    Serial.println();
}

//// PRINT ASCII
void DEBUG_printOneLineFromHTTP_ASCII(EthernetClient client)
{

    // Check HTTP status
    char status[100] = {0};
    client.readBytesUntil('\r\n', status, sizeof(status));
    // Serial.print(F("Serial response: "));
    for (int i = 0; i < sizeof(status); i++)
    {
        Serial.print(status[i]);
        //    switch(status[i]){
        //        case 0x0A:
        //          Serial.print("0A");
        //          break;
        //        case 0x0D:
        //          Serial.print("0D");
        //          break;
        //        default:
        //          Serial.print(status[i]);
        //          break;
        //      }
    }
    Serial.println();
}