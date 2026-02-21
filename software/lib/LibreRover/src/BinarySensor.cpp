#include "BinarySensor.h"


BinarySensor::BinarySensor(uint8_t pin, bool use_pullup)
{
	this->pin = pin;
	this->use_pullup = use_pullup;
	
	if (this->use_pullup)
	{
		pinMode(this->pin, INPUT_PULLUP);
	}
	else
	{
		pinMode(this->pin, INPUT);
	}
}

BinarySensor::operator bool()
{
	return digitalRead(this->pin);
}

bool BinarySensor::operator==(int value)
{
	return digitalRead(this->pin) == value;
}

bool BinarySensor::operator!=(int value)
{
	return !(*this == value);
}

bool BinarySensor::is_high()
{
	return digitalRead(this->pin) == HIGH;
}

bool BinarySensor::is_low()
{
	return digitalRead(this->pin) == LOW;
}
