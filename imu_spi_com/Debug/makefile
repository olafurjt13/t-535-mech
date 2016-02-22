################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
imu_spi_com.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: imu_spi_com.elf secondary-outputs

# Tool invocations
imu_spi_com.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,imu_spi_com.map -mmcu=atmega328p -o "imu_spi_com.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

imu_spi_com.lss: imu_spi_com.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S imu_spi_com.elf  >"imu_spi_com.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: imu_spi_com.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p imu_spi_com.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) imu_spi_com.elf
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
