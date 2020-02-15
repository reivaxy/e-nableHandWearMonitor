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
 #include "Utils.h"

class Api {
public:
   Api(HandMonitorConfig* _config);

   HandMonitorConfig *config;
   ESP8266WebServer* server = NULL;
   Ota* ota = NULL;
   int level = 0;

   void init();
   void close();
   void refresh();
   void printAdminPage();
   void printHomePage();
   void printInitPage();
   void securityDelayWarning(const char* msg);
   void initSave();
   void startOTA();
   void listFiles();
   void readFile(const char *fileName);
   void sendHtml(const char* html, int code);
   void sendHtml(const char* title, const char* message, int code);
   void sendText(const char* html, int code);
   void sendPage(const char* page);
   void sendJs(const char* message, int code);
   void setLevel(int level);
};