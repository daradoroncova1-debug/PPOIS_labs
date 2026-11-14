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