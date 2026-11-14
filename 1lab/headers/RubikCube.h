#pragma once
#include <iostream>
#include <vector>

enum class Color { WHITE, YELLOW, RED, ORANGE, GREEN, BLUE };
enum class Face { UP, DOWN, FRONT, BACK, LEFT, RIGHT };

/**
 * @brief A class representing a 3x3 Rubik's Cube.
 *
 * Stores the state of 6 faces as 3x3 matrices.
 * Implements rotations, scrambling, and state checking.
 */
class RubiksCube {
private:
  /**
   * @brief Matrices representing the six faces of the cube (3x3).
   */
  std::vector<std::vector<Color>> front_face;
  std::vector<std::vector<Color>> back_face;
  std::vector<std::vector<Color>> left_face;
  std::vector<std::vector<Color>> right_face;
  std::vector<std::vector<Color>> up_face;
  std::vector<std::vector<Color>> down_face;

  /**
   * @brief Sets the cube to the initial solved state.
   */
  void initDefaultState();
  /**
   * @brief Rotates a 3x3 matrix clockwise (used for face rotation).
   * @param matrix The matrix to rotate (passed by reference).
   */
  void spinMatrixClockwise(std::vector<std::vector<Color>> &matrix);
  /**
   * @brief Rotates a 3x3 matrix counter-clockwise.
   * @param matrix The matrix to rotate (passed by reference).
   */
  void spinMatrixCounterClockwise(std::vector<std::vector<Color>> &matrix);

  /**
   * @brief Turns the Front face and shifts adjacent strips.
   * @param isClockwise Direction of rotation.
   */
  void turnFrontFace(bool isClockwise);
  /**
   * @brief Turns the Back face and shifts adjacent strips.
   * @param isClockwise Direction of rotation.
   */
  void turnBackFace(bool isClockwise);
  /**
   * @brief Turns the Left face and shifts adjacent strips.
   * @param isClockwise Direction of rotation.
   */
  void turnLeftFace(bool isClockwise);
  /**
   * @brief Turns the Right face and shifts adjacent strips.
   * @param isClockwise Direction of rotation.
   */
  void turnRightFace(bool isClockwise);
  /**
   * @brief Turns the Up face and shifts adjacent strips.
   * @param isClockwise Direction of rotation.
   */
  void turnUpFace(bool isClockwise);
  /**
   * @brief Turns the Down face and shifts adjacent strips.
   * @param isClockwise Direction of rotation.
   */
  void turnDownFace(bool isClockwise);

public:
  /**
   * @brief Default constructor. Initializes a solved cube.
   */
  RubiksCube();
  /**
   * @brief Copy constructor. Creates a deep copy of another cube.
   * @param other The cube to copy from.
   */
  RubiksCube(const RubiksCube &other);
  /**
   * @brief Copy assignment operator.
   * @param other The cube to assign.
   * @return A reference to the current object.
   */
  RubiksCube &operator=(const RubiksCube &other);
  /**
   * @brief Default destructor.
   */
  ~RubiksCube() = default;

  /**
   * @brief Equality comparison operator.
   * @param other The cube to compare with.
   * @return true if both cubes have identical face configurations.
   */
  bool operator==(const RubiksCube &other) const;
  /**
   * @brief Inequality comparison operator.
   * @param other The cube to compare with.
   * @return true if cubes are not identical.
   */
  bool operator!=(const RubiksCube &other) const;

  /**
   * @brief Stream insertion operator (output).
   * @param os The output stream.
   * @param cube The cube to output.
   * @return The output stream.
   */
  friend std::ostream &operator<<(std::ostream &os, const RubiksCube &cube);
  /**
   * @brief Stream extraction operator (input).
   * @param is The input stream.
   * @param cube The cube to fill with data.
   * @return The input stream.
   */
  friend std::istream &operator>>(std::istream &is, RubiksCube &cube);

  /**
   * @brief Rotates a specific face of the cube.
   * @param face The face to rotate.
   * @param clockwise Direction of rotation (true = clockwise, false =
   * counter-clockwise).
   */
  void rotate(Face face, bool clockwise = true);
  /**
   * @brief Scrambles the cube with random moves.
   * @param moves Number of random rotations to perform.
   */
  void scramble(int moves = 25);
  /**
   * @brief Checks if the cube is solved.
   * @return true if all faces are uniform in color.
   */
  bool isSolved() const;
  /**
   * @brief Prints the current state of the cube to standard output.
   */
  void display() const;
};