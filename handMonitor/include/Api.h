 /* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Reivaxy & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : Handle web API
 * 
 * =============================================================================================================================================
 */

 #pragma once
 
 #include <ESP8266WebServer.h>
 #include "config.h"
 #include "mapping.h"

class Api {
public:
   Api(HandMonitorConfig* _config);

   HandMonitorConfig *config;
   ESP8266WebServer* server = NULL;

   void init();
   void loop();
   void printHomePage();
   void sendHtml(const char* html, int code);

};