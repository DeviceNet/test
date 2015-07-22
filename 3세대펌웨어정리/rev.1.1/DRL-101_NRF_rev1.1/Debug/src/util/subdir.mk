################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/util/byte_util.c \
../src/util/trim.c 

OBJS += \
./src/util/byte_util.o \
./src/util/trim.o 

C_DEPS += \
./src/util/byte_util.d \
./src/util/trim.d 


# Each subdirectory must supply rules for building sources it contributes
src/util/%.o: ../src/util/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


