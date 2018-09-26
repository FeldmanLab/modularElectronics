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

//#include "include/AD5764.h"
#include "include/tetra_dac.h"
#include "include/AD7734.h"
// #include "include/dac_adc.h"
#include "include/utils.h"

//AD5764 dac1(4, 52, 6, 1);
//include "src/dac.h"
//Dac dac1(4, 4, 6);

AD7734 adc(52, 10, 48, 1, 44);

uint8_t channels[] = {32, 30, 28, 26};
TetraDac dac(channels, 24, 52, 10, 34); 


String name_ = "DAC:TetraDAC_ADC:AD7734_2018_09_v0.1";

void setup() {
  Serial.begin(115200);
  // uint8_t channels[] = {32, 30, 28, 26};
  // TetraDac dac1(channels, 24, 52, 10, 34); 
  // AD7734 adc(52, 10, 48, 1, 44);
  // DacAdc dac_adc(dac1, adc1);
  dac.Begin();
  adc.Begin();
  dac.Initialize();
  dac.SetVoltage(0, 5);   
  // dac_adc.Begin();
}


uint8_t Router(String cmd[], uint8_t cmd_size) {
  String command = cmd[0];
  if (command == "NOP") {
    Serial.println("NOP");
    return 0;
  } else if (command == "*IDN?") {
    Serial.println(name_);
    return 0;
  } else if (command == "*RDY?") {
    Serial.println("READY");
    return 0;
  } else if (command == "ADCREAD"){
    double voltage;
    voltage = adc.ReadVoltage(cmd[1].toInt());
    Serial.println(voltage);
  } else if (command == "DACWRITE"){
    double voltage;
    voltage = dac.SetVoltage(cmd[1].toInt(), cmd[2].toFloat());
    Serial.println(voltage);
  }
  return 1; // Not found
}




void loop() {
  Serial.flush();
  // if(Serial.available()) {
  //   String inbyte = "";
  //   char resp;
  // /*   uint8_t dac_channels[] = {0,1}; */
  // /*   uint8_t adc_channels[] = {0,1}; */
  // /*   double start_voltages[] = {0, -5}; */
  //    double end_voltages[] = {5, 0}; 
  // /*   uint32_t n_steps = 5; */
  // /*   uint32_t step_delay = 30; */

  // /*   //float v; */
  // /*   //float v2; */
  //   resp = Serial.read();
  //   inbyte += resp;
  /*   //v = dac1.SetVoltage(0,inbyte.toFloat()); */
  /*   //v = adc1.ReadVoltage(inbyte.toInt()); */
  /*   dac_adc.BufferRamp(dac_channels, 2, */
  /* 		       adc_channels, 2, */
  /* 		       start_voltages, end_voltages, */
  /* 		       n_steps, step_delay); */
  /* } */
    //dac_adc.ReadVoltage(0);
    //dac_adc.Initialize();
    //dac_adc.SetVoltage(0, 5);
  // }
  
   if (Serial.available()) { 
     String cmd[30]; 
     uint8_t cmd_size = interface_utils::query_serial(cmd); 
     for(int i=0;i<cmd_size;i++){Serial.println(cmd[i]);}
     Router(cmd, cmd_size); 
   } 
}
