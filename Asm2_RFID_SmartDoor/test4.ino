#include <Servo.h>

#include <SPI.h>
#include <MFRC522.h>

#include <LiquidCrystal_I2C.h>

#include<LiquidCrystal.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#define RST_PIN         9
#define SS_PIN          10

int UID[4], i;
int pos = 0;

int ID1[4] = {90, 38, 119, 127};
int ID2[4] = {163, 29, 63, 8};

int buzzer = 2;
int times;
int dem = 0;

int led1 = 7;
int led2=6;
int led3=5;

Servo servo_A3;
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() 
{
    Serial.begin(9600);   

    pinMode(buzzer, OUTPUT);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    
    digitalWrite(buzzer, HIGH);
    
    servo_A3.attach(A3);
    SPI.begin();    
    mfrc522.PCD_Init();
    
    lcd.init();
    lcd.backlight();
    lcd.setCursor(5,0);
    lcd.print("Welcome");
}

void loop() 
{
  do{
    if ( ! mfrc522.PICC_IsNewCardPresent()) 
    { 
      return; 
    }
    
    if ( ! mfrc522.PICC_ReadCardSerial()) 
    {  
      return;  
    }
    
    for (byte i = 0; i < mfrc522.uid.size; i++) 
    {        
      UID[i] = mfrc522.uid.uidByte[i];
    }
    
    if (UID[i] == ID1[i] ||UID[i] == ID2[i])
    { 
      times=0;
      digitalWrite(buzzer, LOW);
      digitalWrite(led1, HIGH);
      delay(200);
      digitalWrite(buzzer, HIGH);
      digitalWrite(led1, LOW);
      delay(1000);
  
      lcd.clear();
      lcd.setCursor(3,0);
      lcd.print("Valid card");
    
      for(int a=0; a<2; a++){
        digitalWrite(buzzer, LOW);
        digitalWrite(led1, HIGH);
        delay(200);
        digitalWrite(buzzer, HIGH);
        digitalWrite(led1, LOW);
        delay(200);
      }
    
      lcd.setCursor(3,1);
      lcd.print("Door Opened");
      servo_A3.write(180);
      delay(5000);
    
      lcd.clear();
      lcd.setCursor(3,1);
      lcd.print("Door Closed");
      servo_A3.write(90);
      for(int a=0; a<2; a++){
        digitalWrite(buzzer, LOW);
        delay(200);
        digitalWrite(buzzer, HIGH);
        delay(200);
      }
      lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("Welcome");
  }
  
  else
  {
    times++;
    digitalWrite(buzzer, LOW);
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    delay(200);
    digitalWrite(buzzer, HIGH);
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    delay(1000);
    
    lcd.clear();
    lcd.setCursor(2,1);
    lcd.print("Invalid Card");
    
    for(int a=0; a<2; a++){
      tone(buzzer, 800);
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      delay(200);
      tone(buzzer, 500);
      digitalWrite(led1, LOW);
      digitalWrite(led2, LOW);
      delay(200);
    }
    
    noTone(buzzer);
    digitalWrite(buzzer, HIGH);
    
    if(times == 3){
      times=0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("  Wrong 3 times");
      lcd.setCursor(3,1);
      lcd.print("LOCKED!!!");
      
      for(int a=0; a<10; a++){
        tone(buzzer, 800);
        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, HIGH);
        delay(200);
        tone(buzzer, 500);
        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, LOW);
        delay(200);
      }
      noTone(buzzer);
      digitalWrite(buzzer, HIGH);
      
      for(int t = 10; t >=0; t--)
      {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Try again after");
        lcd.setCursor(6,1);
        lcd.print(t);
        delay(1000);
      }
      
      lcd.clear();
      lcd.setCursor(5,0);
      lcd.print("Welcome");
    }
    else{
    digitalWrite(buzzer, HIGH);
    lcd.clear();
    lcd.setCursor(5,0);
    lcd.print("Welcome");
    }
  }  
    mfrc522.PICC_HaltA();  
    mfrc522.PCD_StopCrypto1();
  }
  while( !mfrc522.PICC_IsNewCardPresent());
}
