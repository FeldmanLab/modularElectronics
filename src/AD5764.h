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

#ifndef AD5764_h_
#define AD5764_h_
#define AD5764_LEN 3

#include "dac.h"

class AD5764: public Dac <AD5764_LEN> {
 private:
  uint8_t channel_;
  double full_scale_;
 public:
  AD5764(uint8_t sync_pin, uint8_t spi_bus_config_pin,
         uint8_t ldac_pin, uint8_t channel, double full_scale=10.0);
  using Dac<AD5764_LEN>::VoltageToBytes;
  virtual void VoltageToBytes(float voltage) override {
    uint32_t decimal;
    if (voltage < 0) {
      decimal = voltage*32768/full_scale_ + 65536;
    } else {
      decimal = voltage*32767/full_scale_;
    }
    msg_[0] = 0x10 + channel_;
    msg_[1] = ((byte)((decimal >> 0x8) & 0xFF));
    msg_[2] = ((byte)(decimal & 0xFF));
  }
  using Dac<AD5764_LEN>::BytesToVoltage;
  virtual float BytesToVoltage(byte msg[AD5764_LEN]) override {
    uint32_t decimal = ((uint32_t)((msg[0] << 8) | msg[1]));
    double voltage;
    if (decimal > 32767) {
      voltage = -(65536-decimal)*full_scale_/32768;
    } else {
      voltage = decimal*full_scale_/32767;
    }
    return voltage;
  }
};

AD5764::AD5764(uint8_t sync_pin, uint8_t spi_bus_config_pin,
               uint8_t ldac_pin, uint8_t channel, double full_scale)
    :Dac(sync_pin, spi_bus_config_pin, ldac_pin),
     channel_(channel), full_scale_(full_scale) {
}
#endif
