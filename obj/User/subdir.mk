################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/Prnt.c \
../User/ch32v00x_it.c \
../User/main.c \
../User/system.c \
../User/system_ch32v00x.c 

C_DEPS += \
./User/Prnt.d \
./User/ch32v00x_it.d \
./User/main.d \
./User/system.d \
./User/system_ch32v00x.d 

OBJS += \
./User/Prnt.o \
./User/ch32v00x_it.o \
./User/main.o \
./User/system.o \
./User/system_ch32v00x.o 



# Each subdirectory must supply rules for building sources it contributes
User/%.o: ../User/%.c
	@	riscv-none-embed-gcc -march=rv32ecxw -mabi=ilp32e -msmall-data-limit=0 -msave-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized -g -I"i:/DimaWorkSpace/PetProj/CH32V003F4P6+MC3419P+UART/Debug" -I"i:/DimaWorkSpace/PetProj/CH32V003F4P6+MC3419P+UART/Core" -I"i:/DimaWorkSpace/PetProj/CH32V003F4P6+MC3419P+UART/User" -I"i:/DimaWorkSpace/PetProj/CH32V003F4P6+MC3419P+UART/Peripheral/inc" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
