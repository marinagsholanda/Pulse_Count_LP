################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../segger/SEGGER_RTT.c \
../segger/SEGGER_RTT_Syscalls_GCC.c \
../segger/SEGGER_RTT_printf.c 

OBJS += \
./segger/SEGGER_RTT.o \
./segger/SEGGER_RTT_Syscalls_GCC.o \
./segger/SEGGER_RTT_printf.o 

C_DEPS += \
./segger/SEGGER_RTT.d \
./segger/SEGGER_RTT_Syscalls_GCC.d \
./segger/SEGGER_RTT_printf.d 


# Each subdirectory must supply rules for building sources it contributes
segger/%.o segger/%.su segger/%.cyclo: ../segger/%.c segger/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DMBEDTLS_CONFIG_FILE="mbedtls_config.h"' '-DSEGGER_RTT_BUFFER_SECTION=".data"' -DUSE_HAL_DRIVER -DSTM32WL55xx -DCORE_CM4 -DDEBUG -c -I../../Inc -I../../../../../../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../../../../../../Drivers/CMSIS/Include -I../../../../../../../Drivers/BSP/STM32WLxx_Nucleo -I"C:/Users/Marina/IoG/iog-fw/fw/src/Middlewares/Third_Party/segger" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-segger

clean-segger:
	-$(RM) ./segger/SEGGER_RTT.cyclo ./segger/SEGGER_RTT.d ./segger/SEGGER_RTT.o ./segger/SEGGER_RTT.su ./segger/SEGGER_RTT_Syscalls_GCC.cyclo ./segger/SEGGER_RTT_Syscalls_GCC.d ./segger/SEGGER_RTT_Syscalls_GCC.o ./segger/SEGGER_RTT_Syscalls_GCC.su ./segger/SEGGER_RTT_printf.cyclo ./segger/SEGGER_RTT_printf.d ./segger/SEGGER_RTT_printf.o ./segger/SEGGER_RTT_printf.su

.PHONY: clean-segger

