#include "RubikCube.h"
#include <iostream>
#include <string>

int main() {
  RubiksCube cube;
  int choice;

  while (true) {
    std::cout << "\n=== МЕНЮ КУБИКА РУБИКА ===\n";
    std::cout << "1. Перемешать (25 ходов)\n";
    std::cout << "2. Повернуть грань\n";
    std::cout << "3. Проверить, собран ли\n";
    std::cout << "4. Показать кубик\n";
    std::cout << "0. Выход\n";
    std::cout << "Выбор: ";
    std::cin >> choice;

    if (choice == 0)
      break;

    if (choice == 1) {
      cube.scramble(25);
      std::cout << "Кубик перемешан!\n";
    } else if (choice == 2) {
      int f, dir;
      std::cout << "Грань (0-U, 1-D, 2-F, 3-B, 4-L, 5-R): ";
      std::cin >> f;
      std::cout << "Направление (1-по часовой, 0-против): ";
      std::cin >> dir;
      cube.rotate(static_cast<Face>(f), dir == 1);
      std::cout << "Грань повернута.\n";
    } else if (choice == 3) {
      std::cout << (cube.isSolved() ? "Кубик СОБРАН!" : "Кубик НЕ собран.")
                << "\n";
    } else if (choice == 4) {
      cube.display();
    }
  }
  return 0;
}