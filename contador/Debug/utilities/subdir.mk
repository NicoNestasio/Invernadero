################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../utilities/fsl_debug_console.c 

OBJS += \
./utilities/fsl_debug_console.o 

C_DEPS += \
./utilities/fsl_debug_console.d 


# Each subdirectory must supply rules for building sources it contributes
utilities/%.o: ../utilities/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\contador\board" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\contador\source" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\contador" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\contador\drivers" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\contador\device" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\contador\CMSIS" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\contador\component\uart" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\contador\utilities" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


