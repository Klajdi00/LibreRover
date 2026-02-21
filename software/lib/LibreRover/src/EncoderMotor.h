#ifndef __ENCODER_MOTOR__
#define __ENCODER_MOTOR__

#include "Arduino.h"

#include "Motor.h"
#include "SimpleEncoder.h"
#include "PIDController.h"


class EncoderMotor
{
	Motor& motor;
	SimpleEncoder& encoder;
	PIDController speed_pid;
	
	public:		// only for debugging, should be private
	
	bool backward;
	float target_speed_mm_per_s;
	float current_speed_mm_per_s;
	float last_control_loop_at_ms;
	float max_speed_mm_per_s;
	
	uint32_t last_control_loop_encoder_count;
	
	float distance_per_encoder_pulse_mm;
	uint32_t target_pulse_count;
	
	void calculate_speed();
	uint32_t last_speed_calculation_at_ms;
	uint32_t last_speed_calculation_encoder_count;
	
	void speed_control();
	
	public:
	EncoderMotor(Motor& motor, SimpleEncoder& encoder, uint8_t mm_per_revolution, uint8_t pulses_per_revolution, float max_speed_mm_per_s);
	
	void drive(int16_t speed_percent);
	void drive_ms(uint16_t duration_ms, int16_t speed_percent);
	void drive_distance(int32_t distance_mm, int16_t speed_mm_per_s, bool blocking = false);
	void stop();
	
	int32_t get_target_pulse_count();
	
	bool is_stopped();
	
	void tick();
	
	void wait_until_complete();
	
};


#endif