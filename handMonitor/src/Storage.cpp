
#include "Storage.h"
#include "RTClock.h"

void Storage::recordStateChange(int previousState) {
   uint32_t newState = (previousState == 0?1:0);
   RTClock *clock = new RTClock();
   clock->setup();
   char fileName[30];
   char recordDate[30];
   int result = clock->getFileName(fileName);
   if (result == 0) {
      result = clock->getRecordDate(recordDate);
      if (!SPIFFS.begin()) {
         Serial.println("An Error has occurred while mounting SPIFFS");
         return;
      }      
      // Open current file to append data
      DebugPrintf("Updating file %s\n", fileName);
      File f = SPIFFS.open(fileName, "a+");
      f.printf("%s,%d\n", recordDate, newState);
      f.close();
   }
   // Store new state in ESP RTC mem so that it can be read at next wake up.
   ESP.rtcUserMemoryWrite(LAST_RTC_ADDR, (uint32_t*) &newState, sizeof(uint32_t));

}

void Storage::listFiles(char *list, int size) {
   if (!SPIFFS.begin()) {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
   }    
   DebugPrintln("Listing files:");
   Dir dir = SPIFFS.openDir("/");
   while(dir.next()) {
      DebugPrintln(dir.fileName());
      strlcat(list, dir.fileName().c_str(), size);
   }

   DebugPrintln("File content:");
   File f = SPIFFS.open("/2019/10/data.txt", "r");
   int chr;
   while(f.available()) {
      chr = f.read();
      Serial.printf("%c", chr);
   }
   f.close();
}   

