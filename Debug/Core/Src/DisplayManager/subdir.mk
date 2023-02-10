################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/DisplayManager/Animation.c \
../Core/Src/DisplayManager/Bitmap.c \
../Core/Src/DisplayManager/Button.c \
../Core/Src/DisplayManager/DisplayManager.c \
../Core/Src/DisplayManager/Fill.c \
../Core/Src/DisplayManager/List.c \
../Core/Src/DisplayManager/MessageBox.c \
../Core/Src/DisplayManager/NumPad.c \
../Core/Src/DisplayManager/Text.c \
../Core/Src/DisplayManager/TextBox.c \
../Core/Src/DisplayManager/TickBox.c \
../Core/Src/DisplayManager/TitleBar.c 

C_DEPS += \
./Core/Src/DisplayManager/Animation.d \
./Core/Src/DisplayManager/Bitmap.d \
./Core/Src/DisplayManager/Button.d \
./Core/Src/DisplayManager/DisplayManager.d \
./Core/Src/DisplayManager/Fill.d \
./Core/Src/DisplayManager/List.d \
./Core/Src/DisplayManager/MessageBox.d \
./Core/Src/DisplayManager/NumPad.d \
./Core/Src/DisplayManager/Text.d \
./Core/Src/DisplayManager/TextBox.d \
./Core/Src/DisplayManager/TickBox.d \
./Core/Src/DisplayManager/TitleBar.d 

OBJS += \
./Core/Src/DisplayManager/Animation.o \
./Core/Src/DisplayManager/Bitmap.o \
./Core/Src/DisplayManager/Button.o \
./Core/Src/DisplayManager/DisplayManager.o \
./Core/Src/DisplayManager/Fill.o \
./Core/Src/DisplayManager/List.o \
./Core/Src/DisplayManager/MessageBox.o \
./Core/Src/DisplayManager/NumPad.o \
./Core/Src/DisplayManager/Text.o \
./Core/Src/DisplayManager/TextBox.o \
./Core/Src/DisplayManager/TickBox.o \
./Core/Src/DisplayManager/TitleBar.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/DisplayManager/%.o Core/Src/DisplayManager/%.su: ../Core/Src/DisplayManager/%.c Core/Src/DisplayManager/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-DisplayManager

clean-Core-2f-Src-2f-DisplayManager:
	-$(RM) ./Core/Src/DisplayManager/Animation.d ./Core/Src/DisplayManager/Animation.o ./Core/Src/DisplayManager/Animation.su ./Core/Src/DisplayManager/Bitmap.d ./Core/Src/DisplayManager/Bitmap.o ./Core/Src/DisplayManager/Bitmap.su ./Core/Src/DisplayManager/Button.d ./Core/Src/DisplayManager/Button.o ./Core/Src/DisplayManager/Button.su ./Core/Src/DisplayManager/DisplayManager.d ./Core/Src/DisplayManager/DisplayManager.o ./Core/Src/DisplayManager/DisplayManager.su ./Core/Src/DisplayManager/Fill.d ./Core/Src/DisplayManager/Fill.o ./Core/Src/DisplayManager/Fill.su ./Core/Src/DisplayManager/List.d ./Core/Src/DisplayManager/List.o ./Core/Src/DisplayManager/List.su ./Core/Src/DisplayManager/MessageBox.d ./Core/Src/DisplayManager/MessageBox.o ./Core/Src/DisplayManager/MessageBox.su ./Core/Src/DisplayManager/NumPad.d ./Core/Src/DisplayManager/NumPad.o ./Core/Src/DisplayManager/NumPad.su ./Core/Src/DisplayManager/Text.d ./Core/Src/DisplayManager/Text.o ./Core/Src/DisplayManager/Text.su ./Core/Src/DisplayManager/TextBox.d ./Core/Src/DisplayManager/TextBox.o ./Core/Src/DisplayManager/TextBox.su ./Core/Src/DisplayManager/TickBox.d ./Core/Src/DisplayManager/TickBox.o ./Core/Src/DisplayManager/TickBox.su ./Core/Src/DisplayManager/TitleBar.d ./Core/Src/DisplayManager/TitleBar.o ./Core/Src/DisplayManager/TitleBar.su

.PHONY: clean-Core-2f-Src-2f-DisplayManager

