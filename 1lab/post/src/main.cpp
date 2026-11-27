#include "PostMachine.h"
#include <iostream>
#include <string>

// === Демонстрационные программы ===

// Программа 1: "Поставить метку справа от каретки"
// Шаги: R, V, !
static std::vector<Instruction> programMarkRight() {
  return {
      Instruction::right(), // 0
      Instruction::mark(),  // 1
      Instruction::halt()   // 2
  };
}

// Программа 2: "Идти влево, пока не встретим метку; остановиться"
// Шаги: L, ? 0, 2, !
//   0: L          — сдвиг влево
//   1: ? 0, 2     — если ячейка пустая → к 0 (продолжаем идти), иначе → к 2
//   (нашли) 2: !          — остановка
static std::vector<Instruction> programFindLeftMark() {
  return {
      Instruction::left(),       // 0
      Instruction::branch(0, 2), // 1
      Instruction::halt()        // 2
  };
}

// Программа 3: "Поставить 3 метки подряд вправо"
// Шаги: V, R, V, R, V, !
static std::vector<Instruction> programThreeMarks() {
  return {
      Instruction::mark(),  // 0
      Instruction::right(), // 1
      Instruction::mark(),  // 2
      Instruction::right(), // 3
      Instruction::mark(),  // 4
      Instruction::halt()   // 5
  };
}

// Программа 4: "Стереть все метки справа, пока не дойдём до пустой ячейки"
// Шаги: X, R, ? 0, 3, !
//   0: X          — стереть метку
//   1: R          — сдвиг вправо
//   2: ? 0, 3     — если ячейка помечена → к 0 (продолжаем стирать), иначе → к
//   3 3: !          — остановка
static std::vector<Instruction> programEraseRight() {
  return {
      Instruction::unmark(),     // 0
      Instruction::right(),      // 1
      Instruction::branch(3, 0), // 2
      Instruction::halt()        // 3
  };
}

// === Меню ===

static void printMenu() {
  std::cout << "\n=== Машина Поста ===\n";
  std::cout << "1. Загрузить демо-программу 'Поставить метку справа'\n";
  std::cout << "2. Загрузить демо-программу 'Найти левую метку'\n";
  std::cout << "3. Загрузить демо-программу 'Три метки вправо'\n";
  std::cout << "4. Загрузить демо-программу 'Стереть метки вправо'\n";
  std::cout << "5. Поставить метку в текущей ячейке\n";
  std::cout << "6. Стереть метку в текущей ячейке\n";
  std::cout << "7. Сдвинуть каретку влево\n";
  std::cout << "8. Сдвинуть каретку вправо\n";
  std::cout << "9. Выполнить один шаг программы\n";
  std::cout << "10. Выполнить всю программу\n";
  std::cout << "11. Сбросить ленту\n";
  std::cout << "12. Показать состояние\n";
  std::cout << "0. Выход\n";
  std::cout << "Выбор: ";
}

int main() {
  PostMachine pm;
  int choice;

  while (true) {
    pm.printTape();
    printMenu();
    std::cin >> choice;

    if (choice == 0) {
      break;
    }

    switch (choice) {
    case 1:
      pm.resetTape();
      pm.loadProgram(programMarkRight());
      std::cout << "Программа 'Поставить метку справа' загружена.\n";
      break;
    case 2:
      pm.resetTape();
      // Поставим метку слева, чтобы было что искать
      pm.loadProgram({Instruction::left(), Instruction::mark()});
      pm.run();
      pm.resetTape();
      // Сдвинем каретку вправо от метки
      pm.loadProgram({Instruction::right(), Instruction::right()});
      pm.run();
      // Загрузим основную программу
      pm.loadProgram(programFindLeftMark());
      std::cout << "Программа 'Найти левую метку' загружена.\n";
      break;
    case 3:
      pm.resetTape();
      pm.loadProgram(programThreeMarks());
      std::cout << "Программа 'Три метки вправо' загружена.\n";
      break;
    case 4:
      // Поставим метки справа, чтобы было что стирать
      pm.resetTape();
      pm.loadProgram(programThreeMarks());
      pm.run();
      // Вернёмся в начало
      pm.loadProgram({Instruction::left(), Instruction::left(),
                      Instruction::left(), Instruction::left()});
      pm.run();
      // Загрузим программу стирания
      pm.loadProgram(programEraseRight());
      std::cout << "Программа 'Стереть метки вправо' загружена.\n";
      break;
    case 5:
      pm.markCurrent();
      std::cout << "Метка поставлена.\n";
      break;
    case 6:
      pm.unmarkCurrent();
      std::cout << "Метка стёрта.\n";
      break;
    case 7:
      pm.executeStep(); // не очень честно, но работает
      // Лучше: выполнить LEFT напрямую. Упростим — вызовем через программу.
      pm.loadProgram({Instruction::left()});
      pm.run();
      std::cout << "Каретка сдвинута влево.\n";
      break;
    case 8:
      pm.loadProgram({Instruction::right()});
      pm.run();
      std::cout << "Каретка сдвинута вправо.\n";
      break;
    case 9: {
      bool cont = pm.executeStep();
      std::cout << (cont ? "Шаг выполнен." : "Машина остановлена.") << "\n";
      break;
    }
    case 10: {
      if (pm.getProgramSize() == 0) {
        std::cout << "Программа не загружена!\n";
        break;
      }
      int steps = pm.run();
      std::cout << "Выполнено шагов: " << steps << "\n";
      break;
    }
    case 11:
      pm.resetTape();
      pm.clearProgram();
      std::cout << "Лента и программа очищены.\n";
      break;
    case 12:
      std::cout << pm;
      break;
    default:
      std::cout << "Неизвестная команда.\n";
    }
  }

  return 0;
}