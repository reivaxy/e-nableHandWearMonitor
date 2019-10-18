
#include "Ota.h"
#include "debug.h"
#include "Led.h"

void Ota::start() {
   DebugPrintln("Ready for OTA");
   // We want to handle a timeout, in case no upload occurs.
   otaReadyTime = millis();
   Led::blink(500);
   ArduinoOTA.onStart([&]() {
      otaIsStarted = true;
   });  
   ArduinoOTA.onEnd([&]() {
      Led::off(); // we'll restart, anyway
   });  
   ArduinoOTA.onProgress([&](unsigned int progress, unsigned int total) {
      if(progress == total) {
         DebugPrintln("Uploaded");
      }
      DebugPrintf("Progress: %u%%\n", (progress / (total / 100)));
      Led::toggle();  // no loop refresh during upload
   });
   ArduinoOTA.onError([&](ota_error_t error) {
      char msgErr[50];
      DebugPrintf("OTA Error %u: \n", error);
      if (error == OTA_AUTH_ERROR) sprintf(msgErr,"Auth Failed");
      else if (error == OTA_BEGIN_ERROR) sprintf(msgErr,"Begin Failed");
      else if (error == OTA_CONNECT_ERROR) sprintf(msgErr,"Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) sprintf(msgErr,"Receive Failed");
      else if (error == OTA_END_ERROR) sprintf(msgErr,"End Failed");
      DebugPrintln(msgErr);
   }); 
   ArduinoOTA.begin();  
}

void Ota::refresh() {
   if (otaReadyTime != 0) { 
      time_t now = millis();
      if (!otaIsStarted) {
         int remainingTime = 120 - ((now - otaReadyTime) / 1000);
         // OTA cancelled after 3mn 
         if (remainingTime <= 0) {
            DebugPrintln("OTA Cancelled by timeout, restarting");
            ESP.restart();
         }

      }
      ArduinoOTA.handle();
   }
}