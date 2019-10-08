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
#define FPM_SLEEP_MAX_TIME 0xFFFFFFF

HandMonitor::HandMonitor(HandMonitorConfig* conf, int sda, int scl) {
   config = conf;
}

void HandMonitor::init() {
   clock = new RTClock();
   clock->setup();
   char dateTime[50];
   int error = clock->getTime(dateTime);
   if (!error) {
      DebugPrintln(dateTime);
   } else {
      DebugPrintln("Invalid date time in RTC");
   }     
   
   isOnCharge = digitalRead(PIN_POWER_DETECT);
   int level = analogRead(PIN_SENSOR);
   DebugPrintf("Sensor level: %d\n", level);

   if (isOnCharge) {
      DebugPrintf("Module is being charged");
      // Need to wake wifi up
      WiFi.forceSleepWake();
   } else {
      deepSleep();
   }
}

void HandMonitor::deepSleep() {
   DebugPrintf("Going to sleep for %ds\n", sleepTime);
   WiFi.mode(WIFI_OFF);
   WiFi.forceSleepBegin(0);
   ESP.deepSleep( sleepTime * 1000000, WAKE_RF_DEFAULT );  // do not disable RF here, it's supposed to be handled by WiFi class 
}
void HandMonitor::loop() {
   isOnCharge = digitalRead(PIN_POWER_DETECT);
  
   // when module is bein charged, open the wifi access point
   if (isOnCharge && !wasOnCharge) {
      wasOnCharge = true;
      // wifi_fpm_do_wakeup();
	   // wifi_fpm_close();
      // wifi_set_opmode(SOFTAP_MODE);
      // wifi_station_connect();
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
      // wifi_set_sleep_type(MODEM_SLEEP_T);
      // wifi_fpm_open();
      // wifi_fpm_do_sleep(FPM_SLEEP_MAX_TIME);      
      Utils::checkHeap("onCharge off");
      deepSleep();
   }
   if (isOnCharge && wasOnCharge) {
      wifiAP->refresh();
      wifiSTA->refresh();
   }

   if (clock != NULL) {
      char dateTime[50];
      time_t timeNow = millis();
      if ((timeNow - lastTimeDisplay > 10000)) {
         lastTimeDisplay = timeNow;
         int error = clock->getTime(dateTime);
         if (!error) {
            DebugPrintln(dateTime);
         } else {
            DebugPrintln("Invalid date time in RTC");
         }           
         DebugPrintln(dateTime);
      }      
      
   }
}

