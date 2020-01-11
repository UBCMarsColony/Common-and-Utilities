/*
 * PIR sensor tester
 * +
 * MAX6675 K-Type Thermocouple tester
 */

#include "max6675.h"            // include MAX6675 adafruit library

int thermoSO = 7;               // SO Pin select
int thermoCS = 6;               // CS Pin select
int thermoCLK = 5;              // SCK Pin select

int ledPin = 13;                // choose the pin for the LED
int inputPin = 9;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

MAX6675 thermocouple(thermoCLK, thermoCS, thermoSO);
int vccPin1 = 4;                // choose pin for vcc
int vccPin2 = 10;               // choose pin for vcc
int gndPin1 = 3;                // choose pin for gnd
int gndPin2 = 8;                // choose pin for gnd
  
void setup() {
  Serial.begin(9600);
  // use Arduino pins 
  pinMode(vccPin1, OUTPUT); digitalWrite(vccPin1, HIGH); //set pin as a 5V vcc
  pinMode(gndPin1, OUTPUT); digitalWrite(gndPin1, LOW);  //set pin as a gnd
  pinMode(vccPin2, OUTPUT); digitalWrite(vccPin2, HIGH); //set pin as a 5V vcc
  pinMode(gndPin2, OUTPUT); digitalWrite(gndPin2, LOW);  //set pin as a gnd
  pinMode(ledPin, OUTPUT);      // declare LED as output
  pinMode(inputPin, INPUT);     // declare sensor as input
  
  // Serial.println("MAX6675 test");
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
  
   val = digitalRead(inputPin);  // read input value
   if (val == HIGH) {            // check if the input is HIGH
    digitalWrite(ledPin, HIGH);  // turn LED ON
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Motion detected!");
      // we only want to print on the output change, not state
      pirState = HIGH;
      
      // basic readout test, just print the current temp
      Serial.print("C = "); 
      Serial.println(thermocouple.readCelsius());
      Serial.print("F = ");
      Serial.println(thermocouple.readFahrenheit());
      delay(100);
    }
   } 
   else {
    digitalWrite(ledPin, LOW); // turn LED OFF
    if (pirState == HIGH){
      // we have just turned off
      Serial.println("Motion ended!");
      // we only want to print on the output change, not state
      pirState = LOW;
    }
  }
}