#include "Motor.h"

//ISR(TIMER4_COMPA_vect) {
//	digitalWrite(motor1_pin, HIGH);
//
//	digitalWrite(motor1_pin, LOW);
//	if (TCNT4 > OCR4A)
//		TCNT4 = 0;
//}

Controller::Controller() {
	invert_ = false;
	stopped_ = false;
}
;

void Controller::spin(int pwm) {
	if (stopped_)
		return;

	if (pwm < 0) {

		// driver older
		if (driver_ == ZDRV_C200) {
			digitalWrite(motor_pin_enable_, LOW);

			if (invert_)
				digitalWrite(motor_pin_dir_, HIGH);
			else
				digitalWrite(motor_pin_dir_, LOW);
		}
		// new driver
		else if (driver_ == ZDRV_C300_400L) {

			if (invert_) {

				digitalWrite(motor_pin_dir1_, HIGH);
				digitalWrite(motor_pin_dir2_, LOW);
			} else {
				digitalWrite(motor_pin_dir1_, LOW);
				digitalWrite(motor_pin_dir2_, HIGH);

			}
		}

		analogWrite(motor_pin_pwm_, abs(pwm));

	} else if (pwm > 0) {

		// driver older
		if (driver_ == ZDRV_C200) {
			digitalWrite(motor_pin_enable_, LOW);

			if (invert_)
				digitalWrite(motor_pin_dir_, LOW);
			else
				digitalWrite(motor_pin_dir_, HIGH);
		}
		// new driver
		else if (driver_ == ZDRV_C300_400L) {
			if (invert_) {
				digitalWrite(motor_pin_dir1_, LOW);
				digitalWrite(motor_pin_dir2_, HIGH);
			} else {
				digitalWrite(motor_pin_dir1_, HIGH);
				digitalWrite(motor_pin_dir2_, LOW);
			}
		}

		analogWrite(motor_pin_pwm_, abs(pwm));
	} else {

		analogWrite(motor_pin_pwm_, 0);
		if (driver_ == ZDRV_C200) {
			digitalWrite(motor_pin_enable_, HIGH);
			//digitalWrite(motor_pin_brake_, HIGH);
		} else if (driver_ == ZDRV_C300_400L) {
			digitalWrite(motor_pin_dir1_, LOW);
			digitalWrite(motor_pin_dir2_, LOW);
		}

	}
}

void Controller::stop(bool value) {
	stopped_ = value;

	if (value) {
		digitalWrite(motor_pin_brake_, LOW);
	} else {
		digitalWrite(motor_pin_brake_, HIGH);
	}
}

void Controller::setPinPWM(uint8_t motor_pin_pwm) {

	motor_pin_pwm_ = motor_pin_pwm;
	pinMode(motor_pin_pwm_, OUTPUT);
	//ensure that the motor is in neutral state during bootup
	analogWrite(motor_pin_pwm_, abs(0));

}

void Controller::setPinDir1(uint8_t motor_pin_dir1) {

	motor_pin_dir1_ = motor_pin_dir1;

	pinMode(motor_pin_dir1_, OUTPUT);
	digitalWrite(motor_pin_dir1_, LOW);
}

void Controller::setPinDir2(uint8_t motor_pin_dir2) {

	motor_pin_dir2_ = motor_pin_dir2;
	pinMode(motor_pin_dir2_, OUTPUT);
	digitalWrite(motor_pin_dir2_, LOW);
}

void Controller::setPinFault(uint8_t motor_pin_fault) {
	motor_pin_fault_ = motor_pin_fault;
	pinMode(motor_pin_fault_, INPUT);
}

void Controller::setPinReset(uint8_t motor_pin_reset) {
	pinMode(motor_pin_reset_, OUTPUT);
	digitalWrite(motor_pin_reset, LOW);
}

void Controller::setInvert(bool invert) {
	invert_ = invert;
}

void Controller::setBLDCDriver(BLDC_DRIVE driver) {
	driver_ = driver;
}
