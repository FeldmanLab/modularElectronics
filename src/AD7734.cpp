// Class for a Analog Devices AD7734 ADC
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

#include "../include/AD7734.h"

AD7734::AD7734(uint8_t sync_pin, uint8_t spi_bus_config_pin,
	       uint8_t data_ready_pin, uint8_t slot_number,
	       uint8_t reset_pin)
  : AdcSpi(sync_pin, spi_bus_config_pin,
	   data_ready_pin, 16, reset_pin),
    slot_number_(slot_number) {
}

double AD7734::BytesToVoltage(spi_utils::Message message) {
  byte byte1 = message.msg[2];
  byte byte2 = message.msg[3];

  uint16_t decimal = ((uint16_t)((byte1<<8)| byte2));

  return decimal * 20.0 / 65536 - 10.0;
}

spi_utils::Message AD7734::SingleConversionModeMessage(uint8_t channel) {
  spi_utils::Message msg;
  msg.block_size = 1;
  msg.n_blocks = 2;
  msg.msg[0] = 0x38 + channel;  // Access Mode register on dump mode
  msg.msg[1] = 0x48;  // Starts single conversion mode
  return msg;
}

spi_utils::Message AD7734::ReadDataRegisterMessage(uint8_t channel) {
  spi_utils::Message msg;
  msg.block_size = 1;
  msg.n_blocks = 4;
  msg.msg[0] = 0x48 + channel;  // Access data register on read mode
  msg.msg[1] = 0;  // Status byte
  msg.msg[2] = 0;  // First data byte
  msg.msg[3] = 0;  // Second data byte
  return msg;
}
