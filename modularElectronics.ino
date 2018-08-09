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
