#include <Arduino.h>
#include <Wire.h>
#include "DistanceSensor_VL53L0X.h"

// Dual VL53L0X sensors example
// Sensor 1: XSHUT pin 12, I2C address 0x29 (default)
// Sensor 2: XSHUT pin 13, I2C address 0x30 (after initialization)
DistanceSensor_VL53L0X distance_sensor_1(12, 0x29);
DistanceSensor_VL53L0X distance_sensor_2(13, 0x30);

void setup() {
	Serial.begin(9600);
	
	// Initialize I2C
	Wire.begin();
	
	// Initialize first sensor
	// At this point, sensor_2 is still shut down (XSHUT low)
	// So sensor_1 can safely use the default address 0x29
	if (distance_sensor_1.begin(Wire, 10)) {
		Serial.println("Sensor 1 initialized successfully!");
	} else {
		Serial.println("Failed to initialize sensor 1!");
		while (1);
	}
	
	// Initialize second sensor
	// Sensor_1 is now at 0x29, sensor_2 wakes up and is configured at 0x30
	if (distance_sensor_2.begin(Wire, 10)) {
		Serial.println("Sensor 2 initialized successfully!");
	} else {
		Serial.println("Failed to initialize sensor 2!");
		while (1);
	}
	
	Serial.println("Both sensors ready!");
}

void loop() {
	// Measure from both sensors
	uint16_t distance_1_mm = distance_sensor_1.measure_distance_mm();
	uint16_t distance_2_mm = distance_sensor_2.measure_distance_mm();
	
	// Print sensor 1 result
	Serial.print("Sensor 1: ");
	if (distance_1_mm == UINT16_MAX) {
		Serial.print("Out of range");
	} else {
		Serial.print(distance_1_mm);
		Serial.print(" mm");
	}
	
	// Print sensor 2 result
	Serial.print(" | Sensor 2: ");
	if (distance_2_mm == UINT16_MAX) {
		Serial.println("Out of range");
	} else {
		Serial.print(distance_2_mm);
		Serial.println(" mm");
	}
	
	delay(100);
}
