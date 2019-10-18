

#include "debug.h"
#include "Led.h"

int Led::pin = 2;
time_t Led::period = 1000;
time_t Led::lastLedBlink = 0;
boolean Led::mustBlink = false;
time_t Led::timeOn = 100;

void Led::blink(time_t _period) {
   period = _period;
   on();
   mustBlink = true;
}

void Led::toggle() {
   digitalWrite(pin, !digitalRead(pin));
}
void Led::on() {
   pinMode(pin, OUTPUT);
   digitalWrite(pin, 0);
}
void Led::off() {
   pinMode(pin, OUTPUT);
   digitalWrite(pin, 1);
}
void Led::stop() {
   off();
   mustBlink = false;
}
void Led::refresh() {
   if(mustBlink) {
      time_t now = millis();
      if (now - lastLedBlink > period) {
         lastLedBlink = now;
         on();
      }      
      if (now - lastLedBlink > timeOn) {
         off();
      }

   }
}
