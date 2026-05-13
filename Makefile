all:
	gcc src/game.c src/core/*.c -g -lraylib -lm -o game
clear:
	rm game
