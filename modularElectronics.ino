#include "src/AD5764.h"
AD5764 dac1(4, 4, 6, 1);
//#include "src/dac.h"
//Dac <3> dac1(4, 4, 6);

void setup() {
  Serial.begin(115200);
  Serial.write(2);
  dac1.Begin();
}

void loop() {
  Serial.flush();
  if(Serial.available()) {
    String inbyte = "";
    char resp;
    resp = Serial.read();
    inbyte += resp;
    dac1.UpdateAnalogOutputs();
    Serial.println(inbyte.toFloat());
    dac1.SetVoltage(5.5);
  }
}
