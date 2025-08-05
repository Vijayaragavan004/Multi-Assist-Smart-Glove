#include <TTS.h>
TTS text2speech(25);  // ESP32 DAC PIN

void setup()
{ 
  pinMode(LED_BUILTIN, OUTPUT); 
}

void loop()
{
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

  text2speech.setPitch(2);
  text2speech.sayText("Hello");
  delay(1000);
  text2speech.sayText("My Name is This");
  delay(1000);
}  

