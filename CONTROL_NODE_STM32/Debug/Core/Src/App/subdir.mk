################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/App/Swc_HmiReporting.c \
../Core/Src/App/Swc_SafetyLogic.c \
../Core/Src/App/Swc_SensorFusion.c 

OBJS += \
./Core/Src/App/Swc_HmiReporting.o \
./Core/Src/App/Swc_SafetyLogic.o \
./Core/Src/App/Swc_SensorFusion.o 

C_DEPS += \
./Core/Src/App/Swc_HmiReporting.d \
./Core/Src/App/Swc_SafetyLogic.d \
./Core/Src/App/Swc_SensorFusion.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/App/%.o Core/Src/App/%.su Core/Src/App/%.cyclo: ../Core/Src/App/%.c Core/Src/App/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103x6 -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/Admin/Desktop/DATN/DATN/CONTROL_NODE_STM32/Core/Inc/header/MCAL" -I"C:/Users/Admin/Desktop/DATN/DATN/CONTROL_NODE_STM32/Core/Inc/header/ECU Abs" -I"C:/Users/Admin/Desktop/DATN/DATN/CONTROL_NODE_STM32/Core/Inc/header/Service" -I"C:/Users/Admin/Desktop/DATN/DATN/CONTROL_NODE_STM32/Core/Inc/header/RTE" -I"C:/Users/Admin/Desktop/DATN/DATN/CONTROL_NODE_STM32/Core/Inc/common" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-App

clean-Core-2f-Src-2f-App:
	-$(RM) ./Core/Src/App/Swc_HmiReporting.cyclo ./Core/Src/App/Swc_HmiReporting.d ./Core/Src/App/Swc_HmiReporting.o ./Core/Src/App/Swc_HmiReporting.su ./Core/Src/App/Swc_SafetyLogic.cyclo ./Core/Src/App/Swc_SafetyLogic.d ./Core/Src/App/Swc_SafetyLogic.o ./Core/Src/App/Swc_SafetyLogic.su ./Core/Src/App/Swc_SensorFusion.cyclo ./Core/Src/App/Swc_SensorFusion.d ./Core/Src/App/Swc_SensorFusion.o ./Core/Src/App/Swc_SensorFusion.su

.PHONY: clean-Core-2f-Src-2f-App

