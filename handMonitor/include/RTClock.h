/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : Rtc handling
 * 
 * =============================================================================================================================================
 */

#pragma once

#include "mapping.h"
#include "debug.h"

#include <Wire.h> // Using default SDA = 4, SCL = 5

#include <RtcDS1307.h>


class RTClock {
public:
   void setup();
   void setup(const char* timeStr);
   int getTime(char *timeStr);
   void printDateTime(const RtcDateTime& dt);
   int toInt(const char*, int length);

   RtcDS1307<TwoWire> *clock;
};