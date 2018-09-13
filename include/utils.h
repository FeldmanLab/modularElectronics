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
#endif
