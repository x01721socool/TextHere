#ifndef GAMEH
#define GAMEH
#include "core/grid.h"

typedef struct {
  char options; //bit of unorthodox to not use enum, but WILL add later
  gamemap level;
  int ts;
} game;
#endif
