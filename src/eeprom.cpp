// Class for eeprom chips
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

#include "../include/eeprom.h"

EepromSpi::EepromSpi(uint8_t sync_pin, uint8_t spi_bus_config_pin,
                     uint8_t clock_divider, BitOrder bit_order,
                     uint8_t spi_mode)
    : sync_pin_(sync_pin), spi_bus_config_pin_(spi_bus_config_pin),
      clock_divider_(clock_divider), bit_order_(bit_order),
      spi_mode_(spi_mode) {
}

uint8_t EepromSpi::Begin(void) {
  // Setting pin modes
  pinMode(sync_pin_, OUTPUT);
  // Setting pin values
  digitalWrite(sync_pin_, HIGH);
  // Initializing and configuring SPI
  SPI.begin(spi_bus_config_pin_);
  SPI.setBitOrder(spi_bus_config_pin_, bit_order_);
  SPI.setClockDivider(spi_bus_config_pin_, clock_divider_);
  SPI.setDataMode(spi_bus_config_pin_, spi_mode_);
  return 0;
}
