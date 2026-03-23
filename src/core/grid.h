#ifndef GRIDH
#define GRIDH
typedef struct {
	int width,height,*data;
	float ips;
	Vector2 sp;
} gamemap;
void drawmap(gamemap map, int ts);
gamemap loadmap(const char *filename);
void unloadmap(gamemap *map);
#endif GRIDH
