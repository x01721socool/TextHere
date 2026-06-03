#include "raycast.h"
#include <math.h>
#include "grid.h"
#include "raylib.h"
void raywall(ray r,Vector2 pos, float rad,gamemap map,int ts,int lim) {
  r.length=0;
  r.hitwall=false;
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
       r.hitwall=true;
       r.dx=dx;r.dy=dy;
       break;
      }
    } else { r.length=(int)i; r.dx=dx;r.dy=dy;break;}
 
 }
  r.length=r.lengthlim;
  r.dx=dx;r.dy=dy;
}
void drawray(ray r){
  if (!r.drawoption) return;
  DrawLine((int)r.intpos.x,(int)r.intpos.y,
      (int)r.intpos.x+(int)(r.dx*(float)r.length),(int)r.intpos.y+(int)(r.dy*(float)r.length),GREEN);
}

// Ray-circle intersection using quadratic formula
// Tests if a ray from opos hits a circle at targ with radius tsize
// Formula: (point - center)·(point - center) = r²
// Where point = opos + t*direction
void ray2circ(ray *r, Vector2 opos, Vector2 targ, int lim, int tsize){
  r->hitcirc = false;
  
  // Vector from ray origin to circle center
  float px = targ.x - opos.x;
  float py = targ.y - opos.y;
  
  // Distance to circle center
  float dist = sqrtf(px * px + py * py);
  
  // Handle case where ray origin is at circle center
  if (dist == 0) {
    r->hitcirc = true;
    r->dx = 0;
    r->dy = 1;
    return;
  }
  
  // Normalized ray direction
  float vx = px / dist;
  float vy = py / dist;
  
  r->lengthlim = lim;
  
  // Ray-circle intersection using quadratic formula
  // Discriminant = (v·p)² - (p·p - r²)
  float projLen = vx * px + vy * py;       // Projection of p onto ray direction
  float p_dot_p = px * px + py * py;       // |p|² (squared distance to center)
  float r_squared = (float)tsize * tsize;  // r²
  float discriminant = projLen * projLen - (p_dot_p - r_squared);
  
  if (discriminant >= 0) {
    // Distance to first intersection point
    float t = projLen - sqrtf(discriminant);
    r->hitcirc = (t >= 0 && t <= r->lengthlim) ? true : false;
  }
  
  r->dx = vx;
  r->dy = vy;
}

  

