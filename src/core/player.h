#ifndef PLAYERH
#define PLAYERH
#include "raylib.h"
#include "grid.h"
typedef struct {
	Vector2 pos;
	float size,speed;
} Player;
void Pupdate(Player *p,const int ts, gamemap map);
void Pdraw(const Player *p);

#endif
