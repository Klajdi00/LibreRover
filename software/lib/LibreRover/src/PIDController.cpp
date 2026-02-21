#include "PIDController.h"


PIDController::PIDController(float kp, float ki, float kd)
	: kp(kp)
	, ki(ki)
	, kd(kd)
	, integral(0.0f)
	, prev_error(0.0f)
	, last_output(0.0f)
	, last_time_ms(0)
	, has_prev(false)
	, out_min(0.0f)
	, out_max(0.0f)
	, limit_enabled(false)
{
}

void PIDController::set_tunings(float kp, float ki, float kd)
{
	this->kp = kp;
	this->ki = ki;
	this->kd = kd;
}

void PIDController::set_output_limits(float min_output, float max_output)
{
	out_min = min_output;
	out_max = max_output;
	limit_enabled = true;
	clamp_integral();
}

void PIDController::clear_output_limits()
{
	limit_enabled = false;
}

void PIDController::reset()
{
	integral = 0.0f;
	prev_error = 0.0f;
	last_output = 0.0f;
	last_time_ms = 0;
	has_prev = false;
}

float PIDController::apply_limits(float value) const
{
	if(!limit_enabled)
		return value;

	if(value > out_max)
		return out_max;
	if(value < out_min)
		return out_min;
	return value;
}

void PIDController::clamp_integral()
{
	if(!limit_enabled || ki == 0.0f)
		return;

	float min_i = out_min / ki;
	float max_i = out_max / ki;
	if(min_i > max_i)
	{
		float temp = min_i;
		min_i = max_i;
		max_i = temp;
	}

	integral = constrain(integral, min_i, max_i);
}

float PIDController::update(float setpoint, float measurement)
{
	unsigned long now = millis();
	float error = setpoint - measurement;

	if(!has_prev)
	{
		has_prev = true;
		last_time_ms = now;
		prev_error = error;
		last_output = apply_limits(kp * error);
		return last_output;
	}

	unsigned long delta_ms = now - last_time_ms;
	if(delta_ms == 0)
		return last_output;

	float dt = delta_ms / 1000.0f;

	integral += error * dt;
	clamp_integral();

	float derivative = (error - prev_error) / dt;

	float output = (kp * error) + (ki * integral) + (kd * derivative);
	output = apply_limits(output);

	prev_error = error;
	last_time_ms = now;
	last_output = output;

	return output;
}
