// Class for a DAC_ADC
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

#include "../include/dac_adc.h"

DacAdc::DacAdc(Dac &dac, AdcSpi &adc)
    : dac_(dac), adc_(adc) {
}

bool DacAdc::Begin(void) {
  dac_.Begin();
  adc_.Begin();
  return true;
}

float DacAdc::SetVoltage(uint8_t channel, double voltage) {
  return dac_.SetVoltage(channel, voltage);
}

float DacAdc::ReadVoltage(uint8_t channel) {
  return adc_.ReadVoltage(channel);
}

bool DacAdc::BufferRamp(uint8_t dac_channels[], uint8_t n_dac_channels,
                        uint8_t adc_channels[], uint8_t n_adc_channels,
                        double start_voltages[], double end_voltages[],
                        uint32_t n_steps, uint32_t step_delay) {
  byte meas[10];  // Storage for measurements
  uint8_t n_meas;  // # of bytes of meas[] used by the adc to store a measurement
  // Buffer ramp starts here
  for (uint8_t step = 0; step < n_steps; step++) {
    // Writes the dac register of each channel in dac_channels[]
    for (uint8_t dac_channel_index = 0;
         dac_channel_index < n_dac_channels;
         dac_channel_index++) {
      uint8_t channel = dac_channels[dac_channel_index];
      double start_voltage = start_voltages[dac_channel_index];
      double end_voltage = end_voltages[dac_channel_index];
      // Linear ramp from start_voltage to end_voltage in n_steps
      double next_voltage = start_voltage +
                            (end_voltage - start_voltage) * step / (n_steps - 1);
      // false: Writes the dac register but does not update the analog output.
      dac_.SetVoltage(channel, next_voltage, false);
    }
    // Updates the analog outputs of all channels at the same time
    dac_.UpdateAnalogOutputs();
    // Waits for system to settle
    if (delay_unit_ == 0) {
      delayMicroseconds(step_delay);
    }
    else {
      delay(step_delay);
    }
    // Read the adc voltage 
    for (uint8_t adc_channel_index = 0;
         adc_channel_index < n_adc_channels;
         adc_channel_index++) {
      uint8_t channel = adc_channels[adc_channel_index];
      // The adc takes some time to convert the voltage to bytes.
      // While the adc is converting we send the previous measurement
      // to take advantage of the dead time.
      if (step > 0 || adc_channel_index > 0) {
	// Sends the previous measurement stored in meas.
        n_meas = adc_.ReadVoltage(channel, meas, true);
      }
      else {
	// First measurement, nothing to send so false
        n_meas = adc_.ReadVoltage(channel, meas, false);
      }
    }
  }
  // Sends the last measurement
  for (uint8_t meas_index = 0; meas_index < n_meas; meas_index++) {
    Serial.write(meas[meas_index]);
  }
  return true;
}
