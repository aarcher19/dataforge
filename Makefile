CC = gcc
CFLAGS = -Wall -Wextra -fPIC 
LDFLAGS = -shared 

INCLUDES_DIR = includes
SRC_DIR = src
BUILD_DIR = build
LIB_DIR = lib

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

LIB_NAME = $(LIB_DIR)/libdataforge.so

all: $(LIB_NAME)

$(LIB_NAME): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $(OBJ_FILES) -o $(LIB_NAME)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDES_DIR) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)/*.o $(LIB_DIR)/*.a

INSTALL_INCLUDE_DIR = /usr/local/include/dataforge  
INSTALL_LIB_DIR = /usr/local/lib

install:
	mkdir -p $(INSTALL_INCLUDE_DIR)                  
	mkdir -p $(INSTALL_LIB_DIR)                      
	cp $(LIB_NAME) $(INSTALL_LIB_DIR)/               
	cp -r $(INCLUDES_DIR)/* $(INSTALL_INCLUDE_DIR)
	sudo ldconfig

uninstall:
	rm -f $(INSTALL_LIB_DIR)/libdataforge.so
	rm -rf $(INSTALL_INCLUDE_DIR)
