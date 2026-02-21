#ifndef __BINARY_SENSOR__
#define __BINARY_SENSOR__

#include "Arduino.h"


class BinarySensor
{
	uint8_t pin;
	bool use_pullup;
	
	public:
	BinarySensor(uint8_t pin, bool use_pullup = false);
	
	operator bool();
	bool operator==(int value);
	bool operator!=(int value);
	
	bool is_high();
	bool is_low();
};

#endif
