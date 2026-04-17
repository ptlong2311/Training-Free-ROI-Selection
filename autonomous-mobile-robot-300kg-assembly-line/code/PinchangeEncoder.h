/*
 * PinchangeEncoder.h
 *
 *  Created on: Apr 20, 2020
 *      Author: mamoto
 */

#ifndef PINCHANGEENCODER_PINCHANGEENCODER_H_
#define PINCHANGEENCODER_PINCHANGEENCODER_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#elif defined(WIRING)
#include "Wiring.h"
#else
#include "WProgram.h"
#include "pins_arduino.h"
#endif

#define IO_REG_TYPE      uint8_t
#define PIN_TO_BASEREG(pin)             (portInputRegister(digitalPinToPort(pin)))
#define PIN_TO_BITMASK(pin)             (digitalPinToBitMask(pin))
#define DIRECT_PIN_READ(base, mask)     (((*(base)) & (mask)) ? 1 : 0)

#define CORE_NUM_INTERRUPT  6
#define CORE_INT0_PIN   3
#define CORE_INT1_PIN   13
#define CORE_INT2_PIN   A9
#define CORE_INT3_PIN   19
#define CORE_INT4_PIN   18
#define CORE_INT5_PIN   2

#include "PinChangeInterrupt.h"

typedef struct {
	volatile IO_REG_TYPE *pin1_register;
	volatile IO_REG_TYPE *pin2_register;
	volatile IO_REG_TYPE *pin3_register;

	volatile IO_REG_TYPE pin1_bitmask;
	volatile IO_REG_TYPE pin2_bitmask;
	volatile IO_REG_TYPE pin3_bitmask;

	uint8_t state;
	long  position;
} PinChange_Encoder_internal_state_t;

class PinchangeEncoder {

public:
	PinchangeEncoder(uint8_t pin1, uint8_t pin2, uint8_t pin3,
			double counts_per_rev, bool invert = false) :
			counts_per_rev_(counts_per_rev), invert_(invert) {
		//counts_per_rev_ = counts_per_rev;
		// TODO Auto-generated constructor stub

		pinMode(pin1, INPUT_PULLUP);
		pinMode(pin2, INPUT_PULLUP);
		pinMode(pin3, INPUT_PULLUP);

		encoder_1.pin1_register = PIN_TO_BASEREG(pin1);
		encoder_1.pin1_bitmask = PIN_TO_BITMASK(pin1);
		encoder_1.pin2_register = PIN_TO_BASEREG(pin3);
		encoder_1.pin2_bitmask = PIN_TO_BITMASK(pin3);

		encoder_2.pin1_register = PIN_TO_BASEREG(pin2);
		encoder_2.pin1_bitmask = PIN_TO_BITMASK(pin2);
		encoder_2.pin2_register = PIN_TO_BASEREG(pin1);
		encoder_2.pin2_bitmask = PIN_TO_BITMASK(pin1);

		encoder_3.pin1_register = PIN_TO_BASEREG(pin3);
		encoder_3.pin1_bitmask = PIN_TO_BITMASK(pin3);
		encoder_3.pin2_register = PIN_TO_BASEREG(pin2);
		encoder_3.pin2_bitmask = PIN_TO_BITMASK(pin2);

		encoder_1.position = 0;
		encoder_2.position = 0;
		encoder_3.position = 0;

		// allow time for a passive R-C filter to charge
		// through the pullup resistors, before reading
		// the initial state
		delayMicroseconds(2000);
		uint8_t s = 0;
		if (DIRECT_PIN_READ(encoder_1.pin1_register, encoder_1.pin1_bitmask))
			s |= 1;
		if (DIRECT_PIN_READ(encoder_1.pin2_register, encoder_1.pin2_bitmask))
			s |= 2;
		encoder_1.state = s;

		s = 0;
		if (DIRECT_PIN_READ(encoder_2.pin1_register, encoder_2.pin1_bitmask))
			s |= 1;
		if (DIRECT_PIN_READ(encoder_2.pin2_register, encoder_2.pin2_bitmask))
			s |= 2;
		encoder_2.state = s;

		s = 0;
		if (DIRECT_PIN_READ(encoder_3.pin1_register, encoder_3.pin1_bitmask))
			s |= 1;
		if (DIRECT_PIN_READ(encoder_3.pin2_register, encoder_3.pin2_bitmask))
			s |= 2;
		encoder_3.state = s;

		attach_interrupt(pin1, &encoder_1);
		attach_interrupt(pin2, &encoder_2);
		attach_interrupt(pin3, &encoder_3);

		prev_encoder_ticks_ = 0;
		prev_update_time_ = millis();
	}
	inline long read() {
		long   ret = encoder_1.position + encoder_2.position
				+ encoder_3.position;
//		Serial.println((int) encoder_3.position);
//		Serial.print(", ");
//		Serial.print((int) encoder_2.position);
//		Serial.print(", ");
//		Serial.print((int) encoder_3.position);
//		Serial.print("\n");

		return ret;
	}

