#include "Arduino.h"
#include <SPI.h>
#include <Ethernet.h>
// To use the TCP version of rosserial_arduino
#define ROSSERIAL_ARDUINO_TCP

#include <ros.h>
#include <ArduinoTcpHardware.h>
#include <std_msgs/String.h>

#include "ros/time.h"
//header file for publishing velocities for odom
#include "lino_msgs/Velocities.h"
#include "std_msgs/Float32.h"
#include "std_msgs/Int16.h"
#include "std_msgs/UInt16.h"
#include "std_msgs/Bool.h"

//header file for cmd_subscribing to "cmd_vel"
#include "geometry_msgs/Twist.h"
#include "geometry_msgs/Pose.h"
#include "geometry_msgs/PoseStamped.h"
#include "geometry_msgs/PoseWithCovarianceStamped.h"
#include "lino_msgs/PLCRegister.h"
#include "lino_msgs/PLC_IO.h"
#include "lino_msgs/LidarStatus.h"
#include "lino_msgs/MotorStatus.h"

//header file for pid server
#include "lino_msgs/PID.h"
//header file for imu
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

#define IMU_PUBLISH_RATE 25 // ms //40.0 //hz
#define ETHERNET_PUBLISH_RATE 100 //ms//1hz
#define ROSCONNECT_PUBLISH_RATE 1000 // ms 1hz
#define COMMAND_RATE 40 //ms 15hz
#define BRAKE_RATE 20.0 //hz
#define PUSH_BUTTON_RATE 30.0 //hz
#define MODBUST_RTU_RATE 10.0 //hz
#define LED_STATUS_RATE 33.33 //hz
#define BATTERY_RATE 1.0 //hz
#define LIDAR_STATUS_RATE 1.0 //hz
#define MOTOR_STATUS_RATE 1.0 //hz

#define CONVEYOR_RATE 10.0 //hz
#define DOCK_RATE 10.0 //hz

// Set the shield settings
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 10, 105);

// Set the rosserial socket server IP address
IPAddress server(192, 168, 10, 4);

// Set the rosserial socket server port
const uint16_t serverPort = 11411;

// encoder
PinchangeEncoder motor1_encoder(PIN_ENCODER_MOTOR1_U, PIN_ENCODER_MOTOR1_V,
PIN_ENCODER_MOTOR1_W, COUNTS_PER_REV);
PinchangeEncoder motor2_encoder(PIN_ENCODER_MOTOR2_U, PIN_ENCODER_MOTOR2_V,
PIN_ENCODER_MOTOR2_W, COUNTS_PER_REV, true);

// controller
Controller motor1_controller;
Controller motor2_controller;

// kinematic
Kinematics kinematics(Kinematics::LINO_BASE, MAX_RPM, WHEEL_DIAMETER,
FR_WHEELS_DISTANCE, LR_WHEELS_DISTANCE);

// PID
PID motor1_pid(MIN_RPM, MAX_RPM, K_P, K_I, K_D);
PID motor2_pid(MIN_RPM, MAX_RPM, K_P, K_I, K_D); // quay nhanh hon
LEDStatus led_status(PIN_IO_LED);

float g_req_linear_vel_x = 0;
float g_req_linear_vel_y = 0;
float g_req_angular_vel_z = 0;
unsigned long g_prev_command_time = 0;
unsigned long acce_prev_command_time = 0;

bool last_state_charging = false;
bool last_docking_state = false;

ros::NodeHandle nh;
// Make a IMU publisher

lino_msgs::Imu raw_imu_msg;
ros::Publisher raw_imu_pub("raw_imu", &raw_imu_msg);

//callback function prototypes
void commandCallback(const geometry_msgs::Twist &cmd_msg);
void PIDCallback(const lino_msgs::PID &pid);
void stopBase();
//void publishBattery();
void publishLidarStatus();
void publishMotorStatus();

void publishBrake();
void publishPushButton();
void publishDock();

