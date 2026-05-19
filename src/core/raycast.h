#ifndef raycasth
#define raycasth
#include <stdbool.h>
#include <raylib.h>
#include "grid.h"
typedef struct {
  int length;
  int lengthlim;
  Vector2 intpos;
  bool hitwall;
  bool hitcirc;
  bool drawoption;
  float dx,dy;
} ray;

void raywall(ray r,Vector2 pos, float rad,gamemap map,int ts,int lim);
void drawray(ray r);
void ray2circ(ray *r,Vector2 opos,Vector2 targ,int lim,int tsize);
#endif
