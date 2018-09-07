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

#ifndef MODULARELECTRONICS_EEPROM_h
#define MODULARELECTRONICS_EEPROM_h

#include "Arduino.h"
#include "SPI.h"

///
/// Eeprom class
/// \author Carlos Kometter
/// \version 0.1
/// \date 2018
/// \copyright GNU Public License.
///
class EepromSpi {
 private:
  // sync_pin_ identifies the eeprom chip; each chip has a unique sync_pin_.
  // spi_bus_config_pin_ identifies the bus configurations.
  // More than one eeprom can share the same bus if they use the same spi configs.
  uint8_t sync_pin_, spi_bus_config_pin_, spi_mode_;
  uint8_t clock_divider_;  // For due frequency = 84 MHz / clock_divider_
  BitOrder bit_order_;  // MSBFIRST or LSBFIRST
 public:
  EepromSpi(void) = default;
  ///
  /// Constructor
  /// \param[in] sync_pin The sync or chip select of the dac chip. Different than spi_bus_config_pin
  /// \param[in] spi_bus_config_pin The pin that identifies the bus. More than one dac can share the same pin.
  /// \param[in] clock_divider Sets the spi frequency. spi_freq = clock_freq (84 MHz for arduino due) / clock_divider (default: 4)
  /// \param[in] bit_order Possible values:
  ///   - MSBFIRST Most significant bit first. (default)
  ///   - LSBFIRST Least significant bit first.
  ///
  /// \param[in] spi_mode Possible values:
  ///   - SPI_MODE0
  ///   - SPI_MODE1
  ///   - SPI_MODE2
  ///   - SPI_MODE3 (default)
  ///
  EepromSpi(uint8_t sync_pin, uint8_t spi_bus_config_pin,
            uint8_t clock_divider=7, BitOrder bit_order=MSBFIRST,
            uint8_t spi_mode=SPI_MODE3);
  ///
  /// Configures pins for SPI and initializes SPI communication.
  /// This function must be called before calling any other function.
  /// \returns 0 if successful
  ///
  uint8_t Begin(void);
};

#endif
