//
//	Class for AD5764 DAC:
//

// #include "Arduino.h"
#include "dactests.h"
// #include "SPI.h" 
#define DATA_LEN 2

template < int DATA_LEN> 
DacAD5764 (int sync_pin_, int ldac_pin_,  float full_scale_, int channel) 
		: Dac<DATA_LEN>(int sync_pin_, int ldac_pin_,  float full_scale_, int channel){};


