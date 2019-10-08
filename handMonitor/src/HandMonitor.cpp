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

#include "HandMonitor.h"

HandMonitor::HandMonitor(HandMonitorConfig* conf, int sda, int scl) {
   config = conf;
}

void HandMonitor::init() {
   clock = new RTClock();
   clock->setup();
}

void HandMonitor::loop() {
   isOnCharge = digitalRead(PIN_POWER_DETECT);
  
   // when module is bein charged, open the wifi access point
   if (isOnCharge && !wasOnCharge) {
      wasOnCharge = true;
      wifiAP = new WifiAP(config);
      wifiAP->open();
      wifiSTA = new WifiSTA(config);
      wifiSTA->connect();
      Utils::checkHeap("onCharge on");
   }
   if (wasOnCharge && !isOnCharge) {
      wasOnCharge = false;
      wifiAP->close();
      delete(wifiAP);
      wifiSTA->disconnect();
      delete(wifiSTA);
      Utils::checkHeap("onCharge off");
   }
   if (isOnCharge && wasOnCharge) {
      wifiAP->refresh();
      wifiSTA->refresh();
   }

   if (clock != NULL) {
      char dateTime[50];

      int error = clock->getTime(dateTime);
      time_t timeNow = millis();

      if (!error && (timeNow - lastTimeDisplay > 10000)) {
         lastTimeDisplay = timeNow;
         DebugPrintln(dateTime);
      }      
      
   }
}

