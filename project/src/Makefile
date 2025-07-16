CC = gcc
CFLAGS = -I/opt/homebrew/include -L/opt/homebrew/lib -lraylib -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL
SNAKE_SRCS = main.c
INTRO_SRCS = intro.c
BUILD_DIR = build
SNAKE_TARGET = $(BUILD_DIR)/snake
INTRO_TARGET = $(BUILD_DIR)/intro

all: $(BUILD_DIR) $(SNAKE_TARGET) $(INTRO_TARGET)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(SNAKE_TARGET): $(SNAKE_SRCS) | $(BUILD_DIR)
	$(CC) -o $(SNAKE_TARGET) $(SNAKE_SRCS) $(CFLAGS)

$(INTRO_TARGET): $(INTRO_SRCS) | $(BUILD_DIR)
	$(CC) -o $(INTRO_TARGET) $(INTRO_SRCS) $(CFLAGS)

run-snake: $(SNAKE_TARGET)
	$(SNAKE_TARGET)

run-intro: $(INTRO_TARGET)
	$(INTRO_TARGET)

clean:
	rm -rf $(BUILD_DIR)

leaks-snake: $(SNAKE_TARGET)
	leaks --atExit -- $(SNAKE_TARGET)

leaks-intro: $(INTRO_TARGET)
	leaks --atExit -- $(INTRO_TARGET)
