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

Circuit Diagram : 

![image alt]{https://github.com/iftakerhossainrafi-create/Smart-Voltage-and-Current-Protector/blob/aa42040219b65db621279c128c2b8ba3ce93249a/Circuit%20Diagram%201.jpeg}
