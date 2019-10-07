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

#include "Api.h"
#include "initPage.h"

Api::Api(HandMonitorConfig *_config) {
   config = _config;
   server = new ESP8266WebServer(80);
}

void Api::init() {

   // Display config page
   server->on("/", HTTP_GET, [&]() {
      DebugPrintln("GET /");
      printHomePage();
   });

   // Save config parameters
   server->on("/initSave", HTTP_POST, [&]() {
      DebugPrintln("POST /initSave");
      initSave();
   });   
   
   // Process OTA
   server->on("/ota", HTTP_POST, [&]() {
      DebugPrintln("POST /ota");
      startOTA();
   });


   server->begin();
}

void Api::printHomePage() {
   sendHtml(initPage, 200);
}

void Api::initSave() {
   boolean restart = false;
   // Read and save new AP SSID
   String apSsid = server->arg("apSsid");
   if (apSsid.length() > 0) {
      // TODO: add more checks
      config->setAPSsid(apSsid.c_str());
      restart = true;
   }
   // Read and save new AP Pws
   String apPwd = server->arg("apPwd");
   if (apPwd.length() > 0) {
      // TODO: add more checks
      config->setAPPwd(apPwd.c_str());
      restart = true;
   }

   // Read and save new AP SSID
   String homeSsid = server->arg("homeSsid");
   if (homeSsid.length() > 0) {
      // TODO: add more checks
      config->setAPSsid(homeSsid.c_str());
      restart = true;
   }
   // Read and save new AP Pws
   String homePwd = server->arg("homePwd");
   if (homePwd.length() > 0) {
      // TODO: add more checks
      config->setAPPwd(homePwd.c_str());
      restart = true;
   }

   config->saveToEeprom();
   // TODO: really restart ? or just reinit wifi network(s) ? (but there will be more than wifi config soon)
   if (restart) {
       ESP.restart(); 
   }  
}

void Api::startOTA() {
   if (ota == NULL) {
      ota = new Ota();
      ota->start();
   } else {
      DebugPrintln("OTA already initialized");
   }
}

void Api::sendHtml(const char* html, int code) {
  server->sendHeader("Connection", "close");
  server->send(code, "text/html", html);
}

void Api::refresh() {
   if (server != NULL) {
      server->handleClient();
   }

   if (ota != NULL) {
      // When ota is done, it resets the module, no need to check for the end.
      ota->refresh();
   }
}

