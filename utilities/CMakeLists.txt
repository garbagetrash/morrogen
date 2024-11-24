cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# BSA sub file name printer
add_executable(bsa_file_printer
  bsa_file_printer.cpp
)
target_include_directories(bsa_file_printer
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include
)
target_link_libraries(bsa_file_printer morrogen)
