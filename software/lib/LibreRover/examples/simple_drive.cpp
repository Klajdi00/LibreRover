#include <Arduino.h>
#include <Motor.h>

Motor motor_left(9, 10, 11, 50, 255 / 9. * 6.5);
Motor motor_right(7, 8, 6, 70, 255 / 9. * 6.5);


void setup() {
	pinMode(LED_BUILTIN, OUTPUT);	
	Serial.begin(9600);
}

void loop() {
	digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on
	delay(1000);
	digitalWrite(LED_BUILTIN, LOW);    // turn the LED off
	
	// Turn motors on
	motor_left.forward(50);
	motor_right.forward(50);
	
	// Drive forward for 2 seconds
	delay(2000);
	
	// Turn motors off
	motor_left.stop();
	motor_right.stop();
	
}
