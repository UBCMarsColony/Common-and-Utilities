int inPin = A0;    // input connected to analog pin 0
int val = 0;      // variable to store the read value

void setup() {
  Serial.begin(9600);

  //Use Arduino pins 
  //Using vcc pin on board, no need to set a pin high
  //Using ground pin on board, no need to set a pin low
  pinMode(inPin, INPUT);    // sets the digital pin 7 as input
}

void loop() {
  val = analogRead(inPin);   // read the input pin
  Serial.print("\n Pressure Voltaage = "); 
  Serial.println(val);
}