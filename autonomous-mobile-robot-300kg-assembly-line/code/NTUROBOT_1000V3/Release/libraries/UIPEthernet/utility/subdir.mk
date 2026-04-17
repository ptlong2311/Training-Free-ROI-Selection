################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/Users/mamoto/Documents/Arduino/libraries/UIPEthernet/utility/Enc28J60Network.cpp \
/Users/mamoto/Documents/Arduino/libraries/UIPEthernet/utility/mempool.cpp 

C_SRCS += \
/Users/mamoto/Documents/Arduino/libraries/UIPEthernet/utility/uip.c \
/Users/mamoto/Documents/Arduino/libraries/UIPEthernet/utility/uip_arp.c 

C_DEPS += \
./libraries/UIPEthernet/utility/uip.c.d \
./libraries/UIPEthernet/utility/uip_arp.c.d 

LINK_OBJ += \
./libraries/UIPEthernet/utility/Enc28J60Network.cpp.o \
./libraries/UIPEthernet/utility/mempool.cpp.o \
./libraries/UIPEthernet/utility/uip.c.o \
./libraries/UIPEthernet/utility/uip_arp.c.o 

CPP_DEPS += \
./libraries/UIPEthernet/utility/Enc28J60Network.cpp.d \
./libraries/UIPEthernet/utility/mempool.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/UIPEthernet/utility/Enc28J60Network.cpp.o: /Users/mamoto/Documents/Arduino/libraries/UIPEthernet/utility/Enc28J60Network.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse//arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/cores/arduino" -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/variants/mega" -I"/Users/mamoto/Documents/Arduino/libraries/Modbus-Master-Slave-for-Arduino" -I"/Users/mamoto/Documents/Arduino/libraries/movingAvg/src" -I"/Users/mamoto/Documents/Arduino/libraries/PinChangeInterrupt/src" -I"/Users/mamoto/Documents/Arduino/libraries/ros_lib" -I"/Users/mamoto/Documents/Arduino/libraries/Rtc/src" -I"/Users/mamoto/Documents/Arduino/libraries/SoftwareWire" -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/libraries/SPI/src" -I"/Users/mamoto/Documents/Arduino/libraries/UIPEthernet/utility" -I"/Users/mamoto/Documents/Arduino/libraries/UIPEthernet" -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/libraries/Wire/src" -I"/Users/mamoto/Documents/Arduino/libraries/Ethernet/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"   -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries/UIPEthernet/utility/mempool.cpp.o: /Users/mamoto/Documents/Arduino/libraries/UIPEthernet/utility/mempool.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse//arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/cores/arduino" -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/variants/mega" -I"/Users/mamoto/Documents/Arduino/libraries/Modbus-Master-Slave-for-Arduino" -I"/Users/mamoto/Documents/Arduino/libraries/movingAvg/src" -I"/Users/mamoto/Documents/Arduino/libraries/PinChangeInterrupt/src" -I"/Users/mamoto/Documents/Arduino/libraries/ros_lib" -I"/Users/mamoto/Documents/Arduino/libraries/Rtc/src" -I"/Users/mamoto/Documents/Arduino/libraries/SoftwareWire" -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/libraries/SPI/src" -I"/Users/mamoto/Documents/Arduino/libraries/UIPEthernet/utility" -I"/Users/mamoto/Documents/Arduino/libraries/UIPEthernet" -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/libraries/Wire/src" -I"/Users/mamoto/Documents/Arduino/libraries/Ethernet/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"   -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries/UIPEthernet/utility/uip.c.o: /Users/mamoto/Documents/Arduino/libraries/UIPEthernet/utility/uip.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse//arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-gcc" -c -g -Os -Wall -Wextra -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/cores/arduino" -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/variants/mega" -I"/Users/mamoto/Documents/Arduino/libraries/Modbus-Master-Slave-for-Arduino" -I"/Users/mamoto/Documents/Arduino/libraries/movingAvg/src" -I"/Users/mamoto/Documents/Arduino/libraries/PinChangeInterrupt/src" -I"/Users/mamoto/Documents/Arduino/libraries/ros_lib" -I"/Users/mamoto/Documents/Arduino/libraries/Rtc/src" -I"/Users/mamoto/Documents/Arduino/libraries/SoftwareWire" -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/libraries/SPI/src" -I"/Users/mamoto/Documents/Arduino/libraries/UIPEthernet/utility" -I"/Users/mamoto/Documents/Arduino/libraries/UIPEthernet" -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/libraries/Wire/src" -I"/Users/mamoto/Documents/Arduino/libraries/Ethernet/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 "$<"   -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

libraries/UIPEthernet/utility/uip_arp.c.o: /Users/mamoto/Documents/Arduino/libraries/UIPEthernet/utility/uip_arp.c
	@echo 'Building file: $<'
	@echo 'Starting C compile'
	"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse//arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-gcc" -c -g -Os -Wall -Wextra -std=gnu11 -ffunction-sections -fdata-sections -MMD -flto -fno-fat-lto-objects -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/cores/arduino" -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/variants/mega" -I"/Users/mamoto/Documents/Arduino/libraries/Modbus-Master-Slave-for-Arduino" -I"/Users/mamoto/Documents/Arduino/libraries/movingAvg/src" -I"/Users/mamoto/Documents/Arduino/libraries/PinChangeInterrupt/src" -I"/Users/mamoto/Documents/Arduino/libraries/ros_lib" -I"/Users/mamoto/Documents/Arduino/libraries/Rtc/src" -I"/Users/mamoto/Documents/Arduino/libraries/SoftwareWire" -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/libraries/SPI/src" -I"/Users/mamoto/Documents/Arduino/libraries/UIPEthernet/utility" -I"/Users/mamoto/Documents/Arduino/libraries/UIPEthernet" -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/libraries/Wire/src" -I"/Users/mamoto/Documents/Arduino/libraries/Ethernet/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 "$<"   -o "$@"
	@echo 'Finished building: $<'
	@echo ' '


