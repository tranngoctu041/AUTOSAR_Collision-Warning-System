################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/MCAL/Can.c \
../Core/Src/MCAL/Mcu.c \
../Core/Src/MCAL/Port.c \
../Core/Src/MCAL/Uart.c 

OBJS += \
./Core/Src/MCAL/Can.o \
./Core/Src/MCAL/Mcu.o \
./Core/Src/MCAL/Port.o \
./Core/Src/MCAL/Uart.o 

C_DEPS += \
./Core/Src/MCAL/Can.d \
./Core/Src/MCAL/Mcu.d \
./Core/Src/MCAL/Port.d \
./Core/Src/MCAL/Uart.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/MCAL/%.o Core/Src/MCAL/%.su Core/Src/MCAL/%.cyclo: ../Core/Src/MCAL/%.c Core/Src/MCAL/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103x6 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Admin/Desktop/DATN/DATN/CONTROL_NODE_STM32/Core/Inc/header/MCAL" -I"C:/Users/Admin/Desktop/DATN/DATN/CONTROL_NODE_STM32/Core/Inc/header/ECU Abs" -I"C:/Users/Admin/Desktop/DATN/DATN/CONTROL_NODE_STM32/Core/Inc/header/Service" -I"C:/Users/Admin/Desktop/DATN/DATN/CONTROL_NODE_STM32/Core/Inc/header/RTE" -I"C:/Users/Admin/Desktop/DATN/DATN/CONTROL_NODE_STM32/Core/Inc/common" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-MCAL

clean-Core-2f-Src-2f-MCAL:
	-$(RM) ./Core/Src/MCAL/Can.cyclo ./Core/Src/MCAL/Can.d ./Core/Src/MCAL/Can.o ./Core/Src/MCAL/Can.su ./Core/Src/MCAL/Mcu.cyclo ./Core/Src/MCAL/Mcu.d ./Core/Src/MCAL/Mcu.o ./Core/Src/MCAL/Mcu.su ./Core/Src/MCAL/Port.cyclo ./Core/Src/MCAL/Port.d ./Core/Src/MCAL/Port.o ./Core/Src/MCAL/Port.su ./Core/Src/MCAL/Uart.cyclo ./Core/Src/MCAL/Uart.d ./Core/Src/MCAL/Uart.o ./Core/Src/MCAL/Uart.su

.PHONY: clean-Core-2f-Src-2f-MCAL

