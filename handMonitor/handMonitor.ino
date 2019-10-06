
#include "HandMonitor.h"


HandMonitor *module;
HandMonitorConfig* config;

void setup() {
  Serial.begin(9600);
  config = new HandMonitorConfig(PLANT_MONITOR_VERSION, "HandMonitor", "Hand Monitor");
  config->init();
  module = new HandMonitor(config, D2, D1);
  module->init();
}


void loop() {
  module->loop();
}