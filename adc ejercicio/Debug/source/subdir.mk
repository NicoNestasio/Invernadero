################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/adc\ ejercicio.c \
../source/mtb.c \
../source/semihost_hardfault.c 

OBJS += \
./source/adc\ ejercicio.o \
./source/mtb.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/adc\ ejercicio.d \
./source/mtb.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/adc\ ejercicio.o: ../source/adc\ ejercicio.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio\board" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio\source" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio\drivers" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio\device" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio\CMSIS" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio\component\uart" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio\utilities" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"source/adc ejercicio.d" -MT"source/adc\ ejercicio.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -D__REDLIB__ -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio\board" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio\source" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio\drivers" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio\device" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio\CMSIS" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio\component\uart" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\adc ejercicio\utilities" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


