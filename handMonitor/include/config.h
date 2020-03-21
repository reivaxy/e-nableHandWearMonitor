/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : define the config data structure to be saved in EEPROM
 * 
 * =============================================================================================================================================
 */

#pragma once

#include "Utils.h"
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

#define TIME_MAX_LENGTH 5

#define DEFAULT_NTP_SERVER "time.google.com"

// Fixed size structure to store configuration data that needs to be persisted (eeprom)
struct HandMonitorConfigStruct:public XEEPROMConfigDataStruct {
  char moduleName[NAME_MAX_LENGTH + 1];  // Name of the module, can be edited from config page

  char APSsid[SSID_MAX_LENGTH + 1];  // access point (wifi network) opened by module  
  char APPwd[PWD_MAX_LENGTH + 1];

  char ssid[SSID_MAX_LENGTH + 1];    // home box wifi network
  char pwd[PWD_MAX_LENGTH + 1];

  char ntpServer[URL_MAX_LENGTH + 1];

  uint16_t sensorThreshold;  // 0-1024. Typically, around 200 (depending on resistor values on board)
  uint16_t refreshInterval;  // in seconds
  int timeOffset;  // in minutes

  bool initDone; // false on new module/new config version. True at first config save.
  char startPause[TIME_MAX_LENGTH + 1];
  char endPause[TIME_MAX_LENGTH + 1];
  uint16_t pausePeriod;

};

class HandMonitorConfig:public XEEPROMConfigClass {
public:
  HandMonitorConfig(const char* moduleName);
  HandMonitorConfig(const char* moduleName, unsigned int dataSize);
  virtual void initFromDefault();
  void initRtcMemConfig();

  void init();
  void setName(const char*);
  const char* getName(void);

  void setAPSsid(const char* ssid);
  const char* getAPSsid(void);

  void setAPPwd(const char* pwd);
  const char* getAPPwd(void);

  void setSsid(const char* ssid);
  char* getSsid(void);

  void setPwd(const char* pwd);
  char* getPwd(void);

  void setNtpServer(const char* ntpServer);
  const char* getNtpServer(void);

  void setSensorThreshold(uint16_t value);
  uint16_t getSensorThreshold(void);

  void setRefreshInterval(uint16_t value);
  uint16_t getRefreshInterval(void);

  void setTimeOffset(int value);
  int getTimeOffset(void);

  void setInitDone(bool value);
  bool getInitDone(void);

  void setStartPause(const char*);
  const char* getStartPause();
  void setEndPause(const char*);
  const char* getEndPause();
  void setPausePeriod(uint16_t period);
  uint16_t getPausePeriod(); 

  bool isHomeWifiConfigured(void);
  HandMonitorConfigStruct* _getDataPtr(void);
  char defaultModuleName[NAME_MAX_LENGTH + 1];  // to be able to reset it if needed
};
