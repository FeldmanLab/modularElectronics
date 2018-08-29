// Class for a ADC board
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

#include "../include/adc.h"

AdcSpi::AdcSpi(uint8_t sync_pin, uint8_t spi_bus_config_pin,
	 uint8_t data_ready_pin, uint8_t clock_divider,
	 BitOrder bit_order, uint8_t spi_mode)
    : sync_pin_(sync_pin), spi_bus_config_pin_(spi_bus_config_pin),
      data_ready_pin_(data_ready_pin), clock_divider_(clock_divider),
      bit_order_(bit_order), spi_mode_(spi_mode) {
}

//Configures pins for SPI and initializes SPI
bool AdcSpi::Begin(void) {
  // Setting pin modes
  pinMode(sync_pin_, OUTPUT);
  pinMode(data_ready_pin_, INPUT);
  // Setting pin values
  digitalWrite(sync_pin_, HIGH);
  // Initializing and configuring SPI
  SPI.begin(spi_bus_config_pin_);
  SPI.setBitOrder(bit_order_);
  SPI.setClockDivider(clock_divider_);
  SPI.setDataMode(spi_mode_);
  return true;
}

float AdcSpi::ReadVoltage(uint8_t channel) {
  spi_utils::Message msg;
  msg = SingleConversionModeMessage(channel);
  // Start single conversion mode
  for (uint8_t block = 0; block < msg.n_blocks; block++) {
    digitalWrite(sync_pin_, LOW);
    for (uint8_t db = 0; db < msg.block_size; db++) {
      SPI.transfer(spi_bus_config_pin_, msg.msg[block*msg.block_size+db]);
    }
    digitalWrite(sync_pin_, HIGH);
  }
  // Wait for conversion to finish
  while (digitalRead(data_ready_pin_) == HIGH) {}
  msg = ReadDataRegisterMessage(channel);
  // Start data register readback
  for (uint8_t block = 0; block < msg.n_blocks; block++) {
    digitalWrite(sync_pin_, LOW);
    for (uint8_t db = 0; db < msg.block_size; db++) {
      msg.msg[block*msg.block_size]=SPI.transfer(spi_bus_config_pin_, msg.msg[block*msg.block_size+db]);
    }
    digitalWrite(sync_pin_, HIGH);
  }
  return BytesToVoltage(msg);
}
