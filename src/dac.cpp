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

#include "../include/dac.h"

DacSpi::DacSpi(uint8_t sync_pin, uint8_t spi_bus_config_pin,
	 uint8_t ldac_pin, uint8_t bit_resolution,
	 uint8_t clock_divider, BitOrder bit_order,
	 uint8_t spi_mode)
    : sync_pin_(sync_pin), spi_bus_config_pin_(spi_bus_config_pin),
      ldac_pin_(ldac_pin), bit_resolution_(bit_resolution),
      clock_divider_(clock_divider), bit_order_(bit_order),
      spi_mode_(spi_mode) {
}

//Configures pins for SPI and initializes SPI
uint8_t DacSpi::Begin(void) {
  // Setting pin modes
  pinMode(sync_pin_, OUTPUT);
  pinMode(ldac_pin_, OUTPUT);
  // Setting pin values
  digitalWrite(sync_pin_, HIGH);
  digitalWrite(ldac_pin_, HIGH);
  // Initializing and configuring SPI
  SPI.begin(spi_bus_config_pin_);
  SPI.setBitOrder(spi_bus_config_pin_, bit_order_);
  SPI.setClockDivider(spi_bus_config_pin_, clock_divider_);
  SPI.setDataMode(spi_bus_config_pin_, spi_mode_);
  return 0;
}

uint8_t DacSpi::Initialize(void) {
  spi_utils::Message msg;
  msg = InitializeMessage();
  // SPI data transfer
  SPI.transfer(spi_bus_config_pin_, 0); // Sets CLK and MOSI to proper level
  for (uint8_t block = 0; block < msg.n_blocks; block++) {
    digitalWrite(sync_pin_, LOW);
    for (uint8_t db = 0; db < msg.block_size; db++) {
      SPI.transfer(spi_bus_config_pin_, msg.msg[block*msg.block_size+db]);
    }
    digitalWrite(sync_pin_, HIGH);
  }
  return 0;
}

// Toogles LDAC to update analog outputs.
void DacSpi::UpdateAnalogOutputs(void) {
  digitalWrite(ldac_pin_, LOW);
  digitalWrite(ldac_pin_, HIGH);}

double DacSpi::SetVoltage(uint8_t channel, double voltage,
		       bool update_outputs) {
  spi_utils::Message msg;
  msg = SetVoltageMessage(channel, voltage);
  // SPI data transfer
  SPI.transfer(spi_bus_config_pin_, 0); // Sets CLK and MOSI to proper level
  for (uint8_t block = 0; block < msg.n_blocks; block++) {
    digitalWrite(sync_pin_, LOW);
    for (uint8_t db = 0; db < msg.block_size; db++) {
      SPI.transfer(spi_bus_config_pin_, msg.msg[block*msg.block_size+db]);
    }
    digitalWrite(sync_pin_, HIGH);
  }
  if (update_outputs) {
    UpdateAnalogOutputs();  // Analog output updated
  }
  // Updated voltage may be different than voltage parameter because of
  // resolution
  return BytesToVoltage(msg);
}

double DacSpi::GetVoltage(uint8_t channel) {
  spi_utils::Message msg;
  msg = SetVoltageMessage(channel, 0);
  msg.msg[0] = msg.msg[0] | 0x80;  // Set MSB to 1, which reads the register
  // SPI data transfer
  SPI.transfer(spi_bus_config_pin_, 0); // Sets CLK and MOSI to proper level
  for (uint8_t block = 0; block < msg.n_blocks; block++) {
    digitalWrite(sync_pin_, LOW);
    for (uint8_t db = 0; db < msg.block_size; db++) {
      SPI.transfer(spi_bus_config_pin_, msg.msg[block*msg.block_size+db]);
    }
    digitalWrite(sync_pin_, HIGH);
  }
  // Reads data register
  for (uint8_t block = 0; block < msg.n_blocks; block++) {
    digitalWrite(sync_pin_, LOW);
    for (uint8_t db = 0; db < msg.block_size; db++) {
      msg.msg[block*msg.block_size+db]=SPI.transfer(spi_bus_config_pin_, 0);
    }
    digitalWrite(sync_pin_, HIGH);
  }
  return BytesToVoltage(msg);
}
