#include <Wire.h>
#include <LiquidCrystal_I2C.h>  /* Download this library! */

#define MASSFLOW_PIN A1

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
    lcd.init();
    lcd.backlight();
}

void loop() {
    lcd.clear();

    float flow_analog = analogRead(MASSFLOW_PIN);
    lcd.setCursor(0,0);
    lcd.print("Input: ");
    lcd.print(flow_analog);

    float flow = 2.0 * (flow_analog / 1024.0);
    lcd.setCursor(0,1);
    lcd.print("To L/min: ");
    lcd.print(flow);

    delay(1000);
}
