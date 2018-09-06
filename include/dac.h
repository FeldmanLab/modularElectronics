// Class for a DAC chip or board
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

#ifndef DAC_h_
#define DAC_h_

#include "Arduino.h"
#include "SPI.h"
#include "utils.h"

typedef unsigned char byte;

///
/// Dac class
/// \author Carlos Kometter
/// \author Sasha Zibrov
/// \version 0.1
/// \date 2018
/// \copyright GNU Public License.
///
class Dac {
 private:
  // sync_pin_ identifies the dac chip; each chip has a unique sync_pin_.
  // spi_bus_config_pin_ identifies the bus configurations.
  // More than one dac can share the same bus if they use the same spi configs.
  uint8_t sync_pin_, spi_bus_config_pin_, ldac_pin_, spi_mode_;
  uint8_t clock_divider_;  // For due frequency = 84 MHz / clock_divider_
  BitOrder bit_order_;  // MSBFIRST or LSBFIRST
  uint8_t bit_resolution_;
 public:
  ///
  /// Constructor
  /// \param[in] sync_pin The sync or chip select of the dac chip. Different than spi_bus_config_pin
  /// \param[in] spi_bus_config_pin The pin that identifies the bus. More than one dac can share the same pin.
  /// \param[in] ldac_pin The ldac pin used to update the analog outputs.
  /// \param[in] bit_resolution The bit resolution of the dac (eg. 20 for AD5791).
  /// \param[in] clock_divider Sets the spi frequency. spi_freq = clock_freq (84 MHz for arduino due) / clock_divider (default: 4)
  /// \param[in] bit_order Possible values:
  ///   - MSBFIRST Most significant bit first. (default)
  ///   - LSBFIRST Least significant bit first.
  ///
  /// \param[in] spi_mode Possible values:
  ///   - SPI_MODE0
  ///   - SPI_MODE1 (default)
  ///   - SPI_MODE2
  ///   - SPI_MODE3
  ///
  Dac(uint8_t sync_pin, uint8_t spi_bus_config_pin,
      uint8_t ldac_pin, uint8_t bit_resolution,
      uint8_t clock_divider=4, BitOrder bit_order=MSBFIRST,
      uint8_t spi_mode=SPI_MODE1);
  ///
  /// Configures pins for SPI and initializes SPI communication.
  /// This function must be called before calling any other function.
  /// \returns 0 if successful
  ///
  uint8_t Begin(void);
  ///
  /// Initializes Dac from tristate mode to normal mode.
  /// \returns 0 if successful.
  ///
  uint8_t Initialize(void);
  ///
  /// Sets a voltage to Dac register via SPI.
  /// \param[in] channel The channel to be setted.
  /// \param[in] voltage The voltage to be setted to channel.
  /// \param[in] update_outputs Possible value:
  ///   - true The analog output is updated after setting the dac register.
  ///   - false The analog output is not updated after setting the dac regiser.
  /// \returns The voltage setted.
  ///
  double SetVoltage(uint8_t channel, double voltage,
		    bool update_outputs = true);
  ///
  /// Updates the outputs of all Dac objects sharing the same ldac_pin_.
  ///
  void UpdateAnalogOutputs(void);
  ///
  /// Gets the voltage setted to dac register.
  /// \param[in] channel Specifies the channel to query the voltage.
  /// \returns The voltage setted to dac register.
  ///
  double GetVoltage(uint8_t channel);
 protected:
  ///
  /// Uses chip specifications (e.g. bit resolution, registers, range) to convert
  /// a set of bytes stored in a Message object to voltage.
  /// Each DAC has its own convertion function. Check the datasheet of each chip
  /// and override the function below accordingly.
  /// \param[in] message Message object which stores the bytes for conversion.
  /// \returns The current voltage setted on the dac register.
  ///
  virtual double BytesToVoltage(spi_utils::Message message) = 0;
  ///
  /// \param[in] channel Specifies the channel the Message will set the voltage to.
  /// \param[in] voltage Specifies the voltage.
  /// \returns Message object to write data register.
  ///
  virtual spi_utils::Message SetVoltageMessage(uint8_t channel, double voltage) = 0;
  ///
  /// \returns Message object to intialize Dac.
  ///
  virtual spi_utils::Message InitializeMessage(void) = 0;
};
#endif
