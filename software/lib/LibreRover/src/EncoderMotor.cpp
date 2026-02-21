#include "EncoderMotor.h"


EncoderMotor::EncoderMotor(Motor& motor, SimpleEncoder& encoder, uint8_t mm_per_revolution, uint8_t pulses_per_revolution, float max_speed_mm_per_s)
	: motor(motor)
	, encoder(encoder)
	, speed_pid(0.2f, 0.1f, 0.005f)
{
	backward = false;
	target_speed_mm_per_s = 0;
	current_speed_mm_per_s = 0;
	last_control_loop_at_ms = 0;
	this->max_speed_mm_per_s = max_speed_mm_per_s;
	
	last_speed_calculation_at_ms = 0;
	last_speed_calculation_encoder_count = 0;
	speed_pid.set_output_limits(-100, 100);
	
	distance_per_encoder_pulse_mm = (float) mm_per_revolution / pulses_per_revolution;
	target_pulse_count = 0;
}

void EncoderMotor::drive(int16_t speed_percent)
{
	motor.drive(speed_percent);
}

void EncoderMotor::drive_ms(uint16_t duration_ms, int16_t speed_percent)
{
	motor.drive_ms(speed_percent, duration_ms);
}

void EncoderMotor::drive_distance(int32_t distance_mm, int16_t speed_mm_per_s, bool blocking)
{
	// encoder.reset();
	stop();
	backward = (distance_mm < 0) ^ (speed_mm_per_s < 0);
	
	target_pulse_count = encoder.get_pulse_count() +  abs(distance_mm) / distance_per_encoder_pulse_mm + 0.5;
	target_speed_mm_per_s = abs(speed_mm_per_s);
	
	// if(backward)
	// 	motor.drive(-abs(speed_percent));
	// else
	// 	motor.drive(abs(speed_percent));
	
	// The motor will be started in the tick() function by the control loop
	
	if(blocking)
	{
		wait_until_complete();
	}
}

void EncoderMotor::stop()
{
	motor.stop();
	target_pulse_count = 0;
	target_speed_mm_per_s = 0;
	current_speed_mm_per_s = 0;
	speed_pid.reset();
}

int32_t EncoderMotor::get_target_pulse_count()
{
	return target_pulse_count;
}

bool EncoderMotor::is_stopped()
{
	return target_pulse_count == 0 && motor.is_stopped();
}

void EncoderMotor::tick()
{	
	// Serial.println("EncoderMotor tick: " + String(encoder.get_pulse_count()) + " / " + String(target_pulse_count));
	if(target_pulse_count > 0)
	{
		if(encoder.get_pulse_count() >= target_pulse_count)
		{
			stop();		
		}
	}
	
	calculate_speed();
	speed_control();
	// Serial.println("  Current speed: " + String(current_speed_mm_per_s) + " mm/s");
	// speed_control();
	
	motor.tick();
}

void EncoderMotor::calculate_speed()
{
	auto now = millis();
	auto pulse_count = encoder.get_pulse_count();
	if(last_speed_calculation_at_ms == 0)
	{
		last_speed_calculation_at_ms = now;
		last_speed_calculation_encoder_count = pulse_count;
		current_speed_mm_per_s = 0;
		return;
	}
	
	auto dt_ms = now - last_speed_calculation_at_ms;
	auto delta_pulses = pulse_count - last_speed_calculation_encoder_count;
	
	if(dt_ms < 60)
		return;
		
	if(delta_pulses == 0)
	{
		if(dt_ms < 500)
			return;			// Set speed to 0 only if we haven't seen any pulses for a longer time
	}
		
	float new_speed_mm_per_s = delta_pulses * distance_per_encoder_pulse_mm * 1000. / dt_ms;
	current_speed_mm_per_s = 0.5f * current_speed_mm_per_s + 0.5f * new_speed_mm_per_s; // some filtering to reduce noise
	// current_speed_mm_per_s = 0.8f * current_speed_mm_per_s + 0.2f * new_speed_mm_per_s; // some filtering to reduce noise
	last_speed_calculation_encoder_count = pulse_count;
	last_speed_calculation_at_ms = now;
	// Serial.println("Speed calculation: delta pulses " + String(delta_pulses) + ", dt " + String(dt_ms) + " ms => speed " + String(current_speed_mm_per_s) + " mm/s");
}

void EncoderMotor::speed_control()
{
	if(target_speed_mm_per_s == 0)
		return;
		
	auto now = millis();
	if(last_control_loop_at_ms == 0)
		last_control_loop_at_ms = now;
	
	auto dt_ms = now - last_control_loop_at_ms;
	
	if(dt_ms < 60)
		return;
	
	float feed_forward = target_speed_mm_per_s / max_speed_mm_per_s * 100;	// very rough; it is very non-linear
	float pid_output = speed_pid.update(target_speed_mm_per_s, current_speed_mm_per_s);
	float new_setpoint = feed_forward + pid_output;
	new_setpoint = constrain(new_setpoint, 0, 100);
	
	// Serial.println(
	// 	"Speed control: target " + String(target_speed_mm_per_s) +
	// 	" mm/s, current " + String(current_speed_mm_per_s) +
	// 	" mm/s, new setpoint: " + String(new_setpoint) +
	// 	" % (" + String(feed_forward) + "% feed-forward, " + String(pid_output) + "% PID)"
	// );

	// motor.set_speed(pid_output);
	motor.drive(backward ? -new_setpoint : new_setpoint);
		
	// WIP
	
		
	
	// unsigned long now_ms = millis();
	// float dt_s = (now_ms - last_control_loop_at_ms) / 1000.0f;
	// last_control_loop_at_ms = now_ms;
	
	// float actual_speed_mm_per_s = encoder.get_pulse_count() * distance_per_encoder_pulse_mm / dt_s;
	// current_speed_mm_per_s = 0.5f * current_speed_mm_per_s + 0.5f * actual_speed_mm_per_s; // low-pass filter
	
	// int16_t speed_percent = (int16_t) (current_speed_mm_per_s / target_speed_mm_per_s * 255);
	// if(backward)
	// 	speed_percent = -speed_percent;
	
	// motor.set_speed(abs(speed_percent));
}

void EncoderMotor::wait_until_complete()
{
	if(target_pulse_count == 0)
		return;
	
	while(!is_stopped())
	{
		tick();
	}
}


