//
//	Class for AD5764 DAC:
//

#ifndef DACAD5764_h_
#define DACAD5764_h_

// #include "Arduino.h"
#include "dactests.h"
// #include "SPI.h" 
#define DATA_LEN 2

class DacAD5764: public Dac<DATA_LEN>
{
	public:
	DacAD5764(int sync_pin_, int ldac_pin_,  float full_scale_, int channel)
	: Dac<DATA_LEN>( sync_pin_,  ldac_pin_,   full_scale_,  channel){};
	
	byte  (&VoltageToBytes(float voltage, byte (&msg)[DATA_LEN]))[DATA_LEN];
};

#endif