// Class for a DAC board
// Copyright (C) 2018 Sasha Zibrov
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

#ifndef DAC_h_
#define DAC_h_

#include "Arduino.h"
#include "SPI.h" 

typedef unsigned char byte;
 
class Dac {
 private:
  // sync_pin_ identifies the dac chip; each chip has a unique sync_pin_.
  // spi_bus_config_pin_ identifies the bus configurations.
  // More than one dac can share the same bus if they use the same spi configs.
  uint8_t sync_pin_, spi_bus_config_pin_, ldac_pin_, spi_mode_;  // See above
  uint8_t clock_divider_;  // For due frequency = 84 MHz / clock_divider_
  BitOrder bit_order_;  // MSBFIRST or LSBFIRST
 public:
  Dac(uint8_t sync_pin, uint8_t spi_bus_config_pin,
      uint8_t ldac_pin, uint8_t clock_divider=4,
      BitOrder bit_order=MSBFIRST, uint8_t spi_mode=SPI_MODE1);
  float SetVoltage(float voltage);
  void UpdateAnalogOutputs(void);
  bool Begin(void);  // SPI initialization
 protected:
  static const uint8_t kdata_len_ = 3;
  byte msg_[kdata_len_];  // Dynamic allocation of bytes to send to dac chip
  // Each DAC has its own convertion function. Check the datasheet of each chip
  // and override the function below accordingly.
  virtual void VoltageToBytes(float voltage) = 0;
  virtual float BytesToVoltage(byte msg[kdata_len_]) = 0;
};
#endif
