#ifndef GRIDH
#define GRIDH
#include "raylib.h"
typedef struct {
	int *data,width,height;
	float ips;
	Vector2 sp;
	const char *mapdir;
	Texture2D spritesheet;
} gamemap;
void drawmap(gamemap map, int ts,Camera2D camera);
gamemap loadmap(const char *filename);
void unloadmap(gamemap *map);
#endif GRIDH
