/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : Handle Wifi Access Point
 * 
 * =============================================================================================================================================
 */

#include "WifiAP.h"

WifiAP::WifiAP(HandMonitorConfig* _config) {
   config = _config;
}

void WifiAP::open() {
   WiFi.mode(WIFI_AP);
   DebugPrintf("Opening wifi AP %s\n", config->getAPSsid());
   WiFi.softAP(config->getAPSsid(), config->getAPPwd());
   DebugPrintf("IP: %s\n", WiFi.softAPIP().toString().c_str());
   api = new Api(config);
   api->init();
   opened = true; 
}

void WifiAP::close() {
   if (!opened) {
      return;
   }  
   DebugPrintln("Closing Wifi AP");
   api->close();
   WiFi.mode(WIFI_OFF);
   opened = false;
   delete api;
   api = NULL;
}

void WifiAP::refresh() {
   if (opened) {
      if (api != NULL) {
         api->refresh();
      }   
   }
}