################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/screens/DeviceTestScreen.c \
../Core/Src/screens/HomeScreen.c \
../Core/Src/screens/SettingsTaskScreen.c 

C_DEPS += \
./Core/Src/screens/DeviceTestScreen.d \
./Core/Src/screens/HomeScreen.d \
./Core/Src/screens/SettingsTaskScreen.d 

OBJS += \
./Core/Src/screens/DeviceTestScreen.o \
./Core/Src/screens/HomeScreen.o \
./Core/Src/screens/SettingsTaskScreen.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/screens/%.o Core/Src/screens/%.su: ../Core/Src/screens/%.c Core/Src/screens/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-screens

clean-Core-2f-Src-2f-screens:
	-$(RM) ./Core/Src/screens/DeviceTestScreen.d ./Core/Src/screens/DeviceTestScreen.o ./Core/Src/screens/DeviceTestScreen.su ./Core/Src/screens/HomeScreen.d ./Core/Src/screens/HomeScreen.o ./Core/Src/screens/HomeScreen.su ./Core/Src/screens/SettingsTaskScreen.d ./Core/Src/screens/SettingsTaskScreen.o ./Core/Src/screens/SettingsTaskScreen.su

.PHONY: clean-Core-2f-Src-2f-screens

