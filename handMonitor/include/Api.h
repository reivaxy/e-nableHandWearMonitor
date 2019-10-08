 /* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : Handle web API
 * 
 * =============================================================================================================================================
 */

 #pragma once
 
 #include <ESP8266WebServer.h>
 #include "config.h"
 #include "Ota.h"
 #include "debug.h"

class Api {
public:
   Api(HandMonitorConfig* _config);

   HandMonitorConfig *config;
   ESP8266WebServer* server = NULL;
   Ota* ota = NULL;

   void init();
   void close();
   void refresh();
   void printHomePage();
   void initSave();
   void startOTA();
   void sendHtml(const char* html, int code);

};