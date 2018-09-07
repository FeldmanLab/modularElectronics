// Class for tetraDac
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

#include "../include/tetra_dac.h"

TetraDac::TetraDac(uint8_t dac_sync_pins[n_channels_], uint8_t eeprom_sync_pin,
                   uint8_t spi_bus_config_pin, uint8_t eeprom_spi_bus_config_pin,
                   uint8_t ldac_pin, double full_scale,
                   uint8_t slot_number) {
  for (uint8_t channel = 0; channel < n_channels_; channel++) {
    dacs_[channel] = AD5791(dac_sync_pins[channel], spi_bus_config_pin,
                            ldac_pin, slot_number, full_scale);
  }
  eeprom = EepromSpi(eeprom_sync_pin, eeprom_spi_bus_config_pin);
}

uint8_t TetraDac::Begin(void) {
  for (uint8_t channel = 0; channel < n_channels_; channel++) {
    dacs_[channel].Begin();
  }
  eeprom.Begin();
  return 0;
}

uint8_t TetraDac::Initialize(void) {
  for (uint8_t channel = 0; channel < n_channels_; channel++) {
    dacs_[channel].Initialize();
  }
  return 0;
}

uint8_t TetraDac::SetVoltage(uint8_t channel, double voltage,
                             bool update_outputs) {
  dacs_[channel].SetVoltage(0, voltage, update_outputs);
  return 0;
}

uint8_t TetraDac::GetVoltage(uint8_t channel) {
  dacs_[channel].GetVoltage(0);
  return 0;
}

void TetraDac::UpdateAnalogOutputs(void) {
  dacs_[0].UpdateAnalogOutputs();
}
