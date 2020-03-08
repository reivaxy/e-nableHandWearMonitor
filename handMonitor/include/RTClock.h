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
#include "Utils.h"
#include "EspRtcMem.h"

#include <Wire.h> // default is SDA = 4, SCL = 5, not what we use

#include <RtcDS3231.h>


class RTClock {
public:
   void setup();
   void setup(const char* timeStr);
   void manualSetup(const char* _dateTime);
   int get(RtcDateTime* dt);
   int getTime(char *timeStr);
   int getFileName(char *fileName);
   int getRecordDate(char *recordDate);
   void printDateTime(const RtcDateTime& dt);
   int toMin(int hours, int min);
   boolean isPaused();

   RtcDS3231<TwoWire> *clock;
};