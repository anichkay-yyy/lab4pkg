---
layout: default
title: Схема базы данных
---

# Схема базы данных

## Таблицы

| Таблица | Назначение |
|---------|-----------|
| PB_BROKERS | Маклеры (фамилия, адрес, год рождения) |
| PB_GOODS | Товары (название, вид, цена, поставщик, срок годности, кол-во) |
| PB_DEALS | Сделки (дата, товар, кол-во, маклер, покупатель) |
| PB_BROKER_STATS | Статистика маклеров (кол-во проданного, сумма) |
| PB_USERS | Пользователи (логин, хэш пароля, роль) |

## Связи

- PB_DEALS.broker_id -> PB_BROKERS.id
- PB_BROKER_STATS.broker_id -> PB_BROKERS.id
- PB_USERS.broker_id -> PB_BROKERS.id

## Ограничения целостности

- FOREIGN KEY на все связи
- CHECK: unit_price > 0, quantity_sold > 0, quantity_supplied >= 0
- CHECK: role IN ('admin', 'broker')
- NOT NULL на всех обязательных полях
- UNIQUE на username

[Назад на главную](index)
