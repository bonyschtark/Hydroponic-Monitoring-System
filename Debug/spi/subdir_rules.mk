################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
spi/spi.obj: C:/ti/CC3100SDK_1.3.0/cc3100-sdk/platform/tiva-c-connected-launchpad/spi.c $(GEN_OPTS) | $(GEN_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/bin/armcl" -mv7M4 -me --include_path="C:/Users/brads/workspace/getting_started_with_wlan_station" --include_path="C:/ti/tirtos_tivac_2_16_01_14/products/TivaWare_C_Series-2.1.1.71b" --include_path="C:/ti/CC3100SDK_1.3.0/cc3100-sdk/examples/common" --include_path="C:/ti/CC3100SDK_1.3.0/cc3100-sdk/simplelink/include" --include_path="C:/ti/CC3100SDK_1.3.0/cc3100-sdk/simplelink/source" --include_path="C:/ti/CC3100SDK_1.3.0/cc3100-sdk/platform/tiva-c-connected-launchpad" --include_path="C:/ti/ccs901/ccs/tools/compiler/ti-cgt-arm_18.12.1.LTS/include" --define=PART_TM4C1294NCPDT --define=TARGET_IS_BLIZZARD_RA1=1 --define=_USE_CLI_ --define=ccs -g --diag_warning=225 --diag_wrap=off --display_error_number --preproc_with_compile --preproc_dependency="spi/$(basename $(<F)).d_raw" --obj_directory="spi" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


