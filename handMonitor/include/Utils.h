
#pragma once

#include <ESP8266WiFi.h>
#include "debug.h"

class Utils {
public:
   static void checkHeap(const char* msg);
   static int toInt(const char* str, int length);
};