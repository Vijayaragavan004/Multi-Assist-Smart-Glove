#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>
#include <TTS.h>
#include <BluetoothSerial.h>

// Flex sensor pins
#define FLEX1_PIN 33
#define FLEX2_PIN 32
#define FLEX3_PIN 35
#define FLEX4_PIN 34

// Battery and SOS
#define BATTERY_PIN 36
#define SOS_PIN 14

// TTS and LCD
TTS tts(25);
LiquidCrystal_I2C lcd(0x27, 16, 2);
BluetoothSerial SerialBT;
Adafruit_MPU6050 mpu;

// Flex calibration
const float VCC = 3.3;
const int ADC_RES = 4095;
const float FIXED_RES = 22.0; // 22k

float flex_base_res[] = {14.9, 25.8, 13.0, 12.5}; // in kOhm for FLEX1 to FLEX4
float flex_base_volt[4];

// Battery
const float batteryLowThreshold = 3.3;
const float voltageDividerFactor = 2.0;
unsigned long lastBatteryCheck = 0;

// Shiver detection
float previousAccel = 0;
unsigned long lastShiverCheck = 0;

void speakAndDisplay(const char *message) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(message);
  tts.setPitch(3);
  tts.sayText(message);
  SerialBT.println(message);
}

float getVoltage(int pin) {
  return analogRead(pin) * (VCC / ADC_RES);
}

void checkFlexSensors() {
  float voltages[4];
  voltages[0] = getVoltage(FLEX1_PIN);
  voltages[1] = getVoltage(FLEX2_PIN);
  voltages[2] = getVoltage(FLEX3_PIN);
  voltages[3] = getVoltage(FLEX4_PIN);

  if (abs(voltages[0] - flex_base_volt[0]) > 1.0) {
    speakAndDisplay("I NEED WATER");
  } else if (abs(voltages[1] - flex_base_volt[1]) > 1.0) {
    speakAndDisplay("I AM HUNGRY");
  } else if (abs(voltages[2] - flex_base_volt[2]) > 1.0) {
    speakAndDisplay("WANT TO GO RESTROOM");
  } else if (abs(voltages[3] - flex_base_volt[3]) > 1.0) {
    speakAndDisplay("TAKE ME OUT");
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("NO GESTURES");
  }
}

void checkBattery() {
  if (millis() - lastBatteryCheck >= 20000) {
    lastBatteryCheck = millis();
    float rawVolt = getVoltage(BATTERY_PIN);
    float battVolt = rawVolt * voltageDividerFactor;
    float percentage = ((battVolt - 3.3) / (4.2 - 3.3)) * 100.0;
    percentage = constrain(percentage, 0, 100);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BATTERY: ");
    lcd.print((int)percentage);
    lcd.print("%");

    if (battVolt < batteryLowThreshold) {
      speakAndDisplay("BATTERY LOW");
    }
  }
}

void checkShiver() {
  if (millis() - lastShiverCheck >= 2000) {
    lastShiverCheck = millis();
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    float totalAccel = abs(a.acceleration.x) + abs(a.acceleration.y) + abs(a.acceleration.z);
    if (abs(totalAccel - previousAccel) > 10.0) {
      speakAndDisplay("SHIVER DETECTED");
    }
    previousAccel = totalAccel;
  }
}

void checkSOS() {
  if (digitalRead(SOS_PIN) == LOW) {
    speakAndDisplay("EMERGENCY HELP !");
    delay(3000);  // Debounce delay
  }
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("Smart Glove");
  lcd.init();
  lcd.backlight();

  pinMode(SOS_PIN, INPUT_PULLUP);

  if (!mpu.begin()) {
    lcd.print("MPU Error");
    while (1);
  }

  for (int i = 0; i < 4; i++) {
    flex_base_volt[i] = VCC * (FIXED_RES / (flex_base_res[i] + FIXED_RES));
  }
}

void loop() {
  checkFlexSensors();
  checkBattery();
  checkShiver();
  checkSOS();
  delay(200);
}
