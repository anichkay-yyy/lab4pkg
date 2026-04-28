# Parfume Bazaar / Парфюмерный базар

## Description

Консольное приложение для управления парфюмерным базаром. Система позволяет вести учет маклеров, товаров и сделок между фирмами-поставщиками и фирмами-покупателями. Поддерживается аутентификация пользователей с разграничением прав доступа (администратор и маклер). Приложение работает с базой данных SQLite.

Реализовано в рамках лабораторной работы №4 по дисциплине "Технологии программирования для мобильных приложений".

## Repositories

- Документация и управление проектом: https://github.com/fpmi-tpmp2026/tpmp-gr13b-lab4-dreamteam
- Код проекта: https://github.com/anichkay-yyy/lab4pkg

## Installation

1. Клонируйте репозиторий:
```bash
git clone https://github.com/anichkay-yyy/lab4pkg.git
cd lab4pkg
```

2. Установите зависимости:
```bash
# Ubuntu/Debian
sudo apt-get install libsqlite3-dev libcunit1-dev

# macOS
brew install sqlite3 cunit
```

3. Соберите проект:
```bash
make
```

## Usage

Запустите приложение:
```bash
./bin/perfume_bazaar
```

### Учетные записи по умолчанию

| Логин    | Пароль     | Роль          |
|----------|------------|---------------|
| admin    | admin123   | Администратор |
| ivanov   | broker123  | Маклер        |
| petrov   | broker123  | Маклер        |
| sidorov  | broker123  | Маклер        |
| kozlov   | broker123  | Маклер        |

### Функционал

- **Администратор**: полный CRUD для маклеров, товаров и сделок; аналитические отчеты; специальные функции (статистика, консолидация)
- **Маклер**: просмотр и добавление своих сделок; отчеты; просмотр сделок на дату

## Testing

```bash
make check       # запуск всех тестов
make distcheck   # полная проверка (clean + build + test)
make coverage    # генерация отчета о покрытии кода
```

## Project Structure

```
├── src/        — исходный код приложения
├── includes/   — заголовочные файлы
├── tests/      — unit-тесты (CUnit)
├── sql/        — SQL-скрипты базы данных
├── bin/        — скомпилированные бинарные файлы
├── build/      — объектные файлы сборки
└── .github/    — конфигурация CI/CD (GitHub Actions)
```

## Contributing

Команда dreamteam, группа ГР13Б.

Участники проекта: Давидовская Мария Ивановна (`maryiad`), Семён Шиловский (`semen-shilovski`), Воронович Александр Александрович (`anichkay-yyy`), `Nitroxt5`, Aleksey Zubel (`Leshenka1`), `DariaKvasha`.
