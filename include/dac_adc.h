// Class for a DAC_ADC
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

#ifndef DAC_ADC_h_
#define DAC_DAC_h_

#include "board.h"
#include "dac.h"
#include "adc.h"

///
/// DacAdc class
/// \author Carlos Kometter
/// \version 0.1
/// \date 2018
/// \copyright GNU Public License.
///
class DacAdc {
 private:
  ///
  /// 0: Microseconds 1: Miliseconds
  ///
  uint8_t delay_unit_ = 0;
  board_types::DacBoard &dac_;
  board_types::AdcBoard &adc_;
 public:
  ///
  /// Constructor
  /// \param dac DacBoard object part
  /// \param adc AdcBoard object part
  ///
  DacAdc(board_types::DacBoard &dac, board_types::AdcBoard &adc);
  ///
  /// Configures pins for SPI and initializes SPI communication for both dac and adc objects.
  /// This function must be called before calling any other function.
  /// See also Dac::Begin and AdcSpi::Begin.
  ///
  uint8_t Begin(void);
  ///
  /// See Dac::SetVoltage.
  ///
  double SetVoltage(uint8_t channel, double voltage);
  ///
  /// See AdcSpi::ReadVoltage.
  ///
  double ReadVoltage(uint8_t channel);
  ///
  /// Ramps the voltage of dac_channels[] from start_voltages[] to end_voltages[] in n_steps.
  /// After each step, there is a step_delay, and then the adc_channels[] are read.
  /// Once the adc_channels[] are read, a new step starts. 
  /// \param dac_channels[] The dac channels to be ramped.
  /// \param n_dac_channels The number of dac channels to be ramped.
  /// \param adc_channels[] The adc channels to be read.
  /// \param n_adc_channels The number of adc channels to be read.
  /// \param start_voltages[] The start voltages of the ramp. It must be the same size as dac_channels[] and the element index corresponds to same index on dac_channels[] (eg. start_voltage[2] is the start voltage of dac_channels[2]). 
  /// \param end_voltages[] The end voltages of the ramp. It must be the same size as dac_channels[] and the element index corresponds to same index on dac_channels[] (eg. end_voltage[2] is the end voltage of dac_channels[2]). 
  /// \param n_steps The number of steps the ramp will be divided.
  /// \param step_delay The delay time between setting the dac channels and reading the adc channels.
  ///
  uint8_t BufferRamp(uint8_t dac_channels[], uint8_t n_dac_channels,
                  uint8_t adc_channels[], uint8_t n_adc_channels,
                  double start_voltages[], double end_voltages[],
                  uint32_t n_steps, uint32_t step_delay);
};
#endif
