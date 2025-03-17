CC = gcc
CFLAGS = -Wall -Wextra -g -I../includes
LDFLAGS = -L../../lib -ldataforge -lcriterion 

SRC_DIR = src
UTIL_DIR = $(SRC_DIR)/utils
DS_DIR = $(SRC_DIR)/data_structures
BIN_DIR = bin

TEST_SRC = $(wildcard $(SRC_DIR)/*.c) $(wildcard $(UTIL_DIR)/*.c) $(wildcard $(DS_DIR)/*.c)
TEST_BIN = $(BIN_DIR)/test_runner

all: test

test: $(TEST_BIN)
	LD_LIBRARY_PATH=../lib:$$LD_LIBRARY_PATH ./$(TEST_BIN) --verbose

$(TEST_BIN): $(TEST_SRC)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(TEST_SRC) -o $(TEST_BIN) $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR)

