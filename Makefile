all:
	gcc src/game.c src/core/*.c -g -lraylib -lm -O0 -o game
clear:
	rm game
