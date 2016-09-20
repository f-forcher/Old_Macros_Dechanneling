################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ECLIPSE_MARS_PROJECT_CPP11.cpp 

OBJS += \
./src/ECLIPSE_MARS_PROJECT_CPP11.o 

CPP_DEPS += \
./src/ECLIPSE_MARS_PROJECT_CPP11.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -DDEBUG -UNDEBUG -O0 -g3 -ggdb -fsanitize=address -pedantic -Wall -Wextra -c -fmessage-length=0 -Weffc++ -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


