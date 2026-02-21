#ifndef __PID_CONTROLLER__
#define __PID_CONTROLLER__

#include "Arduino.h"


class PIDController
{
	float kp;
	float ki;
	float kd;

	float integral;
	float prev_error;
	float last_output;
	unsigned long last_time_ms;
	bool has_prev;

	float out_min;
	float out_max;
	bool limit_enabled;

	float apply_limits(float value) const;
	void clamp_integral();

	public:
	PIDController(float kp, float ki, float kd);

	void set_tunings(float kp, float ki, float kd);
	void set_output_limits(float min_output, float max_output);
	void clear_output_limits();

	void reset();

	float update(float setpoint, float measurement);
};


#endif
