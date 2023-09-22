#include <Arduino.h>
#include <M5Stack.h>

void setup(){
    M5.begin();
    M5.Lcd.setTextSize(6);
    M5.Lcd.print("Hello World!");
}

void loop(){
}

