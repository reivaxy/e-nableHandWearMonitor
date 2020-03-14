/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : Hand monitor
 * 
 * =============================================================================================================================================
 */
#pragma once

#include <ESP8266WiFi.h>

#include "config.h"
#include "mapping.h"
#include "debug.h"
#include "WifiAP.h"
#include "WifiSTA.h"
#include "Utils.h"
#include "Storage.h"
#include "EspRtcMem.h"


class HandMonitor {
public:
  HandMonitor();
  void loop();
  void init();
  void handleOnChargeMode();
  void checkLevel(boolean ignoreChanges);
  void deepSleep();
  
  boolean isOnCharge = false;
  boolean wasOnCharge = false;
  WifiAP *wifiAP = NULL;
  WifiSTA *wifiSTA = NULL;
  HandMonitorConfig* config = NULL;
  RTClock *clock = NULL;
  rtcStoredData *rtcData = NULL;
  time_t lastTimeDisplay = 0;
  time_t lastTimeLevelCheck = 0;

};