#ifndef GAMEH
#define GAMEH
#include "core/grid.h"
#include "core/player.h"
#include "raylib.h"

typedef struct {
  enum {
    GMENU,GSETTINGS,GCREDS
  } options;
  gamemap level;
  int ts;
  Player plyr;
  Camera2D cam;
} game;
#endif
