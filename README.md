# PRACTICA7_VOLTIOSYAMPERS
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Dirección I2C de la pantalla LCD

const int pinVoltimeterButton = 2;
const int pinAmperimeterButton = 3;
const int pinVoltageSensor = A0;
const int pinCurrentSensor = A1;

bool isVoltimeterMode = true;

void setup() {
  lcd.begin(16, 2);  // Inicialización de la pantalla LCD
  lcd.backlight();  // Encender la retroiluminación de la pantalla LCD

  pinMode(pinVoltimeterButton, INPUT_PULLUP);
  pinMode(pinAmperimeterButton, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(pinVoltimeterButton) == LOW) {
    isVoltimeterMode = true;
    delay(200);  // Debounce delay
  }

  if (digitalRead(pinAmperimeterButton) == LOW) {
    isVoltimeterMode = false;
    delay(200);  // Debounce delay
  }

  if (isVoltimeterMode) {
    float voltage = readVoltage();
    lcd.clear();
    lcd.print("Voltage: ");
    lcd.print(voltage);
    lcd.print(" V");
  } else {
    float current = readCurrent();
    lcd.clear();
    lcd.print("Current: ");
    lcd.print(current);
    lcd.print(" A");
  }

  delay(500);  // Delay entre mediciones
}

float readVoltage() {
  int rawValue = analogRead(pinVoltageSensor);
  float voltage = map(rawValue, 0, 1023, 0, 5);
  return voltage;
}

float readCurrent() {
  int rawValue = analogRead(pinCurrentSensor);
  float voltage = map(rawValue, 0, 1023, 0, 5);
  float current = voltage / 0.185;  // Reemplaza 0.185 con el factor de sensibilidad de tu módulo ACS712
  return current;
}
