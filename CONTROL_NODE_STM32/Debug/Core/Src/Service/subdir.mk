################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Service/Com.c \
../Core/Src/Service/Os.c \
../Core/Src/Service/PduR.c 

OBJS += \
./Core/Src/Service/Com.o \
./Core/Src/Service/Os.o \
./Core/Src/Service/PduR.o 

C_DEPS += \
./Core/Src/Service/Com.d \
./Core/Src/Service/Os.d \
./Core/Src/Service/PduR.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Service/%.o Core/Src/Service/%.su Core/Src/Service/%.cyclo: ../Core/Src/Service/%.c Core/Src/Service/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103x6 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Admin/Desktop/DATN/DATN/CONTROL_NODE_STM32/Core/Inc/header/MCAL" -I"C:/Users/Admin/Desktop/DATN/DATN/CONTROL_NODE_STM32/Core/Inc/header/ECU Abs" -I"C:/Users/Admin/Desktop/DATN/DATN/CONTROL_NODE_STM32/Core/Inc/header/Service" -I"C:/Users/Admin/Desktop/DATN/DATN/CONTROL_NODE_STM32/Core/Inc/header/RTE" -I"C:/Users/Admin/Desktop/DATN/DATN/CONTROL_NODE_STM32/Core/Inc/common" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Service

clean-Core-2f-Src-2f-Service:
	-$(RM) ./Core/Src/Service/Com.cyclo ./Core/Src/Service/Com.d ./Core/Src/Service/Com.o ./Core/Src/Service/Com.su ./Core/Src/Service/Os.cyclo ./Core/Src/Service/Os.d ./Core/Src/Service/Os.o ./Core/Src/Service/Os.su ./Core/Src/Service/PduR.cyclo ./Core/Src/Service/PduR.d ./Core/Src/Service/PduR.o ./Core/Src/Service/PduR.su

.PHONY: clean-Core-2f-Src-2f-Service