void voiceCallback(const std_msgs::UInt16 &voice_msg);
void volumeCallback(const std_msgs::UInt16 &volume_msg);
void statusCallback(const std_msgs::UInt16 &volume_msg);

void powerChargingCallback(const std_msgs::Bool &msg);
void powerLidarCallback(const std_msgs::Bool &msg);
void PCLRegisterCallback(const lino_msgs::PLCRegister &msg);
void PCLIOCallback(const lino_msgs::PLC_IO &msg);
void chargingCallback(const std_msgs::Bool &msg);
void dockingStateCallback(const std_msgs::String &msg);


int mapFloat(float x, float in_min, float in_max, float out_min, float out_max);

ros::Subscriber<geometry_msgs::Twist> cmd_sub("cmd_vel", commandCallback);
ros::Subscriber<lino_msgs::PID> pid_sub("pid", PIDCallback);
ros::Subscriber<std_msgs::Bool> lidar_power_sub("lidar/power",
		powerLidarCallback);

lino_msgs::Velocities raw_vel_msg;
ros::Publisher raw_vel_pub("raw_vel", &raw_vel_msg);

std_msgs::Bool push_button_msg;
ros::Publisher push_button_pub("push_button", &push_button_msg);

std_msgs::Bool brake_msg;
ros::Publisher brake_pub("brake", &brake_msg);

lino_msgs::LidarStatus lidar_status_msg;
ros::Publisher lidar_status_pub("lidar/status", &lidar_status_msg);

lino_msgs::MotorStatus motor_status_msg;
ros::Publisher motor_status_pub("motor/status", &motor_status_msg);

ros::Subscriber<std_msgs::UInt16> voice_sub("voice", voiceCallback);
ros::Subscriber<std_msgs::UInt16> volume_sub("volume", volumeCallback);
ros::Subscriber<std_msgs::UInt16> status_sub("status_robot", statusCallback);
ros::Subscriber<std_msgs::Bool> powerCharing_sub("power_charging",
		powerChargingCallback);

ros::Subscriber<std_msgs::String> docking_state_sub("docking/state",
		dockingStateCallback);

//std_msgs::Float32 raw_battery_volt;
//ros::Publisher raw_bat_pub("battery", &raw_battery_volt);

std_msgs::Bool dock_msg;
ros::Publisher dock_pub("dock_station", &dock_msg);

ros::Subscriber<lino_msgs::PLCRegister> plc_register_sub("PLC_register_set_get",
		PCLRegisterCallback);

lino_msgs::PLCRegister plc_register_msg;
ros::Publisher pcl_register_pub("PLC_register", &plc_register_msg);

ros::Subscriber<lino_msgs::PLC_IO> plc_io_sub("PLC_IO_set_get", PCLIOCallback);

lino_msgs::PLC_IO plc_io_msg;
ros::Publisher plc_io_pub("PLC_IO", &plc_io_msg);

ros::Subscriber<std_msgs::Bool> charging_sub("charging", chargingCallback);

//ros::Subscriber<std_msgs::Float32> wheels_diameter_sub("wheel_diameter",
//		wheelDiameterCallback);
//ros::Subscriber<std_msgs::Float32> wheels_y_distance_sub("wheel_y_distance",
//		wheelsYDistanceCallback);

//FX1N fx1n(&Serial3, FX1Nbaud_, FX1Nformat_, FX1Ntimeout_);

void (*resetFunc)(void)=0;

/**
 *  Modbus object declaration
 *  u8id : node id = 0 for master, = 1..247 for slave
 *  port : serial port
 *  u8txenpin : 0 for RS-232 and USB-FTDI
 *               or any pin number > 1 for RS-485
 */
//Modbus master(0, Serial2, 4); // this is master and RS-232 or USB-FTDI
/**
 * This is an structe which contains a query to an slave device
 */
modbus_t telegram_voice;
modbus_t telegram_volume;

// data array for modbus network sharing
uint16_t au16data_voice[16];
uint16_t au16data_volume[16];

