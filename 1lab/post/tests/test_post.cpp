#include "PostMachine.h"
#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <string>

// ============================================================
// Вспомогательная функция: перехват вывода printTape()
// (тот же приём, что и в test_rubik.cpp для display())
// ============================================================

static std::string captureTape(const PostMachine &pm, int windowStart = -5,
                               int windowSize = 15) {
  std::stringstream buffer;
  std::streambuf *old = std::cout.rdbuf(buffer.rdbuf());
  pm.printTape(windowStart, windowSize);
  std::cout.rdbuf(old);
  return buffer.str();
}

// ============================================================
// Конструктор и базовое состояние
// ============================================================

TEST_CASE("PostMachine starts with one empty cell", "[post]") {
  PostMachine pm;
  REQUIRE(pm.getTapeSize() == 1);
  REQUIRE(pm.currentCellMarked() == false);
  REQUIRE(pm.isHalted() == false);
}

TEST_CASE("getHeadPosition returns 0 initially", "[post]") {
  PostMachine pm;
  REQUIRE(pm.getHeadPosition() == 0);
}

// ============================================================
// Управление лентой: mark / unmark / currentCellMarked
// ============================================================

TEST_CASE("markCurrent sets the current cell", "[post]") {
  PostMachine pm;
  pm.markCurrent();
  REQUIRE(pm.currentCellMarked() == true);
}

TEST_CASE("unmarkCurrent clears the current cell", "[post]") {
  PostMachine pm;
  pm.markCurrent();
  pm.unmarkCurrent();
  REQUIRE(pm.currentCellMarked() == false);
}

TEST_CASE("unmarkCurrent on empty cell does nothing", "[post]") {
  PostMachine pm;
  pm.unmarkCurrent();
  REQUIRE(pm.currentCellMarked() == false);
}

TEST_CASE("resetTape clears all marks and resets head", "[post]") {
  PostMachine pm;
  pm.markCurrent();
  pm.resetTape();
  REQUIRE(pm.currentCellMarked() == false);
  REQUIRE(pm.getHeadPosition() == 0);
  REQUIRE(pm.getTapeSize() == 1);
  REQUIRE(pm.isHalted() == false);
}

// ============================================================
// Управление программой
// ============================================================

TEST_CASE("loadProgram sets program size and resets state", "[post]") {
  PostMachine pm;
  pm.loadProgram({Instruction::mark(), Instruction::halt()});
  REQUIRE(pm.getProgramSize() == 2);
  REQUIRE(pm.isHalted() == false);
}

TEST_CASE("clearProgram removes the program", "[post]") {
  PostMachine pm;
  pm.loadProgram({Instruction::mark(), Instruction::halt()});
  pm.clearProgram();
  REQUIRE(pm.getProgramSize() == 0);
}

// ============================================================
// executeStep — каждая команда
// ============================================================

TEST_CASE("MARK command marks the current cell", "[post]") {
  PostMachine pm;
  pm.loadProgram({Instruction::mark(), Instruction::halt()});
  pm.executeStep(); // MARK
  REQUIRE(pm.currentCellMarked() == true);
}

TEST_CASE("UNMARK command clears the current cell", "[post]") {
  PostMachine pm;
  pm.loadProgram(
      {Instruction::mark(), Instruction::unmark(), Instruction::halt()});
  pm.executeStep(); // MARK
  pm.executeStep(); // UNMARK
  REQUIRE(pm.currentCellMarked() == false);
}

TEST_CASE("LEFT command moves head left and expands tape", "[post]") {
  PostMachine pm;
  pm.loadProgram({Instruction::left(), Instruction::halt()});
  pm.executeStep(); // LEFT
  // Изначально tape size = 1, head = 0
  // После LEFT: head = -1, расширяем tape → вставляем 1 ячейку в начало, head =
  // 0
  REQUIRE(pm.getHeadPosition() == 0);
  REQUIRE(pm.getTapeSize() == 2);
}

TEST_CASE("RIGHT command moves head right and expands tape", "[post]") {
  PostMachine pm;
  pm.loadProgram({Instruction::right(), Instruction::halt()});
  pm.executeStep(); // RIGHT
  // Изначально tape size = 1, head = 0
  // После RIGHT: head = 1, расширяем tape до size 2
  REQUIRE(pm.getHeadPosition() == 1);
  REQUIRE(pm.getTapeSize() == 2);
}

TEST_CASE("Multiple LEFT moves keep expanding tape left", "[post]") {
  PostMachine pm;
  pm.loadProgram({Instruction::left(), Instruction::left(), Instruction::left(),
                  Instruction::halt()});
  pm.executeStep();
  pm.executeStep();
  pm.executeStep();
  // Каждый LEFT вставляет по ячейке в начало
  REQUIRE(pm.getTapeSize() == 4);
}

