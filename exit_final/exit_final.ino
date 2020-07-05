//#include <Adafruit_Sensor.h>
#include <Stepper.h>
#include <DHT11.h>
#include <Servo.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define DHTPIN 2
#define SERVO1 5
#define SERVO2 6

#include <Stepper.h>

#define C2 523                                         // 높은 도
                                                                   // 음계를 상수로  설정
unsigned long prevPlayTime = 0;           // 이전 시간
unsigned long playDuration = 0;             // 실행 길이
int currentMelody = 0;
int piezoPin = 7; 
int melodySize = 50; 
int soundSensor = A0;

int threshold = 70;

Servo s1;
Servo s2;
int angle;
int angle2;

int melody[]={
C2, C2, C2, C2, C2}; // 음계 설정

int tempo[]={
10, 10, 10, 10, 10};            

DHT11 dht11(DHTPIN);
LiquidCrystal_I2C lcd(0x27, 16, 2);

//DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  pinMode(piezoPin, OUTPUT);
  lcd.init();
  Serial.begin(9600);

  pinMode(soundSensor, INPUT);  // 사운드 센서 입력으로 설정  
  

  s1.attach(SERVO1);
  s2.attach(SERVO2);
  s1.write(0);
  s2.write(60);

  lcd.backlight();
  
}
void loop() {
  // put your main code here, to run repeatedly:
  //  unsigned char humi = dht.readHumidity();
  float temp,humi;
  dht11.read(humi,temp);

  Serial.println(analogRead(soundSensor));
  
  lcd.display();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.print(temp);
  Serial.println(temp);
  lcd.println("C");  
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("Humidity:");
  lcd.print(humi);
  Serial.println(humi);
  lcd.println("%");
  
  delay(1000);  

  if(analogRead(soundSensor) >= threshold){
      Serial.println(analogRead(soundSensor));

      lcd.display();
     lcd.setCursor(0, 0);
     lcd.print("Baby Crying!");
  
      // red
      for(int i=0; i<3; i++){
      for(angle=0;angle<=60; angle++){
      angle2 = 60-angle;
      s1.write(angle);
      delay(30);
      s2.write(angle2);
      delay(30);
      Signal();
    }
   for(angle=60; angle>=0; angle--){
      angle2 = 60-angle;
      s1.write(angle);
      delay(30);
      s2.write(angle2);
      delay(30);
      Signal();
    }  
      }    
//     delay(200);
     return;
  }
}

void Signal() {
       if(millis() < prevPlayTime + playDuration) {         // 부저와 모터 같이 실행하기 위해서 타이머 씀
           return;                                                 // 이미 음 재생 중이면 음 재생이 끝날 때까지 다른  음 재생 X
     }
     noTone(8);                                                  // 다음 음을 재생하기 위해 기존 음 재생 멈춤

     if(currentMelody >= melodySize)         //멜로디를 끝까지 연주했으면 다시 재생하도록 인덱스  초기화
          currentMelody = 0;

     int noteDuration = 1000/tempo[currentMelody];              // 음의  재생시간(음의 길이 또는 박자) 계산

     tone(piezoPin, melody[currentMelody], noteDuration);        // 음 재생
     prevPlayTime = millis();                                                          //현재 시간 넣어줌, 다음엔 이전 시간이 됨
     playDuration = noteDuration * 1.30;                                  //음 재생이 끝날 때까지 기다려야 하는 시간
     
     currentMelody++;                                                               //인덱스  증가시켜  다음  음  재생 준비
}



