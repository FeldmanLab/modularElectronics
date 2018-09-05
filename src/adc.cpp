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
	       uint8_t data_ready_pin, uint8_t bit_resolution,
	       uint8_t clock_divider, BitOrder bit_order,
	       uint8_t spi_mode)
  : sync_pin_(sync_pin), spi_bus_config_pin_(spi_bus_config_pin),
    data_ready_pin_(data_ready_pin), bit_resolution_(bit_resolution),
    clock_divider_(clock_divider), bit_order_(bit_order),
    spi_mode_(spi_mode) {
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
  SPI.setBitOrder(spi_bus_config_pin_, bit_order_);
  SPI.setClockDivider(spi_bus_config_pin_, clock_divider_);
  SPI.setDataMode(spi_bus_config_pin_, spi_mode_);
  return true;
}

double AdcSpi::ReadVoltage(uint8_t channel) {
  spi_utils::Message msg;  // Stores the message to send via spi
  msg = SingleConversionModeMessage(channel);  // Gets the message to start single conversion
  SPI.transfer(spi_bus_config_pin_, 0); // Sets CLK and MOSI to proper level
  // Starts single conversion mode
  for (uint8_t block = 0; block < msg.n_blocks; block++) {
    digitalWrite(sync_pin_, LOW);
    for (uint8_t db = 0; db < msg.block_size; db++) {
      SPI.transfer(spi_bus_config_pin_, msg.msg[block*msg.block_size+db]);
    }
    digitalWrite(sync_pin_, HIGH);
  }
  // Wait for conversion to finish;
  while (digitalRead(data_ready_pin_) == HIGH) {}
  // Once adc finish converting, we read the measurement store on data register
  msg = ReadDataRegisterMessage(channel);  // Gets the message to read data register
  // Start data register readback
  for (uint8_t block = 0; block < msg.n_blocks; block++) {
    digitalWrite(sync_pin_, LOW);
    for (uint8_t db = 0; db < msg.block_size; db++) {
      msg.msg[block*msg.block_size+db]=SPI.transfer(spi_bus_config_pin_, msg.msg[block*msg.block_size+db]);
    }
    digitalWrite(sync_pin_, HIGH);
  }
  return BytesToVoltage(msg);
}

uint8_t AdcSpi::ReadVoltage(uint8_t channel, byte previous_meas[], bool send) {
  uint8_t n_previous_meas = (bit_resolution_ + 8 - 1)/ 8;  // Round up(bit_resolution_/8)
  spi_utils::Message msg;  // Stores the message to send via spi
  msg = SingleConversionModeMessage(channel);  // Gets the message to start single conversion
  SPI.transfer(spi_bus_config_pin_, 0);  // Sets CLK and MOSI to proper level
  // Starts single conversion mode
  for (uint8_t block = 0; block < msg.n_blocks; block++) {
    digitalWrite(sync_pin_, LOW);
    for (uint8_t db = 0; db < msg.block_size; db++) {
      SPI.transfer(spi_bus_config_pin_,
		   msg.msg[block * msg.block_size + db]);
    }
    digitalWrite(sync_pin_, HIGH);
  }
  // If true, it sends previous_meas while adc is converting the voltage
  if (send) {
    for (uint8_t previous_meas_index = 0;
	 previous_meas_index < n_previous_meas;
	 previous_meas_index++) {
      Serial.write(previous_meas[previous_meas_index]);
    }
  }
  // Wait for conversion to finish;
  while (digitalRead(data_ready_pin_) == HIGH) {}
  // Once adc finish converting, we read the measurement store on data register
  msg = ReadDataRegisterMessage(channel);  // Gets the message to read data register
  // Starts data register readback
  for (uint8_t block = 0; block < msg.n_blocks; block++) {
    digitalWrite(sync_pin_, LOW);
    for (uint8_t db = 0; db < msg.block_size; db++) {
      msg.msg[block * msg.block_size + db]
	= SPI.transfer(spi_bus_config_pin_,
		       msg.msg[block * msg.block_size + db]);
    }
    digitalWrite(sync_pin_, HIGH);
  }
  // Writes the new measurement to previous_meas
  for (uint8_t previous_meas_index = 0;
       previous_meas_index < n_previous_meas;
       previous_meas_index++) {
    // The measurement is usually the last n_previous_meas bytes of the received bytes
    // by the last readback
    previous_meas[previous_meas_index]
      = msg.msg[msg.n_blocks * msg.block_size
		- n_previous_meas + previous_meas_index];
  }
  return n_previous_meas;  // Returns the size of previous_meas 
}