uint16_t last_voice_value;
uint16_t last_volume_value;

int last_brake_state = HIGH;
int brake_state;
int last_push_button_state = HIGH;
int push_button_state;
long last_time_push_button;
long last_time_brake_value;

uint16_t last_converyor = 0;
movingAvg movingBat(5);

void setup() {

	// LED STATUS
	pinMode(PIN_LED_STATUS_YELLOW, OUTPUT);
	pinMode(PIN_LED_STATUS_BLUE, OUTPUT);
	digitalWrite(PIN_LED_STATUS_YELLOW, HIGH);
	digitalWrite(PIN_LED_STATUS_BLUE, HIGH);

	led_status.runCompleteStatus(RB_STARTING);

	// khoi tao motor controller 1
	motor1_controller.setPinPWM(PIN_CONTROL_MOTOR1_PULL);
	motor1_controller.setPinDir1(PIN_CONTROL_MOTOR1_DIR1);
	motor1_controller.setPinDir2(PIN_CONTROL_MOTOR1_DIR2);
	motor1_controller.setPinFault(PIN_CONTROL_MOTOR1_FAULT);
	motor1_controller.setPinReset(PIN_CONTROL_MOTOR1_RESET);
	motor1_controller.setInvert(false);
	motor1_controller.setBLDCDriver(ZDRV_C300_400L);

	// khoi tao motor controller 2
	motor2_controller.setPinPWM(PIN_CONTROL_MOTOR2_PULL);
	motor2_controller.setPinDir1(PIN_CONTROL_MOTOR2_DIR1);
	motor2_controller.setPinDir2(PIN_CONTROL_MOTOR2_DIR2);
	motor2_controller.setPinFault(PIN_CONTROL_MOTOR2_FAULT);
	motor2_controller.setPinReset(PIN_CONTROL_MOTOR2_RESET);
	motor2_controller.setInvert(true);
	motor2_controller.setBLDCDriver(ZDRV_C300_400L);

	/*


	 // power motor
	 pinMode(PIN_IO_POWER_MOTOR_1, OUTPUT);
	 pinMode(PIN_IO_POWER_MOTOR_2, OUTPUT);

	 digitalWrite(PIN_IO_POWER_MOTOR_1, HIGH);
	 digitalWrite(PIN_IO_POWER_MOTOR_2, HIGH);

	 // motor status
	 pinMode(PIN_CONTROL_MOTOR1_FAULT, INPUT);
	 pinMode(PIN_CONTROL_MOTOR2_FAULT, INPUT);
	 */
	delay(200);
	digitalWrite(PIN_LED_STATUS_YELLOW, LOW);
	digitalWrite(PIN_LED_STATUS_BLUE, LOW);
	/*
	 // brake
	 pinMode(PIN_IO_BRAKE, INPUT_PULLUP);
	 brake_state = digitalRead(PIN_IO_BRAKE);
	 last_time_brake_value = millis();
	 // push button
	 pinMode(PIN_IO_TASK_FINISH, INPUT_PULLUP);
	 push_button_state = digitalRead(PIN_IO_TASK_FINISH);
	 last_time_push_button = millis();

	 // lidar status
	 pinMode(PIN_IO_LIDAR_1, INPUT);
	 pinMode(PIN_IO_LIDAR_2, INPUT);

	 // charging
	 pinMode(PIN_IO_POWER_DOCK, OUTPUT);
	 digitalWrite(PIN_IO_POWER_DOCK, LOW); // uncontact relay NC
	 pinMode(PIN_IO_SIGNAL_DOCK, INPUT_PULLUP);

	 // lidar power
	 pinMode(PIN_IO_POWER_LIDAR, OUTPUT);
	 digitalWrite(PIN_IO_POWER_LIDAR, HIGH); // uncontact relay NC

	 Serial2.begin(9600); // baud-rate at 19200
	 pinMode(4, OUTPUT);
	 digitalWrite(4, HIGH);
	 master.start();
	 master.setTimeOut(2000); // if there is no answer in 2000 ms, roll over

	 au16data_voice[3] = 0;
	 telegram_voice.u8id = 1; // slave address
	 telegram_voice.u8fct = 6; // function code (this one is registers read)
	 telegram_voice.u16RegAdd = 3; // start address in slave
	 telegram_voice.u16CoilsNo = 1; // number of elements (coils or registers) to read
	 telegram_voice.au16reg = au16data_voice + 3; // pointer to a memory array in the Arduino

	 master.query(telegram_voice); // send query (only once)

	 au16data_volume[4] = 28;

	 movingBat.begin();
	 */
	// Use serial to monitor the process
	Serial.begin(115200);

	// You can use Ethernet.init(pin) to configure the CS pin
	Ethernet.init(53);

	// Connect the Ethernet
	Ethernet.begin(mac, ip);

	// Check for Ethernet hardware present
	if (Ethernet.hardwareStatus() == EthernetNoHardware) {
		Serial.println(
				"Ethernet shield was not found.  Sorry, can't run without hardware. :(");
		while (true) {
			delay(1); // do nothing, no point running without Ethernet hardware
		}
	}

	// Let some time for the Ethernet Shield to be initialized
	delay(1000);
	while (Ethernet.linkStatus() == EthernetLinkStatus::LinkOFF) {
		digitalWrite(PIN_LED_STATUS_YELLOW, HIGH);
		delay(100);
		digitalWrite(PIN_LED_STATUS_YELLOW, LOW);
		delay(100);
		digitalWrite(PIN_LED_STATUS_YELLOW, HIGH);
		delay(100);
		digitalWrite(PIN_LED_STATUS_YELLOW, LOW);
		delay(100);
		digitalWrite(PIN_LED_STATUS_YELLOW, HIGH);
		delay(100);
		digitalWrite(PIN_LED_STATUS_YELLOW, LOW);
		Ethernet.begin(mac, ip);
		Serial.println("Ethernet cable is not connected.");

		delay(400);
	}

	Serial.println("");
	Serial.println("Ethernet connected");
	Serial.println("IP address: ");
	Serial.println(Ethernet.localIP());

	// Set the connection to rosserial socket server
	nh.getHardware()->setConnection(server, serverPort);
	nh.initNode();

	// Another way to get IP
	Serial.print("IP = ");
	Serial.println(nh.getHardware()->getLocalIP());

	// Start to be polite
	nh.advertise(raw_imu_pub);
	nh.subscribe(pid_sub);
	nh.subscribe(cmd_sub);
	nh.advertise(raw_vel_pub);
	nh.advertise(motor_status_pub);
	/*
	 nh.advertise(brake_pub);
	 nh.subscribe(voice_sub);
	 nh.subscribe(volume_sub);
	 nh.subscribe(status_sub);
	 //nh.advertise(raw_bat_pub);
	 nh.advertise(lidar_status_pub);

	 nh.advertise(push_button_pub);
	 nh.subscribe(plc_io_sub);
	 nh.advertise(plc_io_pub);
	 nh.subscribe(powerCharing_sub);
	 nh.advertise(dock_pub);
	 nh.subscribe(lidar_power_sub);
	 nh.subscribe(plc_register_sub);
	 nh.advertise(pcl_register_pub);
	 nh.subscribe(charging_sub);
	 nh.subscribe(docking_state_sub);

	 last_voice_value = 0;
	 last_volume_value = 28;

	 // battery
	 adc0832_setup(
	 adc_data_pin,
	 adc_cs_pin,
	 adc_clk_pin);
	 */

	// thuc thi led bao san sang
	int repeater = 0;
	while (repeater < 10) {
		led_status.update();
		repeater++;
		delay(100);
	}

	led_status.runCompleteStatus(RB_READY);

}

