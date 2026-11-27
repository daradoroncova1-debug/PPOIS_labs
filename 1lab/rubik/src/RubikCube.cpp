#include "RubikCube.h"
#include <algorithm>
#include <iostream>
#include <random>

RubiksCube::RubiksCube() { initDefaultState(); }

RubiksCube::RubiksCube(const RubiksCube &other)
    : front_face(other.front_face), back_face(other.back_face),
      left_face(other.left_face), right_face(other.right_face),
      up_face(other.up_face), down_face(other.down_face) {}

RubiksCube &RubiksCube::operator=(const RubiksCube &other) {
  if (this != &other) {
    front_face = other.front_face;
    back_face = other.back_face;
    left_face = other.left_face;
    right_face = other.right_face;
    up_face = other.up_face;
    down_face = other.down_face;
  }
  return *this;
}

void RubiksCube::initDefaultState() {
  front_face.assign(3, std::vector<Color>(3, Color::GREEN));
  back_face.assign(3, std::vector<Color>(3, Color::BLUE));
  left_face.assign(3, std::vector<Color>(3, Color::ORANGE));
  right_face.assign(3, std::vector<Color>(3, Color::RED));
  up_face.assign(3, std::vector<Color>(3, Color::WHITE));
  down_face.assign(3, std::vector<Color>(3, Color::YELLOW));
}

bool RubiksCube::operator==(const RubiksCube &other) const {
  return front_face == other.front_face && back_face == other.back_face &&
         left_face == other.left_face && right_face == other.right_face &&
         up_face == other.up_face && down_face == other.down_face;
}

bool RubiksCube::operator!=(const RubiksCube &other) const {
  return !(*this == other);
}

void RubiksCube::spinMatrixClockwise(std::vector<std::vector<Color>> &matrix) {
  std::vector<std::vector<Color>> result(3, std::vector<Color>(3));
  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      result[col][2 - row] = matrix[row][col];
    }
  }
  matrix = result;
}

void RubiksCube::spinMatrixCounterClockwise(
    std::vector<std::vector<Color>> &matrix) {
  std::vector<std::vector<Color>> result(3, std::vector<Color>(3));
  for (int row = 0; row < 3; ++row) {
    for (int col = 0; col < 3; ++col) {
      result[2 - col][row] = matrix[row][col];
    }
  }
  matrix = result;
}

void RubiksCube::turnFrontFace(bool isClockwise) {
  if (isClockwise)
    spinMatrixClockwise(front_face);
  else
    spinMatrixCounterClockwise(front_face);

  std::vector<Color> topStrip = up_face[2];
  std::vector<Color> leftStrip = {left_face[0][2], left_face[1][2],
                                  left_face[2][2]};
  std::vector<Color> bottomStrip = down_face[0];
  std::vector<Color> rightStrip = {right_face[0][0], right_face[1][0],
                                   right_face[2][0]};

  if (isClockwise) {
    up_face[2] = {leftStrip[2], leftStrip[1], leftStrip[0]};
    for (int i = 0; i < 3; ++i)
      left_face[i][2] = bottomStrip[i];
    down_face[0] = {rightStrip[2], rightStrip[1], rightStrip[0]};
    for (int i = 0; i < 3; ++i)
      right_face[i][0] = topStrip[i];
  } else {
    up_face[2] = rightStrip;
    for (int i = 0; i < 3; ++i)
      right_face[i][0] = bottomStrip[2 - i];
    down_face[0] = leftStrip;
    for (int i = 0; i < 3; ++i)
      left_face[i][2] = topStrip[2 - i];
  }
}

void RubiksCube::turnRightFace(bool isClockwise) {
  if (isClockwise)
    spinMatrixClockwise(right_face);
  else
    spinMatrixCounterClockwise(right_face);

  std::vector<Color> upStrip = {up_face[0][2], up_face[1][2], up_face[2][2]};
  std::vector<Color> frontStrip = {front_face[0][2], front_face[1][2],
                                   front_face[2][2]};
  std::vector<Color> downStrip = {down_face[0][2], down_face[1][2],
                                  down_face[2][2]};
  std::vector<Color> backStrip = {back_face[2][0], back_face[1][0],
                                  back_face[0][0]};

  if (isClockwise) {
    for (int i = 0; i < 3; ++i)
      up_face[i][2] = frontStrip[i];
    for (int i = 0; i < 3; ++i)
      front_face[i][2] = downStrip[i];
    for (int i = 0; i < 3; ++i)
      down_face[i][2] = backStrip[i];
    back_face[2][0] = upStrip[0];
    back_face[1][0] = upStrip[1];
    back_face[0][0] = upStrip[2];
  } else {
    for (int i = 0; i < 3; ++i)
      up_face[i][2] = backStrip[2 - i];
    for (int i = 0; i < 3; ++i)
      back_face[i][0] = downStrip[2 - i];
    for (int i = 0; i < 3; ++i)
      down_face[i][2] = frontStrip[i];
    for (int i = 0; i < 3; ++i)
      front_face[i][2] = upStrip[i];
  }
}

