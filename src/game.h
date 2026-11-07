#ifndef GAMEH
#define GAMEH
#include "core/grid.h"

typedef struct {
 enum {
  GMENU,GSETTINGS,GCREDS
 } options;
 gamemap level;
 int ts;
} game;
#endif
