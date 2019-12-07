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
#include "WifiSTA.h"
#include "RTClock.h"
#include "initPage.h"
#include "adminPage.h"

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
   // Display "admin" " page
   server->on("/admin", HTTP_GET, [&]() {
      DebugPrintln("GET /admin");
      printAdminPage();
   });

   // some dynamic data for home page
   server->on("/info", HTTP_GET, [&]() {
      DebugPrintln("GET /info");
      RTClock *clock = new RTClock();
      clock->setup();
      char dateTime[50];
      int error = clock->getTime(dateTime);
      if(error != 0) {
         strcpy(dateTime, "Not initialized");
      }
      char messageIPSTA[100];
      *messageIPSTA = 0;
      if (config->getSsid() != 0) {
         sprintf(messageIPSTA, "IP %s: %s\\n", config->getSsid(), WiFi.localIP().toString().c_str());
      }
      char message[200];
      sprintf(message, "document.write(\"Date: %s\\nIP %s: %s\\n%s\");", dateTime, config->getAPSsid(), 
                                                      WiFi.softAPIP().toString().c_str(), messageIPSTA);
      sendJs(message, 200);
   });

   // some dynamic data for admin page
   server->on("/adminInfo", HTTP_GET, [&]() {
      DebugPrintln("GET /adminInfo");
      char spiffs[200];

      if (SPIFFS.begin()) {
         FSInfo fs_info;
         SPIFFS.info(fs_info);
         sprintf(spiffs, "document.write(\"totalBytes: %d\\nusedBytes: %d\\nblockSize: %d\\npageSize: %d\\nmaxOpenFiles: %d\\nmaxPathLength: %d\\nRefresh interval: %ds\\nSensor threshold: %d\");",
                fs_info.totalBytes, fs_info.usedBytes, fs_info.blockSize, fs_info.pageSize, fs_info.maxOpenFiles, fs_info.maxPathLength,
                      config->getRefreshInterval(), config->getSensorThreshold());   
      } else {
         Serial.println("An Error has occurred while mounting SPIFFS");
      }
      sendJs(spiffs, 200);
   });

   // Save config parameters
   server->on("/initSave", HTTP_POST, [&]() {
      DebugPrintln("POST /initSave");
      initSave();
   });   

   // Reset to factory defaults
   server->on("/reset", HTTP_POST, [&]() {
      DebugPrintln("POST /reset");
      securityDelayWarning("full reset");
      Serial.println("Resetting to factory ");
      config->initFromDefault();
      config->saveToEeprom();
      SPIFFS.begin();
      SPIFFS.format();      
      ESP.restart();
   });
    // erase SPIFFS only
   server->on("/erase", HTTP_POST, [&]() {
      DebugPrintln("POST /erase");
      securityDelayWarning("SPIFFS erase");
      Serial.println("Erasing SPIFFS ");
      SPIFFS.begin();
      SPIFFS.format();      
   });

   // Create fake data
   server->on("/createFakeData", HTTP_POST, [&]() {
      DebugPrintln("POST /createFakeData");
      securityDelayWarning("fake data creation");
      Storage::createFakeData();
   });   
   
   // Process OTA
   server->on("/ota", HTTP_POST, [&]() {
      DebugPrintln("POST /ota");
      char message[200];
      sprintf(message, MSG_WAITING_OTA, WiFi.softAPIP().toString().c_str());
      sendHtml(message, 200);
      startOTA();
   });

   // list files in SPIFFS
   server->on("/listFiles", HTTP_GET, [&]() {
      DebugPrintln("GET /files");
      listFiles();
   });

   // Read one file in SPIFFS
   server->on("/readFile", HTTP_GET, [&]() {
      DebugPrintln("GET /files");
      readFile(server->arg("file").c_str());
   });

   server->begin();
}

void Api::securityDelayWarning(const char *msg) {
   char message[100];
   sprintf(message, "Reset module before 10 seconds to cancel %s", msg);
   sendHtml(message, 200);
   delay(10000);
}

void Api::readFile(const char* fileName) {
   DebugPrintf("Reading %s\n", fileName);
   Utils::checkHeap("Before reading file");
   File file;
   Storage::getFile(fileName, &file);
   server->streamFile(file, "text/plain");
   file.close();
   Utils::checkHeap("After reading file");
}

void Api::listFiles() {
   Utils::checkHeap("Before file listing");
   FileList fileList;
   Storage::listFiles(&fileList);
   size_t bufferSize = 1000 ;
   char* page = (char *)malloc(bufferSize);
   *page = 0;
   for (FileList::iterator it = fileList.begin(); it != fileList.end(); it++) {
      char fileLine[100];
      sprintf(fileLine, "<a href='/readFile?file=%s'>%s</a><br/>", *it, *it); // TODO: filename should be urlencoded...
      if(strlen(page) + strlen(fileLine) > bufferSize + 1) {
         DebugPrintln("Reallocating file list buffer");
         bufferSize += 1000;
         page = (char *) realloc(page, bufferSize);
      }
      strlcat(page, fileLine, bufferSize);
      free(*it);
   }
   sendHtml("File list", page, 200);
   free(page);
   Utils::checkHeap("After file listing");
}

void Api::printHomePage() {
   sendHtml(initPage, 200);
}

void Api::printAdminPage() {
   sendHtml(adminPage, 200);
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

   String refreshInterval = server->arg("refreshInterval");
   if (refreshInterval.length() > 0) {
      config->setRefreshInterval(refreshInterval.toInt());
      restart = true;
   }

   String sensorThreshold = server->arg("sensorThreshold");
   if (sensorThreshold.length() > 0) {
      config->setSensorThreshold(sensorThreshold.toInt());
      restart = true;
   }

   String timeOffset = server->arg("timeOffset");
   if (timeOffset.length() > 0) {
      config->setTimeOffset(timeOffset.toInt());
      restart = true;
      NTP.getTime();
   }

   config->saveToEeprom();
   sendHtml(MSG_CONFIG_SAVED, 200);
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
   sendHtml("", message, code);
}

void Api::sendHtml(const char* title, const char* message, int code) {
   char html[] = "<html><head><title>%s</title><meta name='viewport' content='width=device-width, initial-scale=1'><meta charset='utf-8'></head><body>%s</body></html>";
   char *htmlPage = (char *)malloc(strlen(html) + strlen(title) + strlen(message) + 1);  // +1 is actually useless since %s is replaced ;)
   sprintf(htmlPage, html, title, message);
   server->sendHeader("Connection", "close");
   server->send(code, "text/html", htmlPage);
   free(htmlPage);
}

void Api::sendJs(const char* message, int code) {
   server->sendHeader("Connection", "close");
   server->send(code, "text/javascript", message);   
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

