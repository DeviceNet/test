################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/NRF24L01/subdir.mk
-include src/subdir.mk
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
DRT_101_NRF_rev1.1.lss \

FLASH_IMAGE += \
DRT_101_NRF_rev1.1.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: DRT_101_NRF_rev1.1.elf secondary-outputs

# Tool invocations
DRT_101_NRF_rev1.1.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,DRT_101_NRF_rev1.1.map -mmcu=atmega128 -o "DRT_101_NRF_rev1.1.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

DRT_101_NRF_rev1.1.lss: DRT_101_NRF_rev1.1.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S DRT_101_NRF_rev1.1.elf  >"DRT_101_NRF_rev1.1.lss"
	@echo 'Finished building: $@'
	@echo ' '

DRT_101_NRF_rev1.1.hex: DRT_101_NRF_rev1.1.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex DRT_101_NRF_rev1.1.elf  "DRT_101_NRF_rev1.1.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: DRT_101_NRF_rev1.1.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=avr --mcu=atmega128 DRT_101_NRF_rev1.1.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(FLASH_IMAGE)$(ELFS)$(OBJS)$(ASM_DEPS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS)$(LSS)$(C_DEPS) DRT_101_NRF_rev1.1.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
