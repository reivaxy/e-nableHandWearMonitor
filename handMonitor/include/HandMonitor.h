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


class HandMonitor {
public:
  HandMonitor(HandMonitorConfig* config, int sda, int scl);
  void loop();
  void init();
  void debugMem(const char* msg);

  boolean isOnCharge = false;
  boolean wasOnCharge = false;
  WifiAP *wifiAP;
  WifiSTA *wifiSTA;
  HandMonitorConfig* config = NULL;
  RTClock *clock = NULL;
  time_t lastTimeDisplay = 0;
};