#include "RubikCube.h"
#include <catch2/catch_test_macros.hpp>
#include <sstream>

TEST_CASE("Cube is solved by default", "[rubik]") {
  RubiksCube cube;
  REQUIRE(cube.isSolved() == true);
}

TEST_CASE("Rotating face 4 times returns to original state", "[rubik]") {
  RubiksCube cube;
  RubiksCube original = cube;
  for (int i = 0; i < 4; i++) {
    cube.rotate(Face::FRONT, true);
  }
  REQUIRE(cube == original);
}

TEST_CASE("Scramble makes cube unsolved", "[rubik]") {
  RubiksCube cube;
  cube.scramble(10);
  REQUIRE(cube.isSolved() == false);
}

TEST_CASE("Copy constructor creates independent copy", "[rubik]") {
  RubiksCube cube1;
  RubiksCube cube2 = cube1;
  cube2.rotate(Face::UP, true);
  REQUIRE(cube1.isSolved() == true);
  REQUIRE(cube2.isSolved() == false);
}

TEST_CASE("Assignment operator works correctly", "[rubik]") {
  RubiksCube cube1;
  RubiksCube cube2;
  cube1.rotate(Face::DOWN, true);
  cube2 = cube1;
  REQUIRE(cube1 == cube2);
}

TEST_CASE("Equality operator checks all faces", "[rubik]") {
  RubiksCube cube1;
  RubiksCube cube2;
  REQUIRE(cube1 == cube2);
  cube2.rotate(Face::LEFT, true);
  REQUIRE(cube1 != cube2);
}

TEST_CASE("Rotate all faces clockwise and counter-clockwise", "[rubik]") {
  RubiksCube cube;
  for (int f = 0; f <= 5; f++) {
    cube.rotate(static_cast<Face>(f), true);
    cube.rotate(static_cast<Face>(f), false);
  }
  REQUIRE(cube.isSolved() == true);
}

TEST_CASE("Stream output contains all faces", "[rubik]") {
  RubiksCube cube;
  cube.rotate(Face::FRONT, true);
  std::stringstream ss;
  ss << cube;
  std::string output = ss.str();
  REQUIRE(output.find("UP:") != std::string::npos);
  REQUIRE(output.find("FRONT:") != std::string::npos);
  REQUIRE(output.find("DOWN:") != std::string::npos);
  REQUIRE(output.find("BACK:") != std::string::npos);
  REQUIRE(output.find("LEFT:") != std::string::npos);
  REQUIRE(output.find("RIGHT:") != std::string::npos);
}

TEST_CASE("Display method prints something", "[rubik]") {
  RubiksCube cube;
  std::stringstream buffer;
  std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
  cube.display();
  std::string output = buffer.str();
  std::cout.rdbuf(old);
  REQUIRE(output.find("UP") != std::string::npos);
}

TEST_CASE("Inequality operator works correctly", "[rubik]") {
  RubiksCube cube1;
  RubiksCube cube2;
  cube2.rotate(Face::UP, true);
  REQUIRE(cube1 != cube2);
}

TEST_CASE("Scramble with 0 moves does nothing", "[rubik]") {
  RubiksCube cube;
  cube.scramble(0);
  REQUIRE(cube.isSolved() == true);
}

TEST_CASE("Cover all rotation branches", "[rubik]") {
  RubiksCube cube;

  // Крутим каждую грань в обе стороны по 2 раза (чтобы задеть все if/else)
  for (int f = 0; f <= 5; f++) {
    cube.rotate(static_cast<Face>(f), true);
    cube.rotate(static_cast<Face>(f), true);
    cube.rotate(static_cast<Face>(f), false);
    cube.rotate(static_cast<Face>(f), false);
  }

  // Проверяем, что кубик всё ещё валиден (не сломался)
  REQUIRE(cube.isSolved() == true);
}

TEST_CASE("Cover stream operators and copy assignment", "[rubik]") {
  RubiksCube cube1;
  cube1.rotate(Face::FRONT, true);
  cube1.rotate(Face::RIGHT, false);

  // Тест оператора присваивания
  RubiksCube cube2;
  cube2 = cube1;
  REQUIRE(cube1 == cube2);

  // Тест оператора !=
  cube2.rotate(Face::UP, true);
  REQUIRE(cube1 != cube2);
}

TEST_CASE("Stream input reads a solved cube correctly", "[rubik]") {
  std::string input = "W W W W W W W W W " // UP
                      "G G G G G G G G G " // FRONT
                      "Y Y Y Y Y Y Y Y Y " // DOWN
                      "B B B B B B B B B " // BACK
                      "O O O O O O O O O " // LEFT
                      "R R R R R R R R R"; // RIGHT

  std::stringstream ss(input);
  RubiksCube cube;
  ss >> cube;
  REQUIRE(cube.isSolved() == true);
}