#ifdef __IN_ECLIPSE__
//This is a automatic generated file
//Please do not modify this file
//If you touch this file your change will be overwritten during the next build
//This file has been generated on 2024-05-04 16:55:07

#include "Arduino.h"
#include "Arduino.h"
#include <SPI.h>
#include <Ethernet.h>
#define ROSSERIAL_ARDUINO_TCP
#include <ros.h>
#include <ArduinoTcpHardware.h>
#include <std_msgs/String.h>
#include "ros/time.h"
#include "lino_msgs/Velocities.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int16.h"
#include "std_msgs/UInt16.h"
#include "std_msgs/Bool.h"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "lino_msgs/PLCRegister.h"
#include "lino_msgs/PLC_IO.h"
#include "lino_msgs/LidarStatus.h"
#include "lino_msgs/MotorStatus.h"
#include "lino_msgs/PID.h"
#include "lino_msgs/Imu.h"
#include "config/config.h"
#include "imu/Imu.h"
#include "PinchangeEncoder/PinchangeEncoder.h"
#include "motor/Motor.h"
#include "kinematics/Kinematics.h"
#include "pid/PID.h"
#include "ModbusRtu.h"
#include "LEDstatus/LEDStatus.h"
#include "FX1N/FX1N.h"
#include  "movingAvg.h"

void setup() ;
void loop() ;
void publishIMU() ;
void moveBase() ;
void stopBase() ;
int mapFloat(float x, float in_min, float in_max, float out_min, 		float out_max) ;
void publishLidarStatus() ;
void publishMotorStatus() ;
void publishBrake() ;
void publishPushButton() ;
void publishDock() ;


#include "NTUROBOT_1000V3_0.ino"

#endif
