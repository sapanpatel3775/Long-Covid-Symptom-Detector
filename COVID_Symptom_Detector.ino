#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
MAX30105 particleSensor;
SSD1306AsciiWire oled;
#define MAX_BRIGHTNESS 255
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
uint16_t irBuffer[50];
uint16_t redBuffer[50];
#else
uint32_t irBuffer[50];
uint32_t redBuffer[50];
#endif

int32_t spo2;
int8_t validSPO2;
int32_t heartRate;
int8_t validHeartRate;
int onboardLED = 13;
int voltageThreshold = 400;
void setup() {
Serial.begin(115200);
pinMode(10, INPUT); 
pinMode(11, INPUT);
pinMode(onboardLED, OUTPUT);
oled.begin(&Adafruit128x64, 0x3C);
oled.setFont(Arial14);
particleSensor.setup(55, 4, 2, 200, 411, 4096);
}
 
void loop() {
int peakOne = 0;
int peakTwo = 0;
int peakThree = 0;
int peakFour = 0;
int peak = 900;
int distance = 0;
boolean ecgB = false;
boolean emgB = true;
boolean pulseOxB = false;

for (int i = 0; i <= 10000; i++) {
  if (i > 7500) {
    if (analogRead(A0) == peak) {
      if (peakOne == peakTwo && peakTwo == peakThree && peakThree == peakFour && peakOne == 0) {
        peakOne = i;
      } else if (peakOne != 0 && peakTwo == 0 && peakTwo == peakThree && peakThree == peakFour) {
        peakTwo = i;
      } else if (peakOne != 0 && peakTwo != 0 && peakThree == 0 && peakFour == 0) {
        peakThree = i;
      } else {
        peakFour = i;
      }
    }
  } else if (peakOne != 0 && peakTwo != 0 && peakThree != 0 && peakFour != 0) {
    break;
  }
}
distance = peakThree - peakOne;
if (distance > 1000 || distance < 100) {
  ecgB = true;
}
for (int i = 0; i <= 10000; i++) {
  int currentVoltage = analogRead(A1);
  if (currentVoltage > voltageThreshold){
    emgB = false;
  }
}
for (int i = 0 ; i < 50 ; i++)
  {
    while (particleSensor.available() == false)
      particleSensor.check();

    redBuffer[i] = particleSensor.getRed();
    irBuffer[i] = particleSensor.getIR();
    particleSensor.nextSample();
  }
maxim_heart_rate_and_oxygen_saturation(irBuffer, 50, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
  for (int i = 0; i <= 10000; i++)
  {
    for (byte i = 25; i < 50; i++)
    {
      redBuffer[i - 25] = redBuffer[i];
      irBuffer[i - 25] = irBuffer[i];
    }

    
    for (int i = 25; i < 50; i++)
    {
      while (particleSensor.available() == false)
        particleSensor.check();

      redBuffer[i] = particleSensor.getRed();
      irBuffer[i] = particleSensor.getIR();
      particleSensor.nextSample();
      Serial.print(F("red="));
      Serial.print(redBuffer[i], DEC);
      Serial.print(F(", ir="));
      Serial.print(irBuffer[i], DEC);

      Serial.print(F(", HR="));
      Serial.print(heartRate, DEC);

      Serial.print(F(", HRvalid="));
      Serial.print(validHeartRate, DEC);

      Serial.print(F(", SPO2="));
      Serial.print(spo2, DEC);

      Serial.print(F(", SPO2Valid="));
      Serial.println(validSPO2, DEC);
      
    }

    maxim_heart_rate_and_oxygen_saturation(irBuffer, 50, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
    printToScreen();
  }
maxim_heart_rate_and_oxygen_saturation(irBuffer, 50, redBuffer, &spo2, &validSPO2, &heartRate, &validHeartRate);
printToScreen();

int count = 0;
if (ecgB == true) {
  count++
}
if (emgB == true) {
  count++;
}
if (pulseOxB == true) {
  count++;
}
if (count >= 2) {

  pinMode(7, output);
  for (int  i = 0; i < 5000; i++) {
    digitalWrite(7, HIGH);
  }
  digitalWrite(7, LOW);
}

delay(1800000);
}
void printToScreen() {
  oled.clear();
  oled.setCursor(0,0);
  if(validSPO2 && validHeartRate) {
    oled.print(F("HR: ")); oled.println(heartRate, DEC);
    oled.print(F("SPO2: ")); oled.println(spo2, DEC);
  } else {
    oled.print(F("The HeartRate is not valid"));
  }
}
