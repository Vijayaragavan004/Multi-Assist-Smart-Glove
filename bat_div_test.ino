#define batteryPin 36  // ADC-capable GPIO pin

const float referenceVoltage = 3.3;           // Measured 3.3V reference
const int adcResolution = 4095;                // 12-bit ADC
const float voltageDividerFactor = 2.0;        // 2x 10k resistors
const float batteryMinVoltage = 3.3;           // Voltage at 0%
const float batteryMaxVoltage = 4.1;           // Voltage at 100%

const int numSamples = 50;

void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Battery Monitor Started");
}

void loop() {
  long totalRaw = 0;
  for (int i = 0; i < numSamples; i++) {
    totalRaw += analogRead(batteryPin);
    delay(1);  // Slightly faster sampling
  }
  int avgRaw = totalRaw / numSamples;

  float pinVoltage = (avgRaw * referenceVoltage) / adcResolution;
  float batteryVoltage = pinVoltage * voltageDividerFactor;

  int batteryPercent = map(batteryVoltage * 100, batteryMinVoltage * 100, batteryMaxVoltage * 100, 0, 100);
  batteryPercent = constrain(batteryPercent, 0, 100);

  Serial.print("Battery Voltage: ");
  Serial.print(batteryVoltage, 2);
  Serial.print(" V | Battery Level: ");
  Serial.print(batteryPercent);
  Serial.println(" %");

  delay(5000);  // 10 seconds update
}