void loop() {
	static unsigned long prev_imu_time = 0;
	static unsigned long prev_ethernet_time = 0;
	static unsigned long prev_rosconnect_time = 0;
	static unsigned long prev_control_time = 0;
	static unsigned long prev_brake_time = 0;
	static unsigned long prev_push_button_time = 0;
	static unsigned long prev_modbus_rtu_time = 0;
	static unsigned long prev_led_status_time = 0;
	//static unsigned long prev_battery_time = 0;
	static unsigned long prev_lidar_status_time = 0;
	static unsigned long prev_motor_status_time = 0;

	//static unsigned long prev_conveyor_time = 0;
	static unsigned long prev_dock_time = 0;

	unsigned long current_milis = millis();

	static bool imu_is_initialized;

	if (current_milis - prev_rosconnect_time >= ROSCONNECT_PUBLISH_RATE) {
		if (nh.connected()) {
			digitalWrite(PIN_LED_STATUS_BLUE,
					!digitalRead(PIN_LED_STATUS_BLUE));
		} else {
			digitalWrite(PIN_LED_STATUS_BLUE, LOW);
			stopBase();
			// waiting for reconnect
		}
		prev_rosconnect_time = current_milis;
	}

	//this block drives the robot based on defined rate
	if ((current_milis - prev_control_time) >= COMMAND_RATE) {
		moveBase();
		prev_control_time = current_milis;

	}

	//this block stops the motor when no command is received
	if ((current_milis - g_prev_command_time) > 500) {
		stopBase();
	}
	/*
	 if ((millis() - prev_modbus_rtu_time) >= (1000.0 / MODBUST_RTU_RATE)) {
	 master.poll();
	 prev_modbus_rtu_time = millis();
	 }
	 */

	//this block publishes the IMU data based on defined rate
	if ((current_milis - prev_imu_time) >= 25) {
		//sanity check if the IMU is connected
		if (!imu_is_initialized) {
			imu_is_initialized = initIMU();

			if (imu_is_initialized)
				Serial.println("IMU Initialized");
			else
				Serial.println(
						"IMU failed to initialize. Check your IMU connection.");
		} else {
			publishIMU();
		}
		prev_imu_time = current_milis;
	}
	/*
	 if (millis() - prev_brake_time >= (1000.0 / BRAKE_RATE)) {

	 publishBrake();
	 prev_brake_time = millis();
	 }

	 if (millis() - prev_push_button_time >= (1000.0 / PUSH_BUTTON_RATE)) {
	 publishPushButton();
	 prev_push_button_time = millis();
	 }
	 */
	if (current_milis - prev_led_status_time >= LED_STATUS_RATE) {
		led_status.update();
		prev_led_status_time = current_milis;
	}
	/*
	 // battery
	 //	if ((millis() - prev_battery_time) >= (1000.0 / BATTERY_RATE)) {
	 //		publishBattery();
	 //		prev_battery_time = millis();
	 //	}

	 // lidar status
	 if ((millis() - prev_lidar_status_time) >= (1000.0 / LIDAR_STATUS_RATE)) {
	 publishLidarStatus();
	 prev_lidar_status_time = millis();
	 }

	 // motor status
	 if ((millis() - prev_motor_status_time) >= (1000.0 / MOTOR_STATUS_RATE)) {
	 publishMotorStatus();
	 prev_motor_status_time = millis();
	 }
	 */
	// conveyor
	/* if ((millis() - prev_conveyor_time) >= (1000.0 / CONVEYOR_RATE)) {
	 publishConveyor();
	 prev_conveyor_time = millis();
	 }*/
	/*
	 // dock
	 if ((millis() - prev_dock_time) >= (1000.0 / DOCK_RATE)) {
	 publishDock();
	 prev_dock_time = millis();
	 }
	 */

	nh.spinOnce();

}

