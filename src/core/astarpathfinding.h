#ifndef ASTARPARTHINGH
#define ASTARPARTHINGH

#include "raylib.h"
#include "grid.h"
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

int **intsoverrads(int radius,int pos[2]);

pathstruct findpath(npc *n,gamemap map,Vector2 target,int ts);

int countfirstp(int **p);
void remdupfirstp(int **p);
void remblockedp(int **p,gamemap map);
void remnegp(int **p);
void remnray2circp(int **p,Vector2 targ,int ts, int lim,gamemap map);
#endif
