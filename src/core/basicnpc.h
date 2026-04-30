#ifndef NPCH
#define NPCH
#include "raylib.h"

typedef struct {
	Vector2 pos;
} npc;

void Drawnpc(const npc *n);
#endif
