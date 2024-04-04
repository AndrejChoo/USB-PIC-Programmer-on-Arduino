################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/delay.c \
../src/gpio.c \
../src/main.c \
../src/pic.c \
../src/progger.c \
../src/syscalls.c \
../src/system_stm32f0xx.c \
../src/usart.c 

OBJS += \
./src/delay.o \
./src/gpio.o \
./src/main.o \
./src/pic.o \
./src/progger.o \
./src/syscalls.o \
./src/system_stm32f0xx.o \
./src/usart.o 

C_DEPS += \
./src/delay.d \
./src/gpio.d \
./src/main.d \
./src/pic.d \
./src/progger.d \
./src/syscalls.d \
./src/system_stm32f0xx.d \
./src/usart.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F030K6Tx -DDEBUG -DSTM32F030 -DUSE_STDPERIPH_DRIVER -I"D:/STM32_projects/pic_pgm/StdPeriph_Driver/inc" -I"D:/STM32_projects/pic_pgm/inc" -I"D:/STM32_projects/pic_pgm/CMSIS/device" -I"D:/STM32_projects/pic_pgm/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


