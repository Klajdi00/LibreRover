#include "SimpleEncoder.h"


SimpleEncoder::SimpleEncoder(uint8_t pin)
{
	this->pin = pin;
	this->pulse_count = 0;
	pinMode(this->pin, INPUT);
}

void SimpleEncoder::reset()
{
	this->pulse_count = 0;
}

uint32_t SimpleEncoder::get_pulse_count()
{
	return this->pulse_count;
}

void SimpleEncoder::handle_interrupt_rising()
{
	this->pulse_count++;
}

void SimpleEncoder::handle_interrupt_falling()
{
	this->pulse_count++;
}

uint8_t SimpleEncoder::get_pin()
{
	return this->pin;
}
