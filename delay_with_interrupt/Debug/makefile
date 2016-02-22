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
delay_with_interrupt.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: delay_with_interrupt.elf secondary-outputs

# Tool invocations
delay_with_interrupt.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,delay_with_interrupt.map -mmcu=atmega328p -o "delay_with_interrupt.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

delay_with_interrupt.lss: delay_with_interrupt.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S delay_with_interrupt.elf  >"delay_with_interrupt.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: delay_with_interrupt.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p delay_with_interrupt.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) delay_with_interrupt.elf
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
