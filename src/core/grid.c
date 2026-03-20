#include "raylib.h"
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>

gamemap loadmap(const char *filename, int w, int h) {
	gamemap map = {.width = w, .height = h};
	map.data = (int *)malloc(w * h * sizeof(int));

	FILE *file =fopen(filename, "r");
	if (file == NULL){
		TraceLog(LOG_ERROR, "load %s: :(",filename);
		return map;
	}
	for (int i=0; i<w*h;i++){
		if(fscanf(file, "%d,", &map.data[i]) != 1) break;
	}
	fclose(file);
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
