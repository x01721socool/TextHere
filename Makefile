# Compiler and source files
CC = gcc
SRC = src/*.c
OBJ = $(SRC:.c=.o)
TARGET = game

# Detect Operating System
ifeq ($(OS),Windows_NT)
    # Windows settings
    EXE = .exe
    RM = rm -f
    MKDIR = mkdir -p
    # Raylib paths for Windows
    RAYLIB_PATH = C:/raylib
    CFLAGS = -Wall -Wextra -std=c99 -I$(RAYLIB_PATH)/include -I./src
    LDFLAGS = -L$(RAYLIB_PATH)/lib -lraylib -lopengl32 -lgdi32 -lwinmm -static
else
    # Linux/macOS settings
    EXE =
    RM = rm -f
    MKDIR = mkdir -p
    CFLAGS = -Wall -Wextra -std=c99 -I./src
    LDFLAGS = -lraylib -lm -lpthread -ldl -lrt -lX11
endif

TARGET_NAME = $(TARGET)$(EXE)
BUILD_DIR = build

# Targets
all: $(BUILD_DIR) $(BUILD_DIR)/$(TARGET_NAME)

$(BUILD_DIR):
	$(MKDIR) $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET_NAME): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJ)
	$(RM) $(BUILD_DIR)\$(TARGET_NAME)
ifeq ($(OS),Windows_NT)
	rmdir $(BUILD_DIR)
else
	rm -rf $(BUILD_DIR)
endif

run: all
ifeq ($(OS),Windows_NT)
	$(BUILD_DIR)\$(TARGET_NAME)
else
	./$(BUILD_DIR)/$(TARGET_NAME)
endif

.PHONY: all clean run
