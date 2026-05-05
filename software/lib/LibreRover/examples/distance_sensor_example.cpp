#include <Arduino.h>
#include <Wire.h>
#include "DistanceSensor_VL53L0X.h"

// Single VL53L0X sensor example
// XSHUT pin: 12 (Arduino pin)
// I2C Address: 0x29 (default)
DistanceSensor_VL53L0X distance_sensor(12);

void setup() {
	Serial.begin(9600);
	
	// Initialize I2C
	Wire.begin();
	
	// Initialize the distance sensor
	// The sensor will be woken up and configured on the default I2C address
	if (distance_sensor.begin(Wire, 10)) {
		Serial.println("Distance sensor initialized successfully!");
	} else {
		Serial.println("Failed to initialize distance sensor!");
		while (1); // Halt if initialization fails
	}
}

void loop() {
	// Measure distance
	uint16_t distance_mm = distance_sensor.measure_distance_mm();
	
	// Print the result
	if (distance_mm == UINT16_MAX) {
		Serial.println("Distance: Out of range");
	} else {
		Serial.print("Distance: ");
		Serial.print(distance_mm);
		Serial.println(" mm");
	}
	
	// Wait a bit before the next measurement
	delay(100);
}
