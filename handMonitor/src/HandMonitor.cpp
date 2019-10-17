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
#include "Storage.h"

#define FPM_SLEEP_MAX_TIME 0xFFFFFFF
#define LEVEL_CHECK_PERDIO_WHEN_ON_CHARGE 10000 // in milliseconds

HandMonitor::HandMonitor(int sda, int scl) {
}

// We want to keep code here as short as possible to keep consumption low
// when there is nothing to do
void HandMonitor::init() { 
   pinMode(PIN_POWER_DETECT, INPUT);
   isOnCharge = digitalRead(PIN_POWER_DETECT);
   checkLevel();
   if (isOnCharge) {
      handleOnChargeMode();
   } else {
      deepSleep();
   }
}

void HandMonitor::handleOnChargeMode() {
   DebugPrintln("Module is being charged");
   // Get wifi info from eeprom-saved config
   if (config == NULL) {
      config = new HandMonitorConfig("Hand Monitor");
      config->init();   
   }
   clock = new RTClock();
   clock->setup();
   char dateTime[50];
   int error = clock->getTime(dateTime);
   if (!error) {
      DebugPrintln(dateTime);
   } else {
      DebugPrintln("Invalid date time in RTC");
   }        
   // Need to wake wifi up
   WiFi.forceSleepWake();

  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  FSInfo fs_info;
  SPIFFS.info(fs_info);
  Serial.printf("totalBytes : %d\n", fs_info.totalBytes);
  Serial.printf("usedBytes : %d\n", fs_info.usedBytes);
  Serial.printf("blockSize : %d\n", fs_info.blockSize);
  Serial.printf("pageSize : %d\n", fs_info.pageSize);
  Serial.printf("maxOpenFiles : %d\n", fs_info.maxOpenFiles);
  Serial.printf("maxPathLength : %d\n", fs_info.maxPathLength);
}

void HandMonitor::checkLevel() {
   // Level is high when device is not on worn
   int level = analogRead(PIN_SENSOR);
   DebugPrintf("Sensor level: %d\n", level);

   // Read previous state from ESP RTC memory.
   // 0 means off (not worn), 1 means on
   uint32_t previousState;
   ESP.rtcUserMemoryRead(LAST_RTC_ADDR, (uint32_t*) &previousState, sizeof(uint32_t));
   // The first time, memory contains random bits. Let's say previous state was 0
   // Random value could be 0 or 1, so we could store some larger data to better detect init condition. 
   // Not worth it, to me.
   if(previousState != 0 && previousState != 1) {
      previousState = 0;
   }

   // If device was worn, but is no longer, or the opposite: record time and state
   // Endurance test: simulate one change for each check => write in file each time
   if((previousState == 1 && level > 512) || (previousState == 0 && level <= 512)) {
      DebugPrintf("State changed, was %d\n", previousState);
      Storage::recordStateChange(previousState);
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
   
   time_t timeNow = millis();
   // Keep monitoriing level pin
   if ((timeNow - lastTimeLevelCheck > LEVEL_CHECK_PERDIO_WHEN_ON_CHARGE)) {
      checkLevel();
      lastTimeLevelCheck = timeNow; 
   }

   if (clock != NULL) {
      char dateTime[50];
      if ((timeNow - lastTimeDisplay > 60000)) {
         lastTimeDisplay = timeNow;
         int error = clock->getTime(dateTime);
         if (!error) {
            DebugPrintln(dateTime);
         } else {
            DebugPrintln("Invalid date time in RTC");
         }           
      }      
      
   }
}

