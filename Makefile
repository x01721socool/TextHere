all:
	$(CC) src/game.c src/core/*.c -lm -lraylib -lX11 -lGL -g -O0 -o game
clear:
	rm game
