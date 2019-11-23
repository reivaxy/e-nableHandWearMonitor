/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : Rtc handling
 * 
 * =============================================================================================================================================
 */

#include <math.h>
#include "RTClock.h"
#define countof(a) (sizeof(a) / sizeof(a[0]))

void RTClock::setup() {   
   DebugPrintf("SDA: %d\n", SDA);
   DebugPrintf("SCL: %d\n", SCL);
   Wire.begin(PIN_RTC_SDA, PIN_RTC_SCL);
   clock = new RtcDS3231<TwoWire>(Wire);
   clock->Begin();  
}

void RTClock::setup(const char* _dateTime) {
   RTClock::setup();

   // _dateTime is formatted 15:54:10 18/10/2019 
   int year = toInt(_dateTime + 15, 4);
   int month = toInt(_dateTime + 12, 2);
   int day = toInt(_dateTime + 9, 2);

   int hour = toInt(_dateTime, 2);
   int min = toInt(_dateTime + 3, 2);
   int sec = toInt(_dateTime + 6, 2);

   RtcDateTime rtcDateTime = RtcDateTime(year, month, day, hour, min, sec);
   // printDateTime(rtcDateTime);
   clock->SetDateTime(rtcDateTime);
   if (!clock->GetIsRunning()) {
      Serial.println("RTC was not actively running, starting now");
      clock->SetIsRunning(true);
   }   

}

int RTClock::getTime(char* buffer) {
   RtcDateTime dateTime;
   int lastError = get(&dateTime);
   if (lastError == 0) {
      sprintf(buffer, "%02d:%02d:%02d %02d/%02d/%04d", 
                  dateTime.Hour(), dateTime.Minute(), dateTime.Second(),
                  dateTime.Day(), dateTime.Month(), dateTime.Year());
   }  
   return lastError;
}

int RTClock::getFileName(char *fileName) {
   RtcDateTime dateTime;
   int lastError = get(&dateTime);
   if (lastError == 0) {
      sprintf(fileName, "/%04d/%02d.txt", 
                  dateTime.Year(), dateTime.Month());
   }
   return lastError;   
}

int RTClock::getRecordDate(char *fileName) {
   RtcDateTime dateTime;
   int lastError = get(&dateTime);
   if (lastError == 0) {
      sprintf(fileName, "%02d %02d:%02d:%02d", 
                  dateTime.Day(), dateTime.Hour(), dateTime.Minute(), dateTime.Second());
   }
   return lastError;   
}

int RTClock::get(RtcDateTime* dt) {
   int lastError = 0;
   *dt = clock->GetDateTime();
   if (!clock->IsDateTimeValid()) {
      lastError = clock->LastError();
      if (lastError != 0) {
         // we have a communications error
         // see https://www.arduino.cc/en/Reference/WireEndTransmission for 
         // what the number means
         Serial.print("RTC communications error = ");
         Serial.println(lastError);
      }
      else {
         Serial.println("RTC lost confidence in the DateTime!");
         lastError = -1;
      }
   }
   return lastError;   
}

void RTClock::printDateTime(const RtcDateTime& dt) {
    char datestring[20];

    snprintf_P(datestring, 
            countof(datestring),
            PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
            dt.Month(),
            dt.Day(),
            dt.Year(),
            dt.Hour(),
            dt.Minute(),
            dt.Second() );
    Serial.print(datestring);
}

int RTClock::toInt(const char* str, int length) {
   int result = 0;
   for(int i = 0; i < length; i++) {
      result += pow(10, (length - 1 - i)) * (str[i] - 48);
   }
   DebugPrintf("Converted first %d chars from '%s' to %d\n", length, str, result);
   return result;
}