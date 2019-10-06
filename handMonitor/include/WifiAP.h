/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Reivaxy & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : Handle Wifi Access Point
 * 
 * =============================================================================================================================================
 */

#pragma once

#include <ESP8266WiFi.h>
#include "config.h"
#include "mapping.h"
#include "Api.h"

class WifiAP {
public:
  WifiAP(HandMonitorConfig* config);
  
  void expose();
  void close();

  HandMonitorConfig* config;
  Api *api = NULL;

  boolean openedWifiAP = false;
  
  boolean connectedToWifi = false;
  boolean connectingToWifi = false;
  boolean disconnectingFromWifi = false;
};