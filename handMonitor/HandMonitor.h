#pragma once

#include "config.h"

class HandMonitor {
public:
  HandMonitor(HandMonitorConfig* config, int sda, int scl);
  void loop();
  void init();
  void processSettings(String) override;

  boolean openingWifiAP = false;
  boolean closingWifiAP = false;
  boolean openedWifiAP = false;
  
  boolean connectedToWifi = false;
  boolean connectingToWifi = false;
  boolean disconnectingFromWifi = false;
  
  boolean onCharge = false;
  
  // TODO: values
  int powerDetectionPin = 1;
  int presenceDetectionPin = 1;

  

};