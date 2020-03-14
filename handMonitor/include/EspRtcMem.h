/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : Hand monitor class for esp "rctmem" storage
 * 
 * =============================================================================================================================================
 */

#pragma once

#include <Arduino.h>


#define LAST_RTC_ADDR 116  // address of last 12 bytes of 512 bits: (512/4)-12 

typedef struct  {
  uint8_t previous;
  uint8_t counter;
  uint16_t period;
  uint16_t threshold;
  uint8_t hourStartPause;
  uint8_t minStartPause;
  uint8_t hourEndPause;
  uint8_t minEndPause;
  uint16_t pausePeriod;
} rtcStoredData;


class EspRtcMem {
public:
  static rtcStoredData* getRtcData();
  static void saveRtcData(rtcStoredData* rtcData);
  static rtcStoredData _rtcData;
};
 
