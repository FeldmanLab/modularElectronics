// Class for a Analog Devices AD5764 DAC
// Copyright (C) 2018 Carlos Kometter 
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// #include "include/AD5764.h"
#include "include/AD7734.h"

// AD5764 dac1(4, 52, 6, 1);
AD7734 adc1(52, 4, 48, 1);
//include "src/dac.h"
//Dac dac1(4, 4, 6);

void setup() {
  Serial.begin(115200);
  //dac1.Begin();
  adc1.Begin();
  pinMode(44, OUTPUT);
  digitalWrite(44, HIGH);
}

void loop() {
  Serial.flush();
  if(Serial.available()) {
    String inbyte = "";
    char resp;
    float v;
    resp = Serial.read();
    inbyte += resp;
    //v = dac1.SetVoltage(0,inbyte.toFloat());
    v = adc1.ReadVoltage(inbyte.toInt());
    Serial.println(v);
  }
}
