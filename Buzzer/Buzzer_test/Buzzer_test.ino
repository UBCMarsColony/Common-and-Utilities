/* Buzzer Test Code
 * input via terminal for a different sound on the buzzer
 */
 //----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int speaker = 8; //Speaker Pin
int input = 0; //Input delay 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(speaker, OUTPUT); //Speaker Pin setup
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){
        input = Serial.read(); //Read from Serial Monitor input and set input = to that. 
  }
        digitalWrite(speaker,HIGH); //Simulated PWM
        delay(input); //Input delay
        digitalWrite(speaker,LOW);
        delay(input);
}
