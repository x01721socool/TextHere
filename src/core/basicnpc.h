#ifndef NPCH
#define NPCH
#include "raylib.h"
#include "player.h"
#include "stdbool.h"
typedef struct {
	bool active,ismoving;
	Vector2 pos,startpos,endpos,*path;
	float Frametime,duration,speed;
  int pathcount,curpoint;
	 
} npc;

void Drawnpc(const npc *n);
void Updatenpc(npc *n);
void npcgoto(npc *n, Vector2 target);
void npcsetpath(npc *n,gamemap map, Vector2 target,int ts);
#endif
