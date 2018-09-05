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

#include "dac.h"

///
/// AD5764 class
/// \author Carlos Kometter
/// \version 0.1
/// \date 2018
/// \copyright GNU Public License.
///
class AD5764: public Dac {
 private:
  uint8_t slot_number_;
  double full_scale_;
 public:
  ///
  /// Constructor
  /// \param[in] sync_pin The sync or chip select of the dac chip. Different than spi_bus_config_pin.
  /// \param[in] spi_bus_config_pin The pin that identifies the bus. More than one dac can share the same pin.
  /// \param[in] ldac_pin The ldac pin used to update the analog outputs.
  /// \param[in] slot_number This number is used to identify the chip or board. (default: 0) 
  /// \param[in] full_scale The full scale of the dac voltage in V units. (default: 10) 
  ///
  AD5764(uint8_t sync_pin, uint8_t spi_bus_config_pin,
         uint8_t ldac_pin, uint8_t slot_number=0, double full_scale=10.0);
  virtual double BytesToVoltage(spi_utils::Message message);
  virtual spi_utils::Message SetVoltageMessage(uint8_t channel, double voltage);
};
#endif