TEST_CASE("Multiple RIGHT moves keep expanding tape right", "[post]") {
  PostMachine pm;
  pm.loadProgram({Instruction::right(), Instruction::right(),
                  Instruction::right(), Instruction::halt()});
  pm.executeStep();
  pm.executeStep();
  pm.executeStep();
  REQUIRE(pm.getTapeSize() == 4);
  REQUIRE(pm.getHeadPosition() == 3);
}

// ============================================================
// BRANCH — ветвление (обе ветки)
// ============================================================

TEST_CASE("BRANCH jumps to jumpIfMarked when cell is marked", "[post]") {
  PostMachine pm;
  pm.markCurrent();
  // Программа:
  //  0: ? 1, 2    — если пустая → 1, иначе → 2
  //  1: halt      — попали сюда, если ячейка пустая (плохо)
  //  2: right     — попали сюда, если ячейка помечена (хорошо)
  //  3: halt
  pm.loadProgram({
      Instruction::branch(1, 2), // 0
      Instruction::halt(),       // 1 — не должны сюда попасть
      Instruction::right(),      // 2
      Instruction::halt()        // 3
  });
  pm.executeStep(); // BRANCH → перейдёт к команде 2 (right)
  pm.executeStep(); // RIGHT
  REQUIRE(pm.getHeadPosition() == 1);
  REQUIRE(pm.isHalted() == false);
}

TEST_CASE("BRANCH jumps to jumpIfUnmarked when cell is empty", "[post]") {
  PostMachine pm;
  // Ячейка пустая по умолчанию
  //  0: ? 2, 1   — если пустая → 2, иначе → 1
  //  1: halt     — попали сюда, если помечена (не наш случай)
  //  2: right    — попали сюда, если пустая
  //  3: halt
  pm.loadProgram({
      Instruction::branch(2, 1), // 0
      Instruction::halt(),       // 1
      Instruction::right(),      // 2
      Instruction::halt()        // 3
  });
  pm.executeStep(); // BRANCH → перейдёт к команде 2 (right)
  pm.executeStep(); // RIGHT
  REQUIRE(pm.getHeadPosition() == 1);
}

// ============================================================
// HALT — остановка
// ============================================================

TEST_CASE("HALT stops the machine", "[post]") {
  PostMachine pm;
  pm.loadProgram({Instruction::halt()});
  bool cont = pm.executeStep();
  REQUIRE(cont == false);
  REQUIRE(pm.isHalted() == true);
}

TEST_CASE("executeStep on halted machine returns false", "[post]") {
  PostMachine pm;
  pm.loadProgram({Instruction::halt()});
  pm.executeStep();             // HALT
  bool cont = pm.executeStep(); // уже остановлена
  REQUIRE(cont == false);
}

TEST_CASE("executeStep with IP out of bounds halts", "[post]") {
  PostMachine pm;
  // Пустая программа — IP=0 сразу за границами
  pm.loadProgram({});
  bool cont = pm.executeStep();
  REQUIRE(cont == false);
  REQUIRE(pm.isHalted() == true);
}

TEST_CASE("Negative jump halts instead of reading past start", "[post]") {
  PostMachine pm;
  // BRANCH с отрицательными адресами: IP уходит в -1, executeStep не должен
  // обращаться к program[-1], а обязан остановить машину
  pm.loadProgram({Instruction::branch(-1, -1), Instruction::halt()});
  REQUIRE(pm.executeStep() == true);  // BRANCH выполнился, IP = -1
  REQUIRE(pm.executeStep() == false); // IP < 0 → остановка
  REQUIRE(pm.isHalted() == true);
}

// ============================================================
// run — выполнение всей программы
// ============================================================

TEST_CASE("run executes program until HALT", "[post]") {
  PostMachine pm;
  pm.loadProgram({
      Instruction::right(), // 0
      Instruction::mark(),  // 1
      Instruction::halt()   // 2
  });
  int steps = pm.run();
  REQUIRE(steps == 3);
  REQUIRE(pm.isHalted() == true);
  // Проверим, что метка поставлена в позиции 1
  REQUIRE(pm.getHeadPosition() == 1);
}

TEST_CASE("run stops at maxSteps to prevent infinite loop", "[post]") {
  PostMachine pm;
  // Бесконечный цикл: 0: R, 1: ? 0, 0 (всегда прыгаем на 0)
  pm.loadProgram({
      Instruction::right(),     // 0
      Instruction::branch(0, 0) // 1 — всегда к 0
  });
  int steps = pm.run(100);
  REQUIRE(steps == 100);
  REQUIRE(pm.isHalted() == true); // защита сработала
}

TEST_CASE("run on empty program returns 0", "[post]") {
  PostMachine pm;
  int steps = pm.run();
  REQUIRE(steps == 0);
  REQUIRE(pm.isHalted() == true);
}

