#include "Arduino.h"
#include "PID.h"

PID::PID(float min_val, float max_val, float kp, float ki, float kd) :
		min_val_(min_val), max_val_(max_val), kp_(kp), ki_(ki), kd_(kd) {
	integral_ = 0;
	prev_error_ = 0;
	derivative_ = 0;
	pOnE = 1;
	outputSum = 0;
	lastInput = 0;

}

float PID::compute(float setpoint, float measured_value) {
	/*
	 float error;
	 float pid;

	 //setpoint is constrained between min and max to prevent pid from having too much error
	 error = setpoint - measured_value;
	 integral_ += error;
	 derivative_ = error - prev_error_;

	 if(setpoint == 0 && error == 0)
	 {
	 integral_ = 0;
	 }

	 pid = (kp_ * error) + (ki_ * integral_) + (kd_ * derivative_);
	 prev_error_ = error;


	 return constrain(pid, min_val_, max_val_);
	 */

	/*Compute all the working error variables*/
	double input = measured_value;
	double error = setpoint - input;
	double dInput = (input - lastInput);
	outputSum += (ki_ * error);

	/*Add Proportional on Measurement, if P_ON_M is specified*/
	//if(!pOnE) outputSum-= kp_ * dInput;
	if (outputSum > max_val_)
		outputSum = max_val_;
	else if (outputSum < min_val_)
		outputSum = min_val_;

	/*Add Proportional on Error, if P_ON_E is specified*/
	double output;
	if (pOnE)
		output = kp_ * error;
	else
		output = 0;

	/*Compute Rest of PID Output*/
	output += outputSum - kd_ * dInput;

	if (output > max_val_)
		output = max_val_;
	else if (output < min_val_)
		output = min_val_;

	/*Remember some variables for next time*/
	lastInput = input;

	return output;

}

void PID::updateConstants(float kp, float ki, float kd) {
	kp_ = kp;
	ki_ = ki;
	kd_ = kd;
}

void PID::resetPID() {
	integral_ = 0;
	prev_error_ = 0;
	derivative_ = 0;
	outputSum = 0;
	lastInput = 0;
}
