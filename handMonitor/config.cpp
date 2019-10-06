
#include "config.h"

HandMonitorConfig::HandMonitorConfig(unsigned int version, const char* type, const char* name):XEEPROMConfigClass(version + CONFIG_BASE_VERSION, type, sizeof(HandMonitorConfigStruct)) {
  strlcpy(_defaultModuleName, name, NAME_MAX_LENGTH + 1);
  setName(name);
}

HandMonitorConfig::HandMonitorConfig(unsigned int version, const char* type, const char* name, unsigned int dataSize):XEEPROMConfigClass(version + CONFIG_BASE_VERSION, type, dataSize) {
  strlcpy(_defaultModuleName, name, NAME_MAX_LENGTH + 1);
  setName(name);
}

/**
* Reset the config data structure to the default values.
* This is done each time the data structure version is different from the one saved in EEPROM
* NB: version and name are handled by base class
*/
void HandMonitorConfig::initFromDefault() {
  XEEPROMConfigClass::initFromDefault(); // handles version and name init
  HandMonitorConfigStruct* configPtr = _getDataPtr();
  
  setName(_defaultModuleName); // Reset module name to default name
  setAPSsid(DEFAULT_AP_SSID);
  setAPPwd(DEFAULT_AP_PWD);
  setSsid("");
  setPwd(""); 
  setNtpServer(DEFAULT_NTP_SERVER);
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
const char* HandMonitorConfig::getSsid() {
  return _getDataPtr()->ssid;
}

void HandMonitorConfig::setPwd(const char* pwd) {
  strlcpy(_getDataPtr()->pwd, pwd, PWD_MAX_LENGTH + 1);
}
const char* HandMonitorConfig::getPwd() {
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