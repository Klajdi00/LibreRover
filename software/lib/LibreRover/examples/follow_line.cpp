#include <Arduino.h>

#include <Motor.h>
#include <BinarySensor.h>


Motor motor_left(9, 10, 11, 50, 255 / 9. * 6.5);
Motor motor_right(7, 8, 6, 70, 255 / 9. * 6.5);

BinarySensor line_left(4);
BinarySensor line_right(5);


void setup() {
	pinMode(LED_BUILTIN, OUTPUT);	
	Serial.begin(9600);
}

void loop() {

	if(line_left.is_high())
	{
		// Left sensor is on the line
		motor_left.drive(-50);
	}
	else
	{
		motor_left.drive(50);
	}
	
	if(line_right.is_high())
	{
		// Right sensor is on the line
		motor_right.drive(-50);
	}
	else
	{
		motor_right.drive(50);
	}
	
}
