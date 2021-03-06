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
#include "messages.h"

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
      data[JSON_TAG_START_PAUSE] = config->getStartPause();
      data[JSON_TAG_END_PAUSE] = config->getEndPause();
      data[JSON_TAG_IS_PAUSED] = clock->isPaused()?MSG_YES:MSG_NO;
      data[JSON_TAG_PAUSE_PERIOD] = config->getPausePeriod();

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
   
   // reading a file from /w
   server->onNotFound([&]() {
      DebugPrintf("GET %s\n", server->uri().c_str());
      handleNotFound();
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
      Storage::deleteFiles("");
      ESP.restart();
   });
   
    // delete files from /d directory (data files)
   server->on("/deleteAll", HTTP_POST, [&]() {
      DebugPrintln("POST /deleteAll");
      securityDelayWarning("SPIFFS data file delete");
      Serial.println("Erasing Files ");
      Storage::deleteFiles("");      
   });

    // Format SPIFFS only
   server->on("/format", HTTP_POST, [&]() {
      DebugPrintln("POST /format");
      securityDelayWarning("SPIFFS format");
      Serial.println("Format SPIFFS ");
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
      DebugPrintln("GET /listFiles");
      listFiles(server->arg("dir"));
   });

   // Read one file in SPIFFS
   server->on("/readFile", HTTP_GET, [&]() {
      DebugPrintln("GET /readFile");
      readFile(server->arg("file").c_str());
   });
   
   // delete one file from SPIFFS
   server->on("/deleteFile", HTTP_GET, [&]() {
      DebugPrintln("GET /deleteFile");
      String fileName = server->arg("file");
      if(fileName.length() == 0) {
         sendHtml("Error", "file name can't be empty", 500);
         return;
      }
      Storage::deleteFiles(fileName.c_str());
      char message[100];
      char dirName[100];
      sprintf(dirName, "%s", fileName.c_str());
      char* slash = strchr(dirName+1, '/');
      if (slash != NULL) {
         *slash = 0;
      }

      sprintf(message, "File deleted<br><a href='/listFiles?dir=%s'>Retour</a>", dirName);
      sendHtml("deleted", message, 200);
   });

   // Upload a file
   server->on("/upload", HTTP_POST, [&]() {
     server->send(200); },
     [&]() {
     handleFileUpload(); }     
   );

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

void Api::listFiles(String dirName) {
   Utils::checkHeap("Before file listing");
   FileList fileList;
   Storage::listFiles(&fileList, dirName);
   size_t bufferSize = 2000 ;
   char* page = (char *)malloc(bufferSize);
   *page = 0;
   if(dirName.length() == 0) {
      dirName = "/d";
   }
   for (FileList::iterator it = fileList.begin(); it != fileList.end(); it++) {
      char fileLine[400];
      char download[150];
      char erase[200];
      
      if (server->arg("del").length()!=0 || dirName.equals("/w")) {
         sprintf(erase, "<a alt='%s' class='erase' onClick=\"result=confirm('Confirm ?'); if(!result) return false;\" href='/deleteFile?file=%s/%s'>&#8855;</a>",
                        MSG_DELETE, dirName.c_str(), *it);
      } else {
         *erase = 0;
      }
      sprintf(download, "<a alt='%s' class='download' download='%s' href='/readFile?file=%s/%s'>&#x21F2;</a>", MSG_DOWNLOAD, *it, dirName.c_str(), *it);
      sprintf(fileLine, "%s <a href='/readFile?file=%s/%s'>%s</a> %s<br/>", download, dirName.c_str(), *it, *it, erase);
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
   boolean restart = false;
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
   // Read and save new AP Pwd
   String apPwd = server->arg("apPwd");
   if (apPwd.length() > 0) {
      // TODO: add more checks
      config->setAPPwd(apPwd.c_str());
      restart = true;
   }

   // Read and save new Home SSID
   String homeSsid = server->arg("homeSsid");
   // If value changed, will need to restart
   if(strncmp(config->getSsid(), homeSsid.c_str(), SSID_MAX_LENGTH) != 0) {
      restart = true;
   }
   if (homeSsid.length() > 0) {
      // TODO: add more checks ?
      config->setSsid(homeSsid.c_str());
   } else {
      // Emptying ssid in the form is the way to remove this setting
      memset(config->getSsid(), 0, SSID_MAX_LENGTH);
      memset(config->getPwd(), 0, PWD_MAX_LENGTH);
      config->setSsid("");
      config->setPwd("");

   }

   // Read and save new Home Pwd
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
   
   String dateTime = server->arg("manualDateTime");
   if (dateTime.length() > 0) {
      RTClock *clock = new RTClock();
      clock->manualSetup(dateTime.c_str());
   }
   
   String startPause = server->arg("startPause");
   if (startPause.length() > 0) {
      config->setStartPause(startPause.c_str());
   }

   String endPause = server->arg("endPause");
   if (endPause.length() > 0) {
      config->setEndPause(endPause.c_str());
   }

   String intervalPause = server->arg("intervalPause");
   if(intervalPause.toInt() > 0) {
      config->setPausePeriod(intervalPause.toInt());
   }

   sendHtml(MSG_CONFIG_SAVED, 200);
   config->setInitDone(true);
   config->saveToEeprom();
   delay(200);   // make sure page is displayed
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
   char html[] = "<html><head><title>%s</title><meta name='viewport' content='width=device-width, initial-scale=1'>"\
   "<meta charset='utf-8'>" CSS "</head><h1><body><div class='logo'></div><a href='/'>" MSG_TITLE "</a></h1>%s</body></html>";
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

void Api::handleFileUpload() { // upload a new file to the SPIFFS
  SPIFFS.begin();
  HTTPUpload& upload = server->upload();
  if(upload.status == UPLOAD_FILE_START){
    String filename = upload.filename;
    filename = "/w/"+filename;
    DebugPrintf("handleFileUpload Name: %s\n", filename.c_str());
    fsUploadFile = SPIFFS.open(filename, "w");            // Open the file for writing in SPIFFS (create if it doesn't exist)
    filename = String();
  } else if(upload.status == UPLOAD_FILE_WRITE){
    if(fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
  } else if(upload.status == UPLOAD_FILE_END){
    if(fsUploadFile) {                                    // If the file was successfully created
      fsUploadFile.close();                               // Close the file again
      DebugPrintf("handleFileUpload Size: %d\n", upload.totalSize);
      sendHtml("Uploaded", "Upload successful", 200);
    } else {
      sendHtml("Upload failed", "Error can't create file", 500);
    }
  }
}

void Api::handleNotFound() {
   String uri = "/w" + server->uri();
   if (!SPIFFS.exists(uri)) {
      sendText("Not found", 404);
      return;
   }
   readFile(uri.c_str());
}
