################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
FreeRTOS/Source/portable/MemMang/%.obj: ../FreeRTOS/Source/portable/MemMang/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"C:/ti/ccs1270/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me --include_path="C:/Users/youssef/advanced_embedded/FreeRtos/final_project" --include_path="C:/Users/youssef/advanced_embedded/FreeRtos/final_project/Common" --include_path="C:/Users/youssef/advanced_embedded/FreeRtos/final_project/MCAL" --include_path="C:/Users/youssef/advanced_embedded/FreeRtos/final_project/MCAL/GPIO" --include_path="C:/Users/youssef/advanced_embedded/FreeRtos/final_project/MCAL/GPTM" --include_path="C:/Users/youssef/advanced_embedded/FreeRtos/final_project/MCAL/UART" --include_path="C:/Users/youssef/advanced_embedded/FreeRtos/final_project/FreeRTOS/Source/include" --include_path="C:/Users/youssef/advanced_embedded/FreeRtos/final_project/FreeRTOS/Source/portable/CCS/ARM_CM4F" --include_path="C:/ti/ccs1270/ccs/tools/compiler/ti-cgt-arm_20.2.7.LTS/include" --define=ccs="ccs" --define=PART_TM4C123GH6PM -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="FreeRTOS/Source/portable/MemMang/$(basename $(<F)).d_raw" --obj_directory="FreeRTOS/Source/portable/MemMang" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


