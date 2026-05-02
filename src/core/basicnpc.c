//todo:add stuffs based off of basicnpc.h
#include "basicnpc.h"
#include "raylib.h"
#include "player.h"
#include "math.h"
void Drawnpc(const npc *n) {
	if (!n->active) return;
	DrawCircle(n->pos.x,n->pos.y,16,RED);
}

void npcgoto(npc *n, Vector2 target, float speed){
  n->startpos=n->pos;
  n->endpos=target;
  n->speed=speed;
  n->ismoving=true;
}

void Updatenpc (npc *n) {
  if (!n->active) return;
  if(sqrtf(powf(n->pos.x-n->startpos.x,2.0f)+
        powf(n->pos.y-n->startpos.y,2.0f))>=
      sqrtf(powf(n->startpos.x-n->endpos.x,2.0f)+
        powf(n->startpos.y-n->endpos.y,2.0f))) {n->ismoving=false;}
  if (n->ismoving) {
    float dt=GetFrameTime();
    float range=dt*n->speed;
    n->pos.x+=(n->endpos.x-n->startpos.x)*range;
    n->pos.y+=(n->endpos.y-n->startpos.y)*range;
  }
}
