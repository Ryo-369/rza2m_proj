################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/renesas/application/console/command.c \
../src/renesas/application/console/console.c \
../src/renesas/application/console/wild_compare.c 

LST += \
command.lst \
console.lst \
wild_compare.lst 

C_DEPS += \
./src/renesas/application/console/command.d \
./src/renesas/application/console/console.d \
./src/renesas/application/console/wild_compare.d 

OBJS += \
./src/renesas/application/console/command.o \
./src/renesas/application/console/console.o \
./src/renesas/application/console/wild_compare.o 

MAP += \
rza2m_ssif_sample_freertos_gcc.map 


# Each subdirectory must supply rules for building sources it contributes
src/renesas/application/console/%.o: ../src/renesas/application/console/%.c
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-a9 -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -fdiagnostics-parseable-fixits -Og -ffunction-sections -fdata-sections -Wnull-dereference -Wstack-usage=256 -g3 -I"C:\\Users\\ryota\\e2_studio\\workspace\\rza2m_ssif_sample_freertos_gcc\\src\\FreeRTOS\\include" -I"C:\\Users\\ryota\\e2_studio\\workspace\\rza2m_ssif_sample_freertos_gcc\\generate\\sc_drivers\\r_dmac\\inc" -I"C:\\Users\\ryota\\e2_studio\\workspace\\rza2m_ssif_sample_freertos_gcc\\src\\FreeRTOS\\portable\\GCC\\ARM_CA9_RZA2M" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/compiler/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/configuration" -I"C:\\Users\\ryota\\e2_studio\\workspace\\rza2m_ssif_sample_freertos_gcc\\src\\config_files" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/drivers/r_cache/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/drivers/r_cpg/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/drivers/r_gpio/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/drivers/r_intc/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/drivers/r_mmu/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/drivers/r_stb/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/os_abstraction/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/system/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/src/renesas/application/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/src/renesas/application/console/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/sc_drivers/r_scifa/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/sc_drivers/r_cbuffer/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/sc_drivers/r_ostm/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/system/iodefine" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/src/renesas/application/app_sound/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/sc_drivers/r_ssif/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/sc_drivers/r_riic/inc" -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/sc_drivers/r_dmac/inc" -fabi-version=0 -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<")
	@echo $< && arm-none-eabi-gcc @"$@.in"

