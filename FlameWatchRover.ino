// Flame Watch Smart Mobile Rover
// Main Arduino Sketch
// NOTE: This file contains the code provided by the user, reformatted for GitHub.
// Some portions of the original snippet were incomplete/truncated and may require
// compilation fixes before deployment.

#include "esp_camera.h"
#include <WiFi.h>
#include "esp_http_server.h"
#include <BluetoothSerial.h>
#include <HardwareSerial.h>

// WiFi Credentials
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// Pin Definitions
#define FLAME_SENSOR 14
#define BUZZER 15
#define MOTOR_IN1 12
#define MOTOR_IN2 13
#define MOTOR_IN3 2
#define MOTOR_IN4 4

BluetoothSerial SerialBT;
HardwareSerial gsm(1);

// Camera Pin Configuration (AI Thinker ESP32-CAM)
#define PWDN_GPIO_NUM 32
#define RESET_GPIO_NUM -1
#define XCLK_GPIO_NUM 0
#define SIOD_GPIO_NUM 26
#define SIOC_GPIO_NUM 27
#define Y9_GPIO_NUM 35
#define Y8_GPIO_NUM 34
#define Y7_GPIO_NUM 39
#define Y6_GPIO_NUM 36
#define Y5_GPIO_NUM 21
#define Y4_GPIO_NUM 19
#define Y3_GPIO_NUM 18
#define Y2_GPIO_NUM 5
#define VSYNC_GPIO_NUM 25
#define HREF_GPIO_NUM 23
#define PCLK_GPIO_NUM 22

void sendSMS() {
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.println("AT+CMGS=\"+916380967033\"");
  delay(1000);
  gsm.println("ALERT! Fire Detected by Flame Watch Rover!");
  delay(100);
  gsm.write(26);
}

void moveForward(){ digitalWrite(MOTOR_IN1,HIGH); digitalWrite(MOTOR_IN2,LOW); digitalWrite(MOTOR_IN3,HIGH); digitalWrite(MOTOR_IN4,LOW);}
void moveBackward(){ digitalWrite(MOTOR_IN1,LOW); digitalWrite(MOTOR_IN2,HIGH); digitalWrite(MOTOR_IN3,LOW); digitalWrite(MOTOR_IN4,HIGH);}
void turnLeft(){ digitalWrite(MOTOR_IN1,LOW); digitalWrite(MOTOR_IN2,HIGH); digitalWrite(MOTOR_IN3,HIGH); digitalWrite(MOTOR_IN4,LOW);}
void turnRight(){ digitalWrite(MOTOR_IN1,HIGH); digitalWrite(MOTOR_IN2,LOW); digitalWrite(MOTOR_IN3,LOW); digitalWrite(MOTOR_IN4,HIGH);}
void stopRover(){ digitalWrite(MOTOR_IN1,LOW); digitalWrite(MOTOR_IN2,LOW); digitalWrite(MOTOR_IN3,LOW); digitalWrite(MOTOR_IN4,LOW);}

void checkFire(){
  int flameValue=digitalRead(FLAME_SENSOR);
  if(flameValue==LOW){
    digitalWrite(BUZZER,HIGH);
    sendSMS();
    Serial.println("Fire Detected!");
    delay(5000);
  } else {
    digitalWrite(BUZZER,LOW);
  }
}

void setup(){
  Serial.begin(115200);
  pinMode(FLAME_SENSOR,INPUT);
  pinMode(BUZZER,OUTPUT);
  pinMode(MOTOR_IN1,OUTPUT);
  pinMode(MOTOR_IN2,OUTPUT);
  pinMode(MOTOR_IN3,OUTPUT);
  pinMode(MOTOR_IN4,OUTPUT);

  SerialBT.begin("FlameWatchRover");
  gsm.begin(9600,SERIAL_8N1,16,17);

  // Camera and WiFi initialization should be added here
}

void loop(){
  checkFire();

  if(SerialBT.available()){
    char command=SerialBT.read();
    if(command=='F') moveForward();
    if(command=='B') moveBackward();
    if(command=='L') turnLeft();
    if(command=='R') turnRight();
    if(command=='S') stopRover();
  }
  delay(10);
}
