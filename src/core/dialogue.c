#define _DEFAULT_SOURCE
#include "dialogue.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../game.h"
DialogueSystem LoadDialogue(const char *filename,game *g, const char *levelfolder){
  DialogueSystem ds={.lines =NULL,
    .count=0, .lettercount=0,
    .current=0, .texttimer=0.0f,
    .active=true};
  ds.currentPortrait.id=0;
  FILE *file=fopen(filename,"r");
  if (!file)return ds;
  char buffer[256];

  while(fgets(buffer,sizeof(buffer), file)){
    ds.count++;
    ds.lines=realloc(ds.lines,ds.count * sizeof(char *));
    buffer[strcspn(buffer,"\n")]=0;
    ds.lines[ds.count -1]=strdup(buffer);
  }
  fclose(file);
  return ds;
}

void UpdateDialogue(game *g,const char *levelfolder){
  if(!g->curactblock->ds.active)return;
  float speed=0.01f;g->curactblock->ds.texttimer+=GetFrameTime();
  if(g->curactblock->ds.texttimer>=speed){
    if (g->curactblock->ds.lettercount<(int)(strlen(g->curactblock->ds.currentText))){
      g->curactblock->ds.lettercount++;
      if (IsSoundValid(g->curactblock->ds.talking)) {
        PlaySound(g->curactblock->ds.talking);
      }
    }
    g->curactblock->ds.texttimer=0.0f;
  }
  if(IsKeyPressed(KEY_ENTER)){
    if(g->curactblock->ds.lettercount<(int)strlen(g->curactblock->ds.currentText)){
      g->curactblock->ds.lettercount=(int)strlen(g->curactblock->ds.currentText);
    }else{
      g->curactblock->ds.current++;
      if(g->curactblock->ds.current >=g->curactblock->ds.count){
        g->curactblock->ds.active=false;
      }else{
        ParseDialogueLine(g,levelfolder);
      }
    }
  }
}
void ParseDialogueLine(game *g,const char *levelfolder){
  if(g->curactblock->ds.current >= g->curactblock->ds.count)return;
  char linecpy[256];
  strncpy(linecpy,g->curactblock->ds.lines[g->curactblock->ds.current], sizeof(linecpy));
  char *name =strtok(linecpy, ":");
  if (!name) return;
  if (strcmp(name,"CAMERA")==0){
    char *option = strtok(NULL,":");
    if (strcmp(option,"CENTERPLAYER")==0){
      g->cam.target=g->plyr.pos;
    }
    else if (strcmp(option,"TPTOPOS")==0){
      int x,y;
      char *cords=strtok(NULL,":");
      if (sscanf(cords,"%d,%d",&x,&y)<2){
        TraceLog(LOG_WARNING,"faulty g->cam cords"
            "!: %s",linecpy);
        g->cam.target=g->plyr.pos;
      }
      g->cam.target=(Vector2){(float)(x*64),(float)(y*64)};
    } else {
      TraceLog(LOG_WARNING,"BAD g->camera option!: %s",
          linecpy);
    }
    g->curactblock->ds.current++;
    ParseDialogueLine(g,levelfolder);
    return;
  } else if (strcmp(name,"SCENE")==0){
    char *option = strtok(NULL,":");
    if (strcmp(option,"NONE")!=0){
      char path[256];
      snprintf(path,sizeof(path),"%s/scenes/%s.png",levelfolder,option);
      if (g->curactblock->ds.currentScene.id!=0) UnloadTexture(g->curactblock->ds.currentScene);
      g->curactblock->ds.currentScene=LoadTexture(path);
    }g->curactblock->ds.current++;
    ParseDialogueLine(g,levelfolder);
    return;
  } else{
    strcpy(g->curactblock->ds.currentName,name);
    char *emotion=strtok(NULL,":");
    char *text=strtok(NULL,":");
    char snbuf[256];
    if (!IsSoundValid(g->curactblock->ds.talking)) {
      snprintf(snbuf,sizeof(snbuf),"assets/sound/talking/%s.wav",g->curactblock->ds.currentName);
      g->curactblock->ds.talking=LoadSound(snbuf);
      if (!IsSoundValid(g->curactblock->ds.talking)){
        TraceLog(LOG_WARNING,"%s, the file may not exist"
            "for talking reasons",snbuf);
      }
    }

    if (emotion) {
      char path[256];
      snprintf(path,sizeof(path),"assets/sprite/"
          "dialogue/%s/%s.png",name,emotion);
      if (g->curactblock->ds.currentPortrait.id!=0)
        UnloadTexture(g->curactblock->ds.currentPortrait);
      g->curactblock->ds.currentPortrait=LoadTexture(path);
    }
    if (text){
      strncpy(g->curactblock->ds.currentText,text,
          sizeof(g->curactblock->ds.currentText)-1);
    } else {
      strncpy(g->curactblock->ds.currentText,"...",sizeof("...")-1);
    }
    g->curactblock->ds.lettercount=0;g->curactblock->ds.texttimer=0.0f;
  }}

