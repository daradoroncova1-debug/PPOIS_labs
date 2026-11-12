cmake_minimum_required(VERSION 3.15)
project(Lab1_Rubik)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(headers)

add_executable(lab1_app 
    src/main.cpp 
    src/RubikCube.cpp
)

add_executable(1lab_tests 
    tests/test_rubik.cpp 
    src/RubikCube.cpp
)
target_include_directories(1lab_tests PRIVATE headers tests)