/*
 * config.h
 *
 *  Created on: Oct 3, 2020
 *      Author: mamoto
 */

#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

#define PIN_LED_STATUS_YELLOW 48
#define PIN_LED_STATUS_BLUE 49
#define USE_MPU9250_IMU

enum BLDC_DRIVE{
	ZDRV_C200 = 0,
	ZDRV_C300_400L
};

// encoder
#define PIN_ENCODER_MOTOR2_U    3
#define PIN_ENCODER_MOTOR2_V   13
#define PIN_ENCODER_MOTOR2_W   A9


#define PIN_ENCODER_MOTOR1_U   19
#define PIN_ENCODER_MOTOR1_V   18
#define PIN_ENCODER_MOTOR1_W   2

// control motor
#define PIN_CONTROL_MOTOR1_PULL 5
#define PIN_CONTROL_MOTOR1_DIR A13
#define PIN_CONTROL_MOTOR1_DIR1 6
#define PIN_CONTROL_MOTOR1_DIR2 7
#define PIN_CONTROL_MOTOR1_ENABLE A12
#define PIN_CONTROL_MOTOR1_BRAKE 4
#define PIN_CONTROL_MOTOR1_FAULT 40
#define PIN_CONTROL_MOTOR1_RESET 39

#define PIN_CONTROL_MOTOR2_PULL 12
#define PIN_CONTROL_MOTOR2_DIR 11
#define PIN_CONTROL_MOTOR2_DIR1 41
#define PIN_CONTROL_MOTOR2_DIR2 38
#define PIN_CONTROL_MOTOR2_ENABLE 10
#define PIN_CONTROL_MOTOR2_BRAKE 8
#define PIN_CONTROL_MOTOR2_FAULT 47
#define PIN_CONTROL_MOTOR2_RESET 9

#define COUNTS_PER_REV 3000.0 // wheel encoder's no of ticks per rev


//uncomment the base you're building
#define LINO_BASE DIFFERENTIAL_DRIVE // 2WD and Tracked robot w/ 2 motors
// #define LINO_BASE SKID_STEER      // 4WD robot
// #define LINO_BASE ACKERMANN       // Car-like steering robot w/ 2 motors
// #define LINO_BASE ACKERMANN1      // Car-like steering robot w/ 1 motor
// #define LINO_BASE MECANUM         // Mecanum drive robot

#define K_P 1.0//4.2// 4.8// 0.6 // 2.64 P constant //4.8
#define K_I 0.4 //0.3 // 0.22 I constant //8.6
#define K_D 0.05//0.5 // 0.79 D constant //8.4

//define your robot' specs here
#define MAX_RPM 60.0               // motor's maximum RPM
#define MIN_RPM -60.0               // motor's maximum RPM

#define WHEEL_DIAMETER 0.2        //0.1478 wheel's diameter in meters
#define PWM_BITS 8                // PWM Resolution of the microcontroller
#define LR_WHEELS_DISTANCE 0.474 //  474 LIDAR 0.265  // distance between left and right wheels
#define FR_WHEELS_DISTANCE 0.234   // distance between front and rear wheels. Ignore this if you're on 2WD/ACKERMANN
#define MAX_STEERING_ANGLE 0.415  // max steering angle. This only applies to Ackermann steering

#define PWM_MAX (pow(2, PWM_BITS) - 1)
#define PWM_MIN -PWM_MAX


// IO
#define PIN_IO_BRAKE A4
#define PIN_IO_TASK_FINISH A3
#define PIN_IO_LED 29
// Battery
#define adc_data_pin A5
#define adc_clk_pin A6
#define adc_cs_pin A7

#define PIN_IO_LIDAR_1 28
#define PIN_IO_LIDAR_2 29

#define PIN_IO_SIGNAL_DOCK 25
#define PIN_IO_POWER_DOCK 42
#define PIN_IO_POWER_MOTOR_1 44
#define PIN_IO_POWER_MOTOR_2 43
#define PIN_IO_POWER_LIDAR 45


// LiDAR

//Conveyor

#define FX1Nbaud_  9600
#define FX1Nformat_  SERIAL_7E2
#define FX1Ntimeout_ 1000


#endif /* CONFIG_CONFIG_H_ */
