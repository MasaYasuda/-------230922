#include <Arduino.h>

void setup() {
  Serial.begin(115200);
  motor_setup();
}

void loop(){
  receive(&Serial);
  motor_output();
  delay(20);
}

