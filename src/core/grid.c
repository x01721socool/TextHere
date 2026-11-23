#include "raylib.h"
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../game.h"

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
        map.walls=malloc((size_t)w*h*sizeof(char));
        map.render=malloc((size_t)w*h*sizeof(char));
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
void drawmap(game *g){
  if(g->level.width<=0||g->level.width<=0)return;
  if(!g->level.tileset.id)return;
  const float lcx=g->cam.target.x-g->cam.offset.x;
  const float tcy=g->cam.target.y-g->cam.offset.y;
  const float rcx=lcx+2*g->cam.offset.x,bcy=tcy+2*g->cam.offset.y;
  const int xsg=(int)(floorf(lcx/(float)g->ts))<0?0
    :(int)floorf(lcx/(float)g->ts)>g->level.width?g->level.width
    :(int)floorf(lcx/(float)g->ts);
  const int ysg=(int)floorf(tcy/(float)g->ts)<0?0
    :(int)floorf(tcy/(float)g->ts)>g->level.height?g->level.height
    :(int)floorf(tcy/(float)g->ts);
  const int xeg=(int)ceilf(rcx/(float)g->ts)<0?0
    :(int)ceilf(rcx/(float)g->ts)>g->level.width?g->level.width
    :(int)ceilf(rcx/(float)g->ts);
  const int yeg=(int)ceilf(bcy/(float)g->ts)<0?0
    :(int)ceilf(bcy/(float)g->ts)>g->level.height?g->level.height
    :(int)ceilf(bcy/(float)g->ts);
  for (int y=ysg;y<yeg;y++){
    for(int x=xsg;x<xeg;x++){
      const int renid=g->level.render[y*g->level.width+x];
      DrawTexturePro(
          g->level.tileset,
          (Rectangle){
          (float)((renid%(g->level.tileset.width/16))*16),
          (float)(renid/(g->level.tileset.width/16)*16),
          (float)(16.0f),
          (float)(16.0f)
          },
          (Rectangle){
          (float)(x*g->ts+g->ts),
          (float)(y*g->ts+g->ts),
          (float)(g->ts),
          (float)(g->ts)
          },
          (Vector2){
          (float)(g->ts),
          (float)(g->ts)
          },
          0.0f,
          WHITE);
    }
  }

}
