################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_UPPER_SRCS += \
../src/analizza_dechanneling.C \
../src/dech.C \
../src/main_macro.C \
../src/mia_dech.C 

CPP_SRCS += \
../src/DatiChanneling.cpp \
../src/main.cpp 

C_UPPER_DEPS += \
./src/analizza_dechanneling.d \
./src/dech.d \
./src/main_macro.d \
./src/mia_dech.d 

OBJS += \
./src/DatiChanneling.o \
./src/analizza_dechanneling.o \
./src/dech.o \
./src/main.o \
./src/main_macro.o \
./src/mia_dech.o 

CPP_DEPS += \
./src/DatiChanneling.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ "$<" -std=c++0x -DDEBUG -UNDEBUG -O0 -g3 -ggdb -fsanitize=address -pedantic -Wall -Wextra -c -fmessage-length=0 -Weffc++ `root-config --cflags` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.C
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ "$<" -std=c++0x -DDEBUG -UNDEBUG -O0 -g3 -ggdb -fsanitize=address -pedantic -Wall -Wextra -c -fmessage-length=0 -Weffc++ `root-config --cflags` -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@"
	@echo 'Finished building: $<'
	@echo ' '


