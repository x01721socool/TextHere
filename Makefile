all:
	$(CC) src/game.c src/core/*.c -lm -lraylib -lX11 -Wformat=0 -lGL -o game
clear:
	rm game
debug:
	$(CC) src/game.c src/core/*.c -lm -lraylib -lX11 -lGL -O0 -Wextra -g -o game
