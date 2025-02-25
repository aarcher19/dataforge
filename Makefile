CC = gcc
CFLAGS = -Wall -Wextra -Iincludes
LIB_DIR = lib
SRC_DIR = src
BUILD_DIR = build

# source files
ARRAY_SRC = $(SRC_DIR)/df_array.c
STRING_SRC = $(SRC_DIR)/string.c

# object files
ARRAY_OBJ = $(BUILD_DIR)/df_array.o
STRING_OBJ = $(BUILD_DIR)/string.o

# output libs
ARRAY_LIB = $(LIB_DIR)/libdf_array.a
STRING_LIB = $(LIB_DIR)/libstring.a

all: $(ARRAY_LIB) $(STRING_LIB)

# compile libint_array.a
$(ARRAY_LIB): $(ARRAY_OBJ) | $(LIB_DIR)
	ar rcs $@ $^

# compile libstring.a
$(STRING_LIB): $(STRING_OBJ) | $(LIB_DIR)
	ar rcs $@ $^

# compile source files to object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*.o $(LIB_DIR)/*.a
	

$(LIB_DIR):
	mkdir -p $(LIB_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
