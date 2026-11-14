cmake_minimum_required(VERSION 3.15)
project(Lab1_Rubik)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-abs-path")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")

include_directories(headers)

# Основное приложение
add_executable(lab1_app 
    src/main.cpp 
    src/RubikCube.cpp
)

# Ищем установленную Catch2 (версия 3)
find_package(Catch2 3 REQUIRED)

# Тесты
add_executable(lab1_tests 
    tests/test_rubik.cpp 
    src/RubikCube.cpp
)

# Подключаем библиотеку (Catch2WithMain добавит функцию main())
target_link_libraries(lab1_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(lab1_tests PRIVATE headers tests)