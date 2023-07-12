//
//                RFID
//     VICTOR ADRIAN PANTUJ LOBOS
//                2022019
//
// Variables
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Pines a utilizar
#define SS_PIN 10
#define RST_PIN 9
#define LED_V 6 
#define LED_R 7 
#define BUZZER 5 
MFRC522 mfrc522(SS_PIN, RST_PIN);   
Servo myServo; 

LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() 
{
  Serial.begin(9600);   
  SPI.begin();      
  mfrc522.PCD_Init();   
  myServo.attach(3); 
  myServo.write(0); 
  pinMode(LED_V, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  noTone(BUZZER);
  
  // INICIALIZACION DE LCD
  lcd.begin(16, 2); 
  lcd.backlight();  
  
  lcd.setCursor(0, 0);
  lcd.print("ACCESO:");
}

void loop() 
{
  // LEER NUEVA TARJETA
  if (!mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // SeLECCIONAR UNA TARJETA
  if (!mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //UID EN EL MONITOR SERIAL
  Serial.print("UID tag :");
  String content = "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  // USUARIOS PERMITIDOS EN EL ACCESO
  lcd.setCursor(0, 1); // MOVER LA ESCRITURA A LA SEGUNDA LINEA
  if (content.substring(1) == "A1 91 AF 1B" || 
  content.substring(1) == "97 AA 62 B3" || 
  content.substring(1) == "B2 54 9E 1C" || 
  content.substring(1) == "2D 66 DE 5F") // USUARIOS CON ACCESO
  {
    lcd.print("CONCEDIDO"); // SI EL USUARIO ESTA ENTRE LOS PERMITIDOS CONCEDE EL ACCESO
    Serial.println("Acceso autorizado");
    Serial.println();
    delay(500);
    digitalWrite(LED_V, HIGH);
    tone(BUZZER, 500);
    delay(300);
    noTone(BUZZER);
    myServo.write(360);
    delay(5000);
    myServo.write(0);
    digitalWrite(LED_V, LOW);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACCESO:");
  }
  else
  {
    lcd.print("DENEGADO"); // SI EL USUARIO NO SE ENCUENTRA ENTRE LOS PERMITIDOS NIEGA EL ACCESO
    Serial.println(" Acceso denegado");
    digitalWrite(LED_R, HIGH);
    tone(BUZZER, 300);
    delay(1000);
    digitalWrite(LED_R, LOW);
    noTone(BUZZER);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("ACCESO:");
  }
}
