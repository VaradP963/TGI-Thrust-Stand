# TGI-Thrust-Stand
The two in one Thrust Stand features both, thrust calculation of BLDC as well as Rocket Motors.

BLDC Motor Mode: The thrust, current drawn, potentiometer readings, input voltage will be displayed on an oled display.

Rocket Motor Mode: After pressing the switch, and after a safe delay, the rocket motor will be ignited. The thrust readings will be displayed on the oled display.

This repository contains the code for a BLDC (Brushless Direct Current) motor thrust stand using an Arduino microcontroller and a load cell for measuring the thrust produced by the motor. The code uses the HX711 library for communication with the load cell and the Servo library for controlling the motor.

The thrust stand is designed to test the performance of BLDC motors by measuring the thrust produced by the motor at different voltage and current inputs. The code allows for easy adjustment of these inputs, as well as the conversion factor for the load cell readings.

The code uses the setup function to initialize the Serial communication, the Servo library and the HX711 library, and the loop function to repeatedly read the load cell data, control the motor and output the thrust readings to the Serial monitor.
