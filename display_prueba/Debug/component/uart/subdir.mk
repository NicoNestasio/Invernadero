################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/uart/miniusart_adapter.c 

OBJS += \
./component/uart/miniusart_adapter.o 

C_DEPS += \
./component/uart/miniusart_adapter.d 


# Each subdirectory must supply rules for building sources it contributes
component/uart/%.o: ../component/uart/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\display_prueba\board" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\display_prueba\source" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\display_prueba" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\display_prueba\drivers" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\display_prueba\device" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\display_prueba\CMSIS" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\display_prueba\component\uart" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\display_prueba\utilities" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


