################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_UPPER_SRCS += \
../src/analisi_VRtoAM.C \
../src/analisi_VRtoAM2_chi2.C \
../src/dech.C \
../src/main_macro.C \
../src/mia_dech.C \
../src/read_histograms.C \
../src/readandplot.C \
../src/slices.C 

CPP_SRCS += \
../src/DatiChanneling.cpp \
../src/main.cpp 

C_UPPER_DEPS += \
./src/analisi_VRtoAM.d \
./src/analisi_VRtoAM2_chi2.d \
./src/dech.d \
./src/main_macro.d \
./src/mia_dech.d \
./src/read_histograms.d \
./src/readandplot.d \
./src/slices.d 

OBJS += \
./src/DatiChanneling.o \
./src/analisi_VRtoAM.o \
./src/analisi_VRtoAM2_chi2.o \
./src/dech.o \
./src/main.o \
./src/main_macro.o \
./src/mia_dech.o \
./src/read_histograms.o \
./src/readandplot.o \
./src/slices.o 

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


