
#include "config.h"
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

bool HandMonitorConfig::isHomeWifiConfigured() {
    if (*getSsid() != 0) {
    return true;
  }
  return false;

}