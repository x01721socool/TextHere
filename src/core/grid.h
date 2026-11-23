#ifndef GRIDH
#define GRIDH
#include "raylib.h"
typedef struct game game;
typedef struct {
  signed short width,height;
  unsigned char *render;
  signed char *walls;
  float ips;
  Vector2 sp;
  Texture2D tileset;
} gamemap;
void drawmap(game *g);
gamemap loadmap(const char *filename);
void unloadmap(gamemap *map);
#endif // GRIDH
