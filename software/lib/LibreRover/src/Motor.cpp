#include "Motor.h"


void Motor::set_speed(uint8_t speed_percent)
{
	if (speed_percent > 100)
		speed_percent = 100;
	
	this->speed_percent = speed_percent;
	auto pwm = min_speed_pwm + this->speed_percent * (max_speed_pwm - min_speed_pwm) / 100;
	// Serial.println("Setting motor speed to " + String(speed_percent) + "%: " + String(pwm));
	analogWrite(this->pin_pwm, pwm);
}

uint8_t Motor::get_speed()
{
	return speed_percent;
}

Motor::Motor(uint8_t pin_forward, uint8_t pin_backward, uint8_t pin_pwm, uint8_t min_speed_pwm, uint8_t max_speed_pwm, bool reverse)
{
	if(reverse)
	{
		this->pin_forward = pin_backward;
		this->pin_backward = pin_forward;
	}
	else
	{
		this->pin_forward = pin_forward;
		this->pin_backward = pin_backward;
	}
	
	this->pin_pwm = pin_pwm;
	this->max_speed_pwm = max_speed_pwm;
	this->min_speed_pwm = min_speed_pwm;		// Offset for the minimum speed, to compensate for hardware limitations
	this->current_direction = 0;
	this->start_time_ms = 0;
	this->drive_duration_ms = 0;
	
	pinMode(this->pin_forward, OUTPUT);
	pinMode(this->pin_backward, OUTPUT);
	pinMode(this->pin_pwm, OUTPUT);
}

void Motor::forward(uint8_t speed_percent)
{
	current_direction = 1;
	set_speed(speed_percent);
	digitalWrite(this->pin_backward, LOW);
	digitalWrite(this->pin_forward, HIGH);
}

void Motor::backward(uint8_t speed_percent)
{
	current_direction = -1;
	set_speed(speed_percent);
	digitalWrite(this->pin_forward, LOW);
	digitalWrite(this->pin_backward, HIGH);
}
	
void Motor::stop()
{
	current_direction = 0;
	drive_duration_ms = 0;
	set_speed(0);
	digitalWrite(this->pin_forward, LOW);
	digitalWrite(this->pin_backward, LOW);
}

void Motor::drive(int16_t speed_percent)
{
	if(speed_percent > 0)
		forward(speed_percent);
	else if(speed_percent < 0)
		backward(-speed_percent);
	else
		stop();
}

void Motor::drive_ms(int16_t speed_percent, uint16_t duration_ms)
{
	drive_duration_ms = duration_ms;
	drive(speed_percent);
	start_time_ms = millis();
}

bool Motor::is_stopped()
{
	return current_direction == 0;
}

void Motor::tick()
{
	if(drive_duration_ms > 0)
	{
		if(millis() - start_time_ms >= drive_duration_ms)
		{
			stop();
		}
	}
}



