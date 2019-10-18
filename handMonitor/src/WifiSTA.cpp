/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : Handle connection to home Wifi
 * 
 * =============================================================================================================================================
 */

#include "WifiSTA.h"
#include "WifiAP.h"
#include "RTClock.h"
#include "Led.h"

char WifiSTA::ipSta[20];

WifiSTA::WifiSTA(HandMonitorConfig* _config) {
   config = _config;
}

void WifiSTA::connect() {
   if (!config->isHomeWifiConfigured()) {
      return;
   }
   DebugPrintf("Connecting to %s\n", config->getSsid());
   WiFi.mode(WIFI_AP_STA);
   connectionAttemptTime = millis();
   wifiSTAGotIpHandler = WiFi.onStationModeGotIP([&](WiFiEventStationModeGotIP ipInfo) {
      connectionAttemptTime = 0;
      connected = true;
      strcpy(ipSta, ipInfo.ip.toString().c_str());
      DebugPrintf("Connected to %s on IP %s\n", config->getSsid(), ipSta);
      DebugPrintf("Connecting to NTP server %s\n", config->getNtpServer());
      NTP.begin(config->getNtpServer());
      NTP.setInterval(20, 7200);  // retry, refresh
      // TODO NTP.setTimeZone(config->getGmtHourOffset(), config->getGmtMinOffset());
   }); 
   wifiSTADisconnectedHandler = WiFi.onStationModeDisconnected([&](WiFiEventStationModeDisconnected event) {
      connected = false;
   
   });

   WiFi.begin(config->getSsid(), config->getPwd());
   NTP.onNTPSyncEvent([&] (NTPSyncEvent_t event) {
      DebugPrintf("NTP event: %d\n", event);
      if (event) {
         DebugPrint("NTP Time Sync error: ");
         if (event == noResponse)
            DebugPrintln("NTP server not reachable");
         else if (event == invalidAddress)
            DebugPrintln("Invalid NTP server address");
      } else {
         DebugPrint("Got NTP GMTtime: ");
         time_t moment = NTP.getLastNTPSync();
         DebugPrintln(NTP.getTimeDateString(moment).c_str());
         NTP.setInterval(7200, 7200);  // 2h retry, 2h refresh. once we have time, refresh failure is not critical
         clock = new RTClock();
         clock->setup(NTP.getTimeDateString(moment).c_str());
      }      
   });
}

void WifiSTA::disconnect() {

}

char* WifiSTA::getIp() {
   return WifiSTA::ipSta;
}

void WifiSTA::refresh() {
   now();  // Needed to refresh the Time lib, so that NTP server is called
   // If not connected to home wifi after X milliseconds, give up 
   if(connectionAttemptTime > 0 && (millis() - connectionAttemptTime > 45000)) {
      DebugPrintf("Aborting STA connection attemtp to: %s\n", config->getSsid());
      connectionAttemptTime = 0;
      WiFi.mode(WIFI_AP);
      // Need to reopen Access Point
      WifiAP *wifiAP = new WifiAP(config);
      wifiAP->open();     
   }
}