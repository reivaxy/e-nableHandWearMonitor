
#include "HandMonitor.h"
#include "mapping.h"


HandMonitor *module;
HandMonitorConfig* config;


void setup() {
  Serial.begin(9600);
  config = new HandMonitorConfig("Hand Monitor");
  config->init();
  module = new HandMonitor(config, PIN_RTC_SDA, PIN_RTC_SCL);
  module->init();
}


void loop() {
  module->loop();
}