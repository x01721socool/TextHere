all:
	$(CC) src/game.c src/core/*.c -lm -lraylib -lX11 -lGL -o game
clear:
	rm game
