cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(MorroGen VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

# Create a morrogen library
add_library(morrogen
  src/BsaLoader.cpp
  src/CellRecord.cpp
  src/FileHeaderRecord.cpp
  src/LandRecord.cpp
  src/LtexRecord.cpp
  src/ModFile.cpp
  src/ModRecord.cpp
  src/ModSubRecord.cpp
  src/Resources.cpp
  src/simplexnoise.cpp
)
target_include_directories(morrogen
  PUBLIC include
)


add_executable(test_generate
  src/MorroGen.cpp
)
target_include_directories(test_generate
  PRIVATE include
)
target_link_libraries(test_generate
  morrogen
)

# Compile utilities
add_subdirectory(utilities)
