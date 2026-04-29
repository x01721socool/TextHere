all:
	gcc src/game.c src/core/*.c -lraylib -Os -o game

clear:
	rm game
