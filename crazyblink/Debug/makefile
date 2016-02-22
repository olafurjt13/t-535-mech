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
crazyblink.lss \

FLASH_IMAGE += \
crazyblink.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: crazyblink.elf secondary-outputs

# Tool invocations
crazyblink.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,crazyblink.map -mmcu=atmega328p -o "crazyblink.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

crazyblink.lss: crazyblink.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S crazyblink.elf  >"crazyblink.lss"
	@echo 'Finished building: $@'
	@echo ' '

crazyblink.hex: crazyblink.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -R .fuse -R .lock -R .signature -O ihex crazyblink.elf  "crazyblink.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: crazyblink.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=berkeley -t crazyblink.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) crazyblink.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
