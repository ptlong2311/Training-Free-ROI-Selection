#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include "../config/config.h"

class Controller {
public:
	Controller();

	void setPinPWM(uint8_t motor_pin_pwm);
	void setPinDir1(uint8_t motor_pin_dir1);
	void setPinDir2(uint8_t motor_pin_dir2);
	void setPinFault(uint8_t motor_pin_fault);
	void setPinReset(uint8_t motor_pin_reset);
	void setInvert(bool invert);

	void spin(int pwm);
	void stop(bool value);

	void setBLDCDriver(BLDC_DRIVE driver);
private:

	uint8_t motor_pin_pwm_;
	uint8_t motor_pin_dir_;

	uint8_t motor_pin_dir1_;
	uint8_t motor_pin_dir2_;

	uint8_t motor_pin_fault_;
	uint8_t motor_pin_reset_;

	uint8_t motor_pin_enable_;
	uint8_t motor_pin_brake_;
	bool invert_;
	bool stopped_;
	BLDC_DRIVE driver_ = ZDRV_C200;

};

#endif
