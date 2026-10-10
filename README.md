# Smart-Voltage-and-Current-Protector

Introduction :

Voltage variations and excessive current can damage electrical equipment and interrupt operation. A protection controller monitors electrical parameters and disconnects the load when readings leave permitted limits.
This project demonstrates the core control logic of such a controller using an Arduino Uno, an LCD, a relay module, tow potentiometer, a buzzer and a fault indicator. It is an educational and recruitment-project demonstration covering under-voltage, over-voltage and over-current detection, computer communication, configurable thresholds and delayed reconnection.


Circuit Diagram :

![image alt](https://github.com/iftakerhossainrafi-create/Smart-Voltage-and-Current-Protector/blob/ca8996d8f2d20323c8e389cb3fb051669016d35c/Circuit%20Diagram%201.jpeg)
![image alt](https://github.com/iftakerhossainrafi-create/Smart-Voltage-and-Current-Protector/blob/29b885737ab7fa513c6ca691c570d45ebfa7d756/Circuit%20Diagram%202.jpeg)


Working Principle :

The Arduino-Based Voltage and Current Protection System works by continuously monitoring the input voltage and current using an Arduino Uno microcontroller. The voltage potentiometter provides its signal to the A0 analog input pin, while the current potentiometter provides its signal to the A1 pin. The Arduino reads these analog signals and converts them into voltage values using its built-in Analog-to-Digital Converter (ADC). These values are then compared with predefined protection limits, where the high voltage limit is set to 4.0 V, the low voltage limit is set to 2.0 V and the current input limit is set to 2.0 V.
When the input voltage exceeds the high limit, the system identifies an overvoltage condition. Similarly, when the voltage falls below the low limit, an undervoltage condition is detected. If the current potentiometter output exceeds the predefined current input limit, the system identifies an overcurrent condition. In any of these fault conditions, the Arduino sends a trip command to the relay module to disconnect the connected load. At the same time, the fault LED and buzzer turn on to alert the user and the LCD displays the corresponding fault message.
The system also includes a sudden voltage surge detection feature. It continuously compares the current voltage reading with the previous reading. If the voltage increases by 0.5 V or more between consecutive readings while remaining within the predefined voltage limits, the Arduino identifies a potential voltage surge. The system then activates a three-second surge delay, during which the relay remains in the trip state. If no other voltage or current fault is present, the LED and buzzer remain off, while the LCD displays “SURGE ALERT!” followed by “SURGE DELAY.” After three seconds, the surge condition is cleared and the relay can return to its normal state if no other fault remains.
The system uses a 16×2 I2C LCD to display real-time voltage input, current input, and system status. In addition, the USB Serial Monitor enables computer-based monitoring and user configuration. Users can change the protection thresholds and enable or disable voltage and current protection by sending predefined commands. The Arduino continuously repeats the sensing, comparison, fault detection and control process, providing automatic monitoring and protection against specified abnormal conditions. This project demonstrates the practical application of embedded systems, analog signal measurement, relay control and real-time fault indication. The specified thresholds represent Arduino analog input voltages, so appropriate sensors, calibration and electrical isolation are necessary before applying the system to real electrical equipment.






Conclusion :

The Arduino-Based Voltage and Current Protection System successfully demonstrates the concept of automatic electrical protection through continuous monitoring of voltage and current inputs. By using an Arduino Uno, voltage and current potentiometter, a relay module, a buzzer, a LED, and a LCD, the system can identify overvoltage, undervoltage, overcurrent and specific types of sudden voltage surges. When a fault is detected, the Arduino sends a trip command to the relay and activates the necessary warning indicators to alert the user. The three-second surge delay feature provides additional monitoring during sudden voltage changes, while the LCD and Serial Monitor offer real-time information and flexible configuration of protection settings. This project provides practical experience in embedded system programming, fault detection and automatic control. In the future, the system can be improved by incorporating calibrated voltage and current measurements, voltage recovery verification, fault history logging and more advanced protection mechanisms. Overall, the project offers a useful educational prototype for understanding electrical monitoring and protection systems, although proper electrical isolation and hardware safety measures are essential before practical application to real electrical equipment.


