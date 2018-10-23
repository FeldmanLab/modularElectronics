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

#include "../include/AD5764.h"

AD5764::AD5764(uint8_t sync_pin, uint8_t spi_bus_config_pin,
	       uint8_t ldac_pin, uint8_t slot_number, double full_scale)
  :DacSpi(sync_pin, spi_bus_config_pin, ldac_pin, 16),
   slot_number_(slot_number), full_scale_(full_scale) {
}

double AD5764::BytesToVoltage(spi_utils::Message message) {
  byte byte1 = message.msg[1];
  byte byte2 = message.msg[2];
  // The conversion below is for two's complement
  uint16_t decimal = ((uint16_t)((byte1 << 8) | byte2));
  double voltage;
  if (decimal > 32767) {
    voltage = -(65536-decimal)*full_scale_/32768;
  } else {
    voltage = decimal*full_scale_/32767;
  }
  return voltage;
}

spi_utils::Message AD5764::SetVoltageMessage(uint8_t channel, double voltage) {
  uint16_t decimal;
  spi_utils::Message msg;
  msg.block_size = 3;
  msg.n_blocks = 1;
  // The conversion below is for two's complement
  if (voltage < 0) {
    decimal = voltage*32768/full_scale_ + 65536;
  } else {
    decimal = voltage*32767/full_scale_;
  }
  // Check datasheet for details
  msg.msg[0] = 0x10 + channel;  // Writes to dac register
  msg.msg[1] = ((byte)((decimal >> 0x8) & 0xFF));  // Writes first byte
  msg.msg[2] = ((byte)(decimal & 0xFF));  // Writes second byte
  return msg;
}
