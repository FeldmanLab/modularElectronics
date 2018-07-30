//
//	Class DAC:
//

// #include "Arduino.h"
// #include "SPI.h" 
#include "dac.h"
#include <iostream>
using namespace std;

template < int DATA_LEN> 
Dac<DATA_LEN>::Dac(int sync_pin_, int ldac_pin_,  float full_scale_, int channel)
{
	sync_pin = sync_pin_;
	ldac_pin = ldac_pin_;
	full_scale =  full_scale_;
	dac_channel = channel;
};

template < int DATA_LEN>
bool Dac<DATA_LEN>::begin(void){};

template < int DATA_LEN>
void Dac<DATA_LEN>::testfunc()
{
 	cout << sync_pin;
 	cout << ldac_pin<<"bla";
 	cout << DATA_LEN << endl;
};

