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

#include "include/AD5764.h"
#include "include/AD7734.h"
#include "include/dac_adc.h"
#include "include/utils.h"

AD5764 dac1(4, 52, 6, 1);
AD7734 adc1(52, 10, 48, 1);
DacAdc dac_adc(dac1, adc1);
//include "src/dac.h"
//Dac dac1(4, 4, 6);

void setup() {
  Serial.begin(115200);
  //dac1.Begin();
  //adc1.Begin();
  dac_adc.Begin();
  pinMode(44, OUTPUT);
  digitalWrite(44, HIGH);
}

void loop() {
  /* Serial.flush(); */
  /* if(Serial.available()) { */
  /*   String inbyte = ""; */
  /*   char resp; */
  /*   uint8_t dac_channels[] = {0,1}; */
  /*   uint8_t adc_channels[] = {0,1}; */
  /*   double start_voltages[] = {0, -5}; */
  /*   double end_voltages[] = {5, 0}; */
  /*   uint32_t n_steps = 5; */
  /*   uint32_t step_delay = 30; */

  /*   //float v; */
  /*   //float v2; */
  /*   resp = Serial.read(); */
  /*   inbyte += resp; */
  /*   //v = dac1.SetVoltage(0,inbyte.toFloat()); */
  /*   //v = adc1.ReadVoltage(inbyte.toInt()); */
  /*   dac_adc.BufferRamp(dac_channels, 2, */
  /* 		       adc_channels, 2, */
  /* 		       start_voltages, end_voltages, */
  /* 		       n_steps, step_delay); */
  /* } */
  if (Serial.available()) {
    String cmd[30] = "";
    uint8_t cmd_size;
    cmd_size = interface_utils::query_serial(cmd);
    dac_adc.Router(cmd, cmd_size);
  }
}
