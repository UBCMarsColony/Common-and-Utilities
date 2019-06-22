// this example is public domain. enjoy!
// www.ladyada.net/learn/sensors/thermocouple

#include "max6675.h"

int thermoDO = 11;            //SO pin on HW-550 board
int thermoCS = 12;            //CS pin (PWM)
int thermoCLK = 13;           //SCK pin (PWM)

MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);      //Pin definitions
  
void setup() {
  Serial.begin(9600);
  // use Arduino pins 
  
  Serial.println("MAX6675 test");
  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp
  
   Serial.print("C = "); 
   Serial.println(thermocouple.readCelsius());
   Serial.print("F = ");
   Serial.println(thermocouple.readFahrenheit());
 
   delay(1000);
}
