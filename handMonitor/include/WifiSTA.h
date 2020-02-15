/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : Handle connection to home Wifi
 * 
 * =============================================================================================================================================
 */
#pragma once

#include <ESP8266WiFi.h>
#include <NtpClientLib.h>
#include <TimeLib.h>

#include "debug.h"
#include "RTClock.h"
#include "config.h"

class WifiSTA {
public:
  WifiSTA(HandMonitorConfig* config);

  void connect();
  void disconnect();
  void refresh();

  HandMonitorConfig* config;

  boolean connected = false;
  boolean connecting = false;
  boolean disconnecting = false;
  time_t connectionAttemptTime = 0;
  WiFiEventHandler wifiSTAGotIpHandler, wifiSTADisconnectedHandler;
  RTClock *clock = NULL;
};


