#pragma once

#include "config.h"
#include "mapping.h"


class HandMonitor {
public:
  HandMonitor(HandMonitorConfig* config, int sda, int scl);
  void loop();
  void init();
  void processSettings();

  boolean openingWifiAP = false;
  boolean closingWifiAP = false;
  boolean openedWifiAP = false;
  
  boolean connectedToWifi = false;
  boolean connectingToWifi = false;
  boolean disconnectingFromWifi = false;
  
  boolean onCharge = false;
  

};