################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/drivers/DFPlayer.c \
../Core/Src/drivers/DS3231.c \
../Core/Src/drivers/DisplayManager.c \
../Core/Src/drivers/FT5446.c \
../Core/Src/drivers/ILI9488.c 

C_DEPS += \
./Core/Src/drivers/DFPlayer.d \
./Core/Src/drivers/DS3231.d \
./Core/Src/drivers/DisplayManager.d \
./Core/Src/drivers/FT5446.d \
./Core/Src/drivers/ILI9488.d 

OBJS += \
./Core/Src/drivers/DFPlayer.o \
./Core/Src/drivers/DS3231.o \
./Core/Src/drivers/DisplayManager.o \
./Core/Src/drivers/FT5446.o \
./Core/Src/drivers/ILI9488.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/drivers/%.o Core/Src/drivers/%.su: ../Core/Src/drivers/%.c Core/Src/drivers/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-drivers

clean-Core-2f-Src-2f-drivers:
	-$(RM) ./Core/Src/drivers/DFPlayer.d ./Core/Src/drivers/DFPlayer.o ./Core/Src/drivers/DFPlayer.su ./Core/Src/drivers/DS3231.d ./Core/Src/drivers/DS3231.o ./Core/Src/drivers/DS3231.su ./Core/Src/drivers/DisplayManager.d ./Core/Src/drivers/DisplayManager.o ./Core/Src/drivers/DisplayManager.su ./Core/Src/drivers/FT5446.d ./Core/Src/drivers/FT5446.o ./Core/Src/drivers/FT5446.su ./Core/Src/drivers/ILI9488.d ./Core/Src/drivers/ILI9488.o ./Core/Src/drivers/ILI9488.su

.PHONY: clean-Core-2f-Src-2f-drivers

