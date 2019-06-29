//Purpose of this code is to record temperatures from 2 thermocouples at the same time

#include "max6675.h"
//Thermocouple 1
int thermoDO1 = 10;            //SO pin on HW-550 board
int thermoCS1 = 11;            //CS pin (PWM)
int thermoCLK1 = 12;           //SCK pin (PWM)

//Thermocouple 2
int thermoDO2 = 3;            //SO pin on HW-550 board
int thermoCS2 = 4;            //CS pin (PWM)
//No initialization of thermocouple 2 clk because using common clk

MAX6675 thermocouple1(thermoCLK1, thermoCS1, thermoDO1);      //Pin definitions
int vccPin1 = 13;                // choose pin for vcc

MAX6675 thermocouple2(thermoCLK1, thermoCS2, thermoDO2);      //Common clock for both thermocouples
int vccPin2 = 6;                // choose pin for vcc
int gndPin2 = 7;                // choose pin for gnd

void setup() {
  Serial.begin(9600);
  // use Arduino pins 
  pinMode(vccPin1, OUTPUT); digitalWrite(vccPin1, HIGH); //set pin as a 5V vcc
  //Using ground pin on board, no need to set a pin low
  
  pinMode(vccPin2, OUTPUT); digitalWrite(vccPin2, HIGH); //set pin as a 5V vcc
  pinMode(gndPin2, OUTPUT); digitalWrite(gndPin2, LOW);  //set pin as a gnd
  
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp
  
   Serial.print("Thermocouple 1 C = "); 
   Serial.println(thermocouple1.readCelsius());
   Serial.print("Thermocouple 2 C = ");
   Serial.println(thermocouple2.readCelsius());
 
   delay(1000);
}