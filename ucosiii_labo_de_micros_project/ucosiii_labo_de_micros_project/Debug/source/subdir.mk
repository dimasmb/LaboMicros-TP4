################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/drv_FRDM.c \
../source/drv_display.c \
../source/drv_leds.c \
../source/drv_switch.c \
../source/fsm.c \
../source/gpio.c \
../source/lectora.c \
../source/main.c \
../source/thingSpeakLib.c \
../source/timer.c \
../source/uart_drv.c 

OBJS += \
./source/drv_FRDM.o \
./source/drv_display.o \
./source/drv_leds.o \
./source/drv_switch.o \
./source/fsm.o \
./source/gpio.o \
./source/lectora.o \
./source/main.o \
./source/thingSpeakLib.o \
./source/timer.o \
./source/uart_drv.o 

C_DEPS += \
./source/drv_FRDM.d \
./source/drv_display.d \
./source/drv_leds.d \
./source/drv_switch.d \
./source/fsm.d \
./source/gpio.d \
./source/lectora.d \
./source/main.d \
./source/thingSpeakLib.d \
./source/timer.d \
./source/uart_drv.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_MK64FN1M0VLL12 -D__USE_CMSIS -DDEBUG -I"C:\Users\Dimas\Downloads\ucosiii_labo_de_micros_project\ucosiii_labo_de_micros_project\source\ucosiii_config" -I"C:\Users\Dimas\Downloads\ucosiii_labo_de_micros_project\ucosiii_labo_de_micros_project\source\rtos\uCOSIII\src\uC-CPU\ARM-Cortex-M4\GNU" -I"C:\Users\Dimas\Downloads\ucosiii_labo_de_micros_project\ucosiii_labo_de_micros_project\source\rtos\uCOSIII\src\uC-CPU" -I"C:\Users\Dimas\Downloads\ucosiii_labo_de_micros_project\ucosiii_labo_de_micros_project\source\rtos\uCOSIII\src\uC-LIB" -I"C:\Users\Dimas\Downloads\ucosiii_labo_de_micros_project\ucosiii_labo_de_micros_project\source\rtos\uCOSIII\src\uCOS-III\Ports\ARM-Cortex-M4\Generic\GNU" -I"C:\Users\Dimas\Downloads\ucosiii_labo_de_micros_project\ucosiii_labo_de_micros_project\source\rtos\uCOSIII\src\uCOS-III\Source" -I../source -I../ -I../SDK/CMSIS -I../SDK/startup -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


