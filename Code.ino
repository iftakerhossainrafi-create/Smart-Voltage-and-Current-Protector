#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <stdlib.h>
#include <string.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
const byte VOLTAGE_PIN = A0;
const byte CURRENT_PIN = A1;
const byte RELAY_PIN   = 5;
const byte BUZZER_PIN  = 6;
const byte FAULT_LED   = 7;
float HIGH_LIMIT = 4.0;
float LOW_LIMIT = 2.0;
float CURRENT_LIMIT = 2.0;
const float SURGE_STEP = 0.5;
const unsigned long SURGE_DELAY = 3000;
bool voltageProtection = true;
bool currentProtection = true;
bool surgeDetected = false;
bool firstVoltageRead = true;
float previousVoltage = 0.0;
unsigned long surgeAlertSince = 0;
char command[40];
byte commandLength = 0;
void showSettings() {
Serial.println(F("\n--- USER SETTINGS ---"));
 Serial.print(F("HIGH="));
  Serial.println(HIGH_LIMIT, 2);
  Serial.print(F("LOW="));
  Serial.println(LOW_LIMIT, 2);
  Serial.print(F("CURRENT="));
  Serial.println(CURRENT_LIMIT, 2);
  Serial.print(F("VPROT="));
  Serial.println(voltageProtection ? F("ON") : F("OFF"));
  Serial.print(F("IPROT="));
  Serial.println(currentProtection ? F("ON") : F("OFF"));
  Serial.println(F("SURGE STEP=0.50 V"));
  Serial.println(F("SURGE DELAY=3 seconds"));
  Serial.println(F("Commands: HIGH=4.2, LOW=1.8"));
  Serial.println(F("CURRENT=2.5, VPROT=ON/OFF"));
  Serial.println(F("IPROT=ON/OFF, STATUS, HELP"));
}
void processCommand(char *cmd) {
  if (strcmp(cmd, "HELP") == 0 ||
      strcmp(cmd, "STATUS") == 0) {
    showSettings();
  }
  else if (strncmp(cmd, "HIGH=", 5) == 0) {
    float value = atof(cmd + 5);
    if (value > LOW_LIMIT && value <= 5.0) {
      HIGH_LIMIT = value;
      surgeDetected = false;
      Serial.println(F("High limit updated."));
    } else {
      Serial.println(F("Invalid high limit."));
    }
  }
  else if (strncmp(cmd, "LOW=", 4) == 0) {
    float value = atof(cmd + 4);
    if (value >= 0.0 && value < HIGH_LIMIT) {
      LOW_LIMIT = value;
      surgeDetected = false;
      Serial.println(F("Low limit updated."));
    } else {
      Serial.println(F("Invalid low limit."));
    }
  }
  else if (strncmp(cmd, "CURRENT=", 8) == 0) {
    float value = atof(cmd + 8);
    if (value > 0.0 && value <= 5.0) {
      CURRENT_LIMIT = value;
      Serial.println(F("Current limit updated."));
    } else {
      Serial.println(F("Invalid current limit."));
    }
  }
  else if (strcmp(cmd, "VPROT=ON") == 0) {
    voltageProtection = true;
    surgeDetected = false;
    previousVoltage = analogRead(VOLTAGE_PIN) * (5.0 / 1023.0);
    firstVoltageRead = false;
    Serial.println(F("Voltage protection enabled."));
  }
  else if (strcmp(cmd, "VPROT=OFF") == 0) {
    voltageProtection = false;
    surgeDetected = false;
    Serial.println(F("Voltage protection disabled."));
  }
  else if (strcmp(cmd, "IPROT=ON") == 0) {
    currentProtection = true;
    Serial.println(F("Current protection enabled."));
  }
  else if (strcmp(cmd, "IPROT=OFF") == 0) {
    currentProtection = false;
    Serial.println(F("Current protection disabled."));
  }
  else {
    Serial.println(F("Invalid command. Type HELP."));
  }
}
void readComputerCommands() {
  while (Serial.available() > 0) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (commandLength > 0) {
        command[commandLength] = '\0';
        processCommand(command);
        commandLength = 0;
      }
    }
    else if (commandLength < sizeof(command) - 1) {
      if (c >= 'a' && c <= 'z') {
        c = c - 'a' + 'A';
      }
      command[commandLength++] = c;
    }
    else {
      commandLength = 0;
      Serial.println(F("Command too long."));
    }
  }
}
void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(FAULT_LED, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(FAULT_LED, LOW);
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Voltage Protector");
  lcd.setCursor(0, 1);
  lcd.print("System Starting");
  delay(1500);
  lcd.clear();
  Serial.println(F("Voltage Protection System"));
  Serial.println(F("Analog input range: 0-5V"));
  showSettings();
}
void loop() {
  readComputerCommands();
  int voltageRaw = analogRead(VOLTAGE_PIN);
  int currentRaw = analogRead(CURRENT_PIN);
  float voltageInput = voltageRaw * (5.0 / 1023.0);
  float currentInput = currentRaw * (5.0 / 1023.0);
  unsigned long now = millis();
  if (firstVoltageRead) {
    previousVoltage = voltageInput;
    firstVoltageRead = false;
  }
  float voltageChange = voltageInput - previousVoltage;
  bool suddenVoltage =
    voltageProtection &&
    voltageInput >= LOW_LIMIT &&
    voltageInput < HIGH_LIMIT &&
    voltageChange >= SURGE_STEP;

  if (suddenVoltage) {
    surgeDetected = true;
    surgeAlertSince = now;
  }
  previousVoltage = voltageInput;
  bool overVoltage =
    voltageProtection && voltageInput > HIGH_LIMIT;
  bool underVoltage =
    voltageProtection && voltageInput < LOW_LIMIT;
  bool overCurrent =
    currentProtection && currentInput > CURRENT_LIMIT;
  bool fault = overVoltage || underVoltage || overCurrent;
  if (surgeDetected &&
      now - surgeAlertSince >= SURGE_DELAY) {
    surgeDetected = false;
  }
  bool relayTrip = fault || surgeDetected;
  digitalWrite(RELAY_PIN, relayTrip ? HIGH : LOW);
  // LED and buzzer remain OFF during surge delay.
  // They turn ON only for voltage/current faults.
  digitalWrite(BUZZER_PIN, fault ? HIGH : LOW);
  digitalWrite(FAULT_LED, fault ? HIGH : LOW);
  lcd.setCursor(0, 0);
  lcd.print("V:");
  lcd.print(voltageInput, 2);
  lcd.print(" I:");
  lcd.print(currentInput, 2);
  lcd.print("   ");
  lcd.setCursor(0, 1);
  if (surgeDetected && !fault) {
    if (now - surgeAlertSince < 1000) {
      lcd.print("SURGE ALERT!    ");
    } else {
      lcd.print("SURGE DELAY     ");
    }
  }
  else if (overVoltage) {
    lcd.print("OVER VOLTAGE    ");
  }
  else if (underVoltage) {
    lcd.print("UNDER VOLTAGE   ");
  }
  else if (overCurrent) {
    lcd.print("OVER CURRENT    ");
  }
  else {
    lcd.print("STATUS: NORMAL  ");
  }
  static unsigned long previousTime = 0;
  if (now - previousTime >= 1000) {
    previousTime = now;
    Serial.print(F("Voltage: "));
    Serial.print(voltageInput, 2);
    Serial.print(F(" V | Current input: "));
    Serial.print(currentInput, 2);
    Serial.print(F(" V | Status: "));
    if (overVoltage) {
      Serial.println(F("OVER VOLTAGE"));
    }
    else if (underVoltage) {
      Serial.println(F("UNDER VOLTAGE"));
    }
    else if (overCurrent) {
      Serial.println(F("OVER CURRENT"));
    }
    else if (surgeDetected) {
      Serial.println(F("SURGE DELAY"));
    }
    else {
      Serial.println(F("NORMAL"));
    }
    Serial.println(
      relayTrip
        ? F("Relay command: TRIP")
        : F("Relay command: NORMAL")
    );
    Serial.println(
      fault
        ? F("LED/Buzzer: ON")
        : F("LED/Buzzer: OFF")
    );
  }
  delay(100);
}
