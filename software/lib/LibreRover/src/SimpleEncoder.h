#ifndef __SIMPLE_ENCODER__
#define __SIMPLE_ENCODER__

#include "Arduino.h"


class SimpleEncoder
{
	uint8_t pin;
	uint32_t pulse_count;
	
	public:
	SimpleEncoder(uint8_t pin);
	
	void reset();
	
	uint32_t get_pulse_count();

	void handle_interrupt_rising();
	
	void handle_interrupt_falling();
	
	uint8_t get_pin();
	
};

#endif