################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

-include makefile.init

RM := rm -rf *.lst *.lis *.lpp *.map

# All of the sources participating in the build are defined here
-include sources.mk
-include src/user_prog/subdir.mk
-include src/renesas/middle/subdir.mk
-include src/renesas/hwsetup/subdir.mk
-include src/renesas/application/console/cmd/subdir.mk
-include src/renesas/application/console/subdir.mk
-include src/renesas/application/app_sound/src/subdir.mk
-include src/FreeRTOS/portable/GCC/ARM_CA9_RZA2M/subdir.mk
-include src/FreeRTOS/subdir.mk
-include src/subdir.mk
-include generate/system/subdir.mk
-include generate/sc_drivers/r_ssif/src/lld/subdir.mk
-include generate/sc_drivers/r_ssif/src/hld/subdir.mk
-include generate/sc_drivers/r_scifa/src/lld/subdir.mk
-include generate/sc_drivers/r_scifa/src/hld/subdir.mk
-include generate/sc_drivers/r_riic/src/lld/subdir.mk
-include generate/sc_drivers/r_riic/src/hld/subdir.mk
-include generate/sc_drivers/r_ostm/src/lld/subdir.mk
-include generate/sc_drivers/r_ostm/src/hld/subdir.mk
-include generate/sc_drivers/r_dmac/src/lld/subdir.mk
-include generate/sc_drivers/r_dmac/src/hld/subdir.mk
-include generate/sc_drivers/r_cbuffer/src/subdir.mk
-include generate/os_abstraction/src/subdir.mk
-include generate/gr_mango_boot/subdir.mk
-include generate/drivers/r_stb/src/lld/subdir.mk
-include generate/drivers/r_mmu/src/lld/subdir.mk
-include generate/drivers/r_intc/src/lld/subdir.mk
-include generate/drivers/r_gpio/src/lld/subdir.mk
-include generate/drivers/r_gpio/src/hld/subdir.mk
-include generate/drivers/r_cpg/src/lld/subdir.mk
-include generate/drivers/r_cpg/src/hld/subdir.mk
-include generate/drivers/r_cache/src/lld/subdir.mk
-include generate/configuration/subdir.mk
-include generate/compiler/init/subdir.mk
-include generate/compiler/asm/subdir.mk
-include generate/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_UPPER_DEPS)),)
-include $(CC_UPPER_DEPS)
endif
ifneq ($(strip $(C++M_UPPER_DEPS)),)
-include $(C++M_UPPER_DEPS)
endif
ifneq ($(strip $(C++_UPPER_DEPS)),)
-include $(C++_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_UPPER_DEPS)),)
-include $(CPP_UPPER_DEPS)
endif
ifneq ($(strip $(FSY_DEPS)),)
-include $(FSY_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(SRC_UPPER_DEPS)),)
-include $(SRC_UPPER_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(PP_UPPER_DEPS)),)
-include $(PP_UPPER_DEPS)
endif
ifneq ($(strip $(P_DEPS)),)
-include $(P_DEPS)
endif
ifneq ($(strip $(CXXM_DEPS)),)
-include $(CXXM_DEPS)
endif
ifneq ($(strip $(JMP_UPPER_DEPS)),)
-include $(JMP_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(SX_UPPER_DEPS)),)
-include $(SX_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(JMP_DEPS)),)
-include $(JMP_DEPS)
endif
ifneq ($(strip $(CXX_UPPER_DEPS)),)
-include $(CXX_UPPER_DEPS)
endif
ifneq ($(strip $(CCM_DEPS)),)
-include $(CCM_DEPS)
endif
ifneq ($(strip $(P_UPPER_DEPS)),)
-include $(P_UPPER_DEPS)
endif
ifneq ($(strip $(CXXM_UPPER_DEPS)),)
-include $(CXXM_UPPER_DEPS)
endif
ifneq ($(strip $(FSY_UPPER_DEPS)),)
-include $(FSY_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CCM_UPPER_DEPS)),)
-include $(CCM_UPPER_DEPS)
endif
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif
ifneq ($(strip $(PP_DEPS)),)
-include $(PP_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(SX_DEPS)),)
-include $(SX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C++M_DEPS)),)
-include $(C++M_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
rza2m_ssif_sample_freertos_gcc.bin \

SECONDARY_SIZE += \
rza2m_ssif_sample_freertos_gcc.siz \

LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS += \
librza2m_ssif_sample_freertos_gcc.a \


# All Target
all: rza2m_ssif_sample_freertos_gcc.elf secondary-outputs

# Tool invocations
rza2m_ssif_sample_freertos_gcc.elf: $(OBJS) $(USER_OBJS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS)
	$(file > rza2m_ssif_sample_freertos_gcc.elf.in,-mcpu=cortex-a9 -marm -mlittle-endian -mfloat-abi=hard -mfpu=neon -mno-unaligned-access -fdiagnostics-parseable-fixits -Og -ffunction-sections -fdata-sections -Wnull-dereference -Wstack-usage=256 -g3 -o "rza2m_ssif_sample_freertos_gcc.elf" $(OBJS) $(USER_OBJS) -T "C:/Users/ryota/e2_studio/workspace/rza2m_ssif_sample_freertos_gcc/generate/linker_script.ld" -L "C:\\\\Users\\\\ryota\\\\e2_studio\\\\workspace\\\\rza2m_ssif_sample_freertos_gcc\\\\HardwareDebug" -Wl,--start-group -lrza2m_ssif_sample_freertos_gcc -lgcc -lstdc++ -Wl,--end-group -nostartfiles -Xlinker --gc-sections -Wl,-Map,"rza2m_ssif_sample_freertos_gcc.map" -Wl,-eStart)
	@echo Building target: $@ && arm-none-eabi-g++ @"rza2m_ssif_sample_freertos_gcc.elf.in"
rza2m_ssif_sample_freertos_gcc.bin: rza2m_ssif_sample_freertos_gcc.elf
	arm-none-eabi-objcopy -O binary "rza2m_ssif_sample_freertos_gcc.elf"  "rza2m_ssif_sample_freertos_gcc.bin"
rza2m_ssif_sample_freertos_gcc.siz: rza2m_ssif_sample_freertos_gcc.elf
	arm-none-eabi-size --format=berkeley "rza2m_ssif_sample_freertos_gcc.elf"
librza2m_ssif_sample_freertos_gcc.a: 
	arm-none-eabi-libgen  --header-files=all --select-lib=newlib --compiler-options=-fdiagnostics-parseable-fixits,-Og,-ffunction-sections,-fdata-sections,-mcpu=cortex-a9,-mlittle-endian,-mfloat-abi=hard,-mfpu=neon,-mno-unaligned-access,-D_WANT_IO_LONG_LONG --assembler-options=-mcpu=cortex-a9,-mlittle-endian,-mfloat-abi=hard,-mfpu=neon -o "librza2m_ssif_sample_freertos_gcc.a"
# Other Targets
clean:
	-$(file > clean_file.tmp,  $(CC_UPPER_DEPS)  $(C++M_UPPER_DEPS)  $(C++_UPPER_DEPS)  $(CPP_UPPER_DEPS)  $(FSY_DEPS)  $(C_UPPER_DEPS)  $(SECONDARY_SIZE)  $(SRC_UPPER_DEPS)  $(S_DEPS)  $(PP_UPPER_DEPS)  $(P_DEPS)  $(CXXM_DEPS)  $(JMP_UPPER_DEPS)  $(ASM_UPPER_DEPS)  $(SX_UPPER_DEPS)  $(CXX_DEPS)  $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS)  $(JMP_DEPS)  $(CXX_UPPER_DEPS)  $(CCM_DEPS)  $(P_UPPER_DEPS)  $(LST)  $(CXXM_UPPER_DEPS)  $(FSY_UPPER_DEPS)  $(C_DEPS)  $(CCM_UPPER_DEPS)  $(SRC_DEPS)  $(PP_DEPS)  $(CC_DEPS)  $(SX_DEPS)  $(C++_DEPS)  $(OBJS)  $(SECONDARY_FLASH)  $(ASM_DEPS)  $(S_UPPER_DEPS)  $(MAP)  $(C++M_DEPS)  $(CPP_DEPS) )
	-@ xargs -P 2 -s 32000 -t -a clean_file.tmp $(RM)
	-@$(RM) clean_file.tmp
	-$(RM)  rza2m_ssif_sample_freertos_gcc.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents 

-include ../makefile.targets
