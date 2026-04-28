#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "db.h"
#include "auth.h"
#include "broker.h"
#include "goods.h"
#include "deal.h"
#include "queries.h"
#include "functions.h"

static void clear_input(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void read_line(char *buf, int size) {
    if (fgets(buf, size, stdin)) {
        buf[strcspn(buf, "\n")] = '\0';
    }
}

static int read_int(void) {
    int val;
    if (scanf("%d", &val) != 1) {
        clear_input();
        return -1;
    }
    clear_input();
    return val;
}

static void menu_brokers(sqlite3 *db) {
    int choice;
    do {
        printf("\n=== Управление маклерами ===\n");
        printf("1. Список маклеров\n");
        printf("2. Добавить маклера\n");
        printf("3. Обновить маклера\n");
        printf("4. Удалить маклера\n");
        printf("0. Назад\n");
        printf("Выбор: ");
        choice = read_int();

        if (choice == 1) {
            broker_list(db);
        } else if (choice == 2) {
            char surname[64], address[128];
            int year;
            printf("Фамилия: "); read_line(surname, sizeof(surname));
            printf("Адрес: "); read_line(address, sizeof(address));
            printf("Год рождения: "); year = read_int();
            int id = broker_insert(db, surname, address, year);
            if (id > 0) printf("Маклер добавлен (ID: %d)\n", id);
        } else if (choice == 3) {
            char surname[64], address[128];
            int id, year;
            printf("ID маклера: "); id = read_int();
            printf("Новая фамилия: "); read_line(surname, sizeof(surname));
            printf("Новый адрес: "); read_line(address, sizeof(address));
            printf("Новый год рождения: "); year = read_int();
            if (broker_update(db, id, surname, address, year) == 0)
                printf("Маклер обновлен.\n");
        } else if (choice == 4) {
            int id;
            printf("ID маклера: "); id = read_int();
            if (broker_delete(db, id) == 0) printf("Маклер удален.\n");
        }
    } while (choice != 0);
}

static void menu_goods(sqlite3 *db) {
    int choice;
    do {
        printf("\n=== Управление товарами ===\n");
        printf("1. Список товаров\n");
        printf("2. Добавить товар\n");
        printf("3. Обновить товар\n");
        printf("4. Удалить товар\n");
        printf("0. Назад\n");
        printf("Выбор: ");
        choice = read_int();

        if (choice == 1) {
            goods_list(db);
        } else if (choice == 2) {
            char name[64], type[64], supplier[64], expiry[16];
            double price;
            int qty;
            printf("Название: "); read_line(name, sizeof(name));
            printf("Вид: "); read_line(type, sizeof(type));
            printf("Цена за единицу: "); scanf("%lf", &price); clear_input();
            printf("Фирма-поставщик: "); read_line(supplier, sizeof(supplier));
            printf("Срок годности (YYYY-MM-DD): "); read_line(expiry, sizeof(expiry));
            printf("Количество: "); qty = read_int();
            int id = goods_insert(db, name, type, price, supplier, expiry, qty);
            if (id > 0) printf("Товар добавлен (ID: %d)\n", id);
        } else if (choice == 3) {
            char name[64], type[64], supplier[64], expiry[16];
            double price;
            int id, qty;
            printf("ID товара: "); id = read_int();
            printf("Новое название: "); read_line(name, sizeof(name));
            printf("Новый вид: "); read_line(type, sizeof(type));
            printf("Новая цена: "); scanf("%lf", &price); clear_input();
            printf("Новый поставщик: "); read_line(supplier, sizeof(supplier));
            printf("Новый срок годности (YYYY-MM-DD): "); read_line(expiry, sizeof(expiry));
            printf("Новое количество: "); qty = read_int();
            if (goods_update(db, id, name, type, price, supplier, expiry, qty) == 0)
                printf("Товар обновлен.\n");
        } else if (choice == 4) {
            int id;
            printf("ID товара: "); id = read_int();
            if (goods_delete(db, id) == 0) printf("Товар удален.\n");
        }
    } while (choice != 0);
}

static void menu_deals(sqlite3 *db, int broker_id) {
    int choice;
    do {
        printf("\n=== Управление сделками ===\n");
        printf("1. Список сделок\n");
        printf("2. Добавить сделку\n");
        printf("3. Обновить сделку\n");
        printf("4. Удалить сделку\n");
        printf("0. Назад\n");
        printf("Выбор: ");
        choice = read_int();

        if (choice == 1) {
            deal_list(db, broker_id);
        } else if (choice == 2) {
            char date[16], product[64], type[64], buyer[64];
            int qty, bid;
            printf("Дата (YYYY-MM-DD): "); read_line(date, sizeof(date));
            printf("Название товара: "); read_line(product, sizeof(product));
            printf("Вид товара: "); read_line(type, sizeof(type));
            printf("Количество: "); qty = read_int();
            if (broker_id > 0) {
                bid = broker_id;
            } else {
                printf("ID маклера: "); bid = read_int();
            }
            printf("Фирма-покупатель: "); read_line(buyer, sizeof(buyer));
            int deal_id = deal_insert(db, date, product, type, qty, bid, buyer);
            if (deal_id > 0) {
                printf("Сделка добавлена (ID: %d)\n", deal_id);
                fn_update_broker_stats(db, deal_id);
            }
        } else if (choice == 3) {
            char date[16], product[64], type[64], buyer[64];
            int id, qty, bid;
            printf("ID сделки: "); id = read_int();
            printf("Новая дата (YYYY-MM-DD): "); read_line(date, sizeof(date));
            printf("Новое название товара: "); read_line(product, sizeof(product));
            printf("Новый вид товара: "); read_line(type, sizeof(type));
            printf("Новое количество: "); qty = read_int();
            printf("Новый ID маклера: "); bid = read_int();
            printf("Новая фирма-покупатель: "); read_line(buyer, sizeof(buyer));
            if (deal_update(db, id, date, product, type, qty, bid, buyer) == 0)
                printf("Сделка обновлена.\n");
        } else if (choice == 4) {
            int id;
            printf("ID сделки: "); id = read_int();
            if (deal_delete(db, id) == 0) printf("Сделка удалена.\n");
        }
    } while (choice != 0);
}

static void menu_reports(sqlite3 *db) {
    int choice;
    do {
        printf("\n=== Отчеты ===\n");
        printf("1. Продажи товара за период\n");
        printf("2. Покупатели товара\n");
        printf("3. Вид товара с наибольшим спросом\n");
        printf("4. Маклер с наибольшим кол-вом сделок\n");
        printf("5. Маклеры по фирмам-поставщикам\n");
        printf("0. Назад\n");
        printf("Выбор: ");
        choice = read_int();

        if (choice == 1) {
            char name[64], from[16], to[16];
            printf("Название товара: "); read_line(name, sizeof(name));
            printf("Дата начала (YYYY-MM-DD): "); read_line(from, sizeof(from));
            printf("Дата конца (YYYY-MM-DD): "); read_line(to, sizeof(to));
            query_product_sales_by_period(db, name, from, to);
        } else if (choice == 2) {
            char name[64];
            printf("Название товара: "); read_line(name, sizeof(name));
            query_product_buyers(db, name);
        } else if (choice == 3) {
            query_top_demand_type(db);
        } else if (choice == 4) {
            query_top_broker(db);
        } else if (choice == 5) {
            query_supplier_brokers(db);
        }
    } while (choice != 0);
}

static void menu_functions(sqlite3 *db, int broker_id) {
    int choice;
    do {
        printf("\n=== Специальные функции ===\n");
        printf("1. Обновить статистику маклера (по ID сделки)\n");
        printf("2. Консолидация товаров на дату\n");
        printf("3. Сделки маклера на дату\n");
        printf("0. Назад\n");
        printf("Выбор: ");
        choice = read_int();

        if (choice == 1) {
            int deal_id;
            printf("ID сделки: "); deal_id = read_int();
            fn_update_broker_stats(db, deal_id);
        } else if (choice == 2) {
            char date[16];
            printf("Дата периода (YYYY-MM-DD): "); read_line(date, sizeof(date));
            fn_consolidate_goods(db, date);
        } else if (choice == 3) {
            char date[16];
            int bid;
            printf("Дата (YYYY-MM-DD): "); read_line(date, sizeof(date));
            if (broker_id > 0) {
                bid = broker_id;
            } else {
                printf("ID маклера: "); bid = read_int();
            }
            fn_broker_deals_on_date(db, date, bid);
        }
    } while (choice != 0);
}

static void admin_menu(sqlite3 *db) {
    int choice;
    do {
        printf("\n========================================\n");
        printf("  ПАРФЮМЕРНЫЙ БАЗАР - Администратор\n");
        printf("========================================\n");
        printf("1. Управление маклерами\n");
        printf("2. Управление товарами\n");
        printf("3. Управление сделками\n");
        printf("4. Отчеты\n");
        printf("5. Специальные функции\n");
        printf("0. Выход\n");
        printf("Выбор: ");
        choice = read_int();

        if (choice == 1) menu_brokers(db);
        else if (choice == 2) menu_goods(db);
        else if (choice == 3) menu_deals(db, 0);
        else if (choice == 4) menu_reports(db);
        else if (choice == 5) menu_functions(db, 0);
    } while (choice != 0);
}

static void broker_menu(sqlite3 *db, int broker_id) {
    int choice;
    do {
        printf("\n========================================\n");
        printf("  ПАРФЮМЕРНЫЙ БАЗАР - Маклер\n");
        printf("========================================\n");
        printf("1. Мои сделки\n");
        printf("2. Добавить сделку\n");
        printf("3. Отчеты\n");
        printf("4. Мои сделки на дату\n");
        printf("0. Выход\n");
        printf("Выбор: ");
        choice = read_int();

        if (choice == 1) {
            deal_list(db, broker_id);
        } else if (choice == 2) {
            menu_deals(db, broker_id);
        } else if (choice == 3) {
            menu_reports(db);
        } else if (choice == 4) {
            char date[16];
            printf("Дата (YYYY-MM-DD): "); read_line(date, sizeof(date));
            fn_broker_deals_on_date(db, date, broker_id);
        }
    } while (choice != 0);
}

int main(int argc, char *argv[]) {
    const char *db_path = (argc > 1) ? argv[1] : "perfume_bazaar.db";
    sqlite3 *db;

    if (db_open(db_path, &db) != SQLITE_OK) {
        return 1;
    }

    if (db_init(db) != SQLITE_OK) {
        fprintf(stderr, "Ошибка инициализации базы данных.\n");
        db_close(db);
        return 1;
    }

    printf("========================================\n");
    printf("  ПАРФЮМЕРНЫЙ БАЗАР\n");
    printf("  Система управления\n");
    printf("========================================\n");

    char username[64], password[64];
    User user;

    printf("\nВход в систему\n");
    printf("Логин: ");
    read_line(username, sizeof(username));
    printf("Пароль: ");
    read_line(password, sizeof(password));

    if (auth_login(db, username, password, &user) != 0) {
        printf("Ошибка аутентификации. Неверный логин или пароль.\n");
        db_close(db);
        return 1;
    }

    printf("Добро пожаловать, %s! (роль: %s)\n", user.username, user.role);

    if (strcmp(user.role, "admin") == 0) {
        admin_menu(db);
    } else {
        broker_menu(db, user.broker_id);
    }

    printf("До свидания!\n");
    db_close(db);
    return 0;
}
