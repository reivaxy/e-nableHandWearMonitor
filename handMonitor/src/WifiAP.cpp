/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Reivaxy & Thomas Broussard
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

void WifiAP::expose() {
   if (openedWifiAP) {
      return;
   }
   WiFi.mode(WIFI_AP);
   DebugPrintf("Opening wifi AP %s\n", config->getAPSsid());
   WiFi.softAP(config->getAPSsid(), config->getAPPwd());
   DebugPrintf("IP: %s\n", WiFi.softAPIP().toString().c_str());
   openedWifiAP = true;
   api = new Api(config);
   api->init();
}

void WifiAP::close() {
   return; // for now !

   if (!openedWifiAP) {
      return;
   }

   
   DebugPrintln("Closing Wifi AP");
   WiFi.mode(WIFI_OFF);
   openedWifiAP = false;
}

