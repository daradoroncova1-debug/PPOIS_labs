#define CATCH_CONFIG_MAIN
#include "RubikCube.h"
#include "catch.hpp"

TEST_CASE("Cube is solved by default", "[rubik]") {
  RubiksCube cube;
  REQUIRE(cube.isSolved() == true);
}

TEST_CASE("Rotating face 4 times returns to original state", "[rubik]") {
  RubiksCube cube;
  RubiksCube original = cube; // Проверяем конструктор копирования

  for (int i = 0; i < 4; i++) {
    cube.rotate(Face::FRONT, true);
  }

  REQUIRE(cube == original); // Проверяем оператор ==
}

TEST_CASE("Scramble makes cube unsolved", "[rubik]") {
  RubiksCube cube;
  cube.scramble(10);
  REQUIRE(cube.isSolved() == false);
}