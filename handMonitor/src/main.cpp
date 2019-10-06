/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Reivaxy & Thomas Broussard
 * 
 *  ---------------------------------------------------------------------------------------------------------------------------------------------
 *  Description : main
 * 
 * =============================================================================================================================================
 */

#include "HandMonitor.h"

HandMonitor *module;


void setup() {
  DebugBegin();
  DebugPrintln("Started");
  WiFi.mode(WIFI_OFF);
  HandMonitorConfig* config = new HandMonitorConfig("Hand Monitor");
  config->init();
  module = new HandMonitor(config, PIN_RTC_SDA, PIN_RTC_SCL);
  module->init();
}


void loop() {
  module->loop();
}