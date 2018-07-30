//
//	Class for a DAC board
//

#ifndef DAC_h_
#define DAC_h_

// #include "Arduino.h"
// #include "SPI.h" 
#include <iostream>
using namespace std;

typedef unsigned char byte;

template < int DATA_LEN> 
class Dac
{
private :
	byte msg[DATA_LEN];
public:
	Dac(int sync_pin_, int ldac_pin_,  float full_scale_, int channel);
	bool begin(void);
	float DacDataSend(float voltage){};
	void testfunc(void);
	byte  (&VoltageToBytes(float voltage, byte (&msg)[DATA_LEN]))[DATA_LEN]{};
	byte (&BytesToVoltage(float voltage, byte (&msg)[DATA_LEN] ))[DATA_LEN]{};
	// BitOrder bit_order;
	int clock_divider, data_mode;
	int sync_pin, ldac_pin, dac_channel;
	float full_scale;
};
#endif