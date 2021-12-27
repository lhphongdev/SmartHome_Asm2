#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <SimpleTimer.h>
#include <DHT.h>

//Auth token to connect to Blynk app
char token[] = "oOnk5blRNvbkfPV3PpED1Hr_7r_ZGOGk";

//ssid and password to connect Wi-Fi
const char* ssid = "Hoai Phong";
const char* password = "0918547658";


const int DHTPIN = 4;
const int DHTTYPE = DHT11; 
DHT dht (DHTPIN, DHTTYPE);

SimpleTimer timer;

const int relayPurple = 15; 
const int relayRed = 2;
const int ledLong = 16;
const int relayFan = 17;
const int buzzer = 18;
const int lightSensor = 34;
const int flameSensor = 19;
const int alarmLEDs = 0;
const int gasSensor = 33;
const int pir = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);
  
  while(WiFi.status() != WL_CONNECTED){
    Serial.println("Connecting...");
    delay(500);
  }
  Serial.println("Successfully connected!!!");

  Blynk.begin(token, ssid, password, "blynk-server.com", 8080);
  
  pinMode(relayPurple, OUTPUT); // Relay Purple Light
  pinMode(relayRed, OUTPUT); // Relay Red Light
  pinMode(ledLong, OUTPUT); // Led Long
  pinMode(relayFan, OUTPUT); // Relay Fans
  pinMode(buzzer, OUTPUT); // Buzzer
  pinMode(alarmLEDs, OUTPUT); // Alarms LED
  
  pinMode(lightSensor, INPUT); // Input - Light Sensor 
  pinMode(flameSensor, INPUT); // Input - Flame Sensor 
  pinMode(gasSensor, INPUT);
  pinMode(pir, INPUT);  
  
  dht.begin();
  
  digitalWrite(buzzer,HIGH);
  
  timer.setInterval(2000L, sendData);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
}

void sendData(){
  float t = dht.readTemperature(); //Read Temperature
  float h = dht.readHumidity(); //Read Humidity
  
  int light = analogRead(lightSensor); //Read Light Insensity
  
  int gas = analogRead(gasSensor); // Read Gas
  
  int flame = digitalRead(flameSensor);

  int passive = digitalRead(pir);

  Blynk.virtualWrite(V7, t);
  Blynk.virtualWrite(V8, h);
  Blynk.virtualWrite(V10, light);
  Blynk.virtualWrite(V9, gas);
  Blynk.virtualWrite(V11, flame);
  Blynk.virtualWrite(V12, passive);
}

BLYNK_WRITE(V0){
  int value = param.asInt();
  digitalWrite(relayPurple,value);
}
BLYNK_WRITE(V1){
  int value = param.asInt();
  digitalWrite(relayRed,value);
}
BLYNK_WRITE(V2){
  int value = param.asInt();
  digitalWrite(ledLong,value);
}
BLYNK_WRITE(V3){
  int value = param.asInt();
  digitalWrite(relayFan,value);
}

BLYNK_WRITE(V4){
  int value = param.asInt();
  if(value == 1)
  {
    for(int i =0; i<=20; i++){
      digitalWrite(buzzer,LOW);
      digitalWrite(alarmLEDs, HIGH);
      delay(100);
      digitalWrite(buzzer, HIGH);
      digitalWrite(alarmLEDs, LOW);
      delay(100);
    } 
    Blynk.virtualWrite(V4, 0);
  }  
}

BLYNK_WRITE(V5){
  int value = param.asInt();

  digitalWrite(relayPurple, value);
  digitalWrite(relayRed, value);
  digitalWrite(ledLong, value);
  
  Blynk.virtualWrite(V0, value);
  Blynk.virtualWrite(V1, value);
  Blynk.virtualWrite(V2, value);
  Blynk.virtualWrite(V6, value);
  
}

BLYNK_WRITE(V6){
  int value = param.asInt();

  digitalWrite(relayPurple, value);
  digitalWrite(relayRed, value);
  digitalWrite(ledLong, value);
  digitalWrite(relayFan, value);
  Blynk.virtualWrite(V0, value);
  Blynk.virtualWrite(V1, value);
  Blynk.virtualWrite(V2, value);
  Blynk.virtualWrite(V3, value);
  Blynk.virtualWrite(V5, value);
}
