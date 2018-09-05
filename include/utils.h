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
#endif
