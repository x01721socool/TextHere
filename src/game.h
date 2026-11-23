#ifndef GAMEH
#define GAMEH
#include "core/grid.h"
#include "core/player.h"
#include "core/dialogue.h"
#include "raylib.h"

typedef struct game {
  enum {
    GMENU,GSETTINGS,GCREDS
  } options;
  gamemap level;
  int ts;
  Player plyr;
  Camera2D cam;
  DialogueMap dm;
  DialogueBlock *curactblock;
} game;
#endif
