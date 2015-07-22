################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Adc.c \
../src/MSN_Protocol.c \
../src/NC_Protocol.c \
../src/Timer.c \
../src/Uart.c \
../src/XNetProtocol.c \
../src/Xcps.c \
../src/ZRMProtocol.c \
../src/main.c \
../src/rotary.c \
../src/util.c 

OBJS += \
./src/Adc.o \
./src/MSN_Protocol.o \
./src/NC_Protocol.o \
./src/Timer.o \
./src/Uart.o \
./src/XNetProtocol.o \
./src/Xcps.o \
./src/ZRMProtocol.o \
./src/main.o \
./src/rotary.o \
./src/util.o 

C_DEPS += \
./src/Adc.d \
./src/MSN_Protocol.d \
./src/NC_Protocol.d \
./src/Timer.d \
./src/Uart.d \
./src/XNetProtocol.d \
./src/Xcps.d \
./src/ZRMProtocol.d \
./src/main.d \
./src/rotary.d \
./src/util.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega328p -DF_CPU=8000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


