cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
project(MorroGen VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(MorroGen
  src/CellRecord.cpp
  src/FileHeaderRecord.cpp
  src/json.cpp
  src/LandRecord.cpp
  src/LtexRecord.cpp
  src/ModFile.cpp
  src/ModRecord.cpp
  src/ModSubRecord.cpp
  src/MorroGen.cpp
  src/simplexnoise.cpp
  )

target_include_directories(MorroGen
  PUBLIC include
  )
