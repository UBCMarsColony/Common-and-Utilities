
// Display for MFC GFC 17 code 


#include <Wire.h>
#include <LiquidCrystal_I2C.h>  /* Download this library! */
#include <Encoder.h> /* download this library from https://www.arduino.cc/reference/en/language/variables/data-types/string/functions/toint/
Also can find in vFile > Examples > Encoder > TwoKnobs.*/

#define MASSFLOW_PIN A1 
#define MASSFLOW_SETPOINT A6 // Setpoint to control  MFC
#define ENCODER_CLK 02 // Clk for rotary encoder
#define ENCODER_DT 03 // Data lina for rot. encder

const float VMAX = 1024.0; // Avoid rounding error opposed to int values

LiquidCrystal_I2C lcd(0x27, 16, 2); // initializes LCD display
Encoder knobLeft(2, 3); // initializes 1 rotary encoder

long positionLeft  = -999; // rotary limit
int setpoint = VMAX/2; /* setpoint initialized to half of Vmax for initial value.
                          // Convert to float next time
                          
                          */
      
void setup() {
    lcd.init();    
    lcd.backlight();

    // Serial Display for control input.  Press Ctrl+Shift+M after uploading for terminal
    Serial.begin(9600); 
    Serial.println("Rotary Encoder Position:");    

    /* UNCOMMENT ME FOR NEXT SESSION */
    //pinMode(MASSFLOW_SETPOINT, OUTPUT);
    //pinMode(ENCODER_CLK, INPUT); 
    //pinMode(ENCODER_DT, INPUT);
}


void loop() {
    lcd.clear();

    // LCD Display
    // Printing the analog flow rate of MFC on row 0
    float flow_analog = analogRead(MASSFLOW_PIN);
    lcd.setCursor(0,0);
    lcd.print("Input: ");
    lcd.print(flow_analog);    

    // Convert arduinos analog input to L/min
    float F_r = 2.0 * (flow_analog / 1024.0); // F_r = flow_rate
    lcd.setCursor(0,1);
    lcd.print("To L/min: ");
    lcd.print(F_r);
    delay(500); // delay is probably unnecessarily long.. could be as little as 30 ms... 
                // DONT DROP BELOW 10 ms or analogWrite() doesnt work well on pins 5/6 
     

  // Rotary Encoder knob code
  // Will display the encoder's position on serial display
  long newLeft, newRight;
  newLeft = knobLeft.read();
  if (newLeft != positionLeft) {
    Serial.print("Knob position = ");
    Serial.print(newLeft);
    Serial.println();
    positionLeft = newLeft;
  }

 // Serial port for USER INPUT. Can type a number up to magnitude 1E4 as integer
  // User enters integer to set as  setpoint
  if (Serial.available()) {
    String inString = ""; // string to hold the serial input

    // Read user input  
    int inChar = Serial.read();
    if (isDigit(inChar)){       // isDigit tests if myChar is numeric 0-9.
      inString += (char)inChar;
    }
    
    int setpoint = inString.toInt(); // CHANGE TO Get user input as FLOAT // put this into a separate function returning setpoint afterwards

    /* By now we have an analog value where 0-1024 -> 0-4.9V
       To write analog we have to convert to duty cycle 
      (Duty)_cycle (%), 0-255) -> (0%,0),(25%,64),(50%,127),(75%,191),(100%,355)
    */
    
    
    // float setpoint = parseFloat(inString) // Uncomment to convert string to float number
    Serial.print(setpoint); // will have to use another A pin as output to MFC. SEE BELOW
    //analogWrite(MASSFLOW_SETPOINT, X_a);  // NEXT SESSION DO THIS. X_a is 0-1024
  }

}