void DrawDialogue(DialogueSystem *ds){
  if(!ds->active)return;
  if (ds->currentScene.id>0){
    DrawTextureEx(ds->currentScene,
        (Vector2){0,0},
        0,
        1.0f,
        WHITE);
  }
  if (ds->currentPortrait.id>0) {
    DrawTextureEx(ds->currentPortrait,
        (Vector2){GetScreenWidth()-256,GetScreenHeight()*3/4-320},
        0,
        2.0f,
        WHITE);
  }
  int boxY=GetScreenHeight()/4;
  DrawRectangle(0,
      boxY*3,
      GetScreenWidth(),
      GetScreenHeight(),
      WHITE);
  const char* visibletext=TextSubtext(ds->currentText,
      0,
      ds->lettercount);
  DrawText(visibletext,
      10,
      boxY*3+5,
      20,
      BLACK);
}
void UnloadDialogue(DialogueSystem *ds){
  if (ds->lines!=NULL){
    for(int i=0;i<ds->count;i++){
      if (ds->lines[i]!=0){
        free(ds->lines[i]);
        ds->lines[i]=NULL;
      }
    }
    free(ds->lines);
    ds->lines=NULL;
    ds->count=0;
  }
  if (ds->currentScene.id>0){
    UnloadTexture(ds->currentScene);
    ds->currentScene.id=0;
  }
  if (ds->currentPortrait.id>0){
    UnloadTexture(ds->currentPortrait);
    ds->currentPortrait.id=0;
  }
  if (IsSoundValid(ds->talking)){
    UnloadSound(ds->talking);
    ds->talking=(Sound){0};
  }
}
DialogueMap LoadDialogueMap(const char *levelfolder){
  DialogueMap dm={.blocks=NULL,.count=0};
  char csvpath[64];
  snprintf(csvpath,sizeof(csvpath),"%s/map_dialogue.csv",levelfolder);
  FILE *file = fopen(csvpath,"r");
  if (!file){
    TraceLog(LOG_WARNING,
        "level %s:cant find or level has no dialogue",
        csvpath);
    return dm;
  }
  char line[256];
  while (fgets(line,sizeof(line),file)){
    int x=-1,y=-1;
    char custom[128]="";
    int scanned=sscanf(line,
        "%d,%d,%127[^\n]",
        &x,
        &y,
        custom);
    if (scanned<2) continue;
    DialogueBlock *blocks=realloc(dm.blocks,
        (dm.count+1)*sizeof(DialogueBlock));
    if (!blocks){
      TraceLog(LOG_ERROR,"out of memory loading dialogue map %s",csvpath);
      break;
    }
    dm.blocks=blocks;
    DialogueBlock *b=&dm.blocks[dm.count++];
    b->tilex=x;
    b->tiley=y;
    b->alreadyseen=false;
    b->hasds=false;
    b->ds=(DialogueSystem){0};
    if (strlen(custom)>0)
      snprintf(b->txtpath,
          sizeof(b->txtpath),
          "%s/%s",
          levelfolder,
          custom);
    else
      snprintf(b->txtpath,
          sizeof(b->txtpath),
          "%s/x%dy%d.txt",
          levelfolder,
          x,
          y);
  }
  fclose(file);
  return dm;
}
void FreeDialogueMap(DialogueMap *dm){
  if (!dm) return;
  for (int i=0;i<dm->count;++i){
    if(dm->blocks[i].hasds){
      UnloadDialogue(&dm->blocks[i].ds);
    }
  }
  free(dm->blocks);
  dm->blocks=NULL;
  dm->count=0;
}
void CheckAndStartDialogue(game *g,const char *levelfolder){
  if (!g || !g->dm.blocks || g->dm.count<=0) return;
  if (g->curactblock) return;
  int ptilex=(int)(g->plyr.pos.x/g->ts);
  int ptiley=(int)(g->plyr.pos.y/g->ts);
  for (int i=0;i<g->dm.count;++i){
    DialogueBlock *b = &g->dm.blocks[i];
    if (b->alreadyseen) continue;
    if (b->tilex==ptilex&&b->tiley==ptiley){
      b->ds=LoadDialogue(b->txtpath,g,levelfolder);
      if (!b->ds.lines || b->ds.count<=0){
        TraceLog(LOG_WARNING,"could not load dialogue file %s",b->txtpath);
        b->alreadyseen=true;
        break;
      }
      b->hasds=true;
      b->alreadyseen=true;
      g->curactblock=b;
      ParseDialogueLine(g, levelfolder);
      break;
    }
  }
}
void UpdateAndDrawActiveDialogue(game *g,const char *levelfolder){ 
  if (!g->curactblock) return;
  UpdateDialogue(g,levelfolder);
  DrawDialogue(&g->curactblock->ds);
  if (!g->curactblock->ds.active){
    g->curactblock->hasds=false;
  }
}
