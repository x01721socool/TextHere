#include "raylib.h"
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

gamemap loadmap(const char *filename) {
	gamemap map;
	char wallchar[80],infochar[80];
	snprintf(wallchar,sizeof(wallchar),"%s/map_walls.csv",filename);
	snprintf(infochar,sizeof(infochar),"%s/info.txt",filename);
	FILE *filewall =fopen(wallchar, "r");
	FILE *fileinfo =fopen(infochar, "r");
	if (!filewall){
		TraceLog(LOG_ERROR, "load %s: :(",filename);
		return map;
	}
	if (!fileinfo){TraceLog(LOG_WARNING,"%s does not exist - using defaults...",infochar);}
	char line[256];
	while(fgets(line, sizeof line, fileinfo)){
		char key[32],value[64];
		if (sscanf(line,"%31[^:]:%63[^\n]",key,value)!=2){
			TraceLog(LOG_WARNING,"founded an unethical line in %s:%s",infochar,line);
			continue;
		}
		if (strcmp(key,"size")==0){
			int w,h;
			if (sscanf(value,"%d,%d",&w,&h)==2&&w>0&&h>0){
				map.width=w;
				map.height=h;
				map.data=(int*)malloc(map.width*map.height*sizeof(int));
				for (int i=0; i<map.width*map.height;i++){
					if(fscanf(filewall, "%d,", &map.data[i]) != 1) break;
				}
				if (!map.data) {
					TraceLog(LOG_WARNING,"out of memory! D:");
				}
			} else
				TraceLog(LOG_WARNING,"faulty size from info.txt:%s",value);
		}
		if (strcmp(key,"sp")==0){
			float x,y;
			if(sscanf(value,"%f,%f",&x,&y)==2&&x>=0&&x<=(float)map.width&&y>=0&&y<=(float)map.height){
					map.sp.x=x;map.sp.y=y;
			} else 
				TraceLog(LOG_WARNING,"%s:%s<-faulty spos coords %s",infochar,key,value);
		}
		if (strcmp(key,"ips")==0) {
			float speed;
			if(sscanf(value,"%f",&speed)>0&&speed>0.0f) {
				map.ips=speed;
			} else{
				TraceLog(LOG_WARNING,"%s:%s<-faulty ips %s",infochar,key,value);
			}
		}
	}
	fclose(filewall); fclose(fileinfo);
	map.mapdir=filename;
	return map;
}
void unloadmap(gamemap *map){
	if (map->data != NULL){free(map->data); map->data=NULL;}
}
void drawmap(gamemap map,int ts,Camera2D camera){
	for(int j=(int)camera.target.y-300;j<=(int)camera.target.y+300+ts;j+=ts){
		for(int i=(int)camera.target.x-400;i<=(int)camera.target.x+400+ts;i+=ts){
			//supposedly get spritesheet from map struct to do sprite
			//sheet stuff
		}
	}
}
