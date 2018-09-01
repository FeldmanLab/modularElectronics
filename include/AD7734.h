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

#ifndef AD7734_h_
#define AD7734_h_

#include "adc.h"

///
/// AD7734 class
///
class AD7734: public AdcSpi {
 private:
  uint8_t slot_number_;
  const double full_scale_ = 10.0;
 public:
  ///
  /// Constructor
  ///
  AD7734(uint8_t sync_pin, uint8_t spi_bus_config_pin,
         uint8_t data_ready_pin, uint8_t slot_number);
 protected:
  virtual float BytesToVoltage(spi_utils::Message message);
  virtual spi_utils::Message SingleConversionModeMessage(uint8_t channel);
  virtual spi_utils::Message ReadDataRegisterMessage(uint8_t channel);
};
#endif
