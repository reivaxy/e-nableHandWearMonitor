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

#include "HandMonitor.h"

HandMonitor::HandMonitor(HandMonitorConfig* config, int sda, int scl) {
  wifiAP = new WifiAP(config);
}

void HandMonitor::init() {
}

void HandMonitor::loop() {
  onCharge = digitalRead(PIN_POWER_DETECT);
  
  // when module is bein charged, open the wifi access point
  if(onCharge) {
    wifiAP->expose();
  } else {
    wifiAP->close();
  }

}

void HandMonitor::processSettings() {
}
