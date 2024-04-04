################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../StdPeriph_Driver/src/stm32f0xx_adc.c \
../StdPeriph_Driver/src/stm32f0xx_can.c \
../StdPeriph_Driver/src/stm32f0xx_cec.c \
../StdPeriph_Driver/src/stm32f0xx_comp.c \
../StdPeriph_Driver/src/stm32f0xx_crc.c \
../StdPeriph_Driver/src/stm32f0xx_crs.c \
../StdPeriph_Driver/src/stm32f0xx_dac.c \
../StdPeriph_Driver/src/stm32f0xx_dbgmcu.c \
../StdPeriph_Driver/src/stm32f0xx_dma.c \
../StdPeriph_Driver/src/stm32f0xx_exti.c \
../StdPeriph_Driver/src/stm32f0xx_flash.c \
../StdPeriph_Driver/src/stm32f0xx_gpio.c \
../StdPeriph_Driver/src/stm32f0xx_i2c.c \
../StdPeriph_Driver/src/stm32f0xx_iwdg.c \
../StdPeriph_Driver/src/stm32f0xx_misc.c \
../StdPeriph_Driver/src/stm32f0xx_pwr.c \
../StdPeriph_Driver/src/stm32f0xx_rcc.c \
../StdPeriph_Driver/src/stm32f0xx_rtc.c \
../StdPeriph_Driver/src/stm32f0xx_spi.c \
../StdPeriph_Driver/src/stm32f0xx_syscfg.c \
../StdPeriph_Driver/src/stm32f0xx_tim.c \
../StdPeriph_Driver/src/stm32f0xx_usart.c \
../StdPeriph_Driver/src/stm32f0xx_wwdg.c 

OBJS += \
./StdPeriph_Driver/src/stm32f0xx_adc.o \
./StdPeriph_Driver/src/stm32f0xx_can.o \
./StdPeriph_Driver/src/stm32f0xx_cec.o \
./StdPeriph_Driver/src/stm32f0xx_comp.o \
./StdPeriph_Driver/src/stm32f0xx_crc.o \
./StdPeriph_Driver/src/stm32f0xx_crs.o \
./StdPeriph_Driver/src/stm32f0xx_dac.o \
./StdPeriph_Driver/src/stm32f0xx_dbgmcu.o \
./StdPeriph_Driver/src/stm32f0xx_dma.o \
./StdPeriph_Driver/src/stm32f0xx_exti.o \
./StdPeriph_Driver/src/stm32f0xx_flash.o \
./StdPeriph_Driver/src/stm32f0xx_gpio.o \
./StdPeriph_Driver/src/stm32f0xx_i2c.o \
./StdPeriph_Driver/src/stm32f0xx_iwdg.o \
./StdPeriph_Driver/src/stm32f0xx_misc.o \
./StdPeriph_Driver/src/stm32f0xx_pwr.o \
./StdPeriph_Driver/src/stm32f0xx_rcc.o \
./StdPeriph_Driver/src/stm32f0xx_rtc.o \
./StdPeriph_Driver/src/stm32f0xx_spi.o \
./StdPeriph_Driver/src/stm32f0xx_syscfg.o \
./StdPeriph_Driver/src/stm32f0xx_tim.o \
./StdPeriph_Driver/src/stm32f0xx_usart.o \
./StdPeriph_Driver/src/stm32f0xx_wwdg.o 

C_DEPS += \
./StdPeriph_Driver/src/stm32f0xx_adc.d \
./StdPeriph_Driver/src/stm32f0xx_can.d \
./StdPeriph_Driver/src/stm32f0xx_cec.d \
./StdPeriph_Driver/src/stm32f0xx_comp.d \
./StdPeriph_Driver/src/stm32f0xx_crc.d \
./StdPeriph_Driver/src/stm32f0xx_crs.d \
./StdPeriph_Driver/src/stm32f0xx_dac.d \
./StdPeriph_Driver/src/stm32f0xx_dbgmcu.d \
./StdPeriph_Driver/src/stm32f0xx_dma.d \
./StdPeriph_Driver/src/stm32f0xx_exti.d \
./StdPeriph_Driver/src/stm32f0xx_flash.d \
./StdPeriph_Driver/src/stm32f0xx_gpio.d \
./StdPeriph_Driver/src/stm32f0xx_i2c.d \
./StdPeriph_Driver/src/stm32f0xx_iwdg.d \
./StdPeriph_Driver/src/stm32f0xx_misc.d \
./StdPeriph_Driver/src/stm32f0xx_pwr.d \
./StdPeriph_Driver/src/stm32f0xx_rcc.d \
./StdPeriph_Driver/src/stm32f0xx_rtc.d \
./StdPeriph_Driver/src/stm32f0xx_spi.d \
./StdPeriph_Driver/src/stm32f0xx_syscfg.d \
./StdPeriph_Driver/src/stm32f0xx_tim.d \
./StdPeriph_Driver/src/stm32f0xx_usart.d \
./StdPeriph_Driver/src/stm32f0xx_wwdg.d 


# Each subdirectory must supply rules for building sources it contributes
StdPeriph_Driver/src/%.o: ../StdPeriph_Driver/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -mfloat-abi=soft -DSTM32 -DSTM32F0 -DSTM32F030K6Tx -DDEBUG -DSTM32F030 -DUSE_STDPERIPH_DRIVER -I"D:/STM32_projects/pic_pgm/StdPeriph_Driver/inc" -I"D:/STM32_projects/pic_pgm/inc" -I"D:/STM32_projects/pic_pgm/CMSIS/device" -I"D:/STM32_projects/pic_pgm/CMSIS/core" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


