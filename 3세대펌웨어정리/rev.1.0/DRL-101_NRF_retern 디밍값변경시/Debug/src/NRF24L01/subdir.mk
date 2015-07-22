################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/NRF24L01/NRF24L01.c 

OBJS += \
./src/NRF24L01/NRF24L01.o 

C_DEPS += \
./src/NRF24L01/NRF24L01.d 


# Each subdirectory must supply rules for building sources it contributes
src/NRF24L01/%.o: ../src/NRF24L01/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


