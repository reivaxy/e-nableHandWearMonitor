/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Reivaxy & Thomas Broussard
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
#include "WifiAP.h"


class HandMonitor {
public:
  HandMonitor(HandMonitorConfig* config, int sda, int scl);
  void loop();
  void init();
  void processSettings();
  
  boolean onCharge = false;
  WifiAP *wifiAP;
  

};