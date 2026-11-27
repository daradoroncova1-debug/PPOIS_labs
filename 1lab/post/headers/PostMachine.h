#pragma once
#include <iostream>
#include <vector>

/**
 * @brief Типы команд машины Поста.
 *
 * Машина Поста — абстрактная вычислительная машина с бесконечной лентой,
 * разделённой на ячейки. В каждой ячейке может быть метка или её не быть.
 * Каретка (головка) указывает на текущую ячейку.
 */
enum class CommandType {
  MARK,   // V — поставить метку в текущей ячейке
  UNMARK, // X — стереть метку в текущей ячейке
  LEFT,   // L — сдвинуть каретку на одну ячейку влево
  RIGHT,  // R — сдвинуть каретку на одну ячейку вправо
  BRANCH, // ? — ветвление: если ячейка пустая → команда n1, иначе → n2
  HALT    // ! — остановка машины
};

/**
 * @brief Одна инструкция программы машины Поста.
 *
 * Поля jumpIfUnmarked и jumpIfMarked используются только для команды BRANCH.
 * Для остальных команд они равны 0 и игнорируются.
 *
 * Нумерация команд начинается с 0 (как индексы в массиве).
 */
struct Instruction {
  CommandType type;
  int jumpIfUnmarked = 0; // для BRANCH: индекс команды при пустой ячейке
  int jumpIfMarked = 0;   // для BRANCH: индекс команды при помеченной ячейке

  // Статические "конструкторы" для удобства создания команд
  static Instruction mark() { return {CommandType::MARK, 0, 0}; }
  static Instruction unmark() { return {CommandType::UNMARK, 0, 0}; }
  static Instruction left() { return {CommandType::LEFT, 0, 0}; }
  static Instruction right() { return {CommandType::RIGHT, 0, 0}; }
  static Instruction branch(int ifUnmarked, int ifMarked) {
    return {CommandType::BRANCH, ifUnmarked, ifMarked};
  }
  static Instruction halt() { return {CommandType::HALT, 0, 0}; }
};

/**
 * @brief Класс машины Поста.
 *
 * Реализует абстрактную вычислительную машину с бесконечной лентой.
 * Лента представлена как std::vector<bool> и автоматически расширяется,
 * когда каретка выходит за текущие границы.
 *
 * Пример использования:
 * @code
 *   PostMachine pm;
 *   pm.loadProgram({
 *       Instruction::right(),     // 0: сдвиг вправо
 *       Instruction::mark(),      // 1: поставить метку
 *       Instruction::halt()       // 2: остановка
 *   });
 *   pm.run();
 *   pm.printTape();
 * @endcode
 */
class PostMachine {
private:
  std::vector<bool> tape;           ///< Лента: true = есть метка, false = пусто
  int headPos;                      ///< Индекс каретки в массиве tape
  std::vector<Instruction> program; ///< Программа (список инструкций)
  int instructionPointer;           ///< Индекс текущей команды в program
  bool halted;                      ///< Признак остановки

  /**
   * @brief Расширить ленту, если каретка вышла за границы.
   *
   * Если headPos < 0 — вставляет пустые ячейки в начало ленты.
   * Если headPos >= tape.size() — добавляет пустые ячейки в конец.
   */
  void expandTapeIfNeeded();

public:
  /**
   * @brief Конструктор по умолчанию: одна пустая ячейка, каретка в ней,
   * программы нет.
   */
  PostMachine();

  // === Управление лентой ===

  /**
   * @brief Очистить ленту (одна пустая ячейка, каретка в ней).
   */
  void resetTape();

  /**
   * @brief Поставить метку в текущей ячейке.
   */
  void markCurrent();

  /**
   * @brief Стереть метку в текущей ячейке.
   */
  void unmarkCurrent();

  /**
   * @brief Проверить, есть ли метка в текущей ячейке.
   * @return true если метка есть, false если ячейка пустая
   */
  bool currentCellMarked() const;

  /**
   * @brief Получить текущую позицию каретки (индекс в массиве tape).
   * @return Индекс каретки
   */
  int getHeadPosition() const { return headPos; }

  /**
   * @brief Получить размер ленты.
   * @return Количество ячеек
   */
  int getTapeSize() const { return static_cast<int>(tape.size()); }

  // === Управление программой ===

  /**
   * @brief Загрузить программу в машину.
   * @param prog Вектор инструкций
   */
  void loadProgram(const std::vector<Instruction> &prog);

  /**
   * @brief Очистить программу.
   */
  void clearProgram();

  /**
   * @brief Получить количество команд в программе.
   */
  int getProgramSize() const { return static_cast<int>(program.size()); }

  // === Выполнение ===

  /**
   * @brief Выполнить одну инструкцию.
   * @return true если выполнение продолжается, false если машина остановлена
   */
  bool executeStep();

  /**
   * @brief Выполнить всю программу до остановки или до maxSteps.
   * @param maxSteps Защита от зацикливания (по умолчанию 10000)
   * @return Количество выполненных шагов
   */
  int run(int maxSteps = 10000);

  /**
   * @brief Проверить, остановлена ли машина.
   */
  bool isHalted() const { return halted; }

  /**
   * @brief Сбросить состояние выполнения (IP=0, halted=false), не трогая ленту.
   *        Позволяет запустить ту же программу заново на текущей ленте.
   */
  void resetExecution();

  // === Ввод/вывод ===

  /**
   * @brief Распечатать участок ленты вокруг каретки.
   * @param windowStart Смещение начала окна относительно каретки
   * @param windowSize  Размер окна (количество ячеек)
   *
   * Каретка помечается символом '^' под ячейкой.
   */
  void printTape(int windowStart = -5, int windowSize = 15) const;

  /**
   * @brief Оператор вывода: печатает ленту и позицию каретки.
   */
  friend std::ostream &operator<<(std::ostream &os, const PostMachine &pm);

  /**
   * @brief Оператор ввода: читает строку символов, где '1' или '*' = метка,
   *        любой другой символ = пустая ячейка. Каретка устанавливается в
   * центр.
   */
  friend std::istream &operator>>(std::istream &is, PostMachine &pm);
};