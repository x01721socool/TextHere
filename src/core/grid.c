#include "raylib.h"
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

gamemap loadmap(const char *filename) {
	gamemap map={0};
	char wallchar[80],infochar[80],renderchar[80];
	snprintf(wallchar,sizeof(wallchar),"%s/map_walls.csv",filename);
	snprintf(infochar,sizeof(infochar),"%s/info.txt",filename);
	snprintf(renderchar,sizeof(renderchar),"%s/map_render.csv",filename);
	FILE *filewall =fopen(wallchar, "r");
	FILE *fileinfo =fopen(infochar, "r");
	FILE *filerender=fopen(renderchar,"r");
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
				map.walls=malloc((size_t)w*h*sizeof(int));
				map.render=malloc((size_t)w*h*sizeof(int));
				for (size_t i=0; i<(size_t)(w*h);i++){
					if (fscanf(filewall, "%d", &map.walls[i])==1){
						fgetc(filewall);
					}
					if (fscanf(filerender,"%d",&map.render[i])==1){
						fgetc(filerender);
					}

				}
				if (!map.walls) {
					TraceLog(LOG_WARNING,"out of memory! D:");
				}
				Image img=LoadImage("assets/sprite/spritesheet.png");
				if (img.data){
					map.tileset=LoadTextureFromImage(img);
					UnloadImage(img);
					SetTextureFilter(map.tileset,TEXTURE_FILTER_POINT);
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
	fclose(filewall); fclose(fileinfo); fclose(filerender);
	return map;
}
void unloadmap(gamemap *map){
	if (map->walls != NULL){free(map->walls); map->walls=NULL;}
	if (map->render!=NULL){free(map->render);map->render=NULL;}
	UnloadTexture(map->tileset);
}
void drawmap(gamemap map,int ts,Camera2D camera){
	if(map.width<=0||map.width<=0)return;
	if(!map.tileset.id)return;
	const float lcx=camera.target.x-camera.offset.x;
	const float tcy=camera.target.y-camera.offset.y;
	const float rcx=lcx+2*camera.offset.x,bcy=tcy+2*camera.offset.y;
	const int xsg=(int)(floorf(lcx/(float)ts))<0?0
		:(int)floorf(lcx/(float)ts)>map.width?map.width
		:(int)floorf(lcx/(float)ts);
	const int ysg=(int)floorf(tcy/(float)ts)<0?0
		:(int)floorf(tcy/(float)ts)>map.height?map.height
		:(int)floorf(tcy/(float)ts);
	const int xeg=(int)ceilf(rcx/(float)ts)<0?0
		:(int)ceilf(rcx/(float)ts)>map.width?map.width
		:(int)ceilf(rcx/(float)ts);
	const int yeg=(int)ceilf(bcy/(float)ts)<0?0
		:(int)ceilf(bcy/(float)ts)>map.height?map.height
		:(int)ceilf(bcy/(float)ts);
	for (int y=ysg;y<yeg;y++){
		for(int x=xsg;x<xeg;x++){
			const int renid=map.render[y*map.width+x];
			DrawTexturePro(
					map.tileset,
					(Rectangle){
						(float)((renid%(map.tileset.width/16))*16),
						(float)(renid/(map.tileset.width/16)*16),
						(float)(16.0f),
						(float)(16.0f)
						},
					(Rectangle){
						(float)(x*ts+ts),
						(float)(y*ts+ts),
						(float)(ts),
						(float)(ts)
						},
					(Vector2){
						(float)(ts),
						(float)(ts)
						},
					0.0f,
					WHITE);
			}
	}
						
}
