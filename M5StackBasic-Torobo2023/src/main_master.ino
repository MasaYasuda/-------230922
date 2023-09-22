#include <Arduino.h>
#include <M5Stack.h>

void setup(){
    M5.begin();
    Serial.begin(115200);
    M5.Lcd.setTextSize(6);
    M5.Lcd.print("READY");
}

void loop(){
    transmit();
    //送信間隔
    delay(100);
}

//送信用データ
byte data[5]={0x00};

void transmit(){
    for(byte i=0;i<5;i++){
      data[i]={0x00};
    }
    data[0]=0xFF;
    M5.update();
    M5.Lcd.setCursor(0,0);
    if(M5.BtnA.isPressed()){
        if(M5.BtnB.isPressed()){
            M5.Lcd.fillScreen(BLACK);
            M5.Lcd.print("A & B");
            data[1]|=0b00000001;
        }else{
            M5.Lcd.fillScreen(BLACK);
            M5.Lcd.print("A");
            data[1]|=0b00000010;
        }
    }
    if(M5.BtnC.isPressed()){
        if(M5.BtnB.isPressed()){
            M5.Lcd.fillScreen(BLACK);
            M5.Lcd.print("C & B");
            data[1]|=0b00000100;
        }else{
            M5.Lcd.fillScreen(BLACK);
            M5.Lcd.print("C");
            data[1]|=0b00001000;
        }
    }else{
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.print("NO INPUT");
    }
    //calc checksum
    byte checksum=0x00;
    for(byte i=0;i<4;i++){ checksum += data[i]; }
    data[4] = checksum & 0xFF;

    //送信
    Serial.write(data,sizeof(data));
}