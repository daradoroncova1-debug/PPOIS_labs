# PPOIS Labs

Лабораторные работы по курсу «Программирование на основе интерфейсов и структур» (ПОИС).
Каждая лаборатория — отдельный подпроект в папке `Nlab/`, со своими исходниками, тестами и документацией.

## Состав репозитория

| Лаба | Тема | Путь | Статус |
|---|---|---|---|
| №1 — Кубик Рубика | ООП: класс для 3×3 кубика Рубика | `1lab/rubik/` | готово |
| №1 — Машина Поста | Абстрактная вычислительная машина | `1lab/post/` | готово |


## Подпроект 1: Кубик Рубика

**Путь:** `1lab/rubik/`

**Описание:** класс `RubiksCube` для симуляции 3×3 кубика Рубика. Реализованы:
- Хранение состояния 6 граней как 3×3 матриц цветов
- Поворот любой грани по/против часовой стрелки
- Перемешивание случайной последовательностью поворотов
- Проверка, собран ли кубик
- Ввод/вывод состояния через потоки (`operator>>`, `operator<<`)
- Copy constructor, `operator=`, `operator==`, `operator!=`

**Запуск приложения:**
```bash
cd 1lab/rubik
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/rubik_app
```

Интерактивное меню: перемешать, повернуть грань, проверить собранность, показать состояние.

## Подпроект 2: Машина Поста

**Путь:** `1lab/post/`

**Описание:** симулятор абстрактной вычислительной машины Поста. Реализованы:
- Бесконечная лента (расширяется динамически через `std::vector<bool>`)
- 6 стандартных команд: `MARK`, `UNMARK`, `LEFT`, `RIGHT`, `BRANCH`, `HALT`
- Загрузка программы из вектора инструкций
- Пошаговое и непрерывное выполнение
- Защита от зацикливания (параметр `maxSteps`)
- Ввод/вывод состояния через потоки

**Запуск приложения:**
```bash
cd 1lab/post
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/post_app
```

Интерактивное меню с 4 встроенными демо-программами:
1. Поставить метку справа от каретки
2. Найти левую метку
3. Поставить 3 метки вправо
4. Стереть метки вправо до пустой ячейки

## Сборка обеих лаб сразу

Если хочешь собрать обе подпроекта одной командой:
```bash
cd 1lab
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Бинарники появятся в `1lab/build/rubik/` и `1lab/build/post/`.

## Запуск тестов

Тесты написаны на **Catch2 v3**. Каждый подпроект имеет свой тестовый бинарник.

### Кубик Рубика
```bash
cd 1lab/rubik
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
./build/rubik_tests
```

Ожидаемый вывод:
```
All tests passed (XX assertions in 16 test cases)
```

### Машина Поста
```bash
cd 1lab/post
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build/
./build/post_tests
```

Ожидаемый вывод:
```
All tests passed (XX assertions in 37 test cases)
```

### Опции Catch2

Запуск только определённых тестов:
```bash
./build/rubik_tests "[rubik]"              # все тесты с тегом [rubik]
./build/post_tests "[post][run]" -r "junit"  # тесты с двумя тегами, JUnit-отчёт
./build/rubik_tests --list-tests           # список всех тестов
```

Полный список опций: `./build/rubik_tests -?`

## Проверка покрытия кода

Сборка с `CMAKE_BUILD_TYPE=Debug` автоматически включает флаги `--coverage` и `-fprofile-abs-path` (см. `CMakeLists.txt` в каждом подпроекте). После запуска тестов в папке `build/` появляются `.gcda`/`.gcno` файлы.

**Команда для отчёта:**
```bash
cd 1lab/rubik   # или cd 1lab/post
gcovr -r . \
      --fail-under-line 90 \
      --exclude tests/ \
      --exclude '.*main\.cpp' \
      --object-directory build
```

CI требует покрытие **≥ 90%** по строкам. Команда завершится с ошибкой, если порог не достигнут.


## Генерация документации Doxygen

В каждом подпроекте есть `Doxyfile`, сконфигурированный так, чтобы вывод падал в `docs/1lab/<sub>/html/`.

```bash
cd 1lab/rubik   # или cd 1lab/post
doxygen Doxyfile
```

Документация появится в `docs/1lab/rubik/html/index.html` (или `docs/1lab/post/html/index.html`).

Открой в браузере:
```bash
xdg-open ../../docs/1lab/rubik/html/index.html
```

## CI/CD Pipeline

**Файл:** `.github/workflows/ci.yml`

**Когда запускается:**
- На push в ветки `main`, `dev`, `feature/**` (если изменены файлы в `1lab/**` или сам `ci.yml`)
- На pull request в `main` или `dev`

**Что делает** (для каждого подпроекта параллельно через matrix):
1. `Install dependencies` — ставит `cmake`, `g++`, `gcovr`, `cppcheck`, `catch2`, `doxygen`
2. `Configure` — `cmake -B build -DCMAKE_BUILD_TYPE=Debug`
3. `Build` — `cmake --build build`
4. `Run tests` — `./build/<sub>_tests`
5. `Coverage` — `gcovr --fail-under-line 90`
6. `Static analysis` — `cppcheck --enable=warning,style --error-exitcode=1`
7. `Doxygen` — генерирует документацию в `docs/1lab/<sub>/html/`
8. `Upload docs` — загружает HTML как GitHub Actions artifact
9. `Package Application` — собирает Release-версию и упаковывает в `.tar.gz`
10. `Upload Release Asset` — публикует как GitHub Release с тегом `latest-build`

Параллельная сборка означает, что rubik и post собираются независимо — если один упадёт, второй всё равно соберётся (`fail-fast: false`).

**Посмотреть прогоны:** вкладка **Actions** в репозитории на GitHub.

**Скачать артефакты:**
- Doxygen HTML — `1lab-rubik-docs` или `1lab-post-docs` в списке артефактов
- Release binaries — раздел **Releases**