TEST_CASE("run with maxSteps == 0 performs nothing", "[post][run]") {
  PostMachine pm;
  pm.loadProgram({Instruction::mark(), Instruction::halt()});
  int steps = pm.run(0);
  REQUIRE(steps == 0);
  REQUIRE(pm.isHalted() == true);
  REQUIRE(pm.currentCellMarked() == false); // ни одна команда не выполнилась
}

TEST_CASE("run that ends exactly on maxSteps is not treated as a hang",
          "[post][run]") {
  PostMachine pm;
  pm.loadProgram({Instruction::mark(), Instruction::halt()});
  // Программа ровно из 2 шагов: steps == maxSteps, но halted выставлен
  // самим HALT, поэтому счётчик не должен подменяться защитой от зацикливания
  int steps = pm.run(2);
  REQUIRE(steps == 2);
  REQUIRE(pm.isHalted() == true);
  REQUIRE(pm.currentCellMarked() == true);
}

// ============================================================
// resetExecution — сброс состояния
// ============================================================

TEST_CASE("resetExecution allows re-running the program", "[post]") {
  PostMachine pm;
  pm.loadProgram({Instruction::mark(), Instruction::halt()});
  pm.run();
  REQUIRE(pm.isHalted() == true);
  pm.resetExecution();
  REQUIRE(pm.isHalted() == false);
  REQUIRE(pm.getHeadPosition() == 0); // ленту не сбросили
}

// ============================================================
// printTape — вывод ленты
// ============================================================

TEST_CASE("printTape outputs something", "[post]") {
  PostMachine pm;
  pm.markCurrent();
  std::string output = captureTape(pm);
  REQUIRE(output.find("Лента") != std::string::npos);
  REQUIRE(output.find("каретка") != std::string::npos);
}

TEST_CASE("printTape shows marker symbol for marked cell", "[post]") {
  PostMachine pm;
  pm.markCurrent();
  std::string output = captureTape(pm);
  REQUIRE(output.find("[*]") != std::string::npos); // помеченная ячейка
}

TEST_CASE("printTape draws blanks for non-head cells", "[post][io]") {
  PostMachine pm;
  pm.loadProgram({Instruction::right(), Instruction::halt()});
  pm.run(); // tape = 2 ячейки, каретка на позиции 1
  std::string output = captureTape(pm);
  REQUIRE(output.find(" ^ ") != std::string::npos); // каретка под ячейкой
  REQUIRE(output.find("   ") != std::string::npos); // пустая ячейка без каретки
}

TEST_CASE("printTape clamps window to tape bounds", "[post][io]") {
  PostMachine pm;
  pm.markCurrent();
  // Окно заведомо больше ленты в обе стороны:
  // startIdx < 0 и endIdx >= tape.size() должны быть подрезаны
  std::string output = captureTape(pm, -100, 200);
  REQUIRE(output.find("Лента") != std::string::npos);
  REQUIRE(output.find("[*]") != std::string::npos);
  REQUIRE(output.find(" ^ ") != std::string::npos);
}

// ============================================================
// operator<< и operator>>
// ============================================================

TEST_CASE("operator<< outputs head position and tape", "[post]") {
  PostMachine pm;
  pm.markCurrent();
  pm.loadProgram({Instruction::right()});
  pm.run();
  std::stringstream ss;
  ss << pm;
  std::string output = ss.str();
  REQUIRE(output.find("PostMachine") != std::string::npos);
  REQUIRE(output.find("head=") != std::string::npos);
  REQUIRE(output.find("tapeSize=") != std::string::npos);
}

TEST_CASE("operator<< shows halted state", "[post]") {
  PostMachine pm;
  pm.loadProgram({Instruction::halt()});
  pm.run();
  std::stringstream ss;
  ss << pm;
  std::string output = ss.str();
  REQUIRE(output.find("halted=yes") != std::string::npos);
}

TEST_CASE("operator>> reads tape from string of digits", "[post]") {
  PostMachine pm;
  // Ввод "00100": метка ('1') в середине, по индексу 2
  std::stringstream input("00100");
  input >> pm;
  REQUIRE(pm.getTapeSize() == 5);
  // headPos = 5 / 2 = 2 — каретка в центре, там '1' → помечена
  REQUIRE(pm.currentCellMarked() == true);
}

TEST_CASE("operator>> accepts '*' as mark", "[post]") {
  PostMachine pm;
  // Ввод " ** ": 4 символа — пробел, *, *, пробел
  std::stringstream input(" ** ");
  input >> pm;
  REQUIRE(pm.getTapeSize() == 4);
  // headPos = 4 / 2 = 2 — там '*' → помечена
  REQUIRE(pm.currentCellMarked() == true);
}

