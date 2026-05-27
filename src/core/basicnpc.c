#include "basicnpc.h"
#include "raymath.h"
#include "raylib.h"
#include "player.h"
#include "math.h"
#include <stddef.h>
#include "astarpathfinding.h"
#include "raycast.h"
#include <stdlib.h>
void Drawnpc(const npc *n) {
	if (!n->active) return;
	DrawCircle(n->pos.x,n->pos.y,16,RED);
}

void npcgoto(npc *n, Vector2 target){
  n->startpos=n->pos;
  n->endpos=target;
  n->ismoving=true;
}

void Updatenpc (npc *n) {
  if (!n->active) return;
  if (n->ismoving&&Vector2Distance(n->pos,n->endpos)<=8.0f) {
    if (n->path!=NULL&&n->curpoint+1<n->pathcount) {
      n->curpoint++;
      npcgoto(n,n->path[n->curpoint]);
    } else n->ismoving=false;
  }
  if (n->ismoving) {
    float dt=GetFrameTime();
    float range=dt*n->speed;
    float hyp=sqrtf(powf(n->endpos.x-n->startpos.x,2.0f)+powf(n->endpos.y-n->startpos.y,2.0f));
    n->pos.x+=(n->endpos.x-n->startpos.x)/hyp*range;
    n->pos.y+=(n->endpos.y-n->startpos.y)/hyp*range;
  }
}

void npcsetpath(npc *n,gamemap map, Vector2 target,int ts) {
  if (n->path!=NULL){
    free(n->path);
    n->path=NULL;
  }
  pathstruct b=findpath(n,map,target,ts);
  n->path=b.path;
  n->pathcount=b.count;
  n->curpoint=0;
  if (n->pathcount > 0) {
      npcgoto(n, n->path[0]);
  }
}

void findpathinradius(npc *n,Vector2 targetpos,gamemap map, int ts,int rad) {
  int npos[2]={(int)(n->pos.x/ts),(int)(n->pos.y/ts)};
  int **q=intsoverrads(rad,npos);
  int firstk=countfirstp(q);
  remdupfirstp(q);
  remblockedp(q,map);
  remnegp(q);
  remnray2circp(q,targetpos,ts,500);
  int lastk=countfirstp(q);
  int bestpathc=99999;
  pathstruct bestpath;
  for (int i=0;i<lastk;i++){
    Vector2 desiredpos=(Vector2){(float)(q[i][0]*ts+ts/2),
      (float)(q[i][1]*ts+ts/2)};
    pathstruct path=findpath(n, map, desiredpos, ts);
    if (path.count<=bestpathc){
      bestpathc=path.count;bestpath=path;
    }
  }
  n->path=bestpath.path;
  n->pathcount=bestpathc;
  n->curpoint=0;
  if (n->pathcount > 0) {
      npcgoto(n, n->path[0]);
  }
}


//todo:check raycast.c


