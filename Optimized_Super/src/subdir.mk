################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_UPPER_SRCS += \
../src/dech.C \
../src/main_macro.C \
../src/mia_dech.C \
../src/read_histograms.C \
../src/readandplot.C 

CPP_SRCS += \
../src/DatiChanneling.cpp \
../src/main.cpp 

C_UPPER_DEPS += \
./src/dech.d \
./src/main_macro.d \
./src/mia_dech.d \
./src/read_histograms.d \
./src/readandplot.d 

OBJS += \
./src/DatiChanneling.o \
./src/dech.o \
./src/main.o \
./src/main_macro.o \
./src/mia_dech.o \
./src/read_histograms.o \
./src/readandplot.o 

CPP_DEPS += \
./src/DatiChanneling.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++  "$<" -std=c++0x -DNDEBUG -UDEBUG -O3 -flto -march=native -Wall -c -fmessage-length=0 `root-config --cflags` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.C
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++  "$<" -std=c++0x -DNDEBUG -UDEBUG -O3 -flto -march=native -Wall -c -fmessage-length=0 `root-config --cflags` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '


