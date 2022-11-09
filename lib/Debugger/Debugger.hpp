#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <Arduino.h>

class Debugger
{
private:
public:
    /**
     * @brief Construct a new Profibus object
     *
     */
    Debugger()
    {
       // this->startSerial();
    }

    void startSerial()
    {
        // Open serial communications and wait for port to open:
        Serial.begin(9600);
        while (!Serial)
        {
            ; // wait for serial port to connect. Needed for native USB port only
        }
        Serial.println("\nSerial communication startet!\n\n");
    }
};

#endif