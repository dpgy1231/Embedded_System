################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./myLib.obj" \
"./pinout.obj" \
"./tm4c129xnczad_startup_ccs.obj" \
"./touch.obj" \
"./third_party/fatfs/src/option/cc932.obj" \
"./third_party/fatfs/src/ff.obj" \
"./third_party/fatfs/port/mmc-dk-tm4c129x.obj" \
"./drivers/mx66l51235f.obj" \
"../tm4c129xnczad.cmd" \
$(GEN_CMDS__FLAG) \
-l"libc.a" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include third_party/fatfs/src/option/subdir_vars.mk
-include third_party/fatfs/src/subdir_vars.mk
-include third_party/fatfs/port/subdir_vars.mk
-include drivers/subdir_vars.mk
-include subdir_rules.mk
-include third_party/fatfs/src/option/subdir_rules.mk
-include third_party/fatfs/src/subdir_rules.mk
-include third_party/fatfs/port/subdir_rules.mk
-include drivers/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
LCD_TOUCH.out \

EXE_OUTPUTS__QUOTED += \
"LCD_TOUCH.out" \

BIN_OUTPUTS += \
LCD_TOUCH.hex \

BIN_OUTPUTS__QUOTED += \
"LCD_TOUCH.hex" \


# All Target
all: LCD_TOUCH.out

# Tool invocations
LCD_TOUCH.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me -O2 --gcc --define=ccs="ccs" --define=PART_TM4C129XNCZAD --display_error_number --diag_warning=225 --diag_wrap=off -z -m"LCD_TOUCH.map" --heap_size=0 --stack_size=512 -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/lib" -i"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --reread_libs --warn_sections --display_error_number --diag_wrap=off --xml_link_info="LCD_TOUCH_linkInfo.xml" --rom_model -o "LCD_TOUCH.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

LCD_TOUCH.hex: $(EXE_OUTPUTS)
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armhex"  -o "LCD_TOUCH.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(EXE_OUTPUTS__QUOTED)$(BIN_OUTPUTS__QUOTED)
	-$(RM) "main.pp" "myLib.pp" "pinout.pp" "tm4c129xnczad_startup_ccs.pp" "touch.pp" "third_party\fatfs\src\option\cc932.pp" "third_party\fatfs\src\ff.pp" "third_party\fatfs\port\mmc-dk-tm4c129x.pp" "drivers\mx66l51235f.pp" 
	-$(RM) "main.obj" "myLib.obj" "pinout.obj" "tm4c129xnczad_startup_ccs.obj" "touch.obj" "third_party\fatfs\src\option\cc932.obj" "third_party\fatfs\src\ff.obj" "third_party\fatfs\port\mmc-dk-tm4c129x.obj" "drivers\mx66l51235f.obj" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

