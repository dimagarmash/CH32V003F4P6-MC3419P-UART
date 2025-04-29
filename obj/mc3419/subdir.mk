################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../mc3419/MC34X9.c \
../mc3419/accelerometer.c 

C_DEPS += \
./mc3419/MC34X9.d \
./mc3419/accelerometer.d 

OBJS += \
./mc3419/MC34X9.o \
./mc3419/accelerometer.o 



# Each subdirectory must supply rules for building sources it contributes
mc3419/%.o: ../mc3419/%.c
	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"i:/DimaWorkSpace/PetProj/CH32V003F4P6+MC3419P+UART/Debug" -I"i:/DimaWorkSpace/PetProj/CH32V003F4P6+MC3419P+UART/Core" -I"i:/DimaWorkSpace/PetProj/CH32V003F4P6+MC3419P+UART/User" -I"i:/DimaWorkSpace/PetProj/CH32V003F4P6+MC3419P+UART/Peripheral/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
