#ifndef FUNC_H
#define FUNC_H

#include <Arduino.h>
#include <Ethernet.h>
#include <ArduinoJson.h>

void startSerial();
void startEthernet(char server[]);
bool checkHTTPstatus();
bool SkipHTTPheaders();
void json();

extern EthernetClient client;
extern int abc;

#endif