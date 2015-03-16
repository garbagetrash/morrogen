################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../CellRecord.cpp \
../LandRecord.cpp \
../ModFile.cpp \
../ModRecord.cpp \
../ModSubRecord.cpp \
../json.cpp \
../mwScript.cpp \
../simplexnoise.cpp 

OBJS += \
./CellRecord.o \
./LandRecord.o \
./ModFile.o \
./ModRecord.o \
./ModSubRecord.o \
./json.o \
./mwScript.o \
./simplexnoise.o 

CPP_DEPS += \
./CellRecord.d \
./LandRecord.d \
./ModFile.d \
./ModRecord.d \
./ModSubRecord.d \
./json.d \
./mwScript.d \
./simplexnoise.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


