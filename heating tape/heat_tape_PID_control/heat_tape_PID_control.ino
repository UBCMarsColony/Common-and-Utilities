//Purpose of this code is to control the temperature of the heating tape with PID control and 
// the aid of a K-type thermocouple. 

#include "max6675.h"
#include "LiquidCrystal_I2C.h"
LiquidCrystal_I2C lcd(0x27,16,2);  //sometimes the adress is not 0x27. Change to 0x3f if it dosn't work.
/*    i2c LCD Module  ==>   Arduino
 *    SCL             ==>     20
 *    SDA             ==>     21
 *    Vcc             ==>     Vcc (5v)
 *    Gnd             ==>     Gnd      */


//Defining variables
float actualtemp = 0;
float thermocouple1offset = 2; //+2 deg. C offset
float templimit = 450;

//Relay Pin Definitions
int relaytrigger = 11;

//Thermocouple 1 Pin Definitions
int thermoDO1 = 3;            // SO pin on HW-550 board
int thermoCS1 = 4;            // CS pin (PWM)
int thermoCLK1 = 5;           // SCK pin (PWM)
int vccPin1 = 6;              // choose pin for vcc
int gndPin1 = 7;              // choose pin for gnd

MAX6675 thermocouple1(thermoCLK1, thermoCS1, thermoDO1);      //defining MAX6675

void setup() {
  Serial.begin(9600);

    // For I2C LCD
    lcd.init();      // Initialize the LCD
    lcd.backlight(); // Set the lcd background to light

    // For Relay
    pinMode(relaytrigger, OUTPUT);  

    // For thermocouple
    pinMode(vccPin1, OUTPUT); digitalWrite(vccPin1, HIGH); //set pin as a 5V vcc
    pinMode(gndPin1, OUTPUT); digitalWrite(gndPin1, LOW);  //set pin as a gnd

    // wait for MAX chip to stabilize
    delay(500);
}

void loop() {
  // basic readout test, just print the current temp
  
   Serial.print("Thermocouple 1 C = "); 
   Serial.println(thermocouple1.readCelsius());
   actualtemp = (thermocouple1.readCelsius()) - thermocouple1offset;
   Serial.print("\n Actual Temp C = ");
   Serial.println(actualtemp);
   if (actualtemp < templimit) //Temperature Comparison
   {
        digitalWrite(relaytrigger,HIGH);           // Turns ON Relay
        Serial.println("\n Relay closed");
   }
   else
   {
       digitalWrite(relaytrigger,LOW);           // Turns OFF Relay
       Serial.println("\n Relay open");
   }
   

  //lcd.clear();
  
  lcd.setCursor(0,0);
  lcd.print("PID TEMP control");
  lcd.setCursor(0,1);
  lcd.print("S:");
  lcd.setCursor(2,1);
  lcd.print(templimit,1);
  lcd.setCursor(9,1);
  lcd.print("R:");
  lcd.setCursor(11,1);
  lcd.print(actualtemp,1);
   
   
 
   delay(500);
}