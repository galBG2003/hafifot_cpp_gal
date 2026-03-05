cmake_minimum_required(VERSION 3.16)
project(calculator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
add_executable(calculator
    src/main.cpp
    src/BasicCalculator.cpp
    src/AdvancedCalculator.cpp
    src/ComplexCalculator.cpp
    src/Calculator.cpp
)

# Include directories
target_include_directories(calculator PRIVATE include)

