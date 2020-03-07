
#include "Storage.h"
#include "RTClock.h"

void Storage::recordStateChange(int newState, int level) {
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
      f.printf("%s %d %d\n", recordDate, newState, level);
      f.close();
   }
}

void Storage::listFiles(FileList *fileList) {
   if (!SPIFFS.begin()) {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return;
   }  
   DebugPrintln("Listing files");
   Dir dir = SPIFFS.openDir("/");
   while(dir.next()) {
      char *fileName = strdup(dir.fileName().c_str());
      fileList->push_back(fileName);
   }
}

void Storage::getFile(const char *fileName, File *f) {
   if (!SPIFFS.begin()) {
      Serial.println("An Error has occurred while mounting SPIFFS");
      return ;
   } 
   *f = SPIFFS.open(fileName, "r");
   
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
            f.printf("%02d 10:02:30 1 234\n", startDay + d);
            f.printf("%02d 14:12:08 0 456\n", startDay + d);
            f.close();
            yield(); 
         }
      }
   }
}

