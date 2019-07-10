################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./ADCSWTrigger.obj" \
"./PLL.obj" \
"./SysTick.obj" \
"./connect.obj" \
"./main.obj" \
"./tm4c1294ncpdt_startup_ccs.obj" \
"./board/board.obj" \
"./cli_uart/cli_uart.obj" \
"./simplelink/source/device.obj" \
"./simplelink/source/driver.obj" \
"./simplelink/source/flowcont.obj" \
"./simplelink/source/fs.obj" \
"./simplelink/source/netapp.obj" \
"./simplelink/source/netcfg.obj" \
"./simplelink/source/nonos.obj" \
"./simplelink/source/socket.obj" \
"./simplelink/source/spawn.obj" \
"./simplelink/source/wlan.obj" \
"./spi/spi.obj" \
"./timer/timer_tick.obj" \
"../tm4c1294ncpdt.cmd" \
$(GEN_CMDS__FLAG) \
-l"C:/ti/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b/driverlib/ccs/Debug/driverlib.lib" \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include board/subdir_vars.mk
-include cli_uart/subdir_vars.mk
-include simplelink/source/subdir_vars.mk
-include spi/subdir_vars.mk
-include timer/subdir_vars.mk
-include subdir_rules.mk
-include board/subdir_rules.mk
-include cli_uart/subdir_rules.mk
-include simplelink/source/subdir_rules.mk
-include spi/subdir_rules.mk
-include timer/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
getting_started_with_wlan_station.out \

EXE_OUTPUTS__QUOTED += \
"getting_started_with_wlan_station.out" \

BIN_OUTPUTS += \
getting_started_with_wlan_station.bin \

BIN_OUTPUTS__QUOTED += \
"getting_started_with_wlan_station.bin" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "getting_started_with_wlan_station.out"

# Tool invocations
getting_started_with_wlan_station.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: ARM Linker'
	"C:/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/bin/armcl" -mv7M4 -me --define=PART_TM4C1294NCPDT --define=TARGET_IS_BLIZZARD_RA1=1 --define=_USE_CLI_ --define=ccs -g --diag_warning=225 --diag_wrap=off --display_error_number -z -m"$getting_started_with_wlan_ap.map" --heap_size=0 --stack_size=704 -i"C:/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/lib" -i"C:/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/include" --reread_libs --define=PART_TM4C1294NCPDT --define=TARGET_IS_BLIZZARD_RA1=1 --define=_USE_CLI_ --define=ccs --diag_warning=225 --diag_wrap=off --display_error_number --warn_sections --xml_link_info="getting_started_with_wlan_ap_linkInfo.xml" --rom_model -o "getting_started_with_wlan_station.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

getting_started_with_wlan_station.bin: $(EXE_OUTPUTS)
	@echo 'Building files: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo 'Invoking: ARM Hex Utility'
	"C:/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/bin/armhex"  -o "getting_started_with_wlan_station.bin" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building: $(strip $(EXE_OUTPUTS__QUOTED))'
	@echo ' '
	@$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "ADCSWTrigger.obj" "PLL.obj" "SysTick.obj" "connect.obj" "main.obj" "tm4c1294ncpdt_startup_ccs.obj" "board\board.obj" "cli_uart\cli_uart.obj" "simplelink\source\device.obj" "simplelink\source\driver.obj" "simplelink\source\flowcont.obj" "simplelink\source\fs.obj" "simplelink\source\netapp.obj" "simplelink\source\netcfg.obj" "simplelink\source\nonos.obj" "simplelink\source\socket.obj" "simplelink\source\spawn.obj" "simplelink\source\wlan.obj" "spi\spi.obj" "timer\timer_tick.obj" 
	-$(RM) "ADCSWTrigger.d" "PLL.d" "SysTick.d" "connect.d" "main.d" "tm4c1294ncpdt_startup_ccs.d" "board\board.d" "cli_uart\cli_uart.d" "simplelink\source\device.d" "simplelink\source\driver.d" "simplelink\source\flowcont.d" "simplelink\source\fs.d" "simplelink\source\netapp.d" "simplelink\source\netcfg.d" "simplelink\source\nonos.d" "simplelink\source\socket.d" "simplelink\source\spawn.d" "simplelink\source\wlan.d" "spi\spi.d" "timer\timer_tick.d" 
	-@echo 'Finished clean'
	-@echo ' '

post-build:
	-C:/ti/ccs901/ccs/utils/tiobj2bin/tiobj2bin getting_started_with_wlan_station.out getting_started_with_wlan_station.bin C:/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/bin/armofd C:/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/bin/armhex C:/ti/ccs901/ccs/utils/tiobj2bin/mkhex4bin
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