void RubiksCube::turnLeftFace(bool isClockwise) {
  if (isClockwise)
    spinMatrixClockwise(left_face);
  else
    spinMatrixCounterClockwise(left_face);

  std::vector<Color> upStrip = {up_face[0][0], up_face[1][0], up_face[2][0]};
  std::vector<Color> frontStrip = {front_face[0][0], front_face[1][0],
                                   front_face[2][0]};
  std::vector<Color> downStrip = {down_face[0][0], down_face[1][0],
                                  down_face[2][0]};
  std::vector<Color> backStrip = {back_face[0][2], back_face[1][2],
                                  back_face[2][2]};

  if (isClockwise) {
    for (int i = 0; i < 3; ++i)
      up_face[i][0] = backStrip[i];
    for (int i = 0; i < 3; ++i)
      back_face[i][2] = downStrip[2 - i];
    for (int i = 0; i < 3; ++i)
      down_face[i][0] = frontStrip[i];
    for (int i = 0; i < 3; ++i)
      front_face[i][0] = upStrip[i];
  } else {
    for (int i = 0; i < 3; ++i)
      up_face[i][0] = frontStrip[i];
    for (int i = 0; i < 3; ++i)
      front_face[i][0] = downStrip[i];
    for (int i = 0; i < 3; ++i)
      down_face[i][0] = backStrip[2 - i];
    for (int i = 0; i < 3; ++i)
      back_face[i][2] = upStrip[i];
  }
}

void RubiksCube::turnUpFace(bool isClockwise) {
  if (isClockwise)
    spinMatrixClockwise(up_face);
  else
    spinMatrixCounterClockwise(up_face);

  std::vector<Color> frontStrip = front_face[0];
  std::vector<Color> rightStrip = right_face[0];
  std::vector<Color> backStrip = back_face[0];
  std::vector<Color> leftStrip = left_face[0];

  if (isClockwise) {
    front_face[0] = rightStrip;
    right_face[0] = backStrip;
    back_face[0] = leftStrip;
    left_face[0] = frontStrip;
  } else {
    front_face[0] = leftStrip;
    left_face[0] = backStrip;
    back_face[0] = rightStrip;
    right_face[0] = frontStrip;
  }
}

void RubiksCube::turnDownFace(bool isClockwise) {
  if (isClockwise)
    spinMatrixClockwise(down_face);
  else
    spinMatrixCounterClockwise(down_face);

  std::vector<Color> frontStrip = front_face[2];
  std::vector<Color> rightStrip = right_face[2];
  std::vector<Color> backStrip = back_face[2];
  std::vector<Color> leftStrip = left_face[2];

  if (isClockwise) {
    front_face[2] = leftStrip;
    left_face[2] = backStrip;
    back_face[2] = rightStrip;
    right_face[2] = frontStrip;
  } else {
    front_face[2] = rightStrip;
    right_face[2] = backStrip;
    back_face[2] = leftStrip;
    left_face[2] = frontStrip;
  }
}

void RubiksCube::turnBackFace(bool isClockwise) {
  if (isClockwise)
    spinMatrixClockwise(back_face);
  else
    spinMatrixCounterClockwise(back_face);

  std::vector<Color> upStrip = up_face[0];
  std::vector<Color> leftStrip = {left_face[0][0], left_face[1][0],
                                  left_face[2][0]};
  std::vector<Color> downStrip = down_face[2];
  std::vector<Color> rightStrip = {right_face[0][2], right_face[1][2],
                                   right_face[2][2]};

  if (isClockwise) {
    up_face[0] = {rightStrip[0], rightStrip[1], rightStrip[2]};
    right_face[0][2] = downStrip[2];
    right_face[1][2] = downStrip[1];
    right_face[2][2] = downStrip[0];
    down_face[2] = {leftStrip[2], leftStrip[1], leftStrip[0]};
    left_face[0][0] = upStrip[0];
    left_face[1][0] = upStrip[1];
    left_face[2][0] = upStrip[2];
  } else {
    up_face[0] = leftStrip;
    left_face[0][0] = downStrip[2];
    left_face[1][0] = downStrip[1];
    left_face[2][0] = downStrip[0];
    down_face[2] = {rightStrip[2], rightStrip[1], rightStrip[0]};
    right_face[0][2] = upStrip[0];
    right_face[1][2] = upStrip[1];
    right_face[2][2] = upStrip[2];
  }
}

