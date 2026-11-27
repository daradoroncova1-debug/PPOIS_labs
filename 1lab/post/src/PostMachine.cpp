#include "PostMachine.h"
#include <iostream>
#include <string>

PostMachine::PostMachine() {
  resetTape();
  program.clear();
  instructionPointer = 0;
  halted = false;
}

void PostMachine::resetTape() {
  tape.assign(1, false); // одна пустая ячейка
  headPos = 0;
  instructionPointer = 0;
  halted = false;
}

void PostMachine::markCurrent() { tape[headPos] = true; }

void PostMachine::unmarkCurrent() { tape[headPos] = false; }

bool PostMachine::currentCellMarked() const { return tape[headPos]; }

void PostMachine::expandTapeIfNeeded() {
  if (headPos < 0) {
    // Каретка ушла влево за границы — добавим ячейки в начало
    int expandBy = -headPos;
    tape.insert(tape.begin(), expandBy, false);
    headPos = 0; // теперь каретка в начале (логически та же ячейка)
  } else if (headPos >= static_cast<int>(tape.size())) {
    // Каретка ушла вправо за границы — расширим ленту
    tape.resize(headPos + 1, false);
  }
}

void PostMachine::loadProgram(const std::vector<Instruction> &prog) {
  program = prog;
  instructionPointer = 0;
  halted = false;
}

void PostMachine::clearProgram() {
  program.clear();
  instructionPointer = 0;
  halted = false;
}

bool PostMachine::executeStep() {
  if (halted) {
    return false;
  }
  if (instructionPointer < 0 ||
      instructionPointer >= static_cast<int>(program.size())) {
    // Вышли за границы программы — эквивалентно остановке
    halted = true;
    return false;
  }

  const Instruction &instr = program[instructionPointer];

  switch (instr.type) {
  case CommandType::MARK:
    tape[headPos] = true;
    instructionPointer++;
    break;

  case CommandType::UNMARK:
    tape[headPos] = false;
    instructionPointer++;
    break;

  case CommandType::LEFT:
    headPos--;
    expandTapeIfNeeded();
    instructionPointer++;
    break;

  case CommandType::RIGHT:
    headPos++;
    expandTapeIfNeeded();
    instructionPointer++;
    break;

  case CommandType::BRANCH:
    if (tape[headPos]) {
      instructionPointer = instr.jumpIfMarked;
    } else {
      instructionPointer = instr.jumpIfUnmarked;
    }
    break;

  case CommandType::HALT:
    halted = true;
    return false;
  }

  return true;
}

int PostMachine::run(int maxSteps) {
  int steps = 0;
  while (!halted && steps < maxSteps) {
    // Если IP за границами программы — останавливаемся без шага
    // (никакая инструкция не выполняется, поэтому steps не увеличиваем)
    if (instructionPointer < 0 ||
        instructionPointer >= static_cast<int>(program.size())) {
      halted = true;
      break;
    }
    // Выполняем шаг. HALT тоже считается за шаг — инструкция выполнилась.
    bool cont = executeStep();
    steps++;
    if (!cont) {
      break;
    }
  }
  if (steps >= maxSteps && !halted) {
    // Защита от зацикливания — считаем, что программа зависла
    halted = true;
  }
  return steps;
}

void PostMachine::resetExecution() {
  instructionPointer = 0;
  halted = false;
}

void PostMachine::printTape(int windowStart, int windowSize) const {
  // Вычисляем границы окна в абсолютных индексах ленты
  int startIdx = headPos + windowStart;
  int endIdx = headPos + windowStart + windowSize - 1;

  // Подгоняем под реальные размеры ленты
  if (startIdx < 0)
    startIdx = 0;
  if (endIdx >= static_cast<int>(tape.size()))
    endIdx = static_cast<int>(tape.size()) - 1;

  std::cout << "Лента: ";
  for (int i = startIdx; i <= endIdx; ++i) {
    std::cout << (tape[i] ? "[*]" : "[ ]");
  }
  std::cout << "\n";

  // Метка каретки
  std::cout << "       ";
  for (int i = startIdx; i <= endIdx; ++i) {
    if (i == headPos) {
      std::cout << " ^ ";
    } else {
      std::cout << "   ";
    }
  }
  std::cout << "  ← каретка\n";
}

std::ostream &operator<<(std::ostream &os, const PostMachine &pm) {
  os << "PostMachine: head=" << pm.headPos << ", tapeSize=" << pm.tape.size()
     << ", halted=" << (pm.halted ? "yes" : "no") << "\nЛента: ";
  for (size_t i = 0; i < pm.tape.size(); ++i) {
    os << (pm.tape[i] ? "1" : "0");
  }
  os << "\n";
  return os;
}

std::istream &operator>>(std::istream &is, PostMachine &pm) {
  std::string line;
  if (!std::getline(is, line)) {
    return is;
  }

  pm.tape.clear();
  for (char ch : line) {
    // '1', '*', 'V' или 'v' = метка; всё остальное = пусто
    pm.tape.push_back(ch == '1' || ch == '*' || ch == 'V' || ch == 'v');
  }
  if (pm.tape.empty()) {
    pm.tape.assign(1, false);
  }
  pm.headPos = static_cast<int>(pm.tape.size()) / 2; // каретка в центр
  pm.instructionPointer = 0;
  pm.halted = false;
  return is;
}