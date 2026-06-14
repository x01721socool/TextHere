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
  TraceLog(LOG_INFO,"Initial candidates: %d", firstk);
  
  remdupfirstp(q);
  remnegp(q);
  remblockedp(q,map);
  int afterwalls = countfirstp(q);
  TraceLog(LOG_INFO,"After removing walls/negatives: %d", afterwalls);
  
  remnray2circp(q, targetpos, ts, 500, map);
  int afterlos = countfirstp(q);
  TraceLog(LOG_INFO,"After line-of-sight filter: %d", afterlos);
  
	if (afterlos == 0) {
		TraceLog(LOG_WARNING,"no candidates with line of sight to target found");
		int k = firstk;
		for (int i = 0; i < k; i++) {
			free(q[i]);
		}
		free(q);
		return;
	}
  
  int bestpathc=99999;
  pathstruct bestpath={NULL,0};
  int bestidx=-1;
  
  for (int i=0;i<afterlos;i++){
    Vector2 desiredpos=(Vector2){(float)(q[i][0]*ts+ts/2),
      (float)(q[i][1]*ts+ts/2)};
    pathstruct path=findpath(n, map, desiredpos, ts);
    
    TraceLog(LOG_INFO,"Candidate [%d,%d] -> path length: %d", q[i][0], q[i][1], path.count);
    
    if (path.count > 0 && path.count < bestpathc){
      if (bestpath.path != NULL) free(bestpath.path);
      bestpathc=path.count;
      bestpath=path;
      bestidx=i;
    } else if (path.path != NULL) {
      free(path.path);
    }
  }
  
  if (bestidx >= 0) {
    TraceLog(LOG_INFO,"Best candidate selected: [%d,%d] with path length %d", q[bestidx][0], q[bestidx][1], bestpathc);
  } else {
    TraceLog(LOG_WARNING,"No valid path found to any candidate");
  }
  
  n->path=bestpath.path;
  n->pathcount=bestpathc;
  n->curpoint=0;
  if (n->pathcount > 0) {
      npcgoto(n, n->path[0]);
  }
  
  int k = firstk;
  for (int i = 0; i < k; i++) {
    free(q[i]);
  }
  free(q);
}
