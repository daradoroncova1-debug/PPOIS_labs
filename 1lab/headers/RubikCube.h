#pragma once
#include <iostream>
#include <vector>

enum class Color { WHITE, YELLOW, RED, ORANGE, GREEN, BLUE };
enum class Face { UP, DOWN, FRONT, BACK, LEFT, RIGHT };

class RubiksCube {
private:
  // Матрицы граней
  std::vector<std::vector<Color>> front_face;
  std::vector<std::vector<Color>> back_face;
  std::vector<std::vector<Color>> left_face;
  std::vector<std::vector<Color>> right_face;
  std::vector<std::vector<Color>> up_face;
  std::vector<std::vector<Color>> down_face;

  void initDefaultState();
  void spinMatrixClockwise(std::vector<std::vector<Color>> &matrix);
  void spinMatrixCounterClockwise(std::vector<std::vector<Color>> &matrix);

  // Методы поворота граней (называем их turn...)
  void turnFrontFace(bool isClockwise);
  void turnBackFace(bool isClockwise);
  void turnLeftFace(bool isClockwise);
  void turnRightFace(bool isClockwise);
  void turnUpFace(bool isClockwise);
  void turnDownFace(bool isClockwise);

public:
  RubiksCube();
  RubiksCube(const RubiksCube &other);
  RubiksCube &operator=(const RubiksCube &other);
  ~RubiksCube() = default;

  bool operator==(const RubiksCube &other) const;
  bool operator!=(const RubiksCube &other) const;

  friend std::ostream &operator<<(std::ostream &os, const RubiksCube &cube);
  friend std::istream &operator>>(std::istream &is, RubiksCube &cube);

  void rotate(Face face, bool clockwise = true);
  void scramble(int moves = 25);
  bool isSolved() const;
  void display() const;
};