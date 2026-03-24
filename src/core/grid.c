#include "raylib.h"
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>

gamemap loadmap(const char *filename) {
	gamemap map = {.width=10,.height=10};
	char wallchar[80],infochar[80];
	snprintf(wallchar,sizeof(wallchar),"%s/map_walls.csv",filename);
	snprintf(infochar,sizeof(infochar),"%s/info.txt",filename);
	FILE *filewall =fopen(wallchar, "r");
	FILE *fileinfo =fopen(infochar, "r");
	if (filewall == NULL){
		TraceLog(LOG_ERROR, "load %s: :(",filename);
		return map;
	}
	if (fileinfo==NULL){TraceLog(LOG_WARNING,"%s does not exist - using defaults...",infochar);}
	/*fileinfo mechanic could be placed here*/
	map.data=(int*)malloc(map.width*map.height*sizeof(int));
	for (int i=0; i<map.width*map.height;i++){
		if(fscanf(filewall, "%d,", &map.data[i]) != 1) break;
	}
	fclose(filewall); fclose(fileinfo);
	return map;
}
void unloadmap(gamemap *map){
	if (map->data != NULL) free(map->data);
}
void drawmap(gamemap map, int ts){
	for (int i=0; i<map.width * map.height; i++){
		if (map.data[i]>0) {
			int x=i%map.width;
			int y=i/map.width;
			if (map.data[i]==1){
				DrawRectangle(x*ts,y*ts,ts,ts,BLACK);
			}
		}
	}
}
