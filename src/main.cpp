#include <Arduino.h>
#include <Ethernet.h>
#include <Debugger.hpp>
#include <RESTReader.h>

/// @brief
Debugger debugger;

void setup()
{
    debugger.startSerial();
}

void loop()
{
}
