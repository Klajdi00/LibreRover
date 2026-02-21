#include <Arduino.h>

#include <Motor.h>
#include <SimpleEncoder.h>
#include <EncoderMotor.h>


Motor simple_motor_left(9, 10, 11, 50, 255 / 9. * 6.5);
Motor simple_motor_right(7, 8, 6, 50, 255 / 9. * 6.5);
SimpleEncoder encoder_left(3);
SimpleEncoder encoder_right(2);
EncoderMotor motor_left(simple_motor_left, encoder_left, 251, 40, 350);
EncoderMotor motor_right(simple_motor_right, encoder_right, 251, 40, 350);


void wait_until_motors_finished()
{
	while(!motor_left.is_stopped() || !motor_right.is_stopped())
	{
		motor_left.tick();
		motor_right.tick();
	}
}



void setup() {
	pinMode(LED_BUILTIN, OUTPUT);
	
	attachInterrupt(digitalPinToInterrupt(encoder_left.get_pin()), []() { encoder_left.handle_interrupt_rising(); }, RISING);
	attachInterrupt(digitalPinToInterrupt(encoder_left.get_pin()), []() { encoder_left.handle_interrupt_falling(); }, FALLING);
	attachInterrupt(digitalPinToInterrupt(encoder_right.get_pin()), []() { encoder_right.handle_interrupt_rising(); }, RISING);
	attachInterrupt(digitalPinToInterrupt(encoder_right.get_pin()), []() { encoder_right.handle_interrupt_falling(); }, FALLING);
	
	Serial.begin(9600);
}

void loop() {
	digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
	delay(500);                       // wait for a second
	digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
	delay(500);                       // wait for a second
	
	// simple_motor_right.forward(10);
	
	// motor_right.drive_distance(150, 150);
	// wait_until_motors_finished();
	// auto last_debug_msg = millis();
	// motor_left.drive_distance(150, 150);
	// motor_right.drive_distance(-150, 150);
	// while(true)
	// {
	// 	motor_left.tick();
	// 	motor_right.tick();
	// 	if(millis() - last_debug_msg >= 500)
	// 	{
	// 		last_debug_msg = millis();
	// 		Serial.println(
	// 			"Motor right: " + String(encoder_right.get_pulse_count()) + " / " + String(motor_right.get_target_pulse_count()) + 
	// 			", current speed: " + String(motor_right.current_speed_mm_per_s) + " mm/s" + 
	// 			", motor setpoint: " + String(simple_motor_right.get_speed()) + " %"
	// 		);
	// 		// Serial.println("  Current speed: " + String(motor_right.current_speed_mm_per_s) + " mm/s");
	// 	}
	// }
	
	motor_left.drive_distance(250, 150);
	motor_right.drive_distance(250, 150);
	wait_until_motors_finished();
	delay(500);
	motor_left.drive_distance(-75, 150);
	motor_right.drive_distance(75, 150);
	wait_until_motors_finished();
	
	
	
	// Serial.println("Left motor forward");
	// motor_left.drive_distance(50, 60, true);
	// Serial.println("Left motor backward");
	// motor_left.drive_distance(-50, 60, true);
	// Serial.println("Right motor forward");
	// motor_right.drive_distance(50, 65, true);
	// Serial.println("Right motor backward");
	// motor_right.drive_distance(-50, 65, true);
	// delay(5000);
	
	// motor_left.forward(70);
	// motor_right.forward(70);
	// delay(1000);
	// motor_left.stop();
	// motor_right.stop();
	// delay(5000);
}
