#include "DistanceSensor_VL53L0X.h"


DistanceSensor_VL53L0X::DistanceSensor_VL53L0X(uint8_t xshut_pin, uint8_t i2c_address)
{
	this->xshut_pin = xshut_pin;
	this->i2c_address = i2c_address;
	this->initialized = false;

	pinMode(this->xshut_pin, OUTPUT);
	digitalWrite(this->xshut_pin, LOW);
}

void DistanceSensor_VL53L0X::shutdown()
{
	digitalWrite(this->xshut_pin, LOW);
	initialized = false;
}

void DistanceSensor_VL53L0X::wakeup(uint16_t boot_delay_ms)
{
	digitalWrite(this->xshut_pin, HIGH);
	delay(boot_delay_ms);
}

bool DistanceSensor_VL53L0X::begin(TwoWire& wire, uint16_t boot_delay_ms)
{
	shutdown();
	delay(10);
	wakeup(boot_delay_ms);

	initialized = sensor.begin(i2c_address, false, &wire);
	return initialized;
}

uint16_t DistanceSensor_VL53L0X::measure_distance_mm()
{
	if(!initialized)
	{
		return UINT16_MAX;
	}

	VL53L0X_RangingMeasurementData_t measure;
	sensor.rangingTest(&measure, false);

	// RangeStatus 4 means out-of-range according to the Adafruit examples.
	if(measure.RangeStatus == 4)
	{
		return UINT16_MAX;
	}

	return measure.RangeMilliMeter;
}

uint8_t DistanceSensor_VL53L0X::get_i2c_address()
{
	return i2c_address;
}

bool DistanceSensor_VL53L0X::is_initialized()
{
	return initialized;
}