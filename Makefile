
CC=clang
CFLAGS=-Wall -Wextra -Wpedantic -g
# INCLUDE= -I include
# LIB=-L lib -l SDL2-2.0.0

SRC=src
SRCS=$(wildcard src/*.c)

OBJ=obj
OBJS=$(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))

TARGET=lisp

all: $(TARGET)

test: $(TARGET)
	clear
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $(LIB) $(OBJS) -o $@

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) -r $(OBJ)/* $(TARGET)
