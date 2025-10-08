################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Marina/IoG/iog-fw/fw/src/sdk/lp-fwk/src/fw/third-party/st/timer/timer_if.c 

OBJS += \
./timer/timer_if.o 

C_DEPS += \
./timer/timer_if.d 


# Each subdirectory must supply rules for building sources it contributes
timer/timer_if.o: C:/Users/Marina/IoG/iog-fw/fw/src/sdk/lp-fwk/src/fw/third-party/st/timer/timer_if.c timer/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DMBEDTLS_CONFIG_FILE="mbedtls_config.h"' '-DSEGGER_RTT_BUFFER_SECTION=".data"' -DUSE_HAL_DRIVER -DSTM32WL55xx -DCORE_CM4 -DDEBUG -c -I../../Inc -I"C:/Users/Marina/IoG/iog-fw/fw/src/sdk/lp-fwk/src/fw/controller/hal" -I"C:/Users/Marina/IoG/iog-fw/fw/src/sdk/lp-fwk/src/fw/third-party/st/confs" -I"C:/Users/Marina/IoG/iog-fw/fw/src/sdk/st/wba/Utilities/lpm/tiny_lpm" -I../../../../../../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../../../../../../Drivers/CMSIS/Include -I../../../../../../../Drivers/BSP/STM32WLxx_Nucleo -I"C:/Users/Marina/IoG/iog-fw/fw/src/Middlewares/Third_Party/segger" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-timer

clean-timer:
	-$(RM) ./timer/timer_if.cyclo ./timer/timer_if.d ./timer/timer_if.o ./timer/timer_if.su

.PHONY: clean-timer

