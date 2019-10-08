/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : Handle Wifi Access Point
 * 
 * =============================================================================================================================================
 */

#pragma once

#include <ESP8266WiFi.h>
#include "config.h"
#include "debug.h"
#include "Api.h"

class WifiAP {
public:
  WifiAP(HandMonitorConfig* config);

  void open();
  void close();
  void refresh();

  HandMonitorConfig* config;
  Api *api = NULL;

  boolean opened = false;
};