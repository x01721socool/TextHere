#ifndef raycasth
#define raycasth
#include <stdbool.h>
#include <raylib.h>
#include "grid.h"
typedef struct {
  int length;
  int lengthlim;
  Vector2 intpos;
  bool hit;
  bool drawoption;
  float dx,dy;
} ray;

ray raywall(Vector2 pos, float rad,gamemap map,int ts,int lim);
void drawray(ray r);
#endif
