#include "raylib.h"
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
/*module headers*/
#include "core/player.h"
#include "core/grid.h"
#include "core/dialogue.h"
#include "core/basicnpc.h"
#include "core/astarpathfinding.h"
#include "core/raycast.h"
#define TS 64 /*short for tile size*/

DialogueMap dlgmap={0};
game maingame={0};
bool musicbool=false;
//not complete will finish later

int main(int argc, char** argv){
  InitWindow(800,600,"TextHere");
  //gamemap level;
  //char maingame.options=0;
  maingame.ts=64;
  bool levelselected=false; 
  bool playbarsel=false;
  int lettercount=0;
  char premaplevel[15]={0};InitAudioDevice();
  Font deffont=GetFontDefault();
  Music music=LoadMusicStream("assets/music/togrip.wav");
  music.looping=true;
  if (musicbool) PlayMusicStream(music);
  SetTargetFPS(16);
  while(!WindowShouldClose()){
    UpdateMusicStream(music);
    switch (maingame.options) {
      case 0:
        int key=GetCharPressed();
        float dt=GetFrameTime();
        while(key>0){
          if((key>=32&&key<=125)&&(lettercount<16)&&playbarsel)
          {
            premaplevel[lettercount]=(char)key;
            premaplevel[lettercount+1]='\0';
            lettercount++;
          }   
          key=GetCharPressed();}
        if(IsKeyPressed(KEY_BACKSPACE)){ lettercount--;
          if(lettercount<0)lettercount=0;
          premaplevel[lettercount]='\0';
        }
        if (IsKeyPressed(KEY_ENTER)){
          if (playbarsel&&premaplevel[0]!='\0') {
            TraceLog(LOG_INFO,"recieved string:%s",premaplevel);
            TraceLog(LOG_INFO,"length of that:%d",(int)strlen(premaplevel));
            char buffer[64];
            snprintf(buffer,sizeof(buffer),"assets/maps/%s",premaplevel);
            TraceLog(LOG_INFO,"full buffer string:%s",buffer);
            FILE *filey;
            char checkbuffer[70];
            snprintf(checkbuffer,sizeof(checkbuffer),"%s/info.txt",buffer);
            filey=fopen(checkbuffer,"r");
            if (filey){
              fclose(filey);
              maingame.level=loadmap(buffer);
              maingame.options=3;
            } else TraceLog(LOG_WARNING,"failure to open/detect %s!",buffer);}
          if (premaplevel[0]==0||premaplevel[0]=='\0') {
            playbarsel=!playbarsel;
          }
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangle(10,deffont.baseSize*4+10,40*9.75,40,DARKGRAY);
        DrawText((playbarsel)?(premaplevel[0]=='\0')?"ENTER to deselect!":premaplevel:
            "start searching with ENTER!",
            10,deffont.baseSize*4+10,
            25,(premaplevel[0]=='\0')?LIGHTGRAY:BLACK);
        DrawText("[TextHere]",10,10,deffont.baseSize*4,BLACK);
        DrawText("<- N \n credits",10,590-deffont.baseSize*8,deffont.baseSize*4,DARKGRAY);
        DrawText("M -> \nsettings",600,590-deffont.baseSize*8,deffont.baseSize*4,DARKGRAY);
        EndDrawing();
        break;
      case 2:
        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawText("Credits! :D\n\nArtemis Grotosky (me): Software owner,\n                       Software Dev,\n                      basically anything\nelse at the momentlol\n\nShardul Hirolikar: A* Pathfinding\n\nRamon Santamaria: Raylib engine\n\nif u contributed u can write ur name \nhere.jus figure out how to fit ur \nown on ts screen lol",10,10,deffont.baseSize*4,BLACK);
        EndDrawing();
        break;
      case 1:
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Settings:",10,10,deffont.baseSize*4,BLACK);
        DrawText("theres so not many things you can\n make settings out of here rn.. :P",10,10+deffont.baseSize*8,deffont.baseSize*4,
            DARKGRAY);
        EndDrawing();
        break;
      case 3:
        UnloadMusicStream(music);
        char levelfolder[64];
        snprintf(levelfolder,sizeof(levelfolder),"assets/maps/%s",premaplevel);
        dlgmap=LoadDialogueMap(levelfolder);
        maingame.plyr=(Player){
          .pos={maingame.level.sp.x*TS+32,maingame.level.sp.y*TS+32},
          .speed=maingame.level.ips,
          .size=24.0f};
        SetTargetFPS(64);
        maingame.cam=(Camera2D){0};
        maingame.cam.zoom=1.0f;
        maingame.cam.rotation=0.0f;
        DialogueBlock *activedialogueblock=NULL;
        npc npctest = {
          .pos={5*TS+32,7*TS+32},
          .active=true,
          .speed=200.0f};
        while (maingame.options==3) {
          if (activedialogueblock==NULL){
            Pupdate(&maingame.plyr,TS,maingame.level);
            maingame.cam.target=maingame.plyr.pos;
          }
          CheckAndStartDialogue(&maingame.plyr.pos,TS,&dlgmap,
              &activedialogueblock,
              &maingame.cam,
              levelfolder);
          Updatenpc(&npctest);
          maingame.cam.offset=(Vector2){400.0f,300.0f};
          if (IsKeyPressed(KEY_O)){
            levelselected=false;
          }
          if (IsKeyPressed(KEY_G)){
            findpathinradius(&npctest,maingame.plyr.pos,maingame,2);
          }
          BeginDrawing();
          ClearBackground(RAYWHITE);
          BeginMode2D(maingame.cam);
          drawmap(maingame.level, TS, maingame.cam);
          Pdraw(&maingame.plyr);
          Drawnpc(&npctest);
          EndMode2D();
          UpdateAndDrawActiveDialogue(activedialogueblock,
              maingame.plyr.pos,
              &maingame.cam,
              levelfolder);
          if (activedialogueblock
              &&!activedialogueblock->ds.active){
            UnloadDialogue(&activedialogueblock->ds);
            activedialogueblock->hasds=false;
            activedialogueblock=NULL;
          }
          EndDrawing();
        }
    }
    if (IsKeyPressed(KEY_M)&&!playbarsel){
      maingame.options=(maingame.options>=2)?0:maingame.options+1;
    }
    if (IsKeyPressed(KEY_N)&&!playbarsel){
      maingame.options=(maingame.options<=0)?2:maingame.options-1;
    }
  }
  unloadmap(&maingame.level);
  CloseAudioDevice();
  FreeDialogueMap(&dlgmap);
  CloseWindow();
  //return 0;
}
