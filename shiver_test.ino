#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>
#include <TTS.h>
#include <BluetoothSerial.h>

Adafruit_MPU6050 mpu;
LiquidCrystal_I2C lcd(0x27, 16, 2);
TTS tts(25);
BluetoothSerial SerialBT;

float previousAccel = -1;
unsigned long lastShiverCheck = 0;
const float shiverThreshold = 2.5;  // More sensitive now
const unsigned long shiverCooldown = 5000; // 5 seconds delay after shiver

unsigned long lastShiverDetected = 0;

void speakAndDisplay(const char *message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
  tts.setPitch(2);
  tts.sayText(message);
  SerialBT.println(message);
}

void checkShiver() {
  if (millis() - lastShiverCheck >= 500) {
    lastShiverCheck = millis();
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    float totalAccel = sqrt(
      a.acceleration.x * a.acceleration.x +
      a.acceleration.y * a.acceleration.y +
      a.acceleration.z * a.acceleration.z
    );

    if (previousAccel < 0) {
      previousAccel = totalAccel;  // Skip first invalid comparison
      return;
    }

    float diff = abs(totalAccel - previousAccel);
    previousAccel = totalAccel;

    if (diff >= shiverThreshold && (millis() - lastShiverDetected > shiverCooldown)) {
      speakAndDisplay("i am shivering");
      lastShiverDetected = millis();
    }
    else
    {
      speakAndDisplay("nothing");
    }
  }
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();
  tts.setPitch(2);
  SerialBT.begin("ShiverTest");

  if (!mpu.begin()) {
    lcd.print("MPU Error");
    while (1);
  }

  lcd.setCursor(0, 0);
  lcd.print("Shiver Test Ready");
  delay(1000);
}

void loop() {
  checkShiver();
}
