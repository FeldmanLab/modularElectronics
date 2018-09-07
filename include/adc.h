// Class for a ADC chip or board
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

#ifndef ADC_h_
#define ADC_h_

#include "Arduino.h"
#include "SPI.h"
#include "utils.h"

typedef unsigned char byte;

///
/// AdcSpi class
/// \author Carlos Kometter
/// \version 0.1
/// \date 2018
/// \copyright GNU Public License.
///
class AdcSpi {
 private:
  // sync_pin_ identifies the adc chip; each chip has a unique sync_pin_.
  // spi_bus_config_pin_ identifies the bus configurations.
  // More than one adc can share the same bus if they use the same spi configs.
  uint8_t sync_pin_, spi_bus_config_pin_, data_ready_pin_, spi_mode_;
  uint8_t clock_divider_;  // For due frequency = 84 MHz / clock_divider_
  BitOrder bit_order_;  // MSBFIRST or LSBFIRST
  uint8_t bit_resolution_;
 public:
  AdcSpi(void) = default;
  ///
  /// Constructor
  /// \param[in] sync_pin The sync or chip select of the adc chip. Different than spi_bus_config_pin
  /// \param[in] spi_bus_config_pin The pin that identifies the bus. More than one adc can share the same pin.
  /// \param[in] data_ready_pin This pin tells whether the adc has finished measuring a voltage.
  /// \param[in] bit_resolution The bit resolution of the adc (eg. 16 for AD7734).
  /// \param[in] clock_divider Sets the spi frequency. spi_freq = clock_freq (84 MHz for arduino due) / clock_divider (default: 7)
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
  AdcSpi(uint8_t sync_pin, uint8_t spi_bus_config_pin,
	 uint8_t data_ready_pin, uint8_t bit_resolution,
	 uint8_t clock_divider=7, BitOrder bit_order=MSBFIRST,
	 uint8_t spi_mode=SPI_MODE3);
  ///
  /// Configures pins for SPI and initializes SPI communication.
  /// This function must be called before calling any other function.
  ///
  bool Begin(void);
  ///
  /// Read the voltage at one of the analog inputs of the adc.
  /// \param[in] channel The analog input to be read. (default: 0)
  /// \returns The voltage at the analog input.
  ///
  double ReadVoltage(uint8_t channel=0);
  ///
  /// \overload
  ///
  /// This function takes of the dead time the adc is converting (meausering) a new voltage.
  /// During this dead time the arduino sends the previous measurement via serial.
  /// \param[in] channel The analog input to be read. (default: 0)
  /// \param[in,out] previous_meas[] stores values of previous measurement to be sent vias spi. After the adc finishes converting the new voltage, it stores the new values.
  /// \param[in] send Possible values:
  ///   - true Content of previous_meas[] is sent via serial while the adc is converting current voltage.
  ///   - false Content of previous_meas[] is non sent via serial while the adc is converting current voltage.
  /// \returns The voltage at the analog input.
  ///
  uint8_t ReadVoltage(uint8_t channel, byte previous_meas[], bool send);
 protected:
  ///
  /// Uses chip specifications (e.g. bit resolution, registers, range) to convert
  /// a set of bytes stored in a Message object to voltage.
  /// Each ADC has its own convertion function. Check the datasheet of each chip
  /// and override the function below accordingly.
  /// \param[in] message Message object which stores the bytes for conversion.
  /// \returns The measured voltage.
  ///
  virtual double BytesToVoltage(spi_utils::Message message) = 0;
  ///
  /// \param[in] channel The channel to start single conversion mode on dump mode.
  /// \returns Message object to start single conversion mode on dump mode.
  ///
  virtual spi_utils::Message SingleConversionModeMessage(uint8_t channel=0) = 0;
  ///
  /// \param[in] channel The channel to be read.
  /// \returns Message object to read the data register.
  ///
  virtual spi_utils::Message ReadDataRegisterMessage(uint8_t channel=0) = 0;
};
#endif
