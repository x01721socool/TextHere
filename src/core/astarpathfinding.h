#ifndef ASTARPARTHINGH
#define ASTARPARTHINGH

#include "raylib.h"
#include "grid.h"
#include "basicnpc.h"

typedef struct {
  int x,y,nodetype,parentx,parenty;
  bool isopen,isclosed;
  float g,h;
} node;
// isopen defines if a node can be further explored
// isclosed defines if a node has already been explored
// g is the cost required to get this cell
typedef struct {
  Vector2 *path;
  int count;
} pathstruct;

pathstruct findpath(npc *n,gamemap map, Vector2 target,int ts);

#endif
