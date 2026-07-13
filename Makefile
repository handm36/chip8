CC = gcc
CFLAGS = -Wall -Wextra -g 
LDFLAGS = -lSDL3
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, build/%.o, $(SRC))
TARGET = chip8

all: $(TARGET)

debug: CFLAGS += -DDEBUG_MODE
debug: $(TARGET)	

build/%.o: src/%.c 
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf build $(TARGET)
