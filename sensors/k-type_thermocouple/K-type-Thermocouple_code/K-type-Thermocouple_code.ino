//Purpose of this code is to record temperatures from 2 thermocouples at the same time

#include "max6675.h"
//Thermocouple 1
int thermoDO1 = 7;            //SO pin on HW-550 board
int thermoCS1 = 4;            //CS pin (PWM)
int thermoCLK1 = 13;          //SCK pin (PWM)

//Thermocouple 2
int thermoDO2 = 6;            //SO pin on HW-550 board
int thermoCS2 = 3;            //CS pin (PWM)
//No initialization of thermocouple 2 clk because using common clk

//Thermocouple 3
int thermoDO3 = 5;            //SO pin on HW-550 board
int thermoCS3 = 2;            //CS pin (PWM)
//No initialization of thermocouple 3 clk because using common clk

MAX6675 thermocouple1(thermoCLK1, thermoCS1, thermoDO1);      //Pin definitions

MAX6675 thermocouple2(thermoCLK1, thermoCS2, thermoDO2);      //Common clock for both thermocouples

MAX6675 thermocouple3(thermoCLK1, thermoCS3, thermoDO3);      //Common clock for both thermocouples

void setup() {
  Serial.begin(9600);
  //Use Arduino pins 
  //Using vcc pin on board, no need to set a pin high
  //Using ground pin on board, no need to set a pin low

  // wait for MAX chip to stabilize
  delay(500);
}

void loop() {
  // basic readout test, just print the current temp
  
   Serial.print("\n Thermocouple 1 C = "); 
   Serial.println(thermocouple1.readCelsius());
   Serial.print("\n Thermocouple 2 C = ");
   Serial.println(thermocouple2.readCelsius());
   Serial.print("\n Thermocouple 3 C = ");
   Serial.println(thermocouple3.readCelsius());
 
   delay(1000);
}