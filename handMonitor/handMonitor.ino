
#include "HandMonitor.h"


HandMonitor *module;
HandMonitorConfig* config;

#define SDA 14
#define SCL 12

void setup() {
  Serial.begin(9600);
  config = new HandMonitorConfig("Hand Monitor");
  config->init();
  module = new HandMonitor(config, SDA, SCL);
  module->init();
}


void loop() {
  module->loop();
}