TEST_CASE("operator>> accepts 'V' as mark", "[post]") {
  PostMachine pm;
  // Ввод "0V0": 3 символа, 'V' в середине
  std::stringstream input("0V0");
  input >> pm;
  REQUIRE(pm.getTapeSize() == 3);
  // headPos = 3 / 2 = 1 — там 'V' → помечена
  REQUIRE(pm.currentCellMarked() == true);
}

TEST_CASE("operator>> handles empty input gracefully", "[post]") {
  PostMachine pm;
  std::stringstream input("");
  input >> pm;
  // Защита: хотя бы одна ячейка (из конструктора)
  REQUIRE(pm.getTapeSize() == 1);
}

TEST_CASE("operator>> with empty line leaves one empty cell", "[post]") {
  PostMachine pm;
  // "\n" — getline читает пустую строку вместо EOF, поэтому срабатывает
  // проверка на пустую ленту внутри оператора
  std::stringstream input("\n");
  input >> pm;
  REQUIRE(pm.getTapeSize() == 1);
  REQUIRE(pm.currentCellMarked() == false);
  REQUIRE(pm.getHeadPosition() == 0);
  REQUIRE(pm.isHalted() == false);
}

TEST_CASE("operator>> handles all-unmarked input", "[post]") {
  PostMachine pm;
  std::stringstream input("00000"); // всё пусто
  input >> pm;
  REQUIRE(pm.getTapeSize() == 5);
  REQUIRE(pm.currentCellMarked() == false); // каретка в центре, там '0'
}

// ============================================================
// Комбинированные сценарии
// ============================================================

TEST_CASE("Mark three cells to the right", "[post]") {
  PostMachine pm;
  pm.loadProgram({
      Instruction::mark(),  // 0
      Instruction::right(), // 1
      Instruction::mark(),  // 2
      Instruction::right(), // 3
      Instruction::mark(),  // 4
      Instruction::halt()   // 5
  });
  pm.run();
  REQUIRE(pm.getHeadPosition() == 2);
  REQUIRE(pm.getTapeSize() == 3);
}

TEST_CASE("Find leftmost mark", "[post]") {
  PostMachine pm;
  // Поставим метку слева
  pm.loadProgram({Instruction::left(), Instruction::mark()});
  pm.run();
  // Сдвинем каретку вправо от метки
  pm.loadProgram({Instruction::right(), Instruction::right()});
  pm.run();
  // Теперь каретка на позиции 2, метка на позиции 0
  REQUIRE(pm.getHeadPosition() == 2);

  // Программа: идти влево, пока не встретим метку
  pm.loadProgram({
      Instruction::left(),       // 0
      Instruction::branch(0, 2), // 1: пустая → 0, помечена → 2
      Instruction::halt()        // 2
  });
  pm.run();
  REQUIRE(pm.getHeadPosition() == 0);
  REQUIRE(pm.currentCellMarked() == true);
}

TEST_CASE("Erase marks to the right until empty cell", "[post]") {
  PostMachine pm;
  // Поставим 3 метки вправо
  pm.loadProgram({Instruction::mark(), Instruction::right(),
                  Instruction::mark(), Instruction::right(),
                  Instruction::mark(), Instruction::halt()});
  pm.run();
  // Вернёмся в начало
  pm.loadProgram(
      {Instruction::left(), Instruction::left(), Instruction::halt()});
  pm.run();
  REQUIRE(pm.getHeadPosition() == 0);

  // Программа стирания:
  //  0: X       — стереть
  //  1: R       — вправо
  //  2: ? 3, 0  — если пустая → 3, иначе → 0 (продолжаем стирать)
  //  3: !
  pm.loadProgram({
      Instruction::unmark(),     // 0
      Instruction::right(),      // 1
      Instruction::branch(3, 0), // 2
      Instruction::halt()        // 3
  });
  pm.run();
  // Все 3 метки стёрты, каретка должна остановиться на первой пустой после них
  REQUIRE(pm.isHalted() == true);
}

TEST_CASE("Loading a new program resets halted state", "[post]") {
  PostMachine pm;
  pm.loadProgram({Instruction::halt()});
  pm.run();
  REQUIRE(pm.isHalted() == true);
  pm.loadProgram({Instruction::mark(), Instruction::halt()});
  REQUIRE(pm.isHalted() == false);
}

TEST_CASE("Coverage: ensure all command types are exercised", "[post]") {
  PostMachine pm;
  // Программа использует все 6 типов команд
  pm.loadProgram({
      Instruction::mark(),       // 0 — MARK
      Instruction::right(),      // 1 — RIGHT
      Instruction::branch(3, 3), // 2 — BRANCH (перейдёт к 3 в любом случае)
      Instruction::left(),       // 3 — LEFT
      Instruction::unmark(),     // 4 — UNMARK
      Instruction::halt()        // 5 — HALT
  });
  int steps = pm.run();
  REQUIRE(steps > 0);
  REQUIRE(pm.isHalted() == true);
}