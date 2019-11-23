/* 
 *  =============================================================================================================================================
 *  Project : Hand Monitoring e-Nable France
 *  Author  : Xavier Grosjean & Thomas Broussard
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
  DebugPrintln("\nStarting");
  WiFi.mode(WIFI_OFF);
  module = new HandMonitor();
  module->init();
}

void loop() {
  module->loop();
}