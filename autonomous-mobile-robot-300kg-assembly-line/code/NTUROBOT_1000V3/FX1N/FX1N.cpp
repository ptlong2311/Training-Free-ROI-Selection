/*
 * FX1N.cpp
 *
 *  Created on: Jan 13, 2021
 *      Author: mamoto
 */

#include "FX1N.h"

FX1N::FX1N(HardwareSerial *SerialPort, long FX1Nbaud,
		unsigned char FX1NbyteFormat, unsigned int _FX1Ntimeout) {
	// TODO Auto-generated constructor stub
	if (FX1Nbaud > 19200)
		FX1N_T1 = 750;
	else
		FX1N_T1 = 16500000 / FX1Nbaud; // 1T * 1.5 = T1.5

	// initialize

	FX1NPort = SerialPort;
	(*FX1NPort).begin(FX1Nbaud, FX1NbyteFormat);
	FX1Ntimeout = _FX1Ntimeout;
}

FX1N::~FX1N() {
	// TODO Auto-generated destructor stub
}

int FX1N::GetValueD(String address) {
	String cmd;
	cmd = Make(FxAddress(address));

	String rev = send(cmd);
	if (rev != String()) {
		return RawDataToInt(rev.c_str());
	}
	return -1;
}

uint32_t FX1N::FxAddress(String tagName) {
	char _MyAddressType;
	byte _MyAddressLayoutType;
	uint32_t _UniformAddr;
	_MyAddressType = tagName[0];

	switch (_MyAddressType) {
	case 'D':
		_MyAddressLayoutType = 3;
		break;
	default: // Для регистров M и E
		_MyAddressLayoutType = 1;
		break;
	}
	_UniformAddr = GetUniformAddr(tagName, _MyAddressLayoutType);
	return _UniformAddr;
}

uint32_t FX1N::GetUniformAddr(String tagName, byte addrLayoutType) {
	char tagType = tagName[0];

	uint32_t pos = static_cast<uint32_t>((tagName.substring(1)).toInt());
	if (tagType == 'E') {
		pos = pos - 8000;
	}
	if (addrLayoutType == 1) {
		pos = (pos / 16) * 2;
	}

	uint32_t off = 0;

	switch (tagType) {
	case 'X':
		off = 576;
		break;
	case 'Y':
		off = 384;
		break;
	case 'M':
		off = 0;
		break;
	case 'D':
		off = 16384;
		pos *= 2;
		break;
	case 'E':
		off = 448;
		break;
	default:
		off = 16384;
		pos = 0;
		break;
	}

	return (off + pos);
}

String FX1N::Make(uint32_t addr) {
	String sb = "";
	// sb.Append((char)0x05); //Можно использовать как тест связи - на этот запрос ПЛК должен ответить 0х06, если все
	// хорошо
	sb += ((char) 0x02); //
	sb += ((char) 0x45); // 0x45
	sb += ((char) 0x30);
	sb += ((char) 0x30);
	sb += (ToAddressHexString(addr));
	sb += ((char) 0x30);
	sb += ((char) 0x32);
	sb += ((char) 0x03);
	sb += (DecToHex(GetCheckSum(sb, 1)));

	return (sb);
}

String FX1N::ToAddressHexString(uint32_t _UniformAddr) {
	uint32_t addr = _UniformAddr;
	uint32_t r;
	/// integer value to hex-string
	String hexdec_num = "";
	char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B',
			'C', 'D', 'E', 'F' };

	while (addr > 0) {
		r = addr % 16;
		hexdec_num = hex[r] + hexdec_num;
		addr = addr / 16;
	}
	/*
	switch (hexdec_num.length()) {
	case 0:
		hexdec_num = '0' + hexdec_num;
		hexdec_num = '0' + hexdec_num;
		hexdec_num = '0' + hexdec_num;
		hexdec_num = '0' + hexdec_num;
		break;
	case 1:
		hexdec_num = '0' + hexdec_num;
		hexdec_num = '0' + hexdec_num;
		hexdec_num = '0' + hexdec_num;
		break;
	case 2:
		hexdec_num = '0' + hexdec_num;
		hexdec_num = '0' + hexdec_num;
		break;
	case 3:
		hexdec_num = '0' + hexdec_num;
		break;
	default:
		break;
	}

*/

	String sBUFF0 = "0000" + hexdec_num;
	sBUFF0.toUpperCase();
	unsigned int iBUFF1 = sBUFF0.length() - 4;
	String sBUFF2 = sBUFF0.substring(iBUFF1);
	return sBUFF2;
}