void publishIMU() {
//pass accelerometer data to imu object
	raw_imu_msg.linear_acceleration = readAccelerometer();

//pass gyroscope data to imu object
	raw_imu_msg.angular_velocity = readGyroscope();

//pass accelerometer data to imu object
	raw_imu_msg.magnetic_field = readMagnetometer();

//publish raw_imu_msg
	raw_imu_pub.publish(&raw_imu_msg);
}

void PIDCallback(const lino_msgs::PID &pid) {
//callback function every time PID constants are received from lino_pid for tuning
//this callback receives pid object where P,I, and D constants are stored
	motor1_pid.updateConstants(pid.p, pid.i, pid.d);
	motor2_pid.updateConstants(pid.p, pid.i, pid.d);

}

void commandCallback(const geometry_msgs::Twist &cmd_msg) {
//callback function every time linear and angular speed is received from 'cmd_vel' topic
//this callback function receives cmd_msg object where linear and angular speed are stored

	//if (brake_state != LOW) // non breaking
	{
		g_req_linear_vel_x = cmd_msg.linear.x;
		g_req_linear_vel_y = cmd_msg.linear.y;
		g_req_angular_vel_z = cmd_msg.angular.z;

		g_prev_command_time = millis();

	}

}

void moveBase() {

	//get the required rpm for each motor based on required velocities, and base used
	Kinematics::rpm req_rpm = kinematics.getRPM(g_req_linear_vel_x,
			g_req_linear_vel_y, g_req_angular_vel_z);

//	Serial.print("Kinematics::req_rpm 1:");
//	Serial.println(req_rpm.motor1);
//	Serial.print("Kinematics::req_rpm 2:");
//	Serial.println(req_rpm.motor2);

//get the current speed of each motor
	 float current_rpm1 = motor1_encoder.getRPM();
	 float current_rpm2 = motor2_encoder.getRPM();

	 float current_rpm3 = 0;	// motor3_encoder.getRPM();
	 float current_rpm4 = 0;	//motor4_encoder.getRPM();

//the required rpm is capped at -/+ MAX_RPM to prevent the PID from having too much error
//the PWM value sent to the motor driver is the calculated PID based on required RPM vs measured RPM
	float val1 = float(motor1_pid.compute(req_rpm.motor1, current_rpm1));
	float val2 = float(motor2_pid.compute(req_rpm.motor2, current_rpm2));

	Serial.print(current_rpm1);
	Serial.print(",");
	Serial.println(current_rpm2);

	int pwm1 = mapFloat(val1, MIN_RPM, MAX_RPM, PWM_MIN, PWM_MAX);
	int pwm2 = mapFloat(val2, MIN_RPM, MAX_RPM, PWM_MIN, PWM_MAX);

//	Serial.print("pwm1:");
//	Serial.println(pwm1);
//
//	Serial.print("pwm2:");
//	Serial.println(pwm2);

	motor2_controller.spin(pwm2);
	motor1_controller.spin(pwm1);

	Kinematics::velocities current_vel;

	current_vel = kinematics.getVelocities(current_rpm1, current_rpm2,
			current_rpm3, current_rpm4);

//pass velocities to publisher object
	raw_vel_msg.linear_x = current_vel.linear_x;
	raw_vel_msg.linear_y = current_vel.linear_y;
	raw_vel_msg.angular_z = current_vel.angular_z;

//publish raw_vel_msg
	raw_vel_pub.publish(&raw_vel_msg);
}

