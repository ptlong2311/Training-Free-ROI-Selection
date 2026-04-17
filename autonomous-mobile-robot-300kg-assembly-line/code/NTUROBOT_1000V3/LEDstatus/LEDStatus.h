/*
 * LEDStatus.h
 *
 *  Created on: Nov 17, 2020
 *      Author: mamoto
 */

#ifndef LEDSTATUS_LEDSTATUS_H_
#define LEDSTATUS_LEDSTATUS_H_

#include <Arduino.h>

enum ROBOT_STATUS {
RB_NONE,
RB_STARTING,
RB_READY,
RB_MOVING,
RB_COMPLETED,
RB_BUTTON_FINISH,
RB_BRAKE,
RB_IGNORE,
RB_FALL,
RB_CHARGING
};




class LEDStatus {
public:
	LEDStatus(int pin_LED);
	virtual ~LEDStatus();
	void setStatus(ROBOT_STATUS status);
	void update();
	ROBOT_STATUS getStatus() {return mCurrentStatus;}
	void runCompleteStatus(ROBOT_STATUS status);

private:
	int mPinLED;
	ROBOT_STATUS mCurrentStatus;
	int mCurrentState;
	unsigned long mPreviousMillis;

	int NUM_STATE[10] = {1, 2, 6, 1, 4, 6, 2, 10, 8, 2};
	int TIME_STATE_0[1]={300};
	int TIME_STATE_1[2]={300, 1000};
	int TIME_STATE_2[6]={300, 600, 300, 600, 2000, 2000};
	int TIME_STATE_3[1]={1000};
	int TIME_STATE_4[4]={100, 100, 100, 1000};
	int TIME_STATE_5[6]={100, 100, 100, 100, 100, 1000};
	int TIME_STATE_6[2]={1000, 2000};
	int TIME_STATE_7[10]={100, 100, 100, 100, 100, 100, 100, 100, 100, 1000};
	int TIME_STATE_8[8]={100, 100, 100, 100, 100, 100, 100, 1000};
	int TIME_STATE_9[2]={3000, 100};


};

#endif /* LEDSTATUS_LEDSTATUS_H_ */
