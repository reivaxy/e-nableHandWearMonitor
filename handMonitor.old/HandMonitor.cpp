#include "HandMonitor.h"

HandMonitor::HandMonitor(HandMonitorConfig* config, int sda, int scl) {
}

void HandMonitor::init() {
}

void HandMonitor::loop() {
  onCharge = digitalRead(PIN_POWER_DETECT);
  
  if(onCharge && !openingWifiAP && !openedWifiAP) {
  }

}

void HandMonitor::processSettings() {
}
