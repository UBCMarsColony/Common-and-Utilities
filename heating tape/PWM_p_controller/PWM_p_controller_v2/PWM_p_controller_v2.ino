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

//Switch variables for knob
int pinALast;    //pin A last poistion
int aVal;
bool switchBool; // state boolean for knob
int i = 50;      // used to adjust setpoint

//Other variables for main loop;
const int temp_read_Delay = 200;
const int LCD_display_Delay = 300;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long currentMillis = 0;
float real_temperature = 0;
float previous_temperature = 0;

//Controller Variable Declear
float setpoint = 50;  //desired temputure
//int k = 7;          //P
//float PWM = 0;
float Error = 0;      //Timer 1 comparetor value, used to control the power of output or AC circuit
float previous_error = 0;
int Output = 440;     //0 is the highest, 440 is set to be the default lowest, 450 could cause unpredicted pulse, need to recheck

float PID_p = 0;
float PID_i = 0;
float PID_d = 0;
float kp = 7;
float ki = 1.5;
float kd = 0;
float PID = 0;

//Start a MAX6675 communication with the selected pins
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

void setup() {

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
  attachInterrupt(0, zeroCrossingInterrupt, RISING);
  //IRQ0 is pin 2. Call zeroCrossingInterrupt
  //on rising signal

  pinALast = digitalRead(Increment);  //read initial position for increase_pin
  switchBool = false;
}

//Interrupt Service Routines

void zeroCrossingInterrupt() { //zero cross detect
  TCCR1B = 0x04; //start timer with divide by 256 input
  TCNT1 = 0;   //reset timer - count from zero
}

ISR(TIMER1_COMPA_vect) { //comparator match
  digitalWrite(GATE, HIGH); //set TRIAC gate to high
  TCNT1 = 65536 - PULSE;    //trigger pulse width
}

ISR(TIMER1_OVF_vect) { //timer1 overflow
  digitalWrite(GATE, LOW); //turn off TRIAC gate
  TCCR1B = 0x00;          //disable timer stopd unintended triggers
}




void loop() {
  /*  We create this if so we will read the temperature and change values each "temp_read_Delay"
      value. Change that value above iv you want. The MAX6675 read is slow. Tha will affect the
      PID control. I've tried reading the temp each 100ms but it didn't work. With 500ms worked ok.*/

  //Thermocouple Reading*************************************************************************************************************************************
  if (millis() - previousMillis >= temp_read_Delay) {
    previous_temperature = real_temperature;
    real_temperature = thermocouple.readCelsius();  //get the real temperature in Celsius degrees
    previousMillis = millis();
  }


  //Controller test********************************************************************************************************************************************
  setpoint = i;
  previous_error = Error;
  Error = setpoint - real_temperature;

  PID_p = kp * Error;                                                         //Calculate the P value
  PID_i = PID_i + (ki * Error);                                               //Calculate the I value
  PID_d = kd * ((Error - previous_error) / (temp_read_Delay / 1000.0));       //Calculate the D value

  if (PID_i + PID_d + PID_p > 100) {                                          //Prevent PID_i from going above 100, this prevents overshoot.
    if (Error > 0)
      PID_i = PID_i - (ki * Error);
  }

  if (PID_i + PID_d + PID_p < 0) {                                            //Prevent PID_i from going below 0, this prevents undershoot. 
    if (Error < 0)
      PID_i = PID_i - (ki * Error);
  }

  PID = PID_p + PID_i + PID_d;                                                //Calculate total PID value
 // Serial.print(PID_i);
 // Serial.print("  ");
 // Serial.println(PID);

  //PWM = k*Error+30;

  if (PID > 100) {
    PID = 100;
  }

  if (PID < 0) {
    PID = 0;
  }
  PID = PID / 100;
  Output = 440 - (440 * PID);
  if (Output > 440) {
    Output = 440;
  }
  if (Output < 1) {
    Output = 1;
  }
  OCR1A = Output;


  //LCD_display*************************************************************************************************************************************************
  if ( millis() - previousMillis2 >= LCD_display_Delay) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Set: ");
    lcd.setCursor(5, 0);
    lcd.print(setpoint);
    lcd.setCursor(0, 1);
    lcd.print("Real temp: ");
    lcd.setCursor(11, 1);
    lcd.print(real_temperature, 2);
    previousMillis2 = millis();
  }


  //knob adjust setpoint*****************************************************************************************************************************************
  if (switchBool) {
    aVal = digitalRead(Increment);
    if (aVal != pinALast) { //Means the knob is rotating
      //If the knob is rotating, we need to determine direction
      //We do that by reading decrease_pin
      if (digitalRead(Decrement) != aVal) {  //Means increase changed first - We're Rotating Clockwise
        if (i < 440)
        {
          i++;
        }
      } else {  //Otherwise decrease changed first and we're moving CCW
        if (i > 6)
        {
          i--;
        }
      }
    }
    pinALast = aVal;
  }


  //Press knob button*********************************************************************************************************************************************
  if (switchBool & digitalRead(KnobSwitch) == LOW) {
    switchBool = false;
  }
  else if (!switchBool & digitalRead(KnobSwitch) == LOW) {
    switchBool = true;
  }
  while (digitalRead(KnobSwitch) == LOW) {}

}//end of main loop
