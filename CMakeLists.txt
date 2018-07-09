cmake_minimum_required(VERSION 3.1)
project(portaudio-list)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_C_STANDARD 11)

# math lib
find_library(MATH_LIB m)
if(NOT MATH_LIB)
	message(FATAL_ERROR "math library not found")
endif()

# portaudio
find_library(PORTAUDIO_LIB portaudio)
if(NOT PORTAUDIO_LIB)
	message(FATAL_ERROR "portaudio library not found")
endif()

# Build executable
add_executable(
	portaudio-list
	list.c
)

# Link executable
target_link_libraries(portaudio-list portaudio m)
