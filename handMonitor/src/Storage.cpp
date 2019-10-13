
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

void Storage::listFiles(char **list) {
   if (!SPIFFS.begin()) {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
   }  
   int size = DEFAULT_LIST_ALLOC - 1;
   *list = (char*)malloc(DEFAULT_LIST_ALLOC); 
   **list = 0;

   DebugPrintln("Listing files:");
   Dir dir = SPIFFS.openDir("/");
   while(dir.next()) {
      char *fileName = strdup(dir.fileName().c_str());
      DebugPrintln(fileName);
      // If not enough space to store file name, realloc a "big" chunk
      // to avoid reallocating each time
      int remaining = size - strlen(*list);
      if(remaining < (strlen(fileName) + 1)) {
         DebugPrintln("Reallocating list buffer");
         size += 6*DEFAULT_LIST_ALLOC;
         *list = (char *)realloc(*list, size);
      }
      strlcat(*list, fileName, size);
      free(fileName);
   }

   DebugPrintln("File content:");
   File f = SPIFFS.open("/2019/10.txt", "r");
   int chr;
   while(f.available()) {
      chr = f.read();
      Serial.printf("%c", chr);
   }
   f.close();
}

void Storage::createFakeData() {
   int yearCount = 3;
   int monthCount = 4;
   int dayCount = 10;
   int startYear = 2015;
   int startMonth = 8;
   int startDay = 7;
   if (!SPIFFS.begin()) {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
   }    
   for(int y = 0 ; y < yearCount; y++) {
      for(int m = 0 ; m < monthCount; m++) {
         for(int d = 0; d < dayCount; d ++) {
            char fileName[30];
            sprintf(fileName, "/%04d/%02d.txt", startYear + y, startMonth + m);
            File f = SPIFFS.open(fileName, "a+");
            f.printf("%02d 10:02:30,1", startDay + d);
            f.printf("%02d 14:12:08,0", startDay + d);
            f.close();
            yield(); 
         }
      }
   }
}

