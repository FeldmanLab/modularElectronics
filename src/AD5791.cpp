// Class for a Analog Devices AD5791 DAC
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

#include "../include/AD5791.h"

AD5791::AD5791(uint8_t sync_pin, uint8_t spi_bus_config_pin,
	       uint8_t ldac_pin, uint8_t slot_number, double full_scale)
  :DacSpi(sync_pin, spi_bus_config_pin, ldac_pin, 20),
   slot_number_(slot_number), full_scale_(full_scale) {
}

double AD5791::BytesToVoltage(spi_utils::Message message) {
  byte byte1 = message.msg[0];
  byte byte2 = message.msg[1];
  byte byte3 = message.msg[2];
  // The conversion below is for two's complement
  uint32_t decimal = ((uint32_t)(((((byte1 & 0x15) << 0x8) | byte2) << 0x8) | byte3));
  double voltage;
  if (decimal > 524287) {
    voltage = -(1048576-decimal)*full_scale_/524288;
  } else {
    voltage = decimal*full_scale_/524287;
  }
  return voltage;
}

spi_utils::Message AD5791::SetVoltageMessage(uint8_t channel, double voltage) {
  uint32_t decimal;
  spi_utils::Message msg;
  msg.block_size = 3;
  msg.n_blocks = 1;
  // The conversion below is for two's complement
  if (voltage < 0) {
    decimal = voltage*524288/full_scale_ + 1048576;
  } else {
    decimal = voltage*524287/full_scale_;
  }
  // Check datasheet for details
  msg.msg[0] = (byte)((decimal >> 0x10) | 0x10);  // Writes to dac register
  msg.msg[1] = ((byte)((decimal >> 0x8) & 0xFF));  // Writes first byte
  msg.msg[2] = ((byte)(decimal & 0xFF));  // Writes second byte
  return msg;
}

spi_utils::Message AD5791::InitializeMessage(void) {
  spi_utils::Message msg;
  msg.block_size = 3;
  msg.n_blocks = 1;
  msg.msg[0] = 0x20;
  msg.msg[1] = 0x0;
  msg.msg[2] = 0x2;
  return msg;
}
