#include <Arduino.h>
#include <pylontech.h>

Pylonclient pylon;
uint8_t major = 2;  // Set minimum protocol version.
uint8_t minor = 0;
uint8_t module = 0; // Pylontech nmdule address
char json_buf_PyAnalog[1024];

void setup() 
{
    Serial.begin(115200);
    Serial2.begin(9600, SERIAL_8N1);  // Init serial port with default pins 16 and 17 for RX and TX
    pylon.Begin(&Serial2);
}

void loop() 
{
  
  auto frame = pylon.SendCommand(Pylonframe(major, minor, module+2, CommandInformation::AnalogValueFixedPoint));
  if (frame.HasError)
  {
    Serial.print("AnalogValueFixedPoint failed for ");
    Serial.println(module);
    Serial.print("with code");
    Serial.println(frame.Cid2);
  }
  else
  {
    DynamicJsonDocument doc(1024);

    auto analog = Pylonframe::PylonAnalogValue(frame.Info);
    analog.json(doc,module);
    serializeJson(doc, json_buf_PyAnalog, 2048);
    Serial.println(json_buf_PyAnalog);
  }
  delay(1000);
}