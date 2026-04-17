/*
 * LEDStatus.cpp
 *
 *  Created on: Nov 17, 2020
 *      Author: mamoto
 */

#include "LEDStatus.h"

LEDStatus::LEDStatus(int pin_LED) {
	// TODO Auto-generated constructor stub
	mPinLED = pin_LED;
	pinMode(mPinLED, OUTPUT);
	digitalWrite(mPinLED, LOW);
	mCurrentStatus = RB_NONE;
	mCurrentState = 0;
	mPreviousMillis = millis();
}

LEDStatus::~LEDStatus() {
	// TODO Auto-generated destructor stub
}

void LEDStatus::setStatus(ROBOT_STATUS status) {
	if (mCurrentStatus != status) {
		mCurrentStatus = status;
		mCurrentState = 0;
		mPreviousMillis = millis();
	}
}

void LEDStatus::update() {

	unsigned long currentMillis = millis();
	switch (mCurrentStatus) {
	case RB_NONE:
		digitalWrite(mPinLED, LOW);
		break;
	case RB_STARTING:
		if (currentMillis - mPreviousMillis
				> TIME_STATE_1[mCurrentState % NUM_STATE[RB_STARTING]]) {
			mCurrentState++;
			mPreviousMillis = millis();
		}
		digitalWrite(mPinLED, !(mCurrentState % 2));

		break;
	case RB_READY:
		if (currentMillis - mPreviousMillis
				> TIME_STATE_2[mCurrentState % NUM_STATE[RB_READY]]) {
			mCurrentState++;
			mPreviousMillis = millis();

		}
		digitalWrite(mPinLED, !(mCurrentState % 2));

		break;
	case RB_MOVING:
		if (currentMillis - mPreviousMillis
				> TIME_STATE_3[mCurrentState % NUM_STATE[RB_MOVING]]) {
			mPreviousMillis = millis();
			mCurrentState++;
		}
		//digitalWrite(mPinLED, !(mCurrentState % 2));
		digitalWrite(mPinLED, HIGH);

		break;
	case RB_COMPLETED:
		if (currentMillis - mPreviousMillis
				> TIME_STATE_4[mCurrentState % NUM_STATE[RB_COMPLETED]]) {
			mPreviousMillis = millis();
			mCurrentState++;
		}
		digitalWrite(mPinLED, !(mCurrentState % 2));

		break;
	case RB_BUTTON_FINISH:
		if (currentMillis - mPreviousMillis
				> TIME_STATE_5[mCurrentState % NUM_STATE[RB_BUTTON_FINISH]]) {
			mPreviousMillis = millis();
			mCurrentState++;
		}
		if (mCurrentState == NUM_STATE[RB_BUTTON_FINISH]) {
			mCurrentStatus = RB_NONE;
		} else {
			digitalWrite(mPinLED, !(mCurrentState % 2));
		}
		break;
	case RB_BRAKE:
		if (currentMillis - mPreviousMillis
				> TIME_STATE_6[mCurrentState % NUM_STATE[RB_BRAKE]]) {
			mPreviousMillis = millis();
			mCurrentState++;
		}
		if (mCurrentState == NUM_STATE[RB_BRAKE]) {
			mCurrentStatus = RB_NONE;
		} else {
			digitalWrite(mPinLED, !(mCurrentState % 2));
		}
		break;
	case RB_IGNORE:
		if (currentMillis - mPreviousMillis
				> TIME_STATE_7[mCurrentState % NUM_STATE[RB_IGNORE]]) {
			mPreviousMillis = millis();
			mCurrentState++;
		}
		digitalWrite(mPinLED, !(mCurrentState % 2));

		break;
	case RB_FALL:
		if (currentMillis - mPreviousMillis
				> TIME_STATE_8[mCurrentState % NUM_STATE[RB_FALL]]) {
			mPreviousMillis = millis();
			mCurrentState++;
		}
		digitalWrite(mPinLED, !(mCurrentState % 2));

		break;
	case RB_CHARGING:
		if (currentMillis - mPreviousMillis
				> TIME_STATE_9[mCurrentState % NUM_STATE[RB_CHARGING]]) {
			mPreviousMillis = millis();
			mCurrentState++;
		}
		digitalWrite(mPinLED, !(mCurrentState % 2));
		break;
	default:
		digitalWrite(mPinLED, LOW);
		break;
	}

}

void LEDStatus::runCompleteStatus(ROBOT_STATUS status) {

	mCurrentStatus = status;
	switch (mCurrentStatus) {
	case RB_NONE:
		digitalWrite(mPinLED, LOW);
		break;
	case RB_STARTING: {
		for (int i = 0; i < NUM_STATE[mCurrentStatus]; ++i) {
			digitalWrite(mPinLED, !(i % 2));
			delay(TIME_STATE_1[i % NUM_STATE[RB_STARTING]]);
		}
	}
		break;
	case RB_READY: {
		for (int i = 0; i < NUM_STATE[mCurrentStatus]; ++i) {
			digitalWrite(mPinLED, !(i % 2));
			delay(TIME_STATE_2[i % NUM_STATE[mCurrentStatus]]);
		}
	}
		break;
	case RB_MOVING: {
		for (int i = 0; i < NUM_STATE[mCurrentStatus]; ++i) {
			digitalWrite(mPinLED, !(i % 2));
			delay(TIME_STATE_3[i % NUM_STATE[mCurrentStatus]]);
		}
	}
		break;
	case RB_COMPLETED: {
		for (int i = 0; i < NUM_STATE[mCurrentStatus]; ++i) {
			digitalWrite(mPinLED, !(i % 2));
			delay(TIME_STATE_4[i % NUM_STATE[mCurrentStatus]]);
		}
	}
		break;
	case RB_BUTTON_FINISH: {
		for (int i = 0; i < NUM_STATE[mCurrentStatus]; ++i) {
			digitalWrite(mPinLED, !(i % 2));
			delay(TIME_STATE_5[i % NUM_STATE[mCurrentStatus]]);
		}
	}
		break;
	case RB_BRAKE: {
		for (int i = 0; i < NUM_STATE[mCurrentStatus]; ++i) {
			digitalWrite(mPinLED, !(i % 2));
			delay(TIME_STATE_6[i % NUM_STATE[mCurrentStatus]]);
		}
	}
		break;
	case RB_IGNORE: {
		for (int i = 0; i < NUM_STATE[mCurrentStatus]; ++i) {
			digitalWrite(mPinLED, !(i % 2));
			delay(TIME_STATE_7[i % NUM_STATE[mCurrentStatus]]);
		}
	}
		break;
	case RB_FALL: {
		for (int i = 0; i < NUM_STATE[mCurrentStatus]; ++i) {
			digitalWrite(mPinLED, !(i % 2));
			delay(TIME_STATE_8[i % NUM_STATE[mCurrentStatus]]);
		}
	}
		break;
	case RB_CHARGING: {
		for (int i = 0; i < NUM_STATE[mCurrentStatus]; ++i) {
			digitalWrite(mPinLED, !(i % 2));
			delay(TIME_STATE_9[i % NUM_STATE[mCurrentStatus]]);
		}
	}
		break;
	default:
		digitalWrite(mPinLED, LOW);
		break;
	}
	this->setStatus(RB_NONE);
	this->update();
}