uint8_t FX1N::GetCheckSum(String data, int fromIndex) {

	uint8_t chk = 0;
	for (int i = fromIndex; i < data.length(); i++) {
		char c = data[i];

		chk += (uint8_t) (c);
	}
	return (chk);
}

String FX1N::DecToHex(uint32_t v) {
	String answer = "";
	if (v > 0) {
		while (v > 0) {
			int rem = v % 16;
			if (rem > 9) {
				switch (rem) {
				case 10:
					answer = "A" + answer;
					break;
				case 11:
					answer = "B" + answer;
					break;
				case 12:
					answer = "C" + answer;
					break;
				case 13:
					answer = "D" + answer;
					break;
				case 14:
					answer = "E" + answer;
					break;
				case 15:
					answer = "F" + answer;
					break;
				}
			} else {
				answer = String(rem) + answer;
			}
			v = v / 16;
		}
	} else {
		answer += "0";

	}

	if (answer.length() == 1) {
		answer = "0" + answer;
	}
	return answer;
}

void FX1N::SetValueD(String address, int value) {
	String cmd;

	cmd = MakeSet(FxAddress(address), value);
	String rev = send(cmd);
}

String FX1N::MakeSet(uint32_t addr, uint32_t value) {
	String valueString = DecToHex(value);
	String sb;
	// sb.Append((char)0x05); //Можно использовать как тест связи - на этот запрос ПЛК должен ответить 0х06, если все
	// хорошо
	sb += ((char) _STX); //
	sb += ((char) 0x45); // 0x45
	sb += ((char) 0x31);
	sb += ((char) 0x31);
	sb += (ToAddressHexString(addr));
	sb += ((char) 0x30);
	sb += ((char) 0x32);

	String sBUFF0 = "0000" + String(value, HEX);
	sBUFF0.toUpperCase();
	unsigned int iBUFF1 = sBUFF0.length() - 4;
	String sBUFF2 = sBUFF0.substring(iBUFF1);

	sb += (sBUFF2[2]);
	sb += (sBUFF2[3]);
	sb += (sBUFF2[1]);
	sb += (sBUFF2[0]);

	/*
	 switch (valueString.length()) {
	 case 2:
	 sb += (valueString);
	 sb += ((char) 0x30);
	 sb += ((char) 0x30);

	 break;
	 case 3:
	 sb += (valueString[2]);
	 sb += (valueString[3]);
	 sb += (valueString[1]);
	 sb += ((char) 0x30);

	 break;
	 case 4:
	 sb += (valueString[2]);
	 sb += (valueString[3]);
	 sb += (valueString[1]);
	 sb += (valueString[0]);

	 break;
	 }
	 */
	sb += ((char) _ETX);
	sb += (DecToHex(GetCheckSum(sb, 1)));

	return (sb);
}

String FX1N::string_to_hex(const String &input) {
	static const char hex_digits[] = "0123456789ABCDEF";

	String output;
	output.reserve(input.length() * 2);
	for (unsigned char c : input) {
		output += (hex_digits[c >> 4]);
		output += (hex_digits[c & 15]);
	}
	return output;
}

String FX1N::send(String cmd) {
	//while ((*FX1NPort).available()); // is there something to check?

	for (int i = 0; i < cmd.length(); i++) {
		(*FX1NPort).write(cmd[i]);
	}
	(*FX1NPort).flush();

	FX1NdelayStart = millis(); // start the FX1Stimeout delay

	while (!(*FX1NPort).available())
		; // is there something to check?

	//delayMicroseconds(200);
	byte msgframe[FX1N_BUFFER_SIZE];
	unsigned char msgbuffer;
	String string_recv = "";
	if ((*FX1NPort).available()) // is there something to check?
	{
		unsigned char overflowFlag = 0;
		msgbuffer = 0;
		while ((*FX1NPort).available() != 0) {

			if (overflowFlag)
				(*FX1NPort).read();
			else {
				if (msgbuffer == FX1N_BUFFER_SIZE)
					overflowFlag = 1;

				msgframe[msgbuffer] = ASCII_Normalize((*FX1NPort).read());
				msgbuffer++;
			}

			delayMicroseconds(FX1N_T1); // inter character time out
		}

		if (overflowFlag) {

			return String();
		} else if (msgframe[0] == _STX || msgframe[0] == _ACK) {
			string_recv = (char*) msgframe;

			return string_recv;
		} else {

			return String();
		}

	} else if ((millis() - FX1NdelayStart) > FX1Ntimeout) // check FX1Stimeout
			{
		return String();

	}
	return string_recv;

}

