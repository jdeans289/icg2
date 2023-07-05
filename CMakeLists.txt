cmake_minimum_required(VERSION 3.1)

project(ICG VERSION 2.0
            DESCRIPTION "Interface Code Generator (ICG) 2: Electric Boogaloo"
            LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

enable_testing()

add_subdirectory(extern)

# Build ICG
# add_executable (icg icg_main.cpp icg_main.hh)
# target_link_libraries(icg PUBLIC clang)

# Build ICG
add_subdirectory(ICG)

# Build DataTypes
add_subdirectory(DataTypes)

set (FILES_TO_ICG test_files/header.hpp)

# Generate the io_src info
add_custom_command(
    OUTPUT io_src.hh
    DEPENDS icg
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMAND ./build/bin/icg ${FILES_TO_ICG}
    COMMENT "============================= ICGing... ============================="
)

# Build Executable
add_executable (io_src_main io_src_main.cpp io_src.hh)
target_link_libraries(io_src_main PUBLIC decl)
target_include_directories(io_src_main PUBLIC DataTypes/include)
