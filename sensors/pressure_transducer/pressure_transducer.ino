const int analogPin = A0; // input connected to analog pin 0
float inPin;        // variable to store the read value
float val = 0;      // variable to store the read value

void setup() {
  Serial.begin(9600);

  //Use Arduino pins 
  //Using vcc pin on board, no need to set a pin high
  //Using ground pin on board, no need to set a pin low

}

void loop() {
  inPin = analogRead(A0);   // read the input pin
  val = (((inPin - 101)*30)/(922-101));
  /* 
  1024 bits for arduino input, multiplication factor is 30 as transducer reads 0 to 30 PSI
  0.5 volts is an analog reading of 101  (rounded) for 0 PSI
  4.5 volts is an analog reading of 922  (rounded) for 30 PSI
  so the formula is Pressure (PSI) = ( Analog Reading - 101 ) * 30 /  ( 922 - 101 )
  */
  Serial.print("\n Pressure Value (PSI) = "); 
  Serial.println(val);
}