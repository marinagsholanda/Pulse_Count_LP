################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Marina/IoG/iog-fw/fw/src/sdk/st/wba/Utilities/lpm/tiny_lpm/stm32_lpm.c \
C:/Users/Marina/IoG/iog-fw/fw/src/sdk/lp-fwk/src/fw/third-party/st/low-power/stm32_lpm_if.c 

OBJS += \
./tiny_lpm/stm32_lpm.o \
./tiny_lpm/stm32_lpm_if.o 

C_DEPS += \
./tiny_lpm/stm32_lpm.d \
./tiny_lpm/stm32_lpm_if.d 


# Each subdirectory must supply rules for building sources it contributes
tiny_lpm/stm32_lpm.o: C:/Users/Marina/IoG/iog-fw/fw/src/sdk/st/wba/Utilities/lpm/tiny_lpm/stm32_lpm.c tiny_lpm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DMBEDTLS_CONFIG_FILE="mbedtls_config.h"' '-DSEGGER_RTT_BUFFER_SECTION=".segger"' -DUSE_HAL_DRIVER -DSTM32WL55xx -DCORE_CM4 -DDEBUG -c -I../../Inc -I"C:/Users/Marina/IoG/iog-fw/fw/src/sdk/lp-fwk/src/fw/third-party/st/low-power" -I"C:/Users/Marina/IoG/iog-fw/fw/src/sdk/lp-fwk/src/fw/controller/hal" -I"C:/Users/Marina/IoG/iog-fw/fw/src/sdk/lp-fwk/src/fw/third-party/st/confs" -I"C:/Users/Marina/IoG/iog-fw/fw/src/sdk/st/wba/Utilities/lpm/tiny_lpm" -I../../../../../../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../../../../../../Drivers/CMSIS/Include -I../../../../../../../Drivers/BSP/STM32WLxx_Nucleo -I"C:/Users/Marina/IoG/iog-fw/fw/src/Middlewares/Third_Party/segger" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
tiny_lpm/stm32_lpm_if.o: C:/Users/Marina/IoG/iog-fw/fw/src/sdk/lp-fwk/src/fw/third-party/st/low-power/stm32_lpm_if.c tiny_lpm/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 '-DMBEDTLS_CONFIG_FILE="mbedtls_config.h"' '-DSEGGER_RTT_BUFFER_SECTION=".segger"' -DUSE_HAL_DRIVER -DSTM32WL55xx -DCORE_CM4 -DDEBUG -c -I../../Inc -I"C:/Users/Marina/IoG/iog-fw/fw/src/sdk/lp-fwk/src/fw/third-party/st/low-power" -I"C:/Users/Marina/IoG/iog-fw/fw/src/sdk/lp-fwk/src/fw/controller/hal" -I"C:/Users/Marina/IoG/iog-fw/fw/src/sdk/lp-fwk/src/fw/third-party/st/confs" -I"C:/Users/Marina/IoG/iog-fw/fw/src/sdk/st/wba/Utilities/lpm/tiny_lpm" -I../../../../../../../Drivers/STM32WLxx_HAL_Driver/Inc -I../../../../../../../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../../../../../../../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../../../../../../../Drivers/CMSIS/Include -I../../../../../../../Drivers/BSP/STM32WLxx_Nucleo -I"C:/Users/Marina/IoG/iog-fw/fw/src/Middlewares/Third_Party/segger" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-tiny_lpm

clean-tiny_lpm:
	-$(RM) ./tiny_lpm/stm32_lpm.cyclo ./tiny_lpm/stm32_lpm.d ./tiny_lpm/stm32_lpm.o ./tiny_lpm/stm32_lpm.su ./tiny_lpm/stm32_lpm_if.cyclo ./tiny_lpm/stm32_lpm_if.d ./tiny_lpm/stm32_lpm_if.o ./tiny_lpm/stm32_lpm_if.su

.PHONY: clean-tiny_lpm

