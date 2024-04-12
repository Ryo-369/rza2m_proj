################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
../generate/compiler/asm/cpu.asm \
../generate/compiler/asm/initsect.asm \
../generate/compiler/asm/irq.asm \
../generate/compiler/asm/irqfiq_handler.asm \
../generate/compiler/asm/mmu_operation.asm \
../generate/compiler/asm/r_cache_l1_rza2.asm \
../generate/compiler/asm/reset_handler.asm \
../generate/compiler/asm/vector_mirrortable.asm \
../generate/compiler/asm/vector_table.asm \
../generate/compiler/asm/vfp_init.asm 

LST += \
cpu.lst \
initsect.lst \
irq.lst \
irqfiq_handler.lst \
mmu_operation.lst \
r_cache_l1_rza2.lst \
reset_handler.lst \
vector_mirrortable.lst \
vector_table.lst \
vfp_init.lst 

OBJS += \
./generate/compiler/asm/cpu.o \
./generate/compiler/asm/initsect.o \
./generate/compiler/asm/irq.o \
./generate/compiler/asm/irqfiq_handler.o \
./generate/compiler/asm/mmu_operation.o \
./generate/compiler/asm/r_cache_l1_rza2.o \
./generate/compiler/asm/reset_handler.o \
./generate/compiler/asm/vector_mirrortable.o \
./generate/compiler/asm/vector_table.o \
./generate/compiler/asm/vfp_init.o 

ASM_DEPS += \
./generate/compiler/asm/cpu.d \
./generate/compiler/asm/initsect.d \
./generate/compiler/asm/irq.d \
./generate/compiler/asm/irqfiq_handler.d \
./generate/compiler/asm/mmu_operation.d \
./generate/compiler/asm/r_cache_l1_rza2.d \
./generate/compiler/asm/reset_handler.d \
./generate/compiler/asm/vector_mirrortable.d \
./generate/compiler/asm/vector_table.d \
./generate/compiler/asm/vfp_init.d 

MAP += \
rza2m_ssif_sample_freertos_gcc.map 


# Each subdirectory must supply rules for building sources it contributes
generate/compiler/asm/%.o: ../generate/compiler/asm/%.asm
	@echo 'Building file: $<'
	$(file > $@.in,-mcpu=cortex-a9 -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -fdiagnostics-parseable-fixits -Og -ffunction-sections -fdata-sections -Wnull-dereference -Wstack-usage=256 -g3 -x assembler-with-cpp -I"C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/os_abstraction/inc" -Wa,-adhlns="$@.lst" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<")
	@echo $< && arm-none-eabi-gcc @"$@.in"

