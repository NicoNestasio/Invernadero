################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/mtb.c \
../source/prueba_Ej7.c \
../source/semihost_hardfault.c 

OBJS += \
./source/mtb.o \
./source/prueba_Ej7.o \
./source/semihost_hardfault.o 

C_DEPS += \
./source/mtb.d \
./source/prueba_Ej7.d \
./source/semihost_hardfault.d 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=0 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\prueba_Ej7\board" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\prueba_Ej7\source" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\prueba_Ej7" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\prueba_Ej7\drivers" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\prueba_Ej7\device" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\prueba_Ej7\CMSIS" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\prueba_Ej7\component\uart" -I"C:\Users\PC-INC\Documents\MCUXpressoIDE_11.1.1_3241\prueba_Ej7\utilities" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


