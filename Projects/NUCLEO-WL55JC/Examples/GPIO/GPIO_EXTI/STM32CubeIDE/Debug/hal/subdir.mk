################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../hal/hal_rtc.c 

OBJS += \
./hal/hal_rtc.o 

C_DEPS += \
./hal/hal_rtc.d 


# Each subdirectory must supply rules for building sources it contributes
hal/%.o hal/%.su hal/%.cyclo: ../hal/%.c hal/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DMBEDTLS_CONFIG_FILE="mbedtls_config.h"' '-DSEGGER_RTT_BUFFER_SECTION=".data"' -DUSE_HAL_DRIVER -DSTM32WL55xx -DCORE_CM4 -DDEBUG -c -I../../Inc -I"C:/Users/Marina/IoG/iog-fw/fw/src/sdk/lp-fwk/src/fw/controller/hal" -I"C:/Users/Marina/IoG/iog-fw/fw/src/sdk/lp-fwk/src/fw/third-party/st/confs" -I"C:/Users/Marina/IoG/iog-fw/fw/src/sdk/st/wba/Utilities/lpm/tiny_lpm" -I../../../../../../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../../../../../../Drivers/CMSIS/Include -I../../../../../../../Drivers/BSP/STM32WLxx_Nucleo -I"C:/Users/Marina/IoG/iog-fw/fw/src/Middlewares/Third_Party/segger" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-hal

clean-hal:
	-$(RM) ./hal/hal_rtc.cyclo ./hal/hal_rtc.d ./hal/hal_rtc.o ./hal/hal_rtc.su

.PHONY: clean-hal