void stopBase() {
	g_req_linear_vel_x = 0;
	g_req_linear_vel_y = 0;
	g_req_angular_vel_z = 0;

	//motor1_controller.spin(0);
	//motor2_controller.spin(0);

	// reset PID
	motor1_pid.resetPID();
	motor2_pid.resetPID();

}
int mapFloat(float x, float in_min, float in_max, float out_min,
		float out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void voiceCallback(const std_msgs::UInt16 &voice_msg) {

	if (last_voice_value != voice_msg.data) {
		au16data_voice[3] = voice_msg.data;
		last_voice_value = voice_msg.data;

		telegram_voice.u8id = 1; // slave address
		telegram_voice.u8fct = 6; // function code (this one is registers read)
		telegram_voice.u16RegAdd = 3; // start address in slave
		telegram_voice.u16CoilsNo = 1; // number of elements (coils or registers) to read
		telegram_voice.au16reg = au16data_voice + 3; // pointer to a memory array in the Arduino

		//master.query(telegram_voice); // send query (only once)
	}

}
void volumeCallback(const std_msgs::UInt16 &volume_msg) {
	if (last_volume_value != volume_msg.data) {
		au16data_volume[4] = volume_msg.data;
		last_volume_value = volume_msg.data;

		telegram_volume.u8id = 1; // slave address
		telegram_volume.u8fct = 6; // function code (this one is registers read)

		telegram_volume.u16RegAdd = 4; // start address in slave
		telegram_volume.u16CoilsNo = 1; // number of elements (coils or registers) to read
		telegram_volume.au16reg = au16data_volume + 4; // pointer to a memory array in the Arduino

		//master.query(telegram_volume); // send query (only once)
	}

}

void statusCallback(const std_msgs::UInt16 &status_msg) {
	ROBOT_STATUS status = RB_NONE;

	/*char buffer[50];

	 sprintf(buffer, "Status receiver  : %d", status_msg.data);
	 Serial.println(buffer);
	 */
	switch ((int) status_msg.data) {
	case 0:
		status = RB_NONE;
		break;
	case 1:
		status = RB_STARTING;
		break;
	case 2:
		status = RB_READY;
		break;
	case 3:
		status = RB_MOVING;
		break;
	case 4:
		status = RB_COMPLETED;
		break;
	case 5:
		status = RB_BUTTON_FINISH;
		break;
	case 6:
		status = RB_BRAKE;
		break;
	case 7:
		status = RB_IGNORE;
		break;
	case 8:
		status = RB_FALL;
		break;
	default:
		status = RB_NONE;
		break;
	}
//	if (led_status.getStatus() != RB_BRAKE)
//		led_status.setStatus(status);
}



void publishLidarStatus() {
	lidar_status_msg.lidar_front = digitalRead(PIN_IO_LIDAR_1);
	lidar_status_msg.lidar_rear = digitalRead(PIN_IO_LIDAR_2);

	lidar_status_pub.publish(&lidar_status_msg);
}

void publishMotorStatus() {
	motor_status_msg.motor_left = digitalRead(PIN_CONTROL_MOTOR1_FAULT);
	motor_status_msg.motor_right = digitalRead(PIN_CONTROL_MOTOR2_FAULT);

	motor_status_pub.publish(&motor_status_msg);
}

void publishBrake() {

	// read the state of the switch into a local variable:
	int readvalue = digitalRead(PIN_IO_BRAKE);

	// check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH), and you've waited long enough
	// since the last press to ignore any noise:

	// If the switch changed, due to noise or pressing:
	if (readvalue != last_brake_state) {
		// reset the debouncing timer
		last_time_brake_value = millis();
	}

	if (millis() - last_time_brake_value > 200) {
		// whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state:

		// if the button state has changed:
		if (readvalue != brake_state) {
			brake_state = readvalue;

			if (brake_state == LOW) {

				g_req_linear_vel_x = 0;
				g_req_linear_vel_y = 0;
				g_req_angular_vel_z = 0;

				//motor1_controller.spin(0);
				//motor2_controller.spin(0);

				// reset PID
				motor1_pid.resetPID();
				motor2_pid.resetPID();

				//motor1_controller.stop(true);
				//motor2_controller.stop(true);
				//led_status.setStatus(RB_BRAKE);

				digitalWrite(PIN_IO_POWER_MOTOR_1, LOW);
				digitalWrite(PIN_IO_POWER_MOTOR_2, LOW);

			} else {
				//motor1_controller.stop(false);
				//motor2_controller.stop(false);
				digitalWrite(PIN_IO_POWER_MOTOR_1, HIGH);
				digitalWrite(PIN_IO_POWER_MOTOR_2, HIGH);
			}
		}
	}

	// save the reading. Next time through the loop, it'll be the lastButtonState:
	last_brake_state = readvalue;

	brake_msg.data = !brake_state;
	brake_pub.publish(&brake_msg);

}

void publishPushButton() {
	// read the state of the switch into a local variable:
	int readvalue = digitalRead(PIN_IO_TASK_FINISH);

	// check to see if you just pressed the button
	// (i.e. the input went from LOW to HIGH), and you've waited long enough
	// since the last press to ignore any noise:

	// If the switch changed, due to noise or pressing:
	if (readvalue != last_push_button_state) {
		// reset the debouncing timer
		last_time_push_button = millis();
	}

	if (millis() - last_time_push_button > 200) {
		// whatever the reading is at, it's been there for longer than the debounce
		// delay, so take it as the actual current state:

		// if the button state has changed:
		if (readvalue != push_button_state) {
			push_button_state = readvalue;
		}
	}

	// save the reading. Next time through the loop, it'll be the lastButtonState:
	last_push_button_state = readvalue;

	push_button_msg.data = !push_button_state;
	push_button_pub.publish(&push_button_msg);

}

void powerChargingCallback(const std_msgs::Bool &msg) {
	if (msg.data) {
		if (!digitalRead(PIN_IO_POWER_DOCK)) {
			digitalWrite(PIN_IO_POWER_DOCK, HIGH); // enable dock
		}
	} else {
		if (digitalRead(PIN_IO_POWER_DOCK))
			digitalWrite(PIN_IO_POWER_DOCK, LOW);
	}
}

void publishDock() {
	dock_msg.data = !digitalRead(PIN_IO_SIGNAL_DOCK);
	dock_pub.publish(&dock_msg);
}
void powerLidarCallback(const std_msgs::Bool &msg) {
	if (msg.data) {
		if (!digitalRead(PIN_IO_POWER_LIDAR)) {
			digitalWrite(PIN_IO_POWER_LIDAR, HIGH);
		}
	} else {
		if (digitalRead(PIN_IO_POWER_LIDAR)) {
			digitalWrite(PIN_IO_POWER_LIDAR, LOW);
		}
	}
}

void PCLRegisterCallback(const lino_msgs::PLCRegister &msg) {
//	if (msg.set) //set
//	{
//		fx1n.SetValueD(msg.register_ID, msg.value);
//	} else //get
//	{
//		plc_register_msg = msg;
//		plc_register_msg.value = fx1n.GetValueD(plc_register_msg.register_ID);
//		pcl_register_pub.publish(&plc_register_msg);
//	}
}

void PCLIOCallback(const lino_msgs::PLC_IO &msg) {
//	if (msg.set) //set
//	{
//		fx1n.SetValueM(msg.IO_ID, msg.value);
//	} else //get
//	{
//		plc_io_msg = msg;
//		plc_io_msg.value = fx1n.GetValueM(plc_io_msg.IO_ID);
//		plc_io_pub.publish(&plc_io_msg);
//	}
}


void chargingCallback(const std_msgs::Bool &msg) {
	if (last_state_charging != msg.data) {
		if (msg.data) {
			//led_status.runCompleteStatus(RB_CHARGING);
		}
		last_state_charging = msg.data;
	}
}

void dockingStateCallback(const std_msgs::String &msg) {
	bool is_docked = false;

	String docked = "docked";

	if (strcmp(msg.data, docked.c_str()) == 0)
		is_docked = true;

	if (last_docking_state != is_docked) {
		last_docking_state = is_docked;

		if (is_docked) {
//			motor1_controller.spin(0);
//			motor2_controller.spin(0);
//
//			led_status.runCompleteStatus(RB_BRAKE);
		}
		// enable brake
		//motor1_controller.stop(is_docked);
		//motor2_controller.stop(is_docked);
	}

}
