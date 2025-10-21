cmake_minimum_required(VERSION 3.10)
project(Noviq VERSION 2.3.0 LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler warnings
if(MSVC)
    add_compile_options(/W4 /WX-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Source files
set(SOURCES
    src/main.c
    src/interpreter/interpreter.c
    src/interpreter/variables.c
    src/interpreter/expressions.c
    src/operators/arithmetic.c
    src/operators/logical.c
    src/operators/comparison.c
    src/statements/display.c
    src/statements/input.c
    src/statements/control_flow.c
    src/utils/error.c
)

# Create executable
add_executable(noviq ${SOURCES})

# Link math library (required for pow() function)
if(UNIX)
    target_link_libraries(noviq PRIVATE m)
endif()

# Installation
install(TARGETS noviq DESTINATION bin)

# Print build information
message(STATUS "Noviq Interpreter v${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
