// Class for a DAC board
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

typedef unsigned char byte;

template <int DATA_LEN> 
class Dac {
 private:
  uint8_t msg_size_ = sizeof(msg_)/sizeof(byte);
  // sync_pin_ identifies the dac chip; each chip has a unique sync_pin_.
  // spi_bus_config_pin_ identifies the bus configurations.
  // More than one dac can share the same bus if they use the same spi configs.
  uint8_t sync_pin_, spi_bus_config_pin_, ldac_pin_, spi_mode_;  // See above
  uint8_t clock_divider_;  // For due frequency = 84 MHz / clock_divider_
  BitOrder bit_order_;  // MSBFIRST or LSBFIRST
 public:
  Dac(uint8_t sync_pin, uint8_t spi_bus_config_pin,
      uint8_t ldac_pin, uint8_t clock_divider=4,
      BitOrder bit_order=MSBFIRST, uint8_t spi_mode=SPI_MODE1);
  float SetVoltage(float voltage);
  void UpdateAnalogOutputs(void);
  bool Begin(void);  // SPI initialization
  void VoltageToBytes(float voltage);
  float BytesToVoltage(byte msg[DATA_LEN]);
 protected:
  byte msg_[DATA_LEN];  // Dynamic allocation of bytes to send to dac chip
};

////////////////////////////////////
//  Member functions definitions  ///
///////////////////////////////////

template <int DATA_LEN> 
Dac<DATA_LEN>::Dac(uint8_t sync_pin, uint8_t spi_bus_config_pin,
                   uint8_t ldac_pin, uint8_t clock_divider,
                   BitOrder bit_order, uint8_t spi_mode)
    : sync_pin_(sync_pin), spi_bus_config_pin_(spi_bus_config_pin),
      ldac_pin_(ldac_pin), clock_divider_(clock_divider),
      bit_order_(bit_order), spi_mode_(spi_mode) {
}

//Configures pins for SPI and initializes SPI
template <int DATA_LEN>
inline bool Dac<DATA_LEN>::Begin(void) {
  // Setting pin modes
  pinMode(sync_pin_, OUTPUT);
  pinMode(ldac_pin_, OUTPUT);
  // Setting pin values
  digitalWrite(sync_pin_, HIGH);
  digitalWrite(ldac_pin_, HIGH);
  // Initializing and configuring SPI
  SPI.begin(spi_bus_config_pin_);
  SPI.setBitOrder(bit_order_);
  SPI.setClockDivider(clock_divider_);
  SPI.setDataMode(spi_mode_);
  return true;
}

// Toogles LDAC to update data register.
template <int DATA_LEN>
inline void Dac<DATA_LEN>::UpdateAnalogOutputs(void) {
  digitalWrite(ldac_pin_, LOW);
  digitalWrite(ldac_pin_, HIGH);
}

template <int DATA_LEN>
inline float Dac<DATA_LEN>::SetVoltage(float voltage) {
  VoltageToBytes(voltage);
  // SPI data transfer
  digitalWrite(sync_pin_, LOW);  // Dac starts listening
  // Sending bytes
  for (uint8_t i; i < msg_size_; i++) {
    SPI.transfer(spi_bus_config_pin_, msg_[i]);
  }
  digitalWrite(sync_pin_, HIGH);  // Dac register updated
  UpdateAnalogOutputs();  // Analog output updated
  // Updated voltage may be different than voltage parameter because of
  // resolution
  return BytesToVoltage(msg_);
}
#endif
