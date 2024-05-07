CC = gcc
CFLAGS = -g -std=c2x -Wall -Wextra -Werror

SRC_DIR = src
BIN_DIR = bin

$(BIN_DIR)/main: $(SRC_DIR)/main.c
	@mkdir -p bin
	@$(CC) $(CFLAGS) -o $(BIN_DIR)/main $(SRC_DIR)/main.c

.PHONY: push clean

push:
	@git add .
	@git commit -m "$(msg)"
	@git push origin master

clean:
	@rm -r bin