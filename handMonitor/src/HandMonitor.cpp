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
#include "Led.h"

#define FPM_SLEEP_MAX_TIME 0xFFFFFFF
#define LEVEL_CHECK_PERIOD_WHEN_ON_CHARGE 1000 // in milliseconds

HandMonitor::HandMonitor() {
}

// We want to keep code here as short as possible to keep consumption low
// when there is nothing to do
void HandMonitor::init() {
   Serial.begin(19200);  // Not too fast to not get too much garbage on wake up
   // Read config
   if (config == NULL) {
      config = new HandMonitorConfig("Hand Monitor");
      config->init();   
   }
   pinMode(PIN_POWER_DETECT, INPUT);
   pinMode(PIN_IR_EMITTER, OUTPUT);
   isOnCharge = digitalRead(PIN_POWER_DETECT);
   checkLevel(isOnCharge);
   if (isOnCharge) {
      handleOnChargeMode();
   } else {
      deepSleep();
   }
}

void HandMonitor::handleOnChargeMode() {
   DebugPrintln("Module is being charged");
   // Consider device removed.
   Storage::recordStateChange(1, 0);
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

// check sensor level and record changes
void HandMonitor::checkLevel(boolean ignoreChanges) {
   // Activate IR led
   digitalWrite(PIN_IR_EMITTER, HIGH);
   // Level is high when device is not on worn
   int level = analogRead(PIN_SENSOR);
   digitalWrite(PIN_IR_EMITTER, LOW);
   int threshold = config->getSensorThreshold();

   DebugPrintf("Sensor: %d, threshold: %d\n", level, threshold);

   // Read previous state from ESP RTC memory.
   // 0 means off (not worn), 1 means on
   uint32_t previousState;
   ESP.rtcUserMemoryRead(LAST_RTC_ADDR, (uint32_t*) &previousState, sizeof(uint32_t));
   // The first time, memory contains random bits. Let's say previous state was 0
   // Random value could be 0 or 1, so we could store some larger data to better detect init condition. 
   // I don't think it's worth it, the device will be tested before being used.
   if(previousState != 0 && previousState != 1) {
      previousState = 0;
   }

   // If device was worn, but is no longer, or the opposite: record time and state
   if (!ignoreChanges && ((previousState == 1 && level > threshold) || (previousState == 0 && level <= threshold))) {
      DebugPrintf("Changed: was %s\n", previousState==0?"off":"on");
      Storage::recordStateChange(previousState, level);
   }

}

void HandMonitor::deepSleep() {
   int sleepTime = config->getRefreshInterval();
   DebugPrintf("Sleep for %ds\n", sleepTime);
   WiFi.mode(WIFI_OFF);
   WiFi.forceSleepBegin(0);
   ESP.deepSleep( sleepTime * 1000000, WAKE_RF_DEFAULT );  // do not disable RF here, it's supposed to be handled by WiFi class 
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
      deepSleep();
   }
   if (isOnCharge && wasOnCharge) {
      wifiAP->refresh();
      wifiSTA->refresh();
   }
   
   time_t timeNow = millis();
   // Keep monitoriing level pin
   if ((timeNow - lastTimeLevelCheck > LEVEL_CHECK_PERIOD_WHEN_ON_CHARGE)) {
      checkLevel(isOnCharge);  // loop is only when on charge => true to not record level changes
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
   Led::refresh();
}

