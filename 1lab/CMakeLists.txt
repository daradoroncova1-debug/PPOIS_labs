cmake_minimum_required(VERSION 3.16)
project(Lab1_Rubik)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(headers)


add_executable(lab1_app 
    src/main.cpp 
    src/RubikCube.cpp
)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.2
)

FetchContent_MakeAvailable(Catch2)


add_executable(lab1_tests 
    tests/test_rubik.cpp 
    src/RubikCube.cpp
)


target_link_libraries(lab1_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(lab1_tests PRIVATE headers tests)