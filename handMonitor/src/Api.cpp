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

#include "Api.h"
#include "Storage.h"
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

   // Reset to factory defaults
   server->on("/reset", HTTP_POST, [&]() {
      DebugPrintln("POST /reset");
      Serial.println("Reset module before 10 seconds to cancel reset");
      delay(10000);
      sendText("Reset module before 10 seconds to cancel reset", 200);
      Serial.println("Resetting to factory ");
      config->initFromDefault();
      config->saveToEeprom();
      SPIFFS.begin();
      SPIFFS.format();      
      ESP.restart();
   });   
   
   // Process OTA
   server->on("/ota", HTTP_POST, [&]() {
      DebugPrintln("POST /ota");
      startOTA();
      char message[200];
      sprintf(message, MSG_WAITING_OTA, WiFi.softAPIP().toString().c_str());
      sendHtml(message, 200);
   });

   // list files in SPIFFS
   server->on("/files", HTTP_GET, [&]() {
      DebugPrintln("GET /files");
      char list[3000 + 1]; // TODO, this is temporary
      *list = 0;
      Storage::listFiles(list, 3000);
      DebugPrintf("List size : %d\n", strlen(list));
      //sendText(list, 200);
      sendText("ok", 200);

   });

   server->begin();
}

void Api::printHomePage() {
   sendHtml(initPage, 200);
}
void Api::close() {
   server->close();
   delete server;
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

   // Read and save new Home SSID
   String homeSsid = server->arg("homeSsid");
   if (homeSsid.length() > 0) {
      // TODO: add more checks
      config->setSsid(homeSsid.c_str());
      restart = true;
   }
   // Read and save new Home Pws
   String homePwd = server->arg("homePwd");
   if (homePwd.length() > 0) {
      // TODO: add more checks
      config->setPwd(homePwd.c_str());
      restart = true;
   }

   config->saveToEeprom();
   sendHtml(MSG_CONFIG_SAVED, 200);
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

void Api::sendHtml(const char* message, int code) {
   char html[] = "<html><head><meta name='viewport' content='width=device-width, initial-scale=1'><meta charset='utf-8'></head><body>%s</body></html>";
   char *htmlPage = (char *)malloc(strlen(html) + strlen(message) + 1);  // +1 is useless since %s is replaced ;)
   sprintf(htmlPage, html, message);
   server->sendHeader("Connection", "close");
   server->send(code, "text/html", htmlPage);
   free(htmlPage);
}

void Api::sendText(const char* message, int code) {
   server->sendHeader("Connection", "close");
   server->send(code, "text/plain", message);   
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

