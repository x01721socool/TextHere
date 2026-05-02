all:
	gcc src/game.c src/core/*.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -Os -march=native -Wall -o game

clear:
	rm game
