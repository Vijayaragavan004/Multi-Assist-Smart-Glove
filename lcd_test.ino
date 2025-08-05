#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  Wire.begin(21, 22);   // ESP32 default I2C pins: SDA = GPIO21, SCL = GPIO22

  Serial.begin(115200);  // Start serial communication

  lcd.init();            // Initialize the LCD
  lcd.backlight();       // Turn on the LCD backlight
}

void loop()
{
  lcd.setCursor(0, 0);
  lcd.print("ESP32 LCD Test");

  lcd.setCursor(0, 1);
  lcd.print("Hello, World!");

  Serial.println("LCD Test Running");

  delay(10000);  // 10-second delay

  //Ignore the warning message while uploading.
}