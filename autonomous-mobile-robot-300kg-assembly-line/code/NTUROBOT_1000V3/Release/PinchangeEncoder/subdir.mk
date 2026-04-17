################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../PinchangeEncoder/PinchangeEncoder.cpp 

LINK_OBJ += \
./PinchangeEncoder/PinchangeEncoder.cpp.o 

CPP_DEPS += \
./PinchangeEncoder/PinchangeEncoder.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
PinchangeEncoder/PinchangeEncoder.cpp.o: ../PinchangeEncoder/PinchangeEncoder.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse//arduinoPlugin/packages/arduino/tools/avr-gcc/7.3.0-atmel3.6.1-arduino7/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto -mmcu=atmega2560 -DF_CPU=16000000L -DARDUINO=10812 -DARDUINO_AVR_MEGA2560 -DARDUINO_ARCH_AVR     -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/cores/arduino" -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/variants/mega" -I"/Users/mamoto/Documents/Arduino/libraries/Modbus-Master-Slave-for-Arduino" -I"/Users/mamoto/Documents/Arduino/libraries/movingAvg/src" -I"/Users/mamoto/Documents/Arduino/libraries/PinChangeInterrupt/src" -I"/Users/mamoto/Documents/Arduino/libraries/ros_lib" -I"/Users/mamoto/Documents/Arduino/libraries/Rtc/src" -I"/Users/mamoto/Documents/Arduino/libraries/SoftwareWire" -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/libraries/SPI/src" -I"/Users/mamoto/eclipse/cpp-2020-09/Eclipse.app/Contents/Eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.8.3/libraries/Wire/src" -I"/Users/mamoto/Documents/Arduino/libraries/Ethernet/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -D__IN_ECLIPSE__=1 -x c++ "$<"   -o "$@"
	@echo 'Finished building: $<'
	@echo ' '


