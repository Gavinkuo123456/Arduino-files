#include <IRremote.hpp>
bool light = 0;
void setup()
{
  Serial.begin(115200);
  IrReceiver.begin(2, DISABLE_LED_FEEDBACK); // Start the receiver
  Serial.println("Start IR");
  pinMode(13,OUTPUT);
}

void loop() {
  if (IrReceiver.decode()) {
      Serial.println(IrReceiver.decodedIRData.decodedRawData,HEX); // Print "old" raw data
      if(IrReceiver.decodedIRData.decodedRawData == 0xD8D5DD65){
        light = !light;
        digitalWrite(13,light);
        Serial.println("light switch");
      }
      IrReceiver.resume(); // Enable receiving of the next value
      Serial.println();
  }
}
