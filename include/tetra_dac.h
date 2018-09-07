// Class for tetraDac
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

#ifndef TETRADAC_h_
#define TETRADAC_h_

#include "board.h"
#include "AD5791.h"
#include "eeprom.h"

///
/// TetraDac class
/// \author Carlos Kometter
/// \version 0.1
/// \date 2018
/// \copyright GNU Public License.
///
class TetraDac : public Board {
 private:
  static const uint8_t n_channels_ = 4;
  AD5791 dacs_[n_channels_];
  EepromSpi eeprom;
 public:
  ///
  /// Constructor
  /// \param[in] dac_sync_pins[] The sync or chip select of the 4 dac chips. Different than spi_bus_config_pin.
  /// \param[in] eeprom_sync_pin The sync or chip select of the on-board eeprom.
  /// \param[in] dac_spi_bus_config_pin The pin that identifies the dac bus.
  /// \param[in] eeprom_spi_bus_config_pin The pin that identifies the eeprom bus.
  /// \param[in] ldac_pin The ldac pin used to update the analog outputs.
  /// \param[in] slot_number This number is used to identify the chip or board. (default: 0) 
  /// \param[in] full_scale The full scale of the dac voltage in V units. (default: 10) 
  ///
  TetraDac(uint8_t dac_sync_pins[n_channels_], uint8_t eeprom_sync_pin,
           uint8_t dac_spi_bus_config_pin, uint8_t eeprom_spi_bus_config_pin,
           uint8_t ldac_pin, double full_scale = 10.0,
           uint8_t slot_number = 0);
  ///
  /// See Dac::Begin and EepromSpi::Begin
  ///
  uint8_t Begin(void);
  ///
  /// See Dac::Initialize
  ///
  uint8_t Initialize(void);
  ///
  /// See Dac::SetVoltage.
  ///
  uint8_t SetVoltage(uint8_t channel, double voltage,
                     bool update_outputs = true);
  ///
  /// See Dac::GetVoltage.
  ///
  uint8_t GetVoltage(uint8_t channel);
  ///
  /// See Dac::UpdateAnalogOutputs.
  ///
  void UpdateAnalogOutputs(void);
};
#endif
