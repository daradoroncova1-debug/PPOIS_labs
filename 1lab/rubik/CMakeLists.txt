if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-abs-path")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
endif()

include_directories(headers)

add_executable(rubik_app
    src/main.cpp
    src/RubikCube.cpp
)

find_package(Catch2 3 REQUIRED)

add_executable(rubik_tests
    tests/test_rubik.cpp
    src/RubikCube.cpp
)


target_link_libraries(rubik_tests PRIVATE Catch2::Catch2WithMain)
target_include_directories(rubik_tests PRIVATE headers tests)