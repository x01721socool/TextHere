#ifndef GRIDH
#define GRIDH
#include "raylib.h"
typedef struct {
	signed short width,height;
	unsigned char *render;
	signed char *walls;
	float ips;
	Vector2 sp;
	Texture2D tileset;
} gamemap;
void drawmap(gamemap map, int ts,Camera2D camera);
gamemap loadmap(const char *filename);
void unloadmap(gamemap *map);
#endif // GRIDH