	float getRPM() {
		 long encoder_ticks = read();


		//this function calculates the motor's RPM based on encoder ticks and delta time
		unsigned long current_time = millis();
		unsigned long dt = current_time - prev_update_time_;

		//convert the time from milliseconds to minutes
		float dtm = (float) dt / 60000.0;
		float delta_ticks = encoder_ticks - prev_encoder_ticks_;

		//calculate wheel's speed (RPM)

		prev_update_time_ = current_time;
		prev_encoder_ticks_ = encoder_ticks;
		float rpm = (delta_ticks / counts_per_rev_) / dtm;
		if(invert_)
			rpm *=-1.f;

		// Serial.print("Read RPM: ");
		 //Serial.println(rpm);

		return rpm;
	}

private:

	float counts_per_rev_;
	int32_t prev_update_time_;
	long long int prev_encoder_ticks_;
	PinChange_Encoder_internal_state_t encoder_1;
	PinChange_Encoder_internal_state_t encoder_2;
	PinChange_Encoder_internal_state_t encoder_3;
	bool invert_;

public:
	static PinChange_Encoder_internal_state_t *interruptArgs[CORE_NUM_INTERRUPT];

	static void update(PinChange_Encoder_internal_state_t *arg, bool enable = false) {
		// The compiler believes this is just 1 line of code, so
		// it will inline this function into each interrupt
		// handler.  That's a tiny bit faster, but grows the code.
		// Especially when used with ENCODER_OPTIMIZE_INTERRUPTS,
		// the inline nature allows the ISR prologue and epilogue
		// to only save/restore necessary registers, for very nice
		// speed increase.
		if (!enable) {
			asm volatile (
					"ld r30, X+" "\n\t"
					"ld r31, X+" "\n\t"
					"ld r24, Z" "\n\t"  // r24 = pin1 input

					"ld r30, X+" "\n\t"
					"ld r31, X+" "\n\t"
					"ld r25, Z" "\n\t"// r25 = pin2 input
                   
                    
					"ld r30, X+" "\n\t"
					"ld r31, X+" "\n\t"
					"ld r26, Z" "\n\t"// r26 = pin3 input

					"ld r30, X+" "\n\t"// r30 = pin1 mask
					"ld r31, X+" "\n\t"// r31 = pin2 mask
					"ld r32, X+" "\n\t"// r32 = pin3 mask

					"ld r22, X" "\n\t"// r22 = state

					"andi r22, 7" "\n\t"

					"and  r24, r30" "\n\t"
					"breq L%=1" "\n\t"// if (pin1)
					"ori  r22, 8" "\n\t"//  state |= 8

					"L%=1:" "and  r25, r31" "\n\t"
					"breq L%=2" "\n\t"// if (pin2)
					"ori  r22, 16" "\n\t"//  state |= 16

					"L%=2:" "and  r26, r32" "\n\t"
					"breq L%=3" "\n\t"// if (pin3)
					"ori  r22, 32" "\n\t"//  state |= 32

					"L%=3:" "ldi  r30, lo8(pm(L%=table))" "\n\t"
					"ldi  r31, hi8(pm(L%=table))" "\n\t"
					"add  r30, r22" "\n\t"
					"adc  r31, __zero_reg__" "\n\t"
					"asr  r22" "\n\t"
					"asr  r22" "\n\t"
					"asr  r22" "\n\t"
					"st X+, r22" "\n\t"// store new state
					"ld r22, X+" "\n\t"
					"ld r23, X+" "\n\t"
					"ld r24, X+" "\n\t"
					"ld r25, X+" "\n\t"
					"ld r26, X+" "\n\t"
					"ijmp" "\n\t"// jumps to update_finishup()
					// TODO move this table to another static function,
					// so it doesn't get needlessly duplicated.  Easier
					// said than done, due to linker issues and inlining
					"L%=table:" "\n\t"
					"rjmp L%=end" "\n\t"// 0
					"rjmp L%=plus1" "\n\t"// 1
					"rjmp L%=minus1" "\n\t"// 2
					"rjmp L%=plus2" "\n\t"// 3
					"rjmp L%=minus1" "\n\t"// 4
					"rjmp L%=end" "\n\t"// 5
					"rjmp L%=minus2" "\n\t"// 6
					"rjmp L%=plus1" "\n\t"// 7
					"rjmp L%=plus1" "\n\t"// 8
					"rjmp L%=minus2" "\n\t"// 9
					"rjmp L%=end" "\n\t"// 10
					"rjmp L%=minus1" "\n\t"// 11
					"rjmp L%=plus2" "\n\t"// 12
					"rjmp L%=minus1" "\n\t"// 13
					"rjmp L%=plus1" "\n\t"// 14
					"rjmp L%=end" "\n\t"// 15
					"rjmp L%=end" "\n\t"// 16
					"rjmp L%=plus2" "\n\t"// 17
					"rjmp L%=end" "\n\t"// 18
					"rjmp L%=plus1" "\n\t"// 19
					"rjmp L%=minus2" "\n\t"// 20
					"rjmp L%=end" "\n\t"// 21
					"rjmp L%=minus1" "\n\t"// 22
					"rjmp L%=end" "\n\t"// 23
					"rjmp L%=end" "\n\t"// 24
					"rjmp L%=plus1" "\n\t"// 25
					"rjmp L%=minus1" "\n\t"// 26
					"rjmp L%=end" "\n\t"// 27
					"rjmp L%=end" "\n\t"// 28
					"rjmp L%=plus2" "\n\t"// 29
					"rjmp L%=minus2" "\n\t"// 30
					"rjmp L%=end" "\n\t"// 31
					"rjmp L%=end" "\n\t"// 32
					"rjmp L%=minus2" "\n\t"// 33
					"rjmp L%=plus2" "\n\t"// 34
					"rjmp L%=end" "\n\t"// 35
					"rjmp L%=end" "\n\t"// 36
					"rjmp L%=minus1" "\n\t"// 37
					"rjmp L%=plus1" "\n\t"// 38
					"rjmp L%=end" "\n\t"// 39
					"rjmp L%=end" "\n\t"// 40
					"rjmp L%=minus1" "\n\t"// 41
					"rjmp L%=end" "\n\t"// 42
					"rjmp L%=minus2" "\n\t"// 43
					"rjmp L%=plus1" "\n\t"// 44
					"rjmp L%=end" "\n\t"// 45
					"rjmp L%=plus2" "\n\t"// 46
					"rjmp L%=end" "\n\t"// 47
					"rjmp L%=end" "\n\t"// 48
					"rjmp L%=end" "\n\t"// 49
					"rjmp L%=plus1" "\n\t"// 50
					"rjmp L%=plus2" "\n\t"// 51
					"rjmp L%=minus1" "\n\t"// 52
					"rjmp L%=minus2" "\n\t"// 53
					"rjmp L%=end" "\n\t"// 54
					"rjmp L%=end" "\n\t"// 55
					"rjmp L%=end" "\n\t"// 56
					"rjmp L%=end" "\n\t"// 57
					"rjmp L%=end" "\n\t"// 58
					"rjmp L%=end" "\n\t"// 59
					"rjmp L%=end" "\n\t"// 60
					"rjmp L%=end" "\n\t"// 61
					"rjmp L%=end" "\n\t"// 62
					"rjmp L%=end" "\n\t"// 63
					"L%=minus2:" "\n\t"
					"subi r22, 2" "\n\t"
					"sbci r23, 0" "\n\t"
					"sbci r24, 0" "\n\t"
					"sbci r25, 0" "\n\t"
					"rjmp L%=store" "\n\t"
					"L%=minus1:" "\n\t"
					"subi r22, 1" "\n\t"
					"sbci r23, 0" "\n\t"
					"sbci r24, 0" "\n\t"
					"sbci r25, 0" "\n\t"
					"rjmp L%=store" "\n\t"
					"L%=plus2:" "\n\t"
					"subi r22, 254" "\n\t"
					"rjmp L%=z" "\n\t"
					"L%=plus1:" "\n\t"
					"subi r22, 255" "\n\t"
					"L%=z:" "sbci r23, 255" "\n\t"
					"sbci r24, 255" "\n\t"
					"sbci r25, 255" "\n\t"
					"L%=store:" "\n\t"
					"st -X, r25" "\n\t"
					"st -X, r24" "\n\t"
					"st -X, r23" "\n\t"
					"st -X, r22" "\n\t"
					"L%=end:" "\n"
					: : "x" (arg) : "r22", "r23", "r24", "r25","r26", "r30", "r31", "r32");
		} else {
			uint8_t p1val = DIRECT_PIN_READ(arg->pin1_register,
					arg->pin1_bitmask);
			uint8_t p2val = DIRECT_PIN_READ(arg->pin2_register,
					arg->pin2_bitmask);
			uint8_t p3val = DIRECT_PIN_READ(arg->pin3_register,
					arg->pin3_bitmask);
			uint8_t state = arg->state & 7;
			if (p1val)
				state |= 8;
			if (p2val)
				state |= 16;
			if (p3val)
				state |= 32;			
			arg->state = (state >> 3);
			switch (state) {
				case 10:
				case 20:
				case 30:
				case 33:
				case 43:
				case 53:
					arg->position -= 2;
					return;
					
				case 11:
				case 22:
				case 26:
				case 37:
				case 41:
				case 52:
					arg->position--;
					return;

				case 12:
				case 17:
				case 29:
				case 34:
				case 46:
				case 51:
					arg->position += 2;
					return;

				case 13:
				case 19:
				case 25:
				case 38:
				case 44:
				case 50:
					arg->position++;
					return;

				default:
					// Handle default case if needed
					break;
			}


					}
	}

