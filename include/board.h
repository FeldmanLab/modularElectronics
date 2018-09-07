// Class for a Board
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
class Board {};
///
/// DacBoard class
/// \author Carlos Kometter
/// \version 0.1
/// \date 2018
/// \copyright GNU Public License.
///
class DacBoard : public Board {};
///
/// AdcBoard class
/// \author Carlos Kometter
/// \version 0.1
/// \date 2018
/// \copyright GNU Public License.
///
class AdcBoard : public Board {};
}
#endif
