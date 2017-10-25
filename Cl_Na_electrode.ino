
/*
 * Cl_Na_electrode
 * AJW 2017, Cambridge
 * Requires the Grove shield and LCD system. And two boards
 * based on example sketch:
 * 
  Analog input, analog output, serial output
 Original header:
 Reads an analog input pin, maps the result to a range from 0 to 255
 and uses the result to set the pulsewidth modulation (PWM) of an output pin.
 Also prints the results to the serial monitor.

 The circuit:
 * potentiometer connected to analog pin 0.
   Center pin of the potentiometer goes to the analog pin.
   side pins of the potentiometer go to +5V and ground
 * LED connected from digital pin 9 to ground

 created 29 Dec. 2008
 modified 9 Apr 2012
 by Tom Igoe

 This example code is in the public domain.

 */
 //change these parameters
// value to determine the size of the readings array.
//set parameters
const int numReadings = 20;
//set Chloride parameters
const double sl = -5.04;
const double cp = 1200;
//set Sodium parameters
const double sls = -0.5;
const double cps = 1600;




#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;



const int analogInPin = A0;  // Analog input pins Cl-
const int analogInPin2 = A1; //Na+


int sensorValue = 0;        // value read from the pot
int sensorValue2 = 0;
int outputValue = 0;        // value output to the PWM (analog out)
int outputValue2 = 0;
int readings[numReadings];      // the readings from the analog input
int readings2[numReadings];
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int total2 = 0;
int average = 0;                // the average
int average2 = 0;

float af;
float af2;
double dispValue =0;
double dispValue2 =0;
double ppmValue =0;
double ppmValue2 =0;
void setup() {
  // initialize serial communications with computer at 9600 bps:
  Serial.begin(9600);
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
    readings2[thisReading] = 0;
    // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  
  // Print a message to the LCD.
  lcd.print("hello, world!");
  
  delay(1000);


    
  }
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  sensorValue2 = analogRead(analogInPin2);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 5000);
  outputValue2 = map(sensorValue2, 0, 1023, 0, 5000);

 
//moving average code
  // subtract the last reading:
  total = total - readings[readIndex];
  total2 = total2 - readings2[readIndex];
  // read from the sensor:
  readings[readIndex] = outputValue;
  readings2[readIndex] = outputValue2;
  // add the reading to the total:
  total = total + readings[readIndex];
  total2 = total2 + readings2[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  average2 = total2 / numReadings;
  af = (float) average;
  af2 = (float) average2;
  dispValue= af / float(1000);
  dispValue2 = af2/ float(1000);
  ppmValue = (-cp + af) / sl;
  ppmValue2 = (-cps +af2) / sls;

  average2 = total2 / numReadings;
  af2 = (float) average2;
  dispValue2= af2 / float(1000);
  ppmValue2 = (-cps + af2) / sls;

// end of moving average code


  // print the results to the serial monitor:

  // send it to the computer as ASCII digits

   if (readIndex == 0) {
    // ...only every 10:
  lcd.clear();
  Serial.print("\t mV = "); 
  Serial.print(average);
  Serial.print("\t");
  Serial.print(average2);
  Serial.print("\t\t ppm Cl = ");
  Serial.print(ppmValue, 0);
  Serial.print("\tNa = ");
  Serial.print(ppmValue2);
  lcd.print(ppmValue, 0);
  lcd.print(" ");
  lcd.print(ppmValue2, 0);
  Serial.println(" ");
   }
  // wait 50 milliseconds before the next loop
  // for the analog-to-digital converter to settle
  // after the last reading:
  delay(50);
}
