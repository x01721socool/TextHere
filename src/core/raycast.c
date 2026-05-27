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
//todo:define ray2circ from raycast.h
void ray2circ(ray *r,Vector2 opos,Vector2 targ,int lim,int tsize){
  r->hitcirc=false;
  float px=targ.x-opos.x;
  float py=targ.y-opos.y;
  float dist=sqrtf(powf(px,2.0f)+powf(py,2.0f));
  float vx=px/dist;float vy=py/dist;
  r->lengthlim=(r->lengthlim==lim)?r->lengthlim:lim;
  //just for context,i got the formula of raycast algorithm
  //, vxpx+vypy+-sqrt(r^2+(vxpx+vypy)^2-px-py)
  //initially px and py was supposed to be p-n_0, basically
  //difference between target and original pos, but 
  //it just the same as above
  r->hitcirc=((float)tsize+powf(vx*px+vy*py,2.0f)
      -px-py>=0)?(vx*px+vy*py-sqrtf((float)tsize+
          powf(vx*px+vy*py,2.0f)-px-py)<=(float)r->lengthlim)?
            true:false:false;
  r->dx=vx;r->dy=vy;
}

  
