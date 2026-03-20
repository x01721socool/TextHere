#ifndef GRIDH
#define GRIDH
#define MAPW 10
#define MAPH 10
typedef struct {
	int width,height,*data;
} gamemap;
void drawmap(gamemap map, int ts);
gamemap loadmap(const char *filename,int w, int h);
void unloadmap(gamemap *map);
#endif
