all:
	gcc src/game.c src/core/*.c -g -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -march=native -Wall -o game
clear:
	rm game
