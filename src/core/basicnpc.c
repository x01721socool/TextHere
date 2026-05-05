#include "basicnpc.h"
#include "raymath.h"
#include "raylib.h"
#include "player.h"
#include "math.h"
#include <stddef.h>
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
  if (n->ismoving&&Vector2Distance(n->pos,n->endpos)<=1.0f) {
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
void npcsetpath(npc *n,Vector2 *ps, int c) {
  n->path=ps;
  n->pathcount=c;
  n->curpoint=0;
  npcgoto(n,n->path[0]);
}
