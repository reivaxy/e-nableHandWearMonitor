#pragma once

#include <ArduinoOTA.h>

class Ota {
public:
   void start();
   void refresh();
   bool isWaitingOTA();

   bool otaIsStarted = false;
   time_t otaReadyTime = 0;
   time_t lastLedBlink = 0;
   time_t ledStatus = 0;
};