#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// 핀 정의
const int voltagePin = A0; // 배터리 전압 대용
const int tempPin = A1; // 온도 센서
const int greenLed = 6; // 녹색 LED
const int redLed = 7; // 적색 LED
const int buzzer = 8; // 부저

// 임계값 설정
const float MIN_VOLTAGE = 12.0; // 최소전압 12V
const float MAX_VOLTAGE = 14.5; // 최대전압 14.5V
const float MIN_TEMP = 0.0; //최소온도 0C
const float MAX_TEMP = 45.0; // 최대온도 45C

void setup() {
  lcd.begin(16, 2);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(buzzer, OUTPUT);
  
  // 아무짝에도 쓸모없지만 멋진거
  lcd.print("  BMS System  ");
  lcd.setCursor(0, 1);
  lcd.print("  Initializing  ");
  delay(2000); //쓸데없이 기다리기
  lcd.clear();
}

void loop() {
  // 1. 신호변환
  int voltRaw = analogRead(voltagePin);
  float voltage = voltRaw * (20.0 / 1024.0); // 0~20V로 변환(표현상으로만)
 

  // 2. 신호변환2
  int tempRaw = analogRead(tempPin);
  float tempVoltage = tempRaw * (5.0 / 1024.0); 
  float temperature = (tempVoltage - 0.5) * 100.0; //섭씨변환

  // 3. 판별
  bool overVoltage = (voltage > MAX_VOLTAGE); //과전압
  bool underVoltage = (voltage < MIN_VOLTAGE); //저전압
  bool overTemp = (temperature > MAX_TEMP); //고온
  bool underTemp = (temperature < MIN_TEMP); //저온
  bool faultState = (overVoltage || underVoltage || overTemp || underTemp); //뭣중 하나라도

  // 4. 표기
  lcd.setCursor(0, 0);
  lcd.print("V: ");
  lcd.print(voltage, 2);
  lcd.print("V ");

  lcd.setCursor(0, 1);
  lcd.print("T: ");
  lcd.print(temperature, 1);
  lcd.print("C ");

  // 5. 동작
  if (faultState) {
    // 얏됐다
    digitalWrite(greenLed, LOW);
    digitalWrite(redLed, HIGH); // 빨간불
    
    // ㅇㅏ악ㅇ 공습경보
    digitalWrite(buzzer, HIGH); //위용위용
    delay(150); // 쉬었음 버저
    digitalWrite(buzzer, LOW); //위용위용2
    delay(150); // 아주 잘 쉬었음 버저
    
    // 문제표기
    lcd.setCursor(10, 0);
    if (overVoltage)       lcd.print("[O-V] "); // 과전압
    else if (underVoltage) lcd.print("[U-V] "); // 저전압
    else                   lcd.print("      "); // 문제없음 공백

    lcd.setCursor(10, 1);
    if (overTemp)          lcd.print("[O-T] "); // 고온
    else if (underTemp)    lcd.print("[U-T] "); // 저온
    else                   lcd.print("      "); // 문제없음 공백
    
  } else {
    // 야르한 상황
    digitalWrite(greenLed, HIGH); // 초록불
    digitalWrite(redLed, LOW);
    digitalWrite(buzzer, LOW);
    
    lcd.setCursor(10, 0);
    lcd.print("SYS OK");
    lcd.setCursor(10, 1);
    lcd.print("      ");
    
    delay(300); // 과부화 방지 휴식
  }
}