// AC Controllor
//
// This Arduino sketch is for use with the heater 
// control circuit board which includes a zero 
// crossing detect function and an opto-isolated TRIAC.
//
// AC Phase control is accomplished using the internal 
// hardware timer1 in the Arduino
//
// Timing Sequence
// * timer is set up but disabled
// * zero crossing detected on pin 2
// * timer starts counting from zero
// * comparator set to "delay to on" value
// * counter reaches comparator value
// * comparator ISR turns on TRIAC gate
// * counter set to overflow - pulse width
// * counter reaches overflow
// * overflow ISR turns off TRIAC gate
// * TRIAC stops conducting at next zero cross


// The hardware timer runs at 16MHz. Using a
// divide by 256 on the counter each count is 
// 16 microseconds.  1/2 wave of a 60Hz AC signal
// is about 520 counts (8,333 microseconds).

/*    Max6675 Module  ==>   Arduino
      CS              ==>     D10
      SO              ==>     D9
      SCK             ==>     D13
      Vcc             ==>     Vcc (5v)
      Gnd             ==>     Gnd      */

//LCD config
#include "max6675.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); //sometimes the adress is not 0x27. Change to 0x3f if it dosn't work.


#include <avr/io.h>
#include <avr/interrupt.h>

//Pins for zero dectection and pulse firing pin for the TRIAC
#define DETECT 2  //zero cross detect
#define GATE 3    //TRIAC gate
#define PULSE 4   //trigger pulse width (counts)
//knob pins
#define Increment 11 //knob incresment pin, not actuall increment is pin A for the knob
#define Decrement 12 //knob decement pin, pin B 
#define KnobSwitch 13 //Switch for the knob
//thermocoupler pins
#define thermoDO 9
#define thermoCS 10
#define thermoCLK 7

//Timer 1 comparetor value, used to control the power of output or AC circuit
int i=440;//0 is the highest, 440 is set to be the default lowest, 450 could cause unpredicted pulse, need to recheck

//Switch variables for knob
int pinALast;//pin A last poistion
int aVal; 
bool switchBool; // state boolean for knob

int temp_read_Delay = 100; // tempeture reading for thermocoupler 
int real_temperature = 0; 
unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

//Start a MAX6675 communication with the selected pins
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup(){

  // set up pins
  pinMode(DETECT, INPUT);     //zero cross detect
  digitalWrite(DETECT, HIGH); //enable pull-up resistor
  pinMode(GATE, OUTPUT);      //TRIAC gate control

  // set up Timer1 
  //(see ATMEGA 328 data sheet pg 134 for more details)
  OCR1A = 100;      //initialize the comparator
  TIMSK1 = 0x03;    //enable comparator A and overflow interrupts
  TCCR1A = 0x00;    //timer control registers set for
  TCCR1B = 0x00;    //normal operation, timer disabled
  
  lcd.init();       //Start the LC communication
  lcd.backlight();  //Turn on backlight for LCD
  
  Serial.begin(9600);

  // set up zero crossing interrupt
  attachInterrupt(0,zeroCrossingInterrupt, RISING);    
    //IRQ0 is pin 2. Call zeroCrossingInterrupt 
    //on rising signal

  pinALast = digitalRead(Increment);  //read initial position for increase_pin
  switchBool = false;
}  

//Interrupt Service Routines

void zeroCrossingInterrupt(){ //zero cross detect   
  TCCR1B=0x04; //start timer with divide by 256 input
  TCNT1 = 0;   //reset timer - count from zero
}

ISR(TIMER1_COMPA_vect){ //comparator match
  digitalWrite(GATE,HIGH);  //set TRIAC gate to high
  TCNT1 = 65536-PULSE;      //trigger pulse width
}

ISR(TIMER1_OVF_vect){ //timer1 overflow
  digitalWrite(GATE,LOW); //turn off TRIAC gate
  TCCR1B = 0x00;          //disable timer stopd unintended triggers
}

void loop(){ // sample code to exercise the circuit

/*Serial.print("loop");
i--;
OCR1A = i;     //set the compare register brightness desired.
if (i<65){i=500;}                      
delay(30);                             
*/

currentMillis = millis();           //Save the value of time before the loop
  /*  We create this if so we will read the temperature and change values each "temp_read_Delay"
      value. Change that value above iv you want. The MAX6675 read is slow. Tha will affect the
      PID control. I've tried reading the temp each 100ms but it didn't work. With 500ms worked ok.*/
  if (currentMillis - previousMillis >= temp_read_Delay) {
    previousMillis += temp_read_Delay;              //Increase the previous time for next loop  
    //thermo_T = micros();
    real_temperature = thermocouple.readCelsius();  //get the real temperature in Celsius degrees
    //thermo_T = micros()-thermo_T;
    
    //Printe the values on the LCD every 2 second
    
      //LCD_T = micros();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Set: ");
      lcd.setCursor(5, 0);
      lcd.print(i);
      lcd.setCursor(0, 1);
      lcd.print("Real temp: ");
      lcd.setCursor(11, 1);
      lcd.print(real_temperature);
  }

  OCR1A = i;

   if (switchBool) {
    aVal = digitalRead(Increment);
    if (aVal != pinALast) { //Means the knob is rotating
      //If the knob is rotating, we need to determine direction
      //We do that by reading decrease_pin
      if (digitalRead(Decrement) != aVal) {  //Means increase changed first - We're Rotating Clockwise
        if(i<440)
        {i+=5;}
      } else {  //Otherwise decrease changed first and we're moving CCW
        if(i>1)
        {i-=5;}
      }
    }
    pinALast = aVal;
  }
  
  if (switchBool & digitalRead(KnobSwitch) == LOW) {
    switchBool = false;
  }
  else if (!switchBool & digitalRead(KnobSwitch) == LOW) {
    switchBool = true;
  }
  while(digitalRead(KnobSwitch) == LOW) {}
  
}
