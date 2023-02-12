
#include <Wire.h>
#include <SPI.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <HX711.h>
#include "RTClib.h"
#include <Adafruit_SSD1306.h>
#include <SD.h>

#define SWITCH 12;
#define RELAY 13;
#define BUZZER 11;

#define OLED_ADDR 0x3C
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST 16

Adafruit_SSD1306 display(OLED_ADDR, OLED_SDA, OLED_SCL, OLED_RST);

File dataFile;
int currentSensorPin = 34;
int buttonPin = 35;

bool recording = false;

void setup() {
Serial.begin(115200);

// Initialize OLED display
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.display();

// Initialize SD card
  if (!SD.begin(4)) {
    Serial.println("Error initializing SD card!");
    while (1);}

// Set pins as input and output
  pinMode(SWITCH, INPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(BUZZER, OUTPUT);

}

// All the required functions
float readCurrent() {
  // Read raw value from current sensor
  int sensorValue = analogRead(currentSensorPin);

  // Convert raw value to current (in amps)
  float current = (sensorValue - 512) * 0.0244;

  return current;
}

float getBatteryVoltage() {
  // Read raw value from voltage divider
  int sensorValue = analogRead(36);

  // Convert raw value to voltage
  float voltage = (sensorValue / 4095.0) * 3.3 * 2;

  return voltage;
}

void BLDC Motor(){
Serial.println("Selected BLDC Motor mode!");

// Read data from sensors
  float current = readCurrent();

  // Update OLED display with battery level and recording status
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Battery: ");
  display.print(getBatteryVoltage());
  display.print("V");
  if (recording) {
    display.setCursor(0, 16);
    display.print("Recording...");
  }
  display.display();

  // Check if button is pressed to toggle recording
  if (digitalRead(buttonPin) == LOW) {
    recording = !recording;
    delay(200);
  }

  if (recording) {
    // Write data to SD card
    dataFile = SD.open("data.csv", FILE_WRITE);
    
    dataFile.print(current1);
    dataFile.print(",");
    dataFile.close();
  }
}

void Rocket Motor(){
Serial.println("Selected Rocket Motor mode!");
}

void loop() {
Serial.println("Please select a mode:");

if(SWITCH == HIGH){
Rocket Motor();
}
else{
BLDC Motor();
}
}
