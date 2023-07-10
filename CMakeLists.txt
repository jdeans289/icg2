cmake_minimum_required(VERSION 3.1)

project(ICG VERSION 2.0
            DESCRIPTION "Interface Code Generator (ICG) 2: Electric Boogaloo"
            LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set (TEST_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/test_results)

# TODO: this should be behind some option
enable_testing()

add_subdirectory(extern)
add_subdirectory(ICG)
add_subdirectory(DataTypes)
add_subdirectory(test)