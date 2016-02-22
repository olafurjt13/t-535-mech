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
uart_pwm_led.lss \

SIZEDUMMY += \
sizedummy \


# All Target
all: uart_pwm_led.elf secondary-outputs

# Tool invocations
uart_pwm_led.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,uart_pwm_led.map -mmcu=atmega328p -o "uart_pwm_led.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

uart_pwm_led.lss: uart_pwm_led.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S uart_pwm_led.elf  >"uart_pwm_led.lss"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: uart_pwm_led.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p uart_pwm_led.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) uart_pwm_led.elf
	-@echo ' '

secondary-outputs: $(LSS) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
