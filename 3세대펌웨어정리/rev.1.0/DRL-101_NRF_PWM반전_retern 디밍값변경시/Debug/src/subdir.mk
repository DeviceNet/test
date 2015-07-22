################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/MD_Protocol.c \
../src/Uart.c \
../src/XNetProtocol.c \
../src/Xcps.c \
../src/main.c \
../src/timer.c 

OBJS += \
./src/MD_Protocol.o \
./src/Uart.o \
./src/XNetProtocol.o \
./src/Xcps.o \
./src/main.o \
./src/timer.o 

C_DEPS += \
./src/MD_Protocol.d \
./src/Uart.d \
./src/XNetProtocol.d \
./src/Xcps.d \
./src/main.d \
./src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


