################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../startup/startup_lpc845.c 

OBJS += \
./startup/startup_lpc845.o 

C_DEPS += \
./startup/startup_lpc845.d 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\ADS\board" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\ADS\source" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\ADS" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\ADS\drivers" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\ADS\device" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\ADS\CMSIS" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\ADS\component\uart" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\ADS\utilities" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


