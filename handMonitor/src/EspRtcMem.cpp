
#include "EspRtcMem.h"

rtcStoredData EspRtcMem::_rtcData;

rtcStoredData* EspRtcMem::getRtcData() {
   ESP.rtcUserMemoryRead(LAST_RTC_ADDR, (uint32_t*) &_rtcData, sizeof(rtcStoredData));
   return &_rtcData;
}

void EspRtcMem::saveRtcData(rtcStoredData* data) {
   ESP.rtcUserMemoryWrite(LAST_RTC_ADDR, (uint32_t*) data, sizeof(rtcStoredData));
}


