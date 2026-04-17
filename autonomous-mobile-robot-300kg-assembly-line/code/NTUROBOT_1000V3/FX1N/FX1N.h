/*
 * FX1N.h
 *
 *  Created on: Jan 13, 2021
 *      Author: mamoto
 */

#ifndef FX1N_H_
#define FX1N_H_
#include <Arduino.h>
#include <HardwareSerial.h>

#define FX1N_BUFFER_SIZE 128

class FX1N {

	const byte _STX = 0x02;
	const byte _ETX = 0x03;
	const byte _ACK = 0x06;
	const byte _NAK = 0x15;

public:
	FX1N(HardwareSerial *SerialPort, long FX1Nbaud,
			unsigned char FX1NbyteFormat, unsigned int _FX1Ntimeout);
	virtual ~FX1N();

	/**
	 *
	 */
	int GetValueD(String address);

	/**
	 *
	 */
	void SetValueD(String address, int value);

	/**
	 *
	 */
	bool GetValueM(String address);

	/**
	 *
	 */
	void SetValueM(String address, int value);

protected:
	/**
	 *
	 *
	 */
	uint32_t FxAddress(String tagName);

	/**
	 *
	 */
	uint32_t GetUniformAddr(String tagName, byte addrLayoutType);

	/**
	 *
	 *
	 */
	String Make(uint32_t addr);

	/**
	 *
	 */
	String MakeSet(uint32_t addr, uint32_t value);

	/*
	 *
	 */
	String MakeSetM(uint32_t addr, uint32_t value, uint32_t addrV);

	/**
	 *
	 */
	String ToAddressHexString(uint32_t _UniformAddr);

	/**
	 *
	 */
	uint8_t GetCheckSum(String data, int fromIndex);

	/**
	 *
	 */
	String DecToHex(uint32_t v);

	/**
	 *
	 */
	String string_to_hex(const String &input);

	/**
	 *
	 */
	String send(String cmd);

	/**
	 *
	 */
	unsigned char ASCII_Normalize(unsigned char ASCII_Val);

	/**
	 *
	 */
	int RawDataToInt(const char *RawData);

	/**
	 *
	 */
	String RawDataToIntArray(const char *RawData);

	/**
	 *
	 */
	uint32_t hex2int(char *hex);

	/**
	 *
	 */
	String convertIntToBinary16(int value);

private:
	HardwareSerial *FX1NPort;
	unsigned int FX1N_T1;
	unsigned long FX1NdelayStart;
	unsigned int FX1Ntimeout;

};

#endif /* FX1N_H_ */
