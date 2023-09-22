#include <Arduino.h>
#include <HardwareSerial.h>


// COMMUNICATION ----------------------------------------------
byte data[24]={0x00};
unsigned long previousMillis = 0;
const long interval = 500;

void receive(HardwareSerial *ser){
  unsigned long currentMillis = millis(); // 現在の時間を取得

  if(ser->available() >=5){
    byte buf[5]={0x00};
    byte tmp_sum = 0;
    buf[0] = ser->read();
    tmp_sum += buf[0];

    if (buf[0] == 0xFF)
    { // 開始バイトの場合
      //ser.println("RECEIVED");
      for (byte i = 1; i < 4; i++)
      {
        buf[i] = ser->read();
        tmp_sum += buf[i];
      }
      buf[4] = ser->read(); // read checksum
      tmp_sum &= 0xFF;              // calc check sum

      if (buf[4]==tmp_sum)
        { // チェックサムが正しい場合
          for (int i = 0; i < 24; i++) {
            // 下位ビットから順にビットを抽出してdata配列に格納
            data[i] = (buf[i/8 +1] >> i%8) & 0x01;
          }

          byte num=0x00;
          for(;ser->available()>0;){
            num=ser->read();
          }
          previousMillis = currentMillis; // 前回の時間を更新
        }
    }
  }
  

  // 0.5秒経過したかどうかをチェック
  if (currentMillis - previousMillis >= interval) {
    // 0.5秒経過したら特定の配列をすべて0にする
    all_clear();
  }
}

void all_clear(){
  for(int i=0;i<24;i++){
    data[i]=0;
  }
}

void debug_dataprint(HardwareSerial *ser){
  for(int i=0;i<24;i++){
    ser->print(data[i]);
    ser->print(" ");
  }
  ser->println("");
}



// DCMotor ----------------------------------------------------
/**
 * モーター番号
 * 横移動…0番
 * 前後移動…1番
*/
//モーターのボタン[先端上移動,先端下移動,根本上移動,根本下移動]
const int BUTTON_MOTOR[4]={1,0,3,2};
//モーターのDIR,PWM出力ピン番号
const int PIN_MDIR[2]={32,28};
const int PIN_MPWM[2]={8,4};
//モーターの回転速度
const int MAX_SPEED[2]={50,50};

void motor_setup(){
  pinMode(PIN_MDIR[0],OUTPUT);
  pinMode(PIN_MDIR[1],OUTPUT);
  
  //PWM周波数変更(2,3,5が変更される)
  TCCR3B = (TCCR3B & 0b11111000) | 0b00000001;
  //PWM周波数変更(11,12が変更される)
  TCCR1B = (TCCR1B & 0b11111000) | 0b00000001;
}

void motor_output(){
  //左右移動
  int tmp_dir0 = data[BUTTON_MOTOR[0]];
  int tmp_pwm0 = (int)MAX_SPEED[0]*(data[BUTTON_MOTOR[0]]|data[BUTTON_MOTOR[1]]);
  digitalWrite(PIN_MDIR[0],tmp_dir0);
  analogWrite(PIN_MPWM[0],tmp_pwm0);
  //前後移動
  int tmp_dir1 = data[BUTTON_MOTOR[3]];//この時MDはHIGHで正回転命令→前移動
  int tmp_pwm1 = (int)MAX_SPEED[1]*(data[BUTTON_MOTOR[2]]|data[BUTTON_MOTOR[3]]);
  digitalWrite(PIN_MDIR[1],tmp_dir1);
  analogWrite(PIN_MPWM[1],tmp_pwm1);
}
