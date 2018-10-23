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

#include "include/tetra_dac.h"
#include "include/AD7734.h"
#include "include/utils.h"

AD7734 adc(52, 10, 48, 1, 44);
uint8_t channels[] = {32, 30, 28, 26};
TetraDac dac(channels, 24, 52, 10, 34); 


String name_ = "DAC:TetraDAC_ADC:AD7734_2018_09_v0.1";

void setup() {
  Serial.begin(115200);
  dac.Begin();
  adc.Begin();
  dac.Initialize();
  dac.SetVoltage(0, 5);   
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
  } else if (command == "BUFFERRAMP"){
    uint8_t dac_channels[10];
    uint8_t n_dac_channels = interface_utils::string_to_int_array(cmd[1], dac_channels);
    uint8_t adc_channels[10];
    uint8_t n_adc_channels = interface_utils::string_to_int_array(cmd[2], adc_channels);
    double start_voltages[10];
    double end_voltages[10];
    for (uint8_t index = 0; index  < n_dac_channels; ++index) {
      start_voltages[index] = cmd[index +3].toFloat();
      end_voltages[index] = cmd[index + n_dac_channels + 3].toFloat();
    }
    uint32_t n_steps = cmd[n_dac_channels*2 + 3].toInt();
    uint32_t step_delay = cmd[n_dac_channels*2 + 4].toInt();
    meas_utils::BufferRamp<TetraDac, AD7734>(dac, adc, dac_channels, n_dac_channels,
         adc_channels, n_adc_channels,
         start_voltages, end_voltages,
         n_steps, step_delay);
  }
  return 1; // Not found
}




void loop() {
  Serial.flush();
   if (Serial.available()) { 
     String cmd[30]; 
     uint8_t cmd_size = interface_utils::query_serial(cmd); 
     for(int i=0;i<cmd_size;i++){Serial.println(cmd[i]);}
     Router(cmd, cmd_size); 
   } 
}
