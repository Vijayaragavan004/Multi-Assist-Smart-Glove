#define flexPin 34  // Use the correct analog pin

const float referenceVoltage = 3.3;   // ESP32 max ADC voltage
const int adcResolution = 4095;       // 12-bit ADC

float baseVoltage = 0.0;
const float flexTolerance = 0.045;     // ±0.04V

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Take one-time base voltage
  int raw = analogRead(flexPin);
  baseVoltage = (raw * referenceVoltage) / adcResolution;

  Serial.print("Base voltage set at: ");
  Serial.println(baseVoltage, 3);
}

void loop() {
  int raw = analogRead(flexPin);
  float voltage = (raw * referenceVoltage) / adcResolution;

  Serial.print("Current voltage: ");
  Serial.print(voltage, 3);
  Serial.print(" V | ");

  if (voltage > baseVoltage + flexTolerance) {
    Serial.println("Gesture: Bent Forward");
  } else if (voltage < baseVoltage - flexTolerance) {
    Serial.println("Gesture: Bent Backward");
  } else {
    Serial.println("Gesture: None");
  }

  delay(500);
}