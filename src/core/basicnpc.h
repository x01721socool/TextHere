#ifndef NPCH
#define NPCH
#include "raylib.h"
#include "player.h"
#include "stdbool.h"
typedef struct {
	bool active,ismoving;
	Vector2 pos,startpos,endpos;
	float Frametime,duration,speed;
	 
} npc;

void Drawnpc(const npc *n);
void Updatenpc(npc *n);
void npcgoto(npc *n, Vector2 target, float speed);
#endif
