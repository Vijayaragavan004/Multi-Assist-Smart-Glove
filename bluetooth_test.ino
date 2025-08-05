#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup()
{
  Serial.begin(115200);
  Serial.println("Booting...");
  if (!SerialBT.begin("SmartGlove"))
  {
    Serial.println("Bluetooth failed to start. Check wiring or power.");
    while (1);
  }
  Serial.println("Bluetooth started. Ready to pair.");
}

void loop()
{
  if (SerialBT.hasClient())
  {
    SerialBT.println("Hello from SmartGlove!");
    Serial.println("Sent: Hello from SmartGlove!");
  }
  delay(3000);
}
