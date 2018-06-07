################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/home/dom/eclipse/cpp-oxygen/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.21/libraries/SPI/src/SPI.cpp 

LINK_OBJ += \
./libraries/SPI/src/SPI.cpp.o 

CPP_DEPS += \
./libraries/SPI/src/SPI.cpp.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/SPI/src/SPI.cpp.o: /home/dom/eclipse/cpp-oxygen/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.21/libraries/SPI/src/SPI.cpp
	@echo 'Building file: $<'
	@echo 'Starting C++ compile'
	"/home/dom/eclipse/cpp-oxygen/eclipse//arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-g++" -c -g -Os -Wall -Wextra -std=gnu++11 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -flto -mmcu=atmega32u4 -DF_CPU=16000000L -DARDUINO=10802 -DARDUINO_AVR_LEONARDO -DARDUINO_ARCH_AVR -DUSB_VID=0x2341 -DUSB_PID=0x8036 '-DUSB_MANUFACTURER="Unknown"' '-DUSB_PRODUCT="Arduino Leonardo"'  -I"/home/dom/eclipse/cpp-oxygen/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.21/cores/arduino" -I"/home/dom/eclipse/cpp-oxygen/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.21/variants/leonardo" -I"/home/dom/ProgrammingProjects/arduino/libraries/Adafruit_MAX31865_library" -I"/home/dom/eclipse/cpp-oxygen/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.21/libraries/EEPROM" -I"/home/dom/eclipse/cpp-oxygen/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.21/libraries/EEPROM/src" -I"/home/dom/ProgrammingProjects/arduino/libraries/U8g2" -I"/home/dom/ProgrammingProjects/arduino/libraries/U8g2/src" -I"/home/dom/eclipse/cpp-oxygen/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.21/libraries/Wire" -I"/home/dom/eclipse/cpp-oxygen/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.21/libraries/Wire/src" -I"/home/dom/eclipse/cpp-oxygen/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.21/libraries/SPI" -I"/home/dom/eclipse/cpp-oxygen/eclipse/arduinoPlugin/packages/arduino/hardware/avr/1.6.21/libraries/SPI/src" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -D__IN_ECLIPSE__=1 -x c++ "$<"  -o  "$@"
	@echo 'Finished building: $<'
	@echo ' '


