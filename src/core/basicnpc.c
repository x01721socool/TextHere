#include "basicnpc.h"
#include "raylib.h"

void Drawnpc(const npc *n) {
	DrawCircle((int)n->pos.x,(int)n->pos.y,64,BLACK);
}
