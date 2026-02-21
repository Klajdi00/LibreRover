#ifndef __MOTOR__
#define __MOTOR__

#include "Arduino.h"


class Motor
{
	uint8_t pin_forward;
	uint8_t pin_backward;
	uint8_t pin_pwm;
	uint8_t max_speed_pwm;
	uint8_t min_speed_pwm;
	
	int8_t current_direction; // -1 for backward, 0 for stop, 1 for forward
	unsigned long start_time_ms;
	unsigned long drive_duration_ms;
	uint8_t speed_percent;
	
	
	public:
	Motor(uint8_t pin_forward, uint8_t pin_backward, uint8_t pin_pwm, uint8_t min_speed_pwm = 0, uint8_t max_speed_pwm = 255, bool reverse = true);
	
	void forward(uint8_t speed_percent);	
	void backward(uint8_t speed_percent);	
	void stop();
	
	void set_speed(uint8_t speed_percent);
	uint8_t get_speed();
	
	void drive(int16_t speed_percent);
	void drive_ms(int16_t speed_percent, uint16_t duration_ms);
	
	bool is_stopped();
	
	void tick();
	
	// void hard_stop()
	// {
	// 	throw "Not implemented yet";	// Power both direction pins for a short amount of time.
										// Either blocking or we need to be sure that tick() still is called frequently...
	// }
};

#endif