unsigned char FX1N::ASCII_Normalize(unsigned char ASCII_Val) {
	unsigned char ASCII_Normal = 0;
	switch (ASCII_Val) {
	case 0x082:
		ASCII_Normal = 0x02;
		break;
	case 0x0B1:
		ASCII_Normal = 0x031;
		break;
	case 0x0B2:
		ASCII_Normal = 0x032;
		break;
	case 0x0B4:
		ASCII_Normal = 0x034;
		break;
	case 0x0B7:
		ASCII_Normal = 0x037;
		break;
	case 0x0B8:
		ASCII_Normal = 0x038;
		break;
	case 0x0C3:
		ASCII_Normal = 0x043;
		break;
	case 0x0C5:
		ASCII_Normal = 0x045;
		break;
	case 0x0C6:
		ASCII_Normal = 0x046;
		break;
	default:
		ASCII_Normal = ASCII_Val;
		break;
	}

	return ASCII_Normal;
}

int FX1N::RawDataToInt(const char *RawData) {
	int value = 0;

	char reverseRawData[] = { RawData[3], RawData[4], RawData[1], RawData[2] };
	value = hex2int(reverseRawData);

	return value;
}

bool FX1N::GetValueM(String address) {

	String cmd, value;
	cmd = Make(FxAddress(address));
	int intAddress = address.substring(1).toInt();
	char TagType = address[0];
	int startAdd;

	String rev = send(cmd);

	if (rev != String()) {
		value = RawDataToIntArray(rev.c_str());

		switch (TagType) {
		case 'M': {
			return (value[15 - intAddress % 16] == '1') ? true : false;
			break;
		}
		case 'E': {
			bool m[511] = { false };

			intAddress = intAddress - 8000;
			startAdd = (intAddress / 16) * 16;
			for (int i = startAdd; i < startAdd + 16; i++) {
				m[i] = (value[15 - i + startAdd] == '1') ? true : false;
			}
			break;
		}
		case 'X':
		case 'Y': {
			return (value[15 - intAddress % 16] == '1') ? true : false;
		}
		}
	}

	return false;
}

String FX1N::MakeSetM(uint32_t addr, uint32_t value, uint32_t addrV) {
	String addrValue = "0000000000000000";
	if (value) {
		addrValue[15 - addrV % 16] = '1';
	} else {
		addrValue[15 - addrV % 16] = '0';
	}
	int number = strtol(addrValue.c_str(), (char**) NULL, 2); // addrValue.toInt();

	String valueString = DecToHex(number);

	//String valueString = DecToHex(value);
	String sb = "";
	// sb.Append((char)0x05); //Можно использовать как тест связи - на этот запрос ПЛК должен ответить 0х06, если все
	// хорошо
	sb += ((char) _STX); //
	sb += ((char) 0x45); // 0x45
	sb += ((char) 0x31);
	sb += ((char) 0x31);
	sb += (ToAddressHexString(addr));
	sb += ((char) 0x30);
	sb += ((char) 0x31);
	sb += (valueString);
	sb += ((char) _ETX);
	sb += (DecToHex(GetCheckSum(sb, 1)));

	return (sb);
}

void FX1N::SetValueM(String address, int value) {

	String cmd;
	cmd = MakeSetM(FxAddress(address), value,
			static_cast<uint32_t>((address.substring(1)).toInt()));
	String rev = send(cmd);

}

uint32_t FX1N::hex2int(char *hex) {
	uint32_t val = 0;
	while (*hex) {
		// get current character then increment
		uint8_t bytes = *hex++;
		// transform hex character to the 4bit equivalent number, using the ascii table indexes
		if (bytes >= '0' && bytes <= '9')
			bytes = bytes - '0';
		else if (bytes >= 'a' && bytes <= 'f')
			bytes = bytes - 'a' + 10;
		else if (bytes >= 'A' && bytes <= 'F')
			bytes = bytes - 'A' + 10;
		// shift 4 to make space for new digit, and add the 4 bits of the new digit
		val = (val << 4) | (bytes & 0xF);
	}
	return val;
}

String FX1N::RawDataToIntArray(const char *RawData) {
	String value = "";
	int result;

	String s;

	char reverseRawData[] = { RawData[3], RawData[4], RawData[1], RawData[2] };
	result = hex2int(reverseRawData);
	value = convertIntToBinary16(result);

	return value;
}

String FX1N::convertIntToBinary16(int value) {
	String output = "";
	int r;
	while (value != 0) {
		r = value % 2;
		output = String(r) + output;

		value /= 2;
	}

	int length = output.length();
	for (int i = 0; i < (16 - length); ++i) {
		output = "0" + output;
	}

	return output;
}
