################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Client.cpp \
../src/Commands.cpp \
../src/HTManager.cpp \
../src/Server.cpp \
../src/StateTable.cpp \
../src/Ui.cpp \
../src/global.cpp \
../src/main.cpp \
../src/stateTableManager.cpp 

OBJS += \
./src/Client.o \
./src/Commands.o \
./src/HTManager.o \
./src/Server.o \
./src/StateTable.o \
./src/Ui.o \
./src/global.o \
./src/main.o \
./src/stateTableManager.o 

CPP_DEPS += \
./src/Client.d \
./src/Commands.d \
./src/HTManager.d \
./src/Server.d \
./src/StateTable.d \
./src/Ui.d \
./src/global.d \
./src/main.d \
./src/stateTableManager.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -pthread -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


