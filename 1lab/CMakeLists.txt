cmake_minimum_required(VERSION 3.15)
project(Lab1_Rubik)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(headers)

add_executable(lab1_app 
    src/main.cpp 
    src/RubikCube.cpp
)