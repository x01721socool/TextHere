#include "player.h"
#include "raylib.h"

void Pupdate(Player *p, const int ts, gamemap map) {
    float dt = GetFrameTime();
    
    // 1. Calculate the 'intended' next position
    Vector2 nextPos = p->pos;

    if (IsKeyDown(KEY_A)) nextPos.x -= p->speed * dt;
    if (IsKeyDown(KEY_D)) nextPos.x += p->speed * dt;
    if (IsKeyDown(KEY_W)) nextPos.y -= p->speed * dt;
    if (IsKeyDown(KEY_S)) nextPos.y += p->speed * dt;

    // 2. Collision Helper Logic (Horizontal)
    // We check the grid coordinate of the next X position
    int gx = (int)(nextPos.x / ts);
    int gy = (int)(p->pos.y / ts); // Keep current Y

    // Boundary check + Wall check
    if (gx >= 0 && gx < map.width && gy >= 0 && gy < map.height) {
        if (map.data[gy * map.width + gx] != 1) {
            p->pos.x = nextPos.x;
        }
    }

    // 3. Collision Helper Logic (Vertical)
    gx = (int)(p->pos.x / ts); // Use current (possibly updated) X
    gy = (int)(nextPos.y / ts);

    if (gx >= 0 && gx < map.width && gy >= 0 && gy < map.height) {
        if (map.data[gy * map.width + gx] != 1) {
            p->pos.y = nextPos.y;
        }
    }
}
void Pdraw(const Player *p) {
	DrawCircle((int)p->pos.x,(int)p->pos.y,p->size,BLUE);
}
