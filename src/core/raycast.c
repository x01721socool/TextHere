#include "raycast.h"
#include <math.h>
#include "grid.h"
#include "raylib.h"
ray raywall(Vector2 pos, float rad,gamemap map,int ts,int lim) {
  ray r;
  r.length=0;
  r.lengthlim=lim;
  r.intpos=pos;
  float dx=sinf(rad);float dy=cosf(rad);
  for (int i=0;i<=(float)r.lengthlim;i+=1.0f){
    float cx=r.intpos.x+i*dx;
    float cy=r.intpos.y+i*dy;
    int mapx=(int)(cx/ts);
    int mapy=(int)(cy/ts);
    if (mapx>=0&&mapx<map.width&&mapy>=0&&mapy<map.height){
      if (map.walls[map.width*mapy+mapx]==1) {
       r.length=i;
       r.hit=true;
       r.dx=dx;r.dy=dy;
       return r;
      }
    } else { r.length=(int)i; r.dx=dx;r.dy=dy;return r;}
 
 }
  r.length=r.lengthlim;
  r.hit=false;
  r.dx=dx;r.dy=dy;
  return r;
}

void drawray(ray r){
  if (!r.drawoption) return;
  DrawLine((int)r.intpos.x,(int)r.intpos.y,
      (int)r.intpos.x+(int)(r.dx*(float)r.length),(int)r.intpos.y+(int)(r.dy*(float)r.length),GREEN);
}
