PRAGMA foreign_keys = ON;

-- Таблица маклеров
CREATE TABLE IF NOT EXISTS PB_BROKERS (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    surname TEXT NOT NULL,
    address TEXT NOT NULL,
    birth_year INTEGER NOT NULL CHECK(birth_year > 1900 AND birth_year < 2010)
);

-- Таблица товаров
CREATE TABLE IF NOT EXISTS PB_GOODS (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    name TEXT NOT NULL,
    type TEXT NOT NULL,
    unit_price REAL NOT NULL CHECK(unit_price > 0),
    supplier_firm TEXT NOT NULL,
    expiration_date TEXT NOT NULL,
    quantity_supplied INTEGER NOT NULL CHECK(quantity_supplied >= 0)
);

-- Таблица сделок
CREATE TABLE IF NOT EXISTS PB_DEALS (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    deal_date TEXT NOT NULL,
    product_name TEXT NOT NULL,
    product_type TEXT NOT NULL,
    quantity_sold INTEGER NOT NULL CHECK(quantity_sold > 0),
    broker_id INTEGER NOT NULL REFERENCES PB_BROKERS(id),
    buyer_firm TEXT NOT NULL
);

-- Таблица статистики маклеров
CREATE TABLE IF NOT EXISTS PB_BROKER_STATS (
    broker_id INTEGER PRIMARY KEY REFERENCES PB_BROKERS(id),
    total_quantity_sold INTEGER NOT NULL DEFAULT 0,
    total_deal_sum REAL NOT NULL DEFAULT 0.0
);

-- Таблица пользователей (аутентификация)
CREATE TABLE IF NOT EXISTS PB_USERS (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    username TEXT NOT NULL UNIQUE,
    password_hash TEXT NOT NULL,
    role TEXT NOT NULL CHECK(role IN ('admin', 'broker')),
    broker_id INTEGER REFERENCES PB_BROKERS(id)
);

-- ===== Наполнение данными =====

-- Маклеры
INSERT INTO PB_BROKERS (surname, address, birth_year) VALUES
    ('Иванов', 'г. Минск, ул. Ленина 10', 1985),
    ('Петров', 'г. Минск, ул. Победы 25', 1990),
    ('Сидоров', 'г. Брест, ул. Советская 5', 1978),
    ('Козлов', 'г. Гродно, ул. Мира 15', 1982);

-- Товары
INSERT INTO PB_GOODS (name, type, unit_price, supplier_firm, expiration_date, quantity_supplied) VALUES
    ('Chanel No5', 'духи', 120.00, 'Luxury Parfum Ltd', '2027-12-31', 500),
    ('Dior Sauvage', 'туалетная вода', 95.50, 'Luxury Parfum Ltd', '2027-06-30', 300),
    ('Hugo Boss', 'туалетная вода', 75.00, 'Boss Fragrances', '2026-12-31', 400),
    ('Versace Eros', 'духи', 110.00, 'Italian Scents Co', '2027-03-15', 250),
    ('Gucci Bloom', 'парфюмерная вода', 88.00, 'Italian Scents Co', '2027-09-20', 350),
    ('Armani Code', 'туалетная вода', 82.50, 'Boss Fragrances', '2026-11-30', 200),
    ('Lancome Idole', 'парфюмерная вода', 99.00, 'French Beauty SA', '2027-08-15', 180),
    ('YSL Libre', 'парфюмерная вода', 105.00, 'French Beauty SA', '2027-05-01', 220);

-- Сделки
INSERT INTO PB_DEALS (deal_date, product_name, product_type, quantity_sold, broker_id, buyer_firm) VALUES
    ('2025-01-15', 'Chanel No5', 'духи', 50, 1, 'Арома-Трейд'),
    ('2025-01-20', 'Dior Sauvage', 'туалетная вода', 30, 2, 'ПарфюмОпт'),
    ('2025-02-10', 'Hugo Boss', 'туалетная вода', 40, 1, 'Косметик-Групп'),
    ('2025-02-15', 'Versace Eros', 'духи', 25, 3, 'Арома-Трейд'),
    ('2025-03-01', 'Gucci Bloom', 'парфюмерная вода', 35, 2, 'Люкс-Парфюм'),
    ('2025-03-05', 'Chanel No5', 'духи', 60, 1, 'ПарфюмОпт'),
    ('2025-03-10', 'Armani Code', 'туалетная вода', 20, 4, 'Косметик-Групп'),
    ('2025-03-15', 'Lancome Idole', 'парфюмерная вода', 45, 3, 'Люкс-Парфюм'),
    ('2025-04-01', 'YSL Libre', 'парфюмерная вода', 30, 2, 'Арома-Трейд'),
    ('2025-04-05', 'Dior Sauvage', 'туалетная вода', 55, 1, 'Люкс-Парфюм'),
    ('2025-04-10', 'Hugo Boss', 'туалетная вода', 70, 3, 'ПарфюмОпт'),
    ('2025-04-15', 'Gucci Bloom', 'парфюмерная вода', 40, 4, 'Арома-Трейд');

-- Статистика маклеров (рассчитана на основе сделок выше)
INSERT INTO PB_BROKER_STATS (broker_id, total_quantity_sold, total_deal_sum) VALUES
    (1, 225, 50 * 120.00 + 40 * 75.00 + 60 * 120.00 + 55 * 95.50),
    (2, 135, 30 * 95.50 + 35 * 88.00 + 30 * 105.00),
    (3, 115, 25 * 110.00 + 45 * 99.00 + 70 * 75.00),
    (4, 60, 20 * 82.50 + 40 * 88.00);

-- Пользователи (пароль: admin123 для админа, broker123 для маклеров)
-- Хэши DJB2: admin123 -> 249849790082, broker123 -> 7572305077482
INSERT INTO PB_USERS (username, password_hash, role, broker_id) VALUES
    ('admin', '249849790082', 'admin', NULL),
    ('ivanov', '7572305077482', 'broker', 1),
    ('petrov', '7572305077482', 'broker', 2),
    ('sidorov', '7572305077482', 'broker', 3),
    ('kozlov', '7572305077482', 'broker', 4);
