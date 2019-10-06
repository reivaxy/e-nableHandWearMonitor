#pragma once

#include <XEEPROMConfig.h>

// Increment this when new fields (or new default values) are added to HandMonitorConfigStruct
// The module will then use default values, and will need to be configured again.
#define CONFIG_BASE_VERSION 1

// When not initialized yet, a module will open a Wifi access point with these:
#define DEFAULT_AP_SSID "HandMonitor"
#define DEFAULT_AP_PWD "HandMonitor"

#define SSID_MAX_LENGTH 20
#define PWD_MAX_LENGTH 100
#define URL_MAX_LENGTH 200
#define EMAIL_MAX_LENGTH 30
#define NAME_MAX_LENGTH 20
#define AUTH_MAX_LENGTH 200

#define DEFAULT_NTP_SERVER "time.google.com"

// Fixed size structure to store configuration data that needs to be persisted (eeprom)
struct HandMonitorConfigStruct:public XEEPROMConfigDataStruct {
  char moduleName[NAME_MAX_LENGTH + 1];  // Name of the module, can be edited from config page

  char APSsid[SSID_MAX_LENGTH + 1];  // access point (wifi network) opened by module  
  char APPwd[PWD_MAX_LENGTH + 1];

  char ssid[SSID_MAX_LENGTH + 1];    // home box wifi network
  char pwd[PWD_MAX_LENGTH + 1];

  char ntpServer[URL_MAX_LENGTH + 1];  // ntp server differs on adobe network and public network

};

class HandMonitorConfig:public XEEPROMConfigClass {
public:
  HandMonitorConfig(unsigned int version, const char* type, const char* moduleName);
  HandMonitorConfig(unsigned int version, const char* type, const char* moduleName, unsigned int dataSize);
  virtual void initFromDefault();

  void setName(const char*);
  const char* getName(void);

  void setAPSsid(const char* ssid);
  const char* getAPSsid(void);

  void setAPPwd(const char* pwd);
  const char* getAPPwd(void);

  void setSsid(const char* ssid);
  const char* getSsid(void);

  void setPwd(const char* pwd);
  const char* getPwd(void);

  void setNtpServer(const char* ntpServer);
  const char* getNtpServer(void);

protected:
  HandMonitorConfigStruct* _getDataPtr(void);
  char _defaultModuleName[NAME_MAX_LENGTH + 1];  // to be able to reset it if needed
};
