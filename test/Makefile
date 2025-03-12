CC = gcc
CFLAGS = -Wall -Wextra -g -I/usr/local/include
LDFLAGS = -lcriterion -L/usr/local/lib -ldataforge

SRC_DIR = src
BIN_DIR = bin

TEST_SRC = $(wildcard $(SRC_DIR)/*.c)
TEST_BIN = $(BIN_DIR)/test_runner

all: test

test: $(TEST_BIN)
	./$(TEST_BIN) --verbose

$(BIN_DIR)/test_runner: $(TEST_SRC)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_BIN) $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR)

