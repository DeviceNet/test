################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/GC_Protocol.c \
../src/MDT-100Z.c \
../src/MD_Protocol.c \
../src/NCProtocol.c \
../src/Timer.c \
../src/Uart.c \
../src/XNetProtocol.c \
../src/Xcps.c \
../src/ZRMProtocol.c \
../src/rotary.c 

OBJS += \
./src/GC_Protocol.o \
./src/MDT-100Z.o \
./src/MD_Protocol.o \
./src/NCProtocol.o \
./src/Timer.o \
./src/Uart.o \
./src/XNetProtocol.o \
./src/Xcps.o \
./src/ZRMProtocol.o \
./src/rotary.o 

C_DEPS += \
./src/GC_Protocol.d \
./src/MDT-100Z.d \
./src/MD_Protocol.d \
./src/NCProtocol.d \
./src/Timer.d \
./src/Uart.d \
./src/XNetProtocol.d \
./src/Xcps.d \
./src/ZRMProtocol.d \
./src/rotary.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -Wall -g2 -gstabs -Os -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega128 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


