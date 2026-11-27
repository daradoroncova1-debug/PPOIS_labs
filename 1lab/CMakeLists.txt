# 1lab/CMakeLists.txt
# Топ-уровневый CMakeLists для лабы 1.
# Оркестрирует сборку подпроектов "rubik" и "post".

cmake_minimum_required(VERSION 3.15)
project(Lab1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключаем подпроекты
add_subdirectory(rubik)
add_subdirectory(post)