	static uint8_t attach_interrupt(uint8_t pin,
			PinChange_Encoder_internal_state_t *state) {

		switch (pin) {
#ifdef CORE_INT0_PIN
		case CORE_INT0_PIN:
			interruptArgs[0] = state;
			attachInterrupt(digitalPinToInterrupt(pin), isr0, CHANGE);
			break;
#endif
#ifdef CORE_INT1_PIN
		case CORE_INT1_PIN:
			interruptArgs[1] = state;
			attachPCINT(digitalPinToPCINT(pin), isr1, CHANGE);
			break;
#endif
#ifdef CORE_INT2_PIN
		case CORE_INT2_PIN:
			interruptArgs[2] = state;
			attachPCINT(digitalPinToPCINT(pin), isr2, CHANGE);
			break;
#endif
#ifdef CORE_INT3_PIN
		case CORE_INT3_PIN:
			interruptArgs[3] = state;
			//attachPCINT(digitalPinToPCINT(pin), isr3, CHANGE);
			attachInterrupt(digitalPinToInterrupt(pin), isr3, CHANGE);

			break;
#endif
#ifdef CORE_INT4_PIN
		case CORE_INT4_PIN:
			interruptArgs[4] = state;
			//attachPCINT(digitalPinToPCINT(pin), isr4, CHANGE);
			attachInterrupt(digitalPinToInterrupt(pin), isr4, CHANGE);

			break;
#endif
#ifdef CORE_INT5_PIN
		case CORE_INT5_PIN:
			interruptArgs[5] = state;
			//attachPCINT(digitalPinToPCINT(pin), isr5, CHANGE);
			attachInterrupt(digitalPinToInterrupt(pin), isr5, CHANGE);

			break;
#endif
		}
		return 1;
	}
	static void isr0(void) {
		update(interruptArgs[0], false);
	}
	static void isr1(void) {
		update(interruptArgs[1], false);
	}
	static void isr2(void) {
		update(interruptArgs[2], false);
	}
	static void isr3(void) {
		update(interruptArgs[3], true);
	}
	static void isr4(void) {
		update(interruptArgs[4], true);
	}
	static void isr5(void) {
		update(interruptArgs[5], true);
	}
}
;

#endif /* PINCHANGEENCODER_PINCHANGEENCODER_H_ */
