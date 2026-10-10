# Smart-Voltage-and-Current-Protector
Introduction :

Voltage variations and excessive current can damage electrical equipment and interrupt operation. A protection controller monitors electrical parameters and disconnects the load when readings leave permitted limits.
This project demonstrates the core control logic of such a controller using an Arduino Uno, an LCD, a relay module, tow potentiometer, a buzzer and a fault indicator. It is an educational and recruitment-project demonstration covering under-voltage, over-voltage and over-current detection, computer communication, configurable thresholds and delayed reconnection.

Objectives :

The prototype is designed to meet the following objectives:
•	Detect input values above an upper voltage threshold or below a lower voltage threshold.
•	Detect an over-current condition using a current-sensor signal or a simulated analog input.
•	Command a relay to disconnect the load when a configured fault occurs.
•	Display readings and fault status on a 16×2 I2C LCD.
•	Communicate with a computer through USB serial for monitoring and setting changes.
•	Require three seconds of continuously valid input before reconnecting the load.
•	Provide a safe, low-voltage prototype suitable for demonstration.

Hardware Components :

1. Arduino Uno or compatible board — 1 piece
2. 16×2 LCD with I2C backpack — 1 piece
3. 10 kΩ potentiometer — 2 pieces (to simulate voltage and current signals)
4. Relay module — 1 piece
5. Buzzer — 1 piece
6. Fault LED — 1 piece (optional if using the built-in LED on Arduino Uno)
7. Series resistor for external LED — 1 piece (typically 220 Ω–330 Ω)
8. Breadboard — 1 piece
9. Jumper wires — As required
10. USB cable — 1 piece
11. Computer with Arduino IDE and Serial Monitor — 1 unit

Pin Configuration :

1. Voltage input signal — A0 (connected to the wiper of the first potentiometer)
2. Current input signal — A1 (connected to the wiper of the second potentiometer)
3. Relay module IN — D5
4. Buzzer positive/control — D6
5. Fault LED — D13 (built-in LED on Arduino Uno)
6. I2C LCD SDA — A4
7. I2C LCD SCL — A5
8. LCD VCC — 5V (if supported by the module)
9. LCD GND — GND
10. Potentiometer power connections — Both potentiometers' outer terminals to 5V and GND, respectively.
11. Common ground — Connect all module grounds to Arduino GND.

Code :

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


Working Principle :

The Arduino-Based Voltage and Current Protection System works by continuously monitoring the input voltage and current using an Arduino Uno microcontroller. The voltage sensor provides its signal to the A0 analog input pin, while the current sensor provides its signal to the A1 pin. The Arduino reads these analog signals and converts them into voltage values using its built-in Analog-to-Digital Converter (ADC). These values are then compared with predefined protection limits, where the high voltage limit is set to 4.0 V, the low voltage limit is set to 2.0 V, and the current input limit is set to 2.0 V.
When the input voltage exceeds the high limit, the system identifies an overvoltage condition. Similarly, when the voltage falls below the low limit, an undervoltage condition is detected. If the current sensor output exceeds the predefined current input limit, the system identifies an overcurrent condition. In any of these fault conditions, the Arduino sends a trip command to the relay module to disconnect the connected load. At the same time, the fault LED and buzzer turn on to alert the user, and the LCD displays the corresponding fault message.
The system also includes a sudden voltage surge detection feature. It continuously compares the current voltage reading with the previous reading. If the voltage increases by 0.5 V or more between consecutive readings while remaining within the predefined voltage limits, the Arduino identifies a potential voltage surge. The system then activates a three-second surge delay, during which the relay remains in the trip state. If no other voltage or current fault is present, the LED and buzzer remain off, while the LCD displays “SURGE ALERT!” followed by “SURGE DELAY.” After three seconds, the surge condition is cleared, and the relay can return to its normal state if no other fault remains.
The system uses a 16×2 I2C LCD to display real-time voltage input, current sensor input, and system status. In addition, the USB Serial Monitor enables computer-based monitoring and user configuration. Users can change the protection thresholds and enable or disable voltage and current protection by sending predefined commands. The Arduino continuously repeats the sensing, comparison, fault detection, and control process, providing automatic monitoring and protection against specified abnormal conditions. This project demonstrates the practical application of embedded systems, analog signal measurement, relay control, and real-time fault indication. The specified thresholds represent Arduino analog input voltages, so appropriate sensors, calibration, and electrical isolation are necessary before applying the system to real electrical equipment.






Conclusion :

The Arduino-Based Voltage and Current Protection System successfully demonstrates the concept of automatic electrical protection through continuous monitoring of voltage and current inputs. By using an Arduino Uno, voltage and current sensors, a relay module, a buzzer, an LED, and an LCD, the system can identify overvoltage, undervoltage, overcurrent, and specific types of sudden voltage surges. When a fault is detected, the Arduino sends a trip command to the relay and activates the necessary warning indicators to alert the user. The three-second surge delay feature provides additional monitoring during sudden voltage changes, while the LCD and Serial Monitor offer real-time information and flexible configuration of protection settings. This project provides practical experience in embedded system programming, sensor interfacing, fault detection, and automatic control. In the future, the system can be improved by incorporating calibrated voltage and current measurements, voltage recovery verification, fault history logging, and more advanced protection mechanisms. Overall, the project offers a useful educational prototype for understanding electrical monitoring and protection systems, although proper sensor calibration, electrical isolation, and hardware safety measures are essential before practical application to real electrical equipment.