void RubiksCube::rotate(Face face, bool clockwise) {
  switch (face) {
  case Face::FRONT:
    turnFrontFace(clockwise);
    break;
  case Face::BACK:
    turnBackFace(clockwise);
    break;
  case Face::LEFT:
    turnLeftFace(clockwise);
    break;
  case Face::RIGHT:
    turnRightFace(clockwise);
    break;
  case Face::UP:
    turnUpFace(clockwise);
    break;
  case Face::DOWN:
    turnDownFace(clockwise);
    break;
  }
}

void RubiksCube::scramble(int moves) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> faceDist(0, 5);
  std::uniform_int_distribution<> dirDist(0, 1);

  for (int i = 0; i < moves; i++) {
    Face f = static_cast<Face>(faceDist(gen));
    bool cw = dirDist(gen) == 1;
    rotate(f, cw);
  }
}

bool RubiksCube::isSolved() const {
  // Грань одноцветна, если каждая ячейка равна цвету левого верхнего угла.
  auto checkFace = [](const std::vector<std::vector<Color>> &f) {
    const Color first = f[0][0];
    return std::all_of(
        f.begin(), f.end(), [first](const std::vector<Color> &row) {
          return std::all_of(row.begin(), row.end(),
                             [first](Color c) { return c == first; });
        });
  };
  return checkFace(front_face) && checkFace(back_face) &&
         checkFace(left_face) && checkFace(right_face) && checkFace(up_face) &&
         checkFace(down_face);
}

char colorToChar(Color color) {
  switch (color) {
  case Color::WHITE:
    return 'W';
  case Color::YELLOW:
    return 'Y';
  case Color::RED:
    return 'R';
  case Color::ORANGE:
    return 'O';
  case Color::GREEN:
    return 'G';
  case Color::BLUE:
    return 'B';
  default:
    return '?';
  }
}

void RubiksCube::display() const {
  std::cout << "--- UP ---" << std::endl;
  for (const auto &row : up_face) {
    for (Color c : row)
      std::cout << colorToChar(c) << " ";
    std::cout << std::endl;
  }
  std::cout << "\n--- FRONT ---" << std::endl;
  for (const auto &row : front_face) {
    for (Color c : row)
      std::cout << colorToChar(c) << " ";
    std::cout << std::endl;
  }
  std::cout << "\n--- DOWN ---" << std::endl;
  for (const auto &row : down_face) {
    for (Color c : row)
      std::cout << colorToChar(c) << " ";
    std::cout << std::endl;
  }
}

std::ostream &operator<<(std::ostream &os, const RubiksCube &cube) {
  os << "UP:\n";
  for (const auto &row : cube.up_face) {
    for (Color c : row)
      os << colorToChar(c) << " ";
    os << "\n";
  }
  os << "FRONT:\n";
  for (const auto &row : cube.front_face) {
    for (Color c : row)
      os << colorToChar(c) << " ";
    os << "\n";
  }
  os << "DOWN:\n";
  for (const auto &row : cube.down_face) {
    for (Color c : row)
      os << colorToChar(c) << " ";
    os << "\n";
  }
  os << "BACK:\n";
  for (const auto &row : cube.back_face) {
    for (Color c : row)
      os << colorToChar(c) << " ";
    os << "\n";
  }
  os << "LEFT:\n";
  for (const auto &row : cube.left_face) {
    for (Color c : row)
      os << colorToChar(c) << " ";
    os << "\n";
  }
  os << "RIGHT:\n";
  for (const auto &row : cube.right_face) {
    for (Color c : row)
      os << colorToChar(c) << " ";
    os << "\n";
  }
  return os;
}

std::istream &operator>>(std::istream &is, RubiksCube &cube) {
  auto readColor = [&is]() {
    char ch;
    is >> ch;
    switch (ch) {
    case 'W':
      return Color::WHITE;
    case 'Y':
      return Color::YELLOW;
    case 'R':
      return Color::RED;
    case 'O':
      return Color::ORANGE;
    case 'G':
      return Color::GREEN;
    case 'B':
      return Color::BLUE;
    default:
      return Color::WHITE;
    }
  };
  // Заполняем каждую грань построчно с помощью std::generate.
  auto readFace = [&readColor](std::vector<std::vector<Color>> &face) {
    for (auto &row : face) {
      std::generate(row.begin(), row.end(), readColor);
    }
  };
  readFace(cube.up_face);
  readFace(cube.front_face);
  readFace(cube.down_face);
  readFace(cube.back_face);
  readFace(cube.left_face);
  readFace(cube.right_face);
  return is;
}
