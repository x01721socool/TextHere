//*-------------------------* |Game.c           |
//| ||||| ,|||  |   |   |   | |-----------------|
//|   |   |   |  \ /  ==|== | |Original Author: |
//|   |   ||||'   X     |"  | | Artemis Grotosky|
//|   |   |      / \    |   | |-----------------|
//|   |   '|||' |   |   |   | |License:         |
//|/\=/\=/\=/\=/\=/\=/\=/\=/| | GPL v3.0 License|
//| |   | ,|||  ,/--' ,|||  | *-----------------*
//| |___| |   | |/    |   | | *----------------------------------*
//| ||||| ||||' |'    ||||' | |"                                 |
//| |"""| |     |     |     | | This file acts as the core of    |
//| |   | '|||' |     '|||' | | The game, and also menu purposes |
//*-------------------------* |                                 "|
//                            *----------------------------------*
//How's my ascii art skills? cool right =D
//
//basic headers
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

bool musicbool=false;
enum menuoptions {
        mainm,settings,credits
};
//not complete will finish later

int main(int argc, char** argv){
  InitWindow(800,600,"TextHere");
	gamemap level;
  enum menuoptions optionz=0;
  bool levelselected=false;
	int lettercount=0;
  char premaplevel[15]={0};InitAudioDevice();
  Font deffont=GetFontDefault();
  float tinyt=0.0f;
  float oriw=MeasureText("[TextHere]",deffont.baseSize);
  float orih=deffont.baseSize;
  Music music=LoadMusicStream("assets/music/togrip.wav");
  music.looping=true;
  if (musicbool) PlayMusicStream(music);
  while(!levelselected&&!WindowShouldClose()){
    UpdateMusicStream(music);
    switch (optionz) {
            case mainm:
                int key=GetCharPressed();
                float dt=GetFrameTime();
                tinyt=tinyt>=2*PI?0:tinyt+dt;
                while(key>0){
                if((key>=32&&key<=125)&&(lettercount<16))
                {
                        premaplevel[lettercount]=(char)key;
                        premaplevel[lettercount+1]='\0';
                        lettercount++;
                }   
                key=GetCharPressed();}
                if(IsKeyPressed(KEY_BACKSPACE)){
                        lettercount--;
                        if(lettercount<0)lettercount=0;
                        premaplevel[lettercount]='\0';
                }
                if (IsKeyPressed(KEY_ENTER)){
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
																level=loadmap(buffer);
																levelselected=true;
												} else TraceLog(LOG_WARNING,"failure to open/detect %s!",buffer);
                }
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText(premaplevel,200,350,40,BLACK);
                DrawTextPro(deffont,"[TextHere]",(Vector2){390,200},(Vector2){3*oriw,3*orih},20*sin(tinyt),75.0f,1.0f,BLACK);
                EndDrawing();
								break;
				default:
                BeginDrawing();
                ClearBackground(RAYWHITE);
                DrawText("hi ts is other",200,350,40,BLACK);
                EndDrawing();

		}
  }
  UnloadMusicStream(music);
  char levelfolder[64];
  snprintf(levelfolder,sizeof(levelfolder),"assets/maps/%s",premaplevel);
  DialogueMap dlgmap=LoadDialogueMap(levelfolder);
  Player player={
					.pos={level.sp.x*TS+32,level.sp.y*TS+32},
					.speed=level.ips,
					.size=24.0f};
  SetTargetFPS(64);
  Camera2D camera={0};
	camera.zoom=1.0f;
	camera.rotation=0.0f;
  DialogueBlock *activedialogueblock=NULL;
  npc npctest = {
    .pos={5*TS+32,7*TS+32},
    .active=true,
    .speed=200.0f
  };
    while (!WindowShouldClose()) {
    if (activedialogueblock==NULL){
      Pupdate(&player,TS,level);
      camera.target=player.pos;
    }
    CheckAndStartDialogue(&player.pos,TS,&dlgmap,
        &activedialogueblock,
        &camera,
        levelfolder);
    Updatenpc(&npctest);
    camera.offset=(Vector2){400.0f,300.0f};
    if (IsKeyPressed(KEY_ESCAPE)){
      levelselected=false;
    }
    if (IsKeyPressed(KEY_G)){
            findpathinradius(&npctest,player.pos,
                            level,TS,2);
    }
    BeginDrawing();
      ClearBackground(RAYWHITE);
      BeginMode2D(camera);
        drawmap(level, TS, camera);
        Pdraw(&player);
        Drawnpc(&npctest);
      EndMode2D();
      UpdateAndDrawActiveDialogue(activedialogueblock,
          player.pos,
          &camera,
          levelfolder);
      if (activedialogueblock
        &&!activedialogueblock->ds.active){
        UnloadDialogue(&activedialogueblock->ds);
        activedialogueblock->hasds=false;
        activedialogueblock=NULL;
      }
    EndDrawing();
  }
  unloadmap(&level);
  CloseAudioDevice();
  FreeDialogueMap(&dlgmap);
  CloseWindow();
  return 0;

}
