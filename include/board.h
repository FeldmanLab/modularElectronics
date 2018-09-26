// Namespace for Board types.
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

#ifndef BOARD_h_
#define BOARD_h_

#include "dac.h"
#include "adc.h"

///
/// board classes for hierarchy purposes.
/// \author Carlos Kometter
/// \version 0.1
/// \date 2018
/// \copyright GNU Public License.
///
namespace board_types {
///
/// Board class
/// \author Carlos Kometter
/// \version 0.1
/// \date 2018
/// \copyright GNU Public License.
///
class Board {
 protected:
  ///
  /// Board name.
  ///
  String name_ = "";

 public:
  Board(void) = default;
  ///
  /// Router for serial communication.
  /// \param[in] cmd[] The String array storing the command (cmd[0]) and parameters.
  ///   - {"*IDN?"}: Prints name_ to serial with ASCII encoding.
  ///   - {"*RDY?"}: Prints "READY" to serial with ASCII encoding.
  /// \returns 0: success 1: command not found
  ///
  uint8_t Router(String cmd[]) {
    String command = cmd[0];
    if (command == "NOP") {
      Serial.println("NOP");
      return 0;
    } else if (command == "*IDN?") {
      Serial.println(name_);
      return 0;
    } else if (command == "*RDY?") {
      Serial.println("READY");
      return 0;
    }
    return 1; // Not found
  }
};
///
/// DacBoard class
/// \author Carlos Kometter
/// \version 0.1
/// \date 2018
/// \copyright GNU Public License.
///
class DacBoard : public Board, public virtual DacSpi  {
 public:
  ///
  /// Router for serial communication.
  /// \param[in] cmd[] The String array storing the command (cmd[0]) and parameters.
  ///   - {"INITIALIZE"}: Calls  Dac::Initialize ().
  ///   - {"SET", "channel", "voltage"}: Calls Dac::SetVoltage (channel, voltage) and prints its result to serial with ASCII encoding.
  ///   - {"GET", "channel"}: Calls Dac::GetVoltage (channel) and prints its result to serial with ASCII encoding.
  ///
  /// If command cmd[0] is not found, it calls Board::Router with parameter cmd.
  /// \returns 0: success 1: command not found
  ///
  uint8_t Router(String cmd[]) {
    String command = cmd[0];
    if (command == "INITIALIZE") {
      Initialize();
      return 0;
    } else if (command == "SET") {
      double voltage;
      voltage = SetVoltage(cmd[1].toInt(), cmd[2].toFloat());
      Serial.println(voltage);
      return 0;
    } else if (command == "GET") {
      double voltage;
      voltage = GetVoltage(cmd[1].toInt());
      Serial.println(voltage);
      return 0;
    }
    uint8_t result = Board::Router(cmd);
    return result;
  }
};
///
/// AdcBoard class
/// \author Carlos Kometter
/// \version 0.1
/// \date 2018
/// \copyright GNU Public License.
///
class AdcBoard : public Board, public virtual AdcSpi {
 public:
  ///
  /// Router for serial communication.
  /// \param[in] cmd[] The String array storing the command (cmd[0]) and parameters.
  ///   - {"READ", "channel"}: Calls AdcSpi::ReadVoltage (channel) and prints its result to serial with ASCII encoding.
  ///
  /// If command cmd[0] is not found, it calls Board::Router (cmd).
  /// \returns 0: success 1: command not found
  ///
  uint8_t Router(String cmd[]) {
    String command = cmd[0];
    if (command == "READ") {
      double voltage;
      voltage = ReadVoltage(cmd[1].toInt());
      Serial.println(voltage);
      return 0;
    }
    uint8_t result = Board::Router(cmd);
    return result;
  }
};
}
#endif
