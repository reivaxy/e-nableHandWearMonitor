
#include "config.h"
#include "Storage.h"
#include "EspRtcMem.h"
#include <TimeLib.h>
#include <NtpClientLib.h>

HandMonitorConfig::HandMonitorConfig(const char* name):XEEPROMConfigClass(CONFIG_BASE_VERSION, "HandMonitor", sizeof(HandMonitorConfigStruct)) {
  strlcpy(defaultModuleName, name, NAME_MAX_LENGTH + 1);
  setName(name);
}

HandMonitorConfig::HandMonitorConfig(const char* name, unsigned int dataSize):XEEPROMConfigClass(CONFIG_BASE_VERSION, "HandMonitor", dataSize) {
  strlcpy(defaultModuleName, name, NAME_MAX_LENGTH + 1);
  setName(name);
}

/**
* Reset the config data structure to the default values.
* This is done each time the data structure version is different from the one saved in EEPROM
* NB: version and name are handled by base class
*/
void HandMonitorConfig::initFromDefault() {
  XEEPROMConfigClass::initFromDefault(); // handles version and name init

  setName(defaultModuleName); // Reset module name to default name
  setAPSsid(DEFAULT_AP_SSID);
  setAPPwd(DEFAULT_AP_PWD);
  setSsid("");
  setPwd(""); 
  setNtpServer(DEFAULT_NTP_SERVER);
  setSensorThreshold(450);
  setRefreshInterval(5);
  setTimeOffset(60); // +1 hour offset: france winter time.
  setInitDone(false);
  setStartPause("21:00");
  setEndPause("07:30");
  setPausePeriod(1800);

  initRtcMemConfig();
}

void HandMonitorConfig::init() {
  XEEPROMConfigClass::init();
  initRtcMemConfig();
}

void HandMonitorConfig::initRtcMemConfig() {
  rtcStoredData *rtcData = EspRtcMem::getRtcData();
  rtcData->counter = 0;
  rtcData->previous = 0;

  rtcData->threshold = getSensorThreshold();
  rtcData->period = getRefreshInterval();
  rtcData->hourStartPause = Utils::toInt(getStartPause(), 2);
  rtcData->minStartPause = Utils::toInt(getStartPause()+3, 2);
  rtcData->hourEndPause = Utils::toInt(getEndPause(), 2);
  rtcData->minEndPause = Utils::toInt(getEndPause()+3, 2);
  rtcData->pausePeriod = getPausePeriod();
  EspRtcMem::saveRtcData(rtcData);  
}

void HandMonitorConfig::setName(const char* name) {
  strlcpy(_getDataPtr()->moduleName, name, NAME_MAX_LENGTH + 1);
}
const char* HandMonitorConfig::getName(void) {
  return _getDataPtr()->moduleName;
}

void HandMonitorConfig::setAPSsid(const char* ssid) {
  strlcpy(_getDataPtr()->APSsid, ssid, SSID_MAX_LENGTH + 1);
}
const char* HandMonitorConfig::getAPSsid() {
  return _getDataPtr()->APSsid;
}

void HandMonitorConfig::setAPPwd(const char* pwd) {
  strlcpy(_getDataPtr()->APPwd, pwd, PWD_MAX_LENGTH + 1);
}
const char* HandMonitorConfig::getAPPwd() {
  return _getDataPtr()->APPwd;
}

void HandMonitorConfig::setSsid(const char* ssid) {
  strlcpy(_getDataPtr()->ssid, ssid, SSID_MAX_LENGTH + 1);
}
char* HandMonitorConfig::getSsid() {
  return _getDataPtr()->ssid;
}

void HandMonitorConfig::setPwd(const char* pwd) {
  strlcpy(_getDataPtr()->pwd, pwd, PWD_MAX_LENGTH + 1);
}
char* HandMonitorConfig::getPwd() {
  return _getDataPtr()->pwd;
}

void HandMonitorConfig::setNtpServer(const char* ntpServer) {
  strlcpy(_getDataPtr()->ntpServer, ntpServer, URL_MAX_LENGTH + 1);
}
const char* HandMonitorConfig::getNtpServer(void) {
  return _getDataPtr()->ntpServer;
}

HandMonitorConfigStruct* HandMonitorConfig::_getDataPtr(void) {
  return (HandMonitorConfigStruct*)XEEPROMConfigClass::_getDataPtr();
}

bool HandMonitorConfig::isHomeWifiConfigured() {
    if (*getSsid() != 0) {
    return true;
  }
  return false;

}

void HandMonitorConfig::setSensorThreshold(uint16_t value) {
  Serial.printf("SETTING THRESHOLD %d\n", value);
  _getDataPtr()->sensorThreshold = value;    
}

uint16_t HandMonitorConfig::getSensorThreshold(void) {
  return _getDataPtr()->sensorThreshold;
}

void HandMonitorConfig::setRefreshInterval(uint16_t value) {
  _getDataPtr()->refreshInterval = value;  
}

uint16_t HandMonitorConfig::getRefreshInterval(void) {
  return _getDataPtr()->refreshInterval;
}

void HandMonitorConfig::setTimeOffset(int value) {
  _getDataPtr()->timeOffset = value;  
}

int HandMonitorConfig::getTimeOffset(void) {
  return _getDataPtr()->timeOffset;
}

void HandMonitorConfig::setInitDone(bool value) {
  _getDataPtr()->initDone = value;    
}
bool HandMonitorConfig::getInitDone(void) {
  return _getDataPtr()->initDone;
}

void HandMonitorConfig::setStartPause(const char* startPause) {
  strlcpy(_getDataPtr()->startPause, startPause, TIME_MAX_LENGTH + 1);
  initRtcMemConfig();
}
const char* HandMonitorConfig::getStartPause() {
  return _getDataPtr()->startPause;
}
void HandMonitorConfig::setEndPause(const char* endPause) {
  strlcpy(_getDataPtr()->endPause, endPause, TIME_MAX_LENGTH + 1);
  initRtcMemConfig();
}
const char* HandMonitorConfig::getEndPause() {
  return _getDataPtr()->endPause;
}
void HandMonitorConfig::setPausePeriod(uint8_t period) {
  _getDataPtr()->pausePeriod = period;
  initRtcMemConfig();
}
uint8_t HandMonitorConfig::getPausePeriod() {
  return _getDataPtr()->pausePeriod;
}