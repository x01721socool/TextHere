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
  int ppos[2]={(int)(targetpos.x/ts),(int)(targetpos.y/ts)};
  int **q=intsoverrads(rad,ppos);
  int firstk=countfirstp(q);
  for (int i=0;i<firstk;i++){
    TraceLog(LOG_INFO,"%d,%d\n",q[i][0],q[i][1]);
  }
  remdupfirstp(q);
  remnegp(q);
  remblockedp(q,map);
  remnray2circp(q,targetpos,24,500);
  int lastk=countfirstp(q);
	if (lastk==0) {
					TraceLog(LOG_WARNING,"no possible radius spot found. here are the corrdinates given");
					int **k=intsoverrads(rad,ppos);
					int firstkn=countfirstp(k);
					for (int i=0;i<firstkn;i++){
									TraceLog(LOG_INFO,"%d,%d\n",k[i][0],k[i][1]);
					}
					return;
	}
  int bestpathc=99999;
  pathstruct bestpath={NULL,0};
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


