# Репозиторий проекта

## Первый репозиторий (документация и управление)

- **URL:** https://github.com/fpmi-tpmp2026/tpmp-gr13b-lab4-dreamteam
- **Назначение:** Wiki, GitHub Pages, GitHub Projects, документация
- **Содержит:** wiki-страницы, диаграммы, презентация

## Второй репозиторий (код)

- **URL:** https://github.com/anichkay-yyy/lab4pkg
- **Назначение:** исходный код, тесты, CI/CD
- **Содержит:** src/, includes/, tests/, sql/, Makefile, .github/workflows/

## Структура второго репозитория

```
├── .github/workflows/ci.yml  — CI/CD конфигурация
├── .gitignore
├── Makefile                   — система сборки
├── README.md                  — описание проекта
├── sql/schema.sql             — схема БД и тестовые данные
├── includes/                  — заголовочные файлы (.h)
├── src/                       — исходный код (.c)
├── tests/                     — unit-тесты (CUnit)
├── bin/                       — скомпилированные бинарники
└── build/                     — объектные файлы сборки
```
