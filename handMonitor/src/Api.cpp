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
#include "EspRtcMem.h"
#include "WifiSTA.h"
#include "RTClock.h"
#include "homePage.h"
#include "initPage.h"
#include "adminPage.h"
#include <ArduinoJson.h>
#include "jsonTags.h"

Api::Api(HandMonitorConfig *_config) {
   config = _config;
   server = new ESP8266WebServer(80);
}

void Api::init() {

   // Display config page
   server->on("/", HTTP_GET, [&]() {
      DebugPrintln("GET /");
      if (config->getInitDone()) {
         printHomePage();
      } else {
         printInitPage();
      }
   });


   // Display "admin" " page
   server->on("/admin", HTTP_GET, [&]() {
      DebugPrintln("GET /admin");
      printAdminPage();
   });

   // some json data 
   server->on("/data", HTTP_GET, [&]() {
      DebugPrintln("GET /data");
      char jsonData[500];
      String src = server->arg("src"); 
      strcpy(jsonData, "var jsonData=");
      StaticJsonBuffer<JSON_OBJECT_SIZE(15) + 100> jsonBuffer;    
      // Create the root object
      JsonObject& root = jsonBuffer.createObject();
      JsonObject& data = jsonBuffer.createObject();
      root[JSON_TAG_DATA] = data;
      data[JSON_TAG_NAME] = config->getName();
      char dateTime[50];
      RTClock *clock = new RTClock();
      clock->setup();      
      int error = clock->getTime(dateTime);
      if(error == 0) {         
         data[JSON_TAG_DATE] = dateTime;
      }
      // Is it configured to connect to home wifi ?
      if (strlen(config->getSsid()) != 0) {
         data[JSON_TAG_SSID] = config->getSsid();
         data[JSON_TAG_SSID_IP] = WiFi.localIP().toString();
         root[JSON_TAG_CSS_CLASS] = "hideManualTime"; // hide manual time setting
      } else {
         root[JSON_TAG_CSS_CLASS] = "hideTimeOffset"; // hide time offset setting
      }
      data[JSON_TAG_APSSID] = config->getAPSsid();
      data[JSON_TAG_APSSID_IP] = WiFi.softAPIP().toString();
      data[JSON_TAG_LEVEL] = level;
      data[JSON_TAG_WORN] = (level < config->getSensorThreshold())? MSG_YES : MSG_NO ;



      if (src.equals("init") || src.equals("admin")) {
         data[JSON_TAG_THRESHOLD] = config->getSensorThreshold();
         data[JSON_TAG_REFRESH] = config->getRefreshInterval();
      }
      
      if (src.equals("admin")) {
         if (SPIFFS.begin()) {
            FSInfo fs_info;
            SPIFFS.info(fs_info);         
            data[JSON_TAG_MEMORY_SIZE] = fs_info.totalBytes;
            data[JSON_TAG_MEMORY_USED] =  fs_info.usedBytes;
            data[JSON_TAG_MEMORY_MAX_FILES] = fs_info.maxOpenFiles;
         }
      }

      root.printTo(jsonData+strlen(jsonData), 500);
      strcat(jsonData, ";");
      sendJs(jsonData, 200);
   });

   // Display config page
   server->on("/init", HTTP_GET, [&]() {
      DebugPrintln("GET /init");
      printInitPage();
   }); 

   // Save config parameters
   server->on("/init", HTTP_POST, [&]() {
      DebugPrintln("POST /init");
      initSave();
   });   

   // Reset config only
   server->on("/resetConfig", HTTP_POST, [&]() {
      DebugPrintln("POST /resetConfig");
      securityDelayWarning("Config factory reset");
      Serial.println("Resetting config ");
      config->initFromDefault();
      config->saveToEeprom();
      ESP.restart();
   });

   // Reset to factory defaults
   server->on("/reset", HTTP_POST, [&]() {
      DebugPrintln("POST /reset");
      securityDelayWarning("factory reset");
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
      securityDelayWarning("SPIFFS data erase");
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
   sprintf(message, "Reset or unplug module before 10 seconds to cancel %s", msg);
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
   sendPage(homePage);
}

void Api::printInitPage() {
   sendPage(initPage);
}

void Api::printAdminPage() {
   sendPage(adminPage);
}

void Api::close() {
   server->close();
   delete server;
}

void Api::initSave() {
   rtcStoredData *rtcData = EspRtcMem::getRtcData();
   boolean restart = false;
   boolean saveRtcData = false;
   // Read and save new name
   String name = server->arg("name");
   if (name.length() > 0) {
      // TODO: add more checks
      config->setName(name.c_str());
      restart = true;
   }
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
   } else {
      // Emptying ssid in the form is the way to remove this setting
      memset(config->getSsid(), 0, SSID_MAX_LENGTH);
      memset(config->getPwd(), 0, PWD_MAX_LENGTH);
      config->setSsid("");
      config->setPwd("");
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
      saveRtcData = true;
      rtcData->period = refreshInterval.toInt();
   }

   String sensorThreshold = server->arg("sensorThreshold");
   if (sensorThreshold.length() > 0) {
      config->setSensorThreshold(sensorThreshold.toInt());
      restart = true;
      saveRtcData = true;
      rtcData->threshold = sensorThreshold.toInt();
   }

   String timeOffset = server->arg("timeOffset");
   if (timeOffset.length() > 0) {
      config->setTimeOffset(timeOffset.toInt());
      restart = true;
      NTP.getTime();
   }

   sendHtml(MSG_CONFIG_SAVED, 200);
   if (saveRtcData) {
      EspRtcMem::saveRtcData(rtcData);
   }
   delay(500);
   if (restart) {
      config->setInitDone(true);
      config->saveToEeprom();
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
   char html[] = "<html><head><title>%s</title><meta name='viewport' content='width=device-width, initial-scale=1'>"\
   "<meta charset='utf-8'>" CSS "</head><h1><body><a href='/'>" MSG_TITLE "</a></h1>%s</body></html>";
   char *htmlPage = (char *)malloc(strlen(html) + strlen(title) + strlen(message) + 1);  // +1 is actually useless since %s is replaced ;)
   sprintf(htmlPage, html, title, message);
   server->sendHeader("Connection", "close");
   server->send(code, "text/html", htmlPage);
   free(htmlPage);
}

void Api::sendPage(const char* page) {
   server->sendHeader("Connection", "close");
   server->send(200, "text/html", page);
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

void Api::setLevel(int _level) {
   level = _level;
}

