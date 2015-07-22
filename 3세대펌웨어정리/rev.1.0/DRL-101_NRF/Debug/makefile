################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/util/subdir.mk
-include src/NRF24L01/subdir.mk
-include src/subdir.mk
-include .metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
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
DRL-101_NRF_PWMno반전_retern.lss \

FLASH_IMAGE += \
DRL-101_NRF_PWMno반전_retern.hex \

EEPROM_IMAGE += \
DRL-101_NRF_PWMno반전_retern.eep \

SIZEDUMMY += \
sizedummy \


# All Target
all: DRL-101_NRF_PWMno반전_retern.elf secondary-outputs

# Tool invocations
DRL-101_NRF_PWMno반전_retern.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,DRL-101_NRF_PWMno반전_retern.map -mmcu=atmega328p -o "DRL-101_NRF_PWMno반전_retern.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

DRL-101_NRF_PWMno반전_retern.lss: DRL-101_NRF_PWMno반전_retern.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S DRL-101_NRF_PWMno반전_retern.elf  >"DRL-101_NRF_PWMno반전_retern.lss"
	@echo 'Finished building: $@'
	@echo ' '

DRL-101_NRF_PWMno반전_retern.hex: DRL-101_NRF_PWMno반전_retern.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex DRL-101_NRF_PWMno반전_retern.elf  "DRL-101_NRF_PWMno반전_retern.hex"
	@echo 'Finished building: $@'
	@echo ' '

DRL-101_NRF_PWMno반전_retern.eep: DRL-101_NRF_PWMno반전_retern.elf
	@echo 'Create eeprom image (ihex format)'
	-avr-objcopy -j .eeprom --no-change-warnings --change-section-lma .eeprom=0 -O ihex DRL-101_NRF_PWMno반전_retern.elf  "DRL-101_NRF_PWMno반전_retern.eep"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: DRL-101_NRF_PWMno반전_retern.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega328p DRL-101_NRF_PWMno반전_retern.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(EEPROM_IMAGE)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) DRL-101_NRF_PWMno반전_retern.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(EEPROM_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
