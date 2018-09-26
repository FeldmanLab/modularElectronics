// Utils
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

#ifndef MODULARELECTRONICS_UTILS_h_
#define MODULARELECTRONICS_UTILS_h_

///
/// Utilites for spi communication
/// \author Carlos Kometter
/// \version 0.1
/// \date 2018
/// \copyright GNU Public License.
///
namespace spi_utils {
  ///
  /// struct use as a SPI messsage
  /// \author Carlos Kometter
  /// \version 0.1
  /// \date 2018
  /// \copyright GNU Public License.
  ///
  struct Message {
    ///
    /// Size of msg. (block_size*n_blocks <= kdata_len_)
    ///
    static const uint8_t kdata_len_ = 10;
    ///
    /// Message to be sent via SPI. Each element represents a byte. This message is
    /// separated coul be divided in blocks. 
    ///
    byte msg[kdata_len_];
    ///
    /// The size in bytes of the registers to be written.
    /// It can be also be think as the number of bytes to be sent before the setting
    /// the sync_pin_ to HIGH. (block_size*n_blocks <= kdata_len_)
    ///
    uint8_t block_size;
    ///
    /// The number of blocks. Each block starts with a sync_pin_ to LOW and ends with
    /// a sync_pin_ to HIGH. (block_size*n_blocks <= kdata_len_)
    ///
    uint8_t n_blocks;
  };
}

///
/// Utilites for interface
/// \author Carlos Kometter
/// \version 0.1
/// \date 2018
/// \copyright GNU Public License.
///
namespace interface_utils {
  ///
  /// Reads and parses a serial message until the until '\\r' and stores the command to cmd[].
  /// A ',' or ':' indicates a new element. For example, the message "DAC:SET,0,1.3\r", will be
  /// parsed as {"DAC", "SET", "0", "1.3"}.
  /// \param[out] cmd[] The array to store the message.
  ///
  inline uint8_t query_serial(String cmd[]) {
    char received;
    String cmd_element = "";
    uint8_t cmd_size = 0;
    while (received != '\r') {
      if(Serial.available()) {
	received = Serial.read();
	if (received == '\n' || received == ' ') {
	} else if (received == ',' || received == '\r' || received == ':') {
	  cmd[cmd_size] = cmd_element;
	  cmd_element = "";
	  ++cmd_size;
	} else {
	  cmd_element += received;
	}
      }
    }
    return cmd_size;
  }
  ///
  /// Separates String characters into an array of unsigned intergers. The String characters must be numbers. 
  /// \param[in] string_in The String to be separate.
  /// \param[out] array_out The array to store the String characters.
  /// \returns The size of string_in.
  ///
  inline uint8_t string_to_int_array(String string_in, uint8_t array_out[]) {
    for (uint8_t index = 0; index < string_in.length(); ++index) {
      array_out[index] = String(string_in[index]).toInt();
    }
    return string_in.length();
  }
  ///
  /// Shifts array to the left.
  /// \param[in] array_in[] The array to be shifted.
  /// \param[in] array_in_size The size of array_in[].
  /// \param[out] array_out[] Array to store shifted array.
  /// \param[in] n_shift The number of places to shift.
  ///
  inline void shift_array_left(String array_in[], uint8_t array_in_size,
			       String array_out[], uint8_t n_shift) {
    for (uint8_t index = 0; index < array_in_size; ++index) {
      array_out[index] = array_in[index + n_shift];
    }
  }
}

///
/// Utilities for measurements
/// \author Carlos Kometter
/// \version 0.1
/// \date 2018
/// \copyright GNU Public License.
///
namespace meas_utils {
  ///
  /// Ramps the voltage of dac_channels[] from start_voltages[] to end_voltages[] in n_steps.
  /// After each step, there is a step_delay, and then the adc_channels[] are read.
  /// Once the adc_channels[] are read, a new step starts.
  /// \param[in] dac Dac
  /// \param[in] adc Adc
  /// \param[in] dac_channels[] The dac channels to be ramped.
  /// \param[in] n_dac_channels The number of dac channels to be ramped.
  /// \param[in] adc_channels[] The adc channels to be read.
  /// \param[in] n_adc_channels The number of adc channels to be read.
  /// \param[in] start_voltages[] The start voltages of the ramp. It must be the same size as dac_channels[] and the element index corresponds to same index on dac_channels[] (eg. start_voltage[2] is the start voltage of dac_channels[2]). 
  /// \param[in] end_voltages[] The end voltages of the ramp. It must be the same size as dac_channels[] and the element index corresponds to same index on dac_channels[] (eg. end_voltage[2] is the end voltage of dac_channels[2]). 
  /// \param[in] n_steps The number of steps the ramp will be divided.
  /// \param[in] step_delay The delay time between setting the dac channels and reading the adc channels.
  /// \param[in] delay_unit Specifies step_delay unit in us (0: default) or ms (1); 
  ///
  template <typename T1, typename T2>
  uint8_t BufferRamp(T1 dac, T2 adc,
		     uint8_t dac_channels[], uint8_t n_dac_channels,
		     uint8_t adc_channels[], uint8_t n_adc_channels,
		     double start_voltages[], double end_voltages[],
		     uint32_t n_steps, uint32_t step_delay,
		     uint8_t delay_unit=0) {
    byte meas[10];  // Storage for measurements
    uint8_t n_meas;  // # of bytes of meas[] used by the adc to store a measurement
    // Buffer ramp starts here
    for (uint32_t step = 0; step < n_steps; step++) {
      // Writes the dac register of each channel in dac_channels[]
      for (uint8_t dac_channel_index = 0;
           dac_channel_index < n_dac_channels;
           dac_channel_index++) {
        uint8_t channel = dac_channels[dac_channel_index];
        double start_voltage = start_voltages[dac_channel_index];
        double end_voltage = end_voltages[dac_channel_index];
        // Linear ramp from start_voltage to end_voltage in n_steps
        double next_voltage = start_voltage +
                              (end_voltage - start_voltage) * step / (n_steps - 1);
        // false: Writes the dac register but does not update the analog output.
        dac.SetVoltage(channel, next_voltage, false);
      }
      // Updates the analog outputs of all channels at the same time
      dac.UpdateAnalogOutputs();
      // Waits for system to settle
      if (delay_unit == 0) {
        delayMicroseconds(step_delay);
      }
      else {
        delay(step_delay);
      }
      // Read the adc voltage 
      for (uint8_t adc_channel_index = 0;
           adc_channel_index < n_adc_channels;
           adc_channel_index++) {
        uint8_t channel = adc_channels[adc_channel_index];
        // The adc takes some time to convert the voltage to bytes.
        // While the adc is converting we send the previous measurement
        // to take advantage of the dead time.
        if (step > 0 || adc_channel_index > 0) {
	  // Sends the previous measurement stored in meas.
          n_meas = adc.ReadVoltage(channel, meas, true);
        }
        else {
	  // First measurement, nothing to send so false
          n_meas = adc.ReadVoltage(channel, meas, false);
        }
      }
    }
    // Sends the last measurement
    for (uint8_t meas_index = 0; meas_index < n_meas; meas_index++) {
      Serial.write(meas[meas_index]);
    }
    return 0;
  }  
}
#endif
