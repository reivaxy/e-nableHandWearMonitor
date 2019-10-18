/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : Handle web API
 * 
 * =============================================================================================================================================
 */

#pragma once
#include "Arduino.h"

class Led {
public:
   static void blink(time_t period);
   static void on();
   static void off();
   static void refresh();
   static void stop();
   static void toggle();

   static int pin;
   static time_t lastLedBlink;
   static time_t timeOn;
   static time_t period; // ms
   static boolean mustBlink; // ms
};
