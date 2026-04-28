CC = gcc
CFLAGS = -Wall -Wextra -Iincludes -g --coverage
LDFLAGS = -lsqlite3 --coverage

# CUnit paths (Homebrew on macOS, standard on Linux)
CUNIT_CFLAGS := $(shell [ -d /opt/homebrew/include ] && echo "-I/opt/homebrew/include" || echo "")
CUNIT_LDFLAGS := $(shell [ -d /opt/homebrew/lib ] && echo "-L/opt/homebrew/lib" || echo "") -lcunit

SRC = src/db.c src/auth.c src/broker.c src/goods.c src/deal.c src/queries.c src/functions.c
MAIN_SRC = src/main.c
OBJ = $(SRC:src/%.c=build/%.o)
MAIN_OBJ = build/main.o

TEST_NAMES = test_db test_auth test_broker test_goods test_deal test_queries test_functions
TEST_BIN = $(TEST_NAMES:%=build/%)

.PHONY: all clean check distcheck coverage

all: bin/perfume_bazaar

bin/perfume_bazaar: $(OBJ) $(MAIN_OBJ) | bin
	$(CC) $^ -o $@ $(LDFLAGS)

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build/test_%: tests/test_%.c $(OBJ) | build
	$(CC) $(CFLAGS) $(CUNIT_CFLAGS) $< $(OBJ) -o $@ $(LDFLAGS) $(CUNIT_LDFLAGS)

check: $(TEST_BIN)
	@for t in $(TEST_BIN); do \
		echo "=== Running $$t ==="; \
		./$$t || exit 1; \
	done
	@echo "All tests passed!"

distcheck: clean all check
	@echo "distcheck passed"

coverage: check
	gcov -o build $(SRC)
	@echo "Coverage files generated (*.gcov)"

clean:
	rm -rf build/*.o build/test_* build/*.gcno build/*.gcda build/*.gcov *.gcov bin/perfume_bazaar

bin:
	mkdir -p bin

build:
	mkdir -p build
