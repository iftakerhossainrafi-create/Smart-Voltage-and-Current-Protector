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


Working Principle :

The Arduino-Based Voltage and Current Protection System works by continuously monitoring the input voltage and current using an Arduino Uno microcontroller. The voltage sensor provides its signal to the A0 analog input pin, while the current sensor provides its signal to the A1 pin. The Arduino reads these analog signals and converts them into voltage values using its built-in Analog-to-Digital Converter (ADC). These values are then compared with predefined protection limits, where the high voltage limit is set to 4.0 V, the low voltage limit is set to 2.0 V, and the current input limit is set to 2.0 V.
When the input voltage exceeds the high limit, the system identifies an overvoltage condition. Similarly, when the voltage falls below the low limit, an undervoltage condition is detected. If the current sensor output exceeds the predefined current input limit, the system identifies an overcurrent condition. In any of these fault conditions, the Arduino sends a trip command to the relay module to disconnect the connected load. At the same time, the fault LED and buzzer turn on to alert the user, and the LCD displays the corresponding fault message.
The system also includes a sudden voltage surge detection feature. It continuously compares the current voltage reading with the previous reading. If the voltage increases by 0.5 V or more between consecutive readings while remaining within the predefined voltage limits, the Arduino identifies a potential voltage surge. The system then activates a three-second surge delay, during which the relay remains in the trip state. If no other voltage or current fault is present, the LED and buzzer remain off, while the LCD displays “SURGE ALERT!” followed by “SURGE DELAY.” After three seconds, the surge condition is cleared, and the relay can return to its normal state if no other fault remains.
The system uses a 16×2 I2C LCD to display real-time voltage input, current sensor input, and system status. In addition, the USB Serial Monitor enables computer-based monitoring and user configuration. Users can change the protection thresholds and enable or disable voltage and current protection by sending predefined commands. The Arduino continuously repeats the sensing, comparison, fault detection, and control process, providing automatic monitoring and protection against specified abnormal conditions. This project demonstrates the practical application of embedded systems, analog signal measurement, relay control, and real-time fault indication. The specified thresholds represent Arduino analog input voltages, so appropriate sensors, calibration, and electrical isolation are necessary before applying the system to real electrical equipment.






Conclusion :

The Arduino-Based Voltage and Current Protection System successfully demonstrates the concept of automatic electrical protection through continuous monitoring of voltage and current inputs. By using an Arduino Uno, voltage and current sensors, a relay module, a buzzer, an LED, and an LCD, the system can identify overvoltage, undervoltage, overcurrent, and specific types of sudden voltage surges. When a fault is detected, the Arduino sends a trip command to the relay and activates the necessary warning indicators to alert the user. The three-second surge delay feature provides additional monitoring during sudden voltage changes, while the LCD and Serial Monitor offer real-time information and flexible configuration of protection settings. This project provides practical experience in embedded system programming, sensor interfacing, fault detection, and automatic control. In the future, the system can be improved by incorporating calibrated voltage and current measurements, voltage recovery verification, fault history logging, and more advanced protection mechanisms. Overall, the project offers a useful educational prototype for understanding electrical monitoring and protection systems, although proper sensor calibration, electrical isolation, and hardware safety measures are essential before practical application to real electrical equipment.


