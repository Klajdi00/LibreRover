#ifndef __DISTANCE_SENSOR_VL53L0X__
#define __DISTANCE_SENSOR_VL53L0X__

#include "Arduino.h"
#include "Wire.h"
#include "Adafruit_VL53L0X.h"


class DistanceSensor_VL53L0X
{
	uint8_t xshut_pin;
	uint8_t i2c_address;
	bool initialized;
	Adafruit_VL53L0X sensor;

	public:
	DistanceSensor_VL53L0X(uint8_t xshut_pin, uint8_t i2c_address = 0x29);

	void shutdown();
	void wakeup(uint16_t boot_delay_ms = 10);

	bool begin(TwoWire& wire = Wire, uint16_t boot_delay_ms = 10);
	uint16_t measure_distance_mm();

	uint8_t get_i2c_address();
	bool is_initialized();
};

#endif