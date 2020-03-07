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
#include "EspRtcMem.h"
#include "Led.h"

#define FPM_SLEEP_MAX_TIME 0xFFFFFFF
#define LEVEL_CHECK_PERIOD_WHEN_ON_CHARGE 2000 // in milliseconds

HandMonitor::HandMonitor() {
}

// We want to keep code here as short as possible to keep consumption low
// when there is nothing to do
void HandMonitor::init() {
   Serial.begin(19200);  // Not too fast to not get too much garbage on wake up
   // Read ESP rct memory for threshold, wake up period and previous state
   rtcStoredData *rtcData = EspRtcMem::getRtcData();

   pinMode(PIN_POWER_DETECT, INPUT);
   pinMode(PIN_IR_EMITTER, OUTPUT);
   isOnCharge = digitalRead(PIN_POWER_DETECT);
   checkLevel(isOnCharge, rtcData);
   if (isOnCharge) {
      handleOnChargeMode();
   } else {
      deepSleep(rtcData);
   }
}

void HandMonitor::handleOnChargeMode() {
  DebugPrintln("Module is being charged");
   if (config == NULL) {
      config = new HandMonitorConfig("Hand Monitor");
      config->init();   
   }
  // Consider device removed.
  Storage::recordStateChange(0, 0);
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
  wasOnCharge = true;
  wifiAP = new WifiAP(config);
  wifiAP->open();
  wifiSTA = new WifiSTA(config);
  wifiSTA->connect();
  Utils::checkHeap("onCharge on");
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
void HandMonitor::checkLevel(boolean isOnCharge, rtcStoredData* rtcData) {
   // Activate IR led
   digitalWrite(PIN_IR_EMITTER, HIGH);
   // Level is high when device is not on worn
   int level = analogRead(PIN_SENSOR);
   digitalWrite(PIN_IR_EMITTER, LOW);
   uint16_t threshold = rtcData->threshold;    // sensor threshold between worn and not worn
   uint8_t previousState = rtcData->previous;  // previous state. 0: not worn, 1: worn
   uint8_t counter = rtcData->counter;         // to register a change after 3 consecutive measures.

   DebugPrintf("Sensor: %d, threshold: %d, previousState: %d, counter: %d\n", level, threshold, previousState, counter);

   // The first time, rtc memory contains random bits. If values are unexpected set them to 0
   if(previousState > 1) {
      previousState = 0;
   }
   if(counter > 3) {
      counter = 0;
   }

   // If device was worn, but is no longer, or the opposite: record time and state (except if on charge)
   // Level > Threshold means device is not worn.
   if (((previousState == 1 && level > threshold) || (previousState == 0 && level <= threshold))) {
      DebugPrintf("Changed: was %s\n", previousState==0?"'off'":"'on'");
      // If this is the third consecutive measure for this level ?
      if (counter == 2) {
         DebugPrintln("Third change in a row");
         uint32_t newState = (previousState == 0?1:0);
         rtcData->previous = newState;
         // Record only when not in charge.
         if (!isOnCharge) {
            DebugPrintln("Not on charge, recording change");
            Storage::recordStateChange(newState, level);
         } else {
            DebugPrintln("On charge, not recording change");
         }
         counter = 0;
      } else {
         counter ++;
      }
      rtcData->counter = counter;
      EspRtcMem::saveRtcData(rtcData);
   } else {
      // Current level is same as previous level
      // If counter was not 0, reset it to 0
      if (counter != 0) {
         rtcData->counter = 0;
         EspRtcMem::saveRtcData(rtcData);
      }
   }

   if (isOnCharge && wifiAP != NULL) {
      Api* api = wifiAP->getApi();
      if (api != NULL) {
         api->setLevel(level);
      }
   }
}

void HandMonitor::deepSleep(rtcStoredData* rtcData) {
   uint8_t sleepTime = rtcData->period;
   DebugPrintf("Sleep for %ds\n", sleepTime);
   WiFi.mode(WIFI_OFF);
   WiFi.forceSleepBegin(0);
   ESP.deepSleep( sleepTime * 1000000, WAKE_RF_DEFAULT );  // do not disable RF here, it's supposed to be handled by WiFi class 
}

void HandMonitor::loop() {
   isOnCharge = digitalRead(PIN_POWER_DETECT);
   rtcStoredData *rtcData = EspRtcMem::getRtcData();
   // when module is no longer being charged, close the wifi access point
   if (wasOnCharge && !isOnCharge) {
      wasOnCharge = false;
      wifiAP->close();
      delete(wifiAP);
      wifiAP = NULL;
      wifiSTA->disconnect();
      delete(wifiSTA);  
      wifiSTA = NULL;  
      Utils::checkHeap("onCharge off");
      deepSleep(rtcData);
   }
   if (isOnCharge && wasOnCharge) {
      wifiAP->refresh();
      wifiSTA->refresh();
   }
   
   time_t timeNow = millis();
   // Keep monitoriing level pin
   if ((timeNow - lastTimeLevelCheck > LEVEL_CHECK_PERIOD_WHEN_ON_CHARGE)) {
      checkLevel(isOnCharge, rtcData);  // loop is only when on charge => true to not record level changes
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

