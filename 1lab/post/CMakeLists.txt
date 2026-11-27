# 1lab/post/CMakeLists.txt
# Подпроект "Машина Поста"

# Бинарники: post_app (приложение), post_tests (тесты)

# Флаги покрытия включаем только в Debug-сборке
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-abs-path")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

include_directories(headers)

# Основное приложение
add_executable(post_app
    src/main.cpp
    src/PostMachine.cpp
)

# Ищем установленную Catch2 (версия 3)
find_package(Catch2 3 REQUIRED)

# Тесты
add_executable(post_tests
    tests/test_post.cpp
    src/PostMachine.cpp
)

# Подключаем библиотеку (Catch2WithMain добавит функцию main())
target_link_libraries(post_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(post_tests PRIVATE headers tests)