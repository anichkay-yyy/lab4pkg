# Схема базы данных

## ER-модель

Базу данных рекомендуется визуализировать в [Vertabelo](http://www.vertabelo.com).

## Таблицы

### PB_BROKERS (Маклеры)
| Поле | Тип | Ограничения | Описание |
|------|-----|-------------|----------|
| id | INTEGER | PRIMARY KEY AUTOINCREMENT | Уникальный ID |
| surname | TEXT | NOT NULL | Фамилия маклера |
| address | TEXT | NOT NULL | Адрес маклера |
| birth_year | INTEGER | NOT NULL, CHECK(1900 < birth_year < 2010) | Год рождения |

### PB_GOODS (Товары)
| Поле | Тип | Ограничения | Описание |
|------|-----|-------------|----------|
| id | INTEGER | PRIMARY KEY AUTOINCREMENT | Уникальный ID |
| name | TEXT | NOT NULL | Название товара |
| type | TEXT | NOT NULL | Вид товара |
| unit_price | REAL | NOT NULL, CHECK(> 0) | Цена за единицу |
| supplier_firm | TEXT | NOT NULL | Фирма-поставщик |
| expiration_date | TEXT | NOT NULL | Срок годности (YYYY-MM-DD) |
| quantity_supplied | INTEGER | NOT NULL, CHECK(>= 0) | Кол-во поставленных единиц |

### PB_DEALS (Сделки)
| Поле | Тип | Ограничения | Описание |
|------|-----|-------------|----------|
| id | INTEGER | PRIMARY KEY AUTOINCREMENT | Уникальный ID |
| deal_date | TEXT | NOT NULL | Дата сделки (YYYY-MM-DD) |
| product_name | TEXT | NOT NULL | Название товара |
| product_type | TEXT | NOT NULL | Вид товара |
| quantity_sold | INTEGER | NOT NULL, CHECK(> 0) | Кол-во проданных единиц |
| broker_id | INTEGER | NOT NULL, FK -> PB_BROKERS(id) | ID маклера |
| buyer_firm | TEXT | NOT NULL | Фирма-покупатель |

### PB_BROKER_STATS (Статистика маклеров)
| Поле | Тип | Ограничения | Описание |
|------|-----|-------------|----------|
| broker_id | INTEGER | PRIMARY KEY, FK -> PB_BROKERS(id) | ID маклера |
| total_quantity_sold | INTEGER | NOT NULL, DEFAULT 0 | Общее кол-во проданных единиц |
| total_deal_sum | REAL | NOT NULL, DEFAULT 0.0 | Общая сумма сделок |

### PB_USERS (Пользователи)
| Поле | Тип | Ограничения | Описание |
|------|-----|-------------|----------|
| id | INTEGER | PRIMARY KEY AUTOINCREMENT | Уникальный ID |
| username | TEXT | NOT NULL, UNIQUE | Логин |
| password_hash | TEXT | NOT NULL | Хэш пароля (DJB2) |
| role | TEXT | NOT NULL, CHECK(IN ('admin','broker')) | Роль |
| broker_id | INTEGER | FK -> PB_BROKERS(id) | Связь с маклером (NULL для admin) |

## Связи

- `PB_DEALS.broker_id` -> `PB_BROKERS.id` (многие-к-одному)
- `PB_BROKER_STATS.broker_id` -> `PB_BROKERS.id` (один-к-одному)
- `PB_USERS.broker_id` -> `PB_BROKERS.id` (один-к-одному, nullable)

## SQL-скрипт

Полный скрипт: [sql/schema.sql](../sql/schema.sql)
