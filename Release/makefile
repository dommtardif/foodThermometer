################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -f

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include libraries/Wire/src/utility/subdir.mk
-include libraries/Wire/src/subdir.mk
-include libraries/U8g2/src/clib/subdir.mk
-include libraries/U8g2/src/subdir.mk
-include libraries/SPI/src/subdir.mk
-include libraries/Adafruit_MAX31865_library/subdir.mk
-include core/core/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
AR += \
/home/dom/prog-workspace/foodThermometer/Release/arduino.ar \

ELF += \
foodThermometer.elf \

HEX += \
foodThermometer.hex \


# All Target
all: foodThermometer

# Tool invocations
foodThermometer: $(HEX)
	@echo 'Building target: $@'
	@echo 'Printing size:'
	"/home/dom/eclipse/cpp-oxygen/eclipse//arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-size" -A "/home/dom/prog-workspace/foodThermometer/Release/foodThermometer.elf"
	@echo 'Finished building target: $@'
	@echo ' '

/home/dom/prog-workspace/foodThermometer/Release/arduino.ar: $(AR_OBJ)
	@echo 'Starting archiver'
	"/home/dom/eclipse/cpp-oxygen/eclipse//arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-gcc-ar" rcs    /home/dom/prog-workspace/foodThermometer/Release/arduino.ar $(AR_OBJ)
	@echo 'Finished building: $@'
	@echo ' '

foodThermometer.elf: $(LINK_OBJ) $(AR)
	@echo 'Starting combiner'
	"/home/dom/eclipse/cpp-oxygen/eclipse//arduinoPlugin/packages/arduino/tools/avr-gcc/4.9.2-atmel3.5.4-arduino2/bin/avr-gcc" -Wall -Wextra -Os -g -flto -fuse-linker-plugin -Wl,--gc-sections -mmcu=atmega32u4 -o "/home/dom/prog-workspace/foodThermometer/Release/foodThermometer.elf"   $(LINK_OBJ)   /home/dom/prog-workspace/foodThermometer/Release/arduino.ar  "-L/home/dom/prog-workspace/foodThermometer/Release" -lm
	@echo 'Finished building: $@'
	@echo ' '

foodThermometer.hex: $(ELF)
	@echo 'Create eeprom image'
	${A.RECIPE.OBJCOPY.EEP.PATTERN}
	${A.RECIPE.OBJCOPY.HEX.PATTERN}
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C_UPPER_DEPS)$(PDE_DEPS)$(C_DEPS)$(AR)$(CC_DEPS)$(AR_OBJ)$(C++_DEPS)$(LINK_OBJ)$(CXX_DEPS)$(ASM_DEPS)$(HEX)$(INO_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(ELF)$(CPP_DEPS)
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
