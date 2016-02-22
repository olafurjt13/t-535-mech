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
pwm_diode.lss \

FLASH_IMAGE += \
pwm_diode.hex \

EEPROM_IMAGE += \
pwm_diode.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: pwm_diode.elf secondary-outputs

# Tool invocations
pwm_diode.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,pwm_diode.map -mmcu=atmega328p -o "pwm_diode.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

pwm_diode.lss: pwm_diode.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S pwm_diode.elf  >"pwm_diode.lss"
	@echo 'Finished building: $@'
	@echo ' '

pwm_diode.hex: pwm_diode.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex pwm_diode.elf  "pwm_diode.hex"
	@echo 'Finished building: $@'
	@echo ' '

pwm_diode.eep: pwm_diode.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex pwm_diode.elf  "pwm_diode.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: pwm_diode.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p pwm_diode.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) pwm_diode.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
