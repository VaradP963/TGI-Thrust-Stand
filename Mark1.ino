// TGI - THRUST STAND 
// PINOUTS:  


///////////////////////////////////// LIBRARIES //////////////////////////////////////////
#include <Wire.h>
#include <SPI.h>
#include <Servo.h>
#include <Adafruit_Sensor.h>
#include <HX711.h>
#include <RTClib.h>
#include <Adafruit_SSD1306.h>
#include <SD.h>

#define SD_CARD 4
#define FILE_NAME "TGI_Thrust_Stand.txt"

#define OLED_ADDR 0x3C
#define OLED_SDA 21
#define OLED_SCL 22
#define OLED_RST 16

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define SWITCH 12;
#define RELAY 13;
#define BUZZER 11;
#define currentPin 10;


int buttonPin = 35;

bool recording = false;

File myFile;
RTC_DS3231 rtc;

///////////////////////////////////////////////// Void Setup ///////////////////////////////////////////////////////////////////
void setup() {
Serial.begin(115200);

// Set pins as input and output
//  pinMode(SWITCH, INPUT);
//  pinMode(RELAY, OUTPUT);
//  pinMode(BUZZER, OUTPUT);
  
 // SETUP RTC MODULE
  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    while (1);
  }
  
// Initialize OLED display
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
  display.display();

// Initialize SD card
  if (!SD.begin(SD_CARD)) {
    Serial.println("Error initializing SD card!");
    while (1);}

}

/////////////////////////////////////////////////// All the required functions ///////////////////////////////////////////////////////
float readCurrent() {
  // Read raw value from current sensor
  int sensorValue = analogRead(currentPin);

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

float readThrust(){
  
  }

float readPotentiometer(){
  
  }

//////////////////////////////////////////// BLDC THRUST CALCULATION MODE: ////////////////////////////////////////////////
void BLDC_Motor(){
Serial.println("Selected BLDC Motor mode!");

// Read data from sensors
  float Thrust = readThrust();
  float Pot = readPotentiometer();
  float Current = readCurrent();
  float Voltage = getBatteryVoltage();
  
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
// open file for writing
  myFile = SD.open(FILE_NAME, FILE_WRITE);

  if (myFile) {
    Serial.println(F("Writing log to SD Card"));

    // write timestamp
    DateTime now = rtc.now();
    myFile.print(now.year(), DEC);
    myFile.print('-');
    myFile.print(now.month(), DEC);
    myFile.print('-');
    myFile.print(now.day(), DEC);
    myFile.print(' ');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);

    myFile.print("  "); // delimiter between timestamp and data

    // write data
    myFile.print("Thrust =");
    myFile.print(Thrust);
    myFile.print(", "); // delimiter between data 
    myFile.print("Potentiometer =");
    myFile.print(Pot);
    myFile.print(", "); // delimiter between data 
    myFile.print("Current = ");
    myFile.print(Current);
    myFile.print(", "); // delimiter between data
    myFile.print("Battery Voltage =");
    myFile.print(Voltage);

    myFile.write("\n"); // new line

    myFile.close();
  } 
  else {
    Serial.print(F("SD Card: error on opening file "));
    Serial.println(FILE_NAME);
  }
  delay(2000);  
  }
  }
}

//////////////////////////////////////////////// ROCKET MOTOR THRUST CALCULATION MODE/////////////////////////////////
void Rocket Motor(){
Serial.println("Selected Rocket Motor mode!");
}

////////////////////////////////////////////////// Void Loop ////////////////////////////////////////////////////////
void loop() {
Serial.println("Please select a mode:");

if(SWITCH == HIGH){
Rocket Motor();
}
else{
BLDC Motor();
}
}


#include <HX711.h> // library for interfacing with the load cell amplifier

const int DOUT = 2; // DOUT pin of the load cell amplifier
const int CLK = 3;  // CLK pin of the load cell amplifier

HX711 scale; // create an instance of the HX711 library

int motorPin = 9; // motor pin to control the BLDC motor

void setup() {
  scale.begin(DOUT, CLK); // initialize the load cell amplifier
  scale.set_scale(); // set the scale for the load cell
  scale.tare(); // tare the load cell to set the current reading as the zero point
  pinMode(motorPin, OUTPUT); // set the motor pin as an output pin
}

void loop() {
  int value = scale.get_units(); // get the current weight reading from the load cell
  int motorSpeed = map(value, 0, 1000, 0, 255); // map the weight reading to a value between 0 and 255

  // control the BLDC motor based on the weight reading
  analogWrite(motorPin, motorSpeed);

  delay(100); // wait for 100 milliseconds
}